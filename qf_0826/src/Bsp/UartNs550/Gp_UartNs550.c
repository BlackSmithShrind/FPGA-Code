/*@file     Gp_UartNs550.c
* @brief    uartns550串口模块实现文件
* @details  提供uartns550串口类的相关操作
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/07
*/

#include "Gp_UartNs550.h"
#include "../Intc/Gp_Intc.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
XUartNs550 gCg2ZkRs644UartNs550Inst;                                            //  CG  To 主控板全局单例
XUartNs550 gCg2TxRs644UartNs550Inst;                                            //  CG  To 通信板全局单例

static volatile int TotalReceivedCount;
static volatile int TotalSentCount;
static volatile int TotalErrorCount;

/****************************************************************************
 *              初始化（外部调用使用） 
 * pUartNs550Inst:
 * usUartNs550DeviceId: device id
 * usUartNs550IntrId:
 * pCbHandler:
 ****************************************************************************/
int uartNs550Init(XIntc* pIntcInst, XUartNs550* pUartNs550Inst, unsigned short usUartNs550DeviceId, unsigned short usUartNs550IntrId, XUartNs550_Handler pCbHandler, XUartNs550Format* pDataFormat, unsigned int unOptions)
{
    int nRet = SEC_ERROR;

    // 参数检查
    if (pUartNs550Inst == NULL || pIntcInst == NULL)
    {
        return SEC_ERROR;
    }

    // 初始化UartLiteInst驱动程序
    nRet = uartNs550Initialize(pUartNs550Inst, usUartNs550DeviceId);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 执行自检以确保硬件构建正确     
    nRet = uartNs550SelfTest(pUartNs550Inst);
    if (nRet != XST_SUCCESS) 
    {
        return SEC_ERROR;
    }

    // 连接到中断子系统，以便中断可以发生
    nRet = uartNs550SetupInterruptSystem(pIntcInst, usUartNs550IntrId, pUartNs550Inst);
    if (nRet != XST_SUCCESS) 
    {
        return SEC_ERROR;
    }

    // 设置发送/接收数据回调函数接口
    nRet = uartNs550SetHandler(pUartNs550Inst, pCbHandler, pUartNs550Inst);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 设置数据格式
    nRet = uartNs550SetDataFormat(pUartNs550Inst, pDataFormat);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 使能FIFO和数据终端
    nRet = uartNs550SetBaudRate(pUartNs550Inst, unOptions);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 使能中断
    nRet = uartNs550EnableInterrupt(pIntcInst, usUartNs550IntrId);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  初始化
 * pUartNs550Inst:
 * usDeviceId:
 ****************************************************************************/
int uartNs550Initialize(XUartNs550* pUartNs550Inst, unsigned short usDeviceId)
{
    // 参数检查
    if (!pUartNs550Inst)
    {
        return SEC_ERROR;
    }

    // 实例对象初始化
    int nRet = XUartNs550_Initialize(pUartNs550Inst, usDeviceId);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  自检
 * pUartNs550Inst:
 ****************************************************************************/
int uartNs550SelfTest(XUartNs550* pUartNs550Inst)
{
    // 参数检查
    if (!pUartNs550Inst)
    {
        return SEC_ERROR;
    }

    // 自检
    int nRet = XUartNs550_SelfTest(pUartNs550Inst);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *              rs422 中断子系统
 * pIntcInst:
 * usUartNs550IntrId:
 * pCallBackRef:
 ****************************************************************************/
int uartNs550SetupInterruptSystem(XIntc* pIntcInst, unsigned short usUartNs550IntrId, void* pCallBackRef)
{
    // 参数检查
    if (pIntcInst == NULL)
    {
        return SEC_ERROR;
    }

    // 连接中断子系统
    int nRet = intcConnect(pIntcInst, usUartNs550IntrId, (XInterruptHandler)XUartNs550_InterruptHandler, (void *)pCallBackRef);
    if (nRet != XST_SUCCESS) 
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  设置中断响应函数
 * pUartNs550Inst:
 * pCbHandler:
 * pCallBackRef:
 ****************************************************************************/
int uartNs550SetHandler(XUartNs550* pUartNs550Inst, XUartNs550_Handler pCbHandler,void* pCallBackRef)
{
    // 参数检查
    if (!pUartNs550Inst && !pCbHandler)
    {
        return SEC_ERROR;
    }

    // 设置响应函数
    XUartNs550_SetHandler(pUartNs550Inst, pCbHandler, pCallBackRef);
    
    return SEC_SUCCESS;
}

/****************************************************************************
 *  使能中断
 * pUartNs550Inst:
 ****************************************************************************/
int uartNs550EnableInterrupt(XIntc* pIntcInst, unsigned short usUartNs550IntrId)
{
    // 参数检查
    if (!pIntcInst)
    {
        return SEC_ERROR;
    }

    // 使能中断
    intcEnable(pIntcInst, usUartNs550IntrId);
    
    return SEC_SUCCESS;
}

/****************************************************************************
 *  设置数据格式
 * pUartNs550Inst:
 * Format:
 ****************************************************************************/
int uartNs550SetDataFormat(XUartNs550* pUartNs550Inst, XUartNs550Format* pDataFormat)
{
    int nRet = SEC_ERROR;

    // 参数检查
    if (!pUartNs550Inst || !pDataFormat)
    {
        return SEC_ERROR;
    }

    // 设置数据格式
    nRet = XUartNs550_SetDataFormat(pUartNs550Inst, pDataFormat);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  设置波特率
 * pUartNs550Inst:
 * usOptions:
 ****************************************************************************/
int uartNs550SetBaudRate(XUartNs550* pUartNs550Inst, unsigned int unBaudRate)
{
    // 参数检查
    if (!pUartNs550Inst)
    {
        return SEC_ERROR;
    }

    // 自检
    int nRet = XUartNs550_SetBaudRate(pUartNs550Inst, unBaudRate);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  设置数据位
 * pUartNs550Inst:
 * usOptions:
 ****************************************************************************/
int uartNs550SetDataBits(XUartNs550* pUartNs550Inst, unsigned int unDataBits)
{
    // 参数检查
    if (!pUartNs550Inst)
    {
        return SEC_ERROR;
    }

    XUartNs550_SetLineControlReg(pUartNs550Inst->BaseAddress, unDataBits);

    return SEC_SUCCESS;
}

/****************************************************************************
 *  设置属性
 * pUartNs550Inst:
 * usOptions:
 ****************************************************************************/
int uartNs550SetOption(XUartNs550* pUartNs550Inst, unsigned short usOptions)
{
    // 参数检查
    if (!pUartNs550Inst)
    {
        return SEC_ERROR;
    }

    // 自检
    int nRet = XUartNs550_SetOptions(pUartNs550Inst, usOptions);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *              异步发送
 * pUartNs550Inst:
 * pDataBuffer:
 * unNumBytes:
 ****************************************************************************/
int uartNs550Send(XUartNs550* pUartNs550Inst, unsigned char* pDataBuffer, unsigned int unNumBytes)
{
    // 参数检查
    if (pUartNs550Inst == NULL || pDataBuffer == NULL)
    {
        return -1;
    }

    // 发送数据
    int unBytes = XUartNs550_Send(pUartNs550Inst, pDataBuffer, unNumBytes);
    return unBytes;
}

/****************************************************************************
 *              异步接收
 * pUartNs550Inst:
 * pDataBuffer:
 * unNumBytes:
 ****************************************************************************/
int uartNs550Recv(XUartNs550* pUartNs550Inst, unsigned char* pDataBuffer, unsigned int unNumBytes)
{
    // 参数检查
    if (pUartNs550Inst == NULL || pDataBuffer == NULL)
    {
        return -1;
    }

    // 接收数据
    int unBytes = XUartNs550_Recv(pUartNs550Inst, pDataBuffer, unNumBytes);
    return unBytes;
}

/****************************************************************************
 *              中断响应函数
 * pUartNs55pCbHandle0Inst:
 * unEvent:
 * unEventData:
 ****************************************************************************/
void UartNs550IntrHandler(void* pCallBackRef, unsigned int unEvent, unsigned int unEventData)
{
    // 参数检查
    if (!pCallBackRef)
    {
        return;
    }

    //XUartNs550* pUartNs550Inst = (XUartNs550 *)pCallBackRef;

    // 所有数据已经被发送
    if (unEvent == XUN_EVENT_SENT_DATA)
    {
        TotalSentCount = unEventData;
    }

    // 所有数据都被接收
    if (unEvent == XUN_EVENT_RECV_DATA)
    {
        TotalReceivedCount = unEventData;
    }

    // 部分数据被接收
    if (unEvent == XUN_EVENT_RECV_TIMEOUT)
    {
        TotalReceivedCount = unEventData;
    }

    // 数据接收出错
    if (unEvent == XUN_EVENT_RECV_ERROR)
    {
        TotalReceivedCount = unEventData;
        TotalErrorCount++;

        //unsigned char ucErrors = XUartNs550_GetLastErrors(pUartNs550Inst);
    }
}