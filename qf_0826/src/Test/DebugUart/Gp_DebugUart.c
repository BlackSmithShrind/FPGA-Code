/*@file     Gp_DebugUart.c
* @brief    开发板的debug uart
* @details  开发板的uart转usb串口，用于串口通信和调试
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/21
*/
#include "Gp_DebugUart.h"
#include "../../Bsp/Intc/Gp_Intc.h"
#include "../../Bsp/UartLite/Gp_UartLite.h"
#include "../../App/Xm/Gp_Xm.h"
#include "../XmTest/Gp_XmTest.h"
#include "../../App/Edfa/Gp_Edfa.h"
#include "../EdfaTest/Gp_EdfaTest.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
unsigned char gDebugUartTxDataBuffer[DEBUG_UART_TX_DATA_BUFFER_SIZE] = "";
unsigned char gDebugUartRxDataBuffer[DEBUG_UART_RX_DATA_BUFFER_SIZE] = "";
volatile unsigned short gDebugUartInitIsReady = FALSE;                      // 初始化准备就绪
volatile unsigned short gDebugUartRecvIsReady = TRUE;                       // 已经接收到数据

volatile unsigned int gDebugUartTotalReceivedCount = 0;                     // 接收到多少数据
volatile unsigned int gDebugUartTotalSentCount = 0;                         // 发送到多少数据
volatile unsigned short gDebugUartHeaderConstBytes = 0;                     // 头部固定数据
volatile unsigned short gDebugUartRemainRecvBytes = 0;                      // 剩余字节数

/****************************************************************************
 *              debug uart 初始化（外部接口）
 ****************************************************************************/
int debugUartInit()
{
    // 初始化全局变量
    // 发送缓存
    memset(gDebugUartTxDataBuffer, 0, DEBUG_UART_TX_DATA_BUFFER_SIZE);
    // 接收缓存
    memset(gDebugUartRxDataBuffer, 0, DEBUG_UART_RX_DATA_BUFFER_SIZE);
    // 接收到多少数据
    gDebugUartTotalReceivedCount = 0;
    // 发送到多少数据
    gDebugUartTotalSentCount = 0;
    // 剩余字节数
    gDebugUartRemainRecvBytes = 0;
    // 已经接收到数据
    gDebugUartRecvIsReady = TRUE;

    XUartLite_Handler pSendCbHandler = debugUartSendHandler;
    XUartLite_Handler pRecvCbHandler = debugUartRecvHandler;
#ifdef EDFA_DEBUG_UART_TEST
    pSendCbHandler = edfaDebugUartSendHandler;
    pRecvCbHandler = edfaDebugUartRecvHandler;
#else
    pSendCbHandler = xmDebugUartSendHandler;
    pRecvCbHandler = xmDebugUartRecvHandler;
#endif
    //初始化rs422串口
    int nRet = uartLiteInit(&gIntcInst0, &gDebugUartLiteInst, GP_DEBUG_UARTLITE_DEVICE_ID, GP_INTC_DEBUG_UART_INT_ID, edfaDebugUartSendHandler, edfaDebugUartRecvHandler);
    if (nRet != SEC_SUCCESS)
	{
		print("debugUartInit: uartLiteInit is error...\r\n");
		return SEC_ERROR;
	}

    // 中断使能
	nRet = intcEnable(&gIntcInst0, GP_INTC_DEBUG_UART_INT_ID);
	if (nRet != SEC_SUCCESS)
	{
		print("debugUartInit: intcEnable is error...\r\n");
		return SEC_ERROR;
	}

    // 准备就绪
    gDebugUartInitIsReady = TRUE;

    return SEC_SUCCESS;
}

/****************************************************************************
 *              DebugUart loop
 ****************************************************************************/
int debugUartLoop()
{
#ifdef XM_DEBUG_UART_TEST
    // xm debug uart recv loop
    xmDebugUartRecvProcess();

    // xm test loop
    xmTestLoop();
#endif

#ifdef EDFA_DEBUG_UART_TEST
    // edfa debug uart recv loop
    edfaDebugUartRecvProcess();

    // edfa test loop
    edfaTestLoop();
#endif

    return SEC_SUCCESS;
}

/****************************************************************************
 *              debug uart 发送中断回调函数
 * pCallbackRef:
 * unSendBytes:
 ****************************************************************************/
void debugUartSendHandler(void* pCallbackRef, unsigned int unSendBytes)
{
    // 发送完毕
    if (gDebugUartTotalSentCount == unSendBytes)
    {
        gDebugUartTotalSentCount = 0;
    }
}

/****************************************************************************
 *              debug uart 接收中断回调函数
 * pCallbackRef:
 * unRecvBytes:
 ****************************************************************************/
void debugUartRecvHandler(void* pCallbackRef, unsigned int unRecvBytes)
{
    // 接收多少个字节
    gDebugUartTotalReceivedCount = unRecvBytes;
}

/****************************************************************************
 *             debug uart 数据发送处理
 * pBuffer:
 * unSize:
 ****************************************************************************/
int xmDebugUartSendProcess(unsigned char* pBuffer, unsigned int unSize)
{
    int nRet = SEC_ERROR;

    // 参数检查
    if (!pBuffer || !unSize)
    {
        return nRet;
    }

    // 判断是否已经初始化
    if (!gDebugUartInitIsReady)
    {
        return nRet;
    }

    // 异步发送数据
    nRet = uartLiteSend(&gDebugUartLiteInst, pBuffer, unSize);

    return nRet;
}

/****************************************************************************
 *             debug uart 数据接收处理
 ****************************************************************************/
int xmDebugUartRecvProcess()
{
	// 判断是否已经初始化
    if (!gDebugUartInitIsReady)
    {
        return SEC_ERROR;
    }

    // 已经接收数据
    if (gDebugUartRecvIsReady)
    {
    	// 固定接收8个字节
        uartLiteRecv(&gDebugUartLiteInst, gDebugUartRxDataBuffer, 8);

        gDebugUartRecvIsReady = FALSE;
    }

    // 进行数据copy,然后xm模块去处理
    if(gDebugUartTotalReceivedCount == 8)
    {
    	// copy数据到xm test模块buffer
    	memcpy(gXmDebugUartRxDataBuffer, gDebugUartRxDataBuffer, 8);
    	// 设置新数据标识
        gXmDebugUartRxNewDataIsFin = TRUE;
    	// 重置接收到的数据
    	gDebugUartTotalReceivedCount = 0;
    	// 接收下一帧
    	gDebugUartRecvIsReady = TRUE;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *              xm debug uart 发送中断回调函数
 * pCallbackRef:
 * unSendBytes:
 ****************************************************************************/
void xmDebugUartSendHandler(void* pCallbackRef, unsigned int unSendBytes)
{
    // 发送完毕
    if (gDebugUartTotalSentCount == unSendBytes)
    {
        gDebugUartTotalSentCount = 0;
    }
}

/****************************************************************************
 *              xm debug uart 接收中断回调函数
 * pCallbackRef:
 * unRecvBytes:
 ****************************************************************************/
void xmDebugUartRecvHandler(void* pCallbackRef, unsigned int unRecvBytes)
{
    // 接收多少个字节
    gDebugUartTotalReceivedCount = unRecvBytes;
}

/****************************************************************************
 *             edfa debug uart 数据发送处理
 * pBuffer:
 * unSize:
 ****************************************************************************/
int edfaDebugUartSendProcess(unsigned char* pBuffer, unsigned int unSize)
{
    int nRet = SEC_ERROR;

    // 参数检查
    if (!pBuffer || !unSize)
    {
        return nRet;
    }

    // 判断是否已经初始化
    if (!gDebugUartInitIsReady)
    {
        return nRet;
    }

    // 异步发送数据
    nRet = uartLiteSend(&gDebugUartLiteInst, pBuffer, unSize);

    return nRet;
}

/****************************************************************************
 *             edfa debug uart 数据接收处理
 ****************************************************************************/
int edfaDebugUartRecvProcess()
{
    // 判断是否已经初始化
    if (!gDebugUartInitIsReady)
    {
        return SEC_ERROR;
    }

    // 已经接收数据
    if (gDebugUartRecvIsReady)
    {
    	uartLiteSend(&gDebugUartLiteInst, "123456", 6);

        // 先获取头部固定长度8字节
        gDebugUartHeaderConstBytes = EDFA_SF_HEADER_SIZE;
        // 先获取头部固定8个数据
        uartLiteRecv(&gDebugUartLiteInst, gDebugUartRxDataBuffer, gDebugUartHeaderConstBytes);

        gDebugUartRecvIsReady = FALSE;
    }

//    // 进行数据copy,然后xm模块去处理
//    if (gDebugUartTotalReceivedCount == gDebugUartHeaderConstBytes)
//    {
//    	// 解析数据长度
//		gDebugUartRemainRecvBytes = *(unsigned short*)(gDebugUartRxDataBuffer + 6) + 1;
//		// 然后抓取剩余长度
//		uartLiteRecv(&gDebugUartLiteInst, gDebugUartRxDataBuffer + EDFA_SF_HEADER_SIZE, gDebugUartRemainRecvBytes);
//
//        // 接收数据清0
//        gDebugUartTotalReceivedCount = 0;
//    }

    // 全部接收到后进行数据解析
    if (gDebugUartTotalReceivedCount == gDebugUartRemainRecvBytes + gDebugUartHeaderConstBytes && gDebugUartRemainRecvBytes > 0)
    {
        // copy数据到xm test模块buffer
        memcpy(gEdfaDebugUartRxDataBuffer, gDebugUartRxDataBuffer, gDebugUartTotalReceivedCount);
        // 设置新数据标识
        gEdfaDebugUartRxNewDataIsFin = TRUE;
        //
        gEdfaDebugUartTotalReceivedCount = gDebugUartTotalReceivedCount;
        // 固定数据清0
        gDebugUartHeaderConstBytes = 0;
        // 剩余数据清0
        gDebugUartRemainRecvBytes = 0;
        // 接收下一帧
        gDebugUartRecvIsReady = TRUE;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *              debug uart 发送中断回调函数
 * pCallbackRef:
 * unSendBytes:
 ****************************************************************************/
void edfaDebugUartSendHandler(void* pCallbackRef, unsigned int unSendBytes)
{
    // 发送完毕
    if (gDebugUartTotalSentCount == unSendBytes)
    {
        gDebugUartTotalSentCount = 0;
    }
}

/****************************************************************************
 *              debug uart 接收中断回调函数
 * pCallbackRef:
 * unRecvBytes:
 ****************************************************************************/
void edfaDebugUartRecvHandler(void* pCallbackRef, unsigned int unRecvBytes)
{
    // 接收多少个字节
    if (unRecvBytes == gDebugUartHeaderConstBytes)
	{
		// 解析数据长度
		gDebugUartRemainRecvBytes = *(unsigned short*)(gDebugUartRxDataBuffer + 6) + 1;
		// 然后抓取剩余长度
		uartLiteRecv(&gDebugUartLiteInst, gDebugUartRxDataBuffer + EDFA_SF_HEADER_SIZE, gDebugUartRemainRecvBytes);

		// 接收数据清0
		gDebugUartTotalReceivedCount += gDebugUartHeaderConstBytes;
	}

    // 接收剩余字节字节
	if (unRecvBytes == gDebugUartRemainRecvBytes)
	{
		gDebugUartTotalReceivedCount += gDebugUartRemainRecvBytes;
	}
}

