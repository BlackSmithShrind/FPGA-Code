/*@file     Gp_Xm.cpp
* @brief    星敏感器外部机构源文件
* @details  通过该文件实现对星敏感器的遥控遥测指令
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/13
*/
#include "Gp_Xm.h"
#include "../../Bsp/UartLite/Gp_UartLite.h"
#include "../../Bsp/Intc/Gp_Intc.h"
#include "../../Bsp/Axi/Gp_Axi.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
unsigned char gXmTxDataBuffer[XM_TX_DATA_BUFFER_SIZE] = "";
unsigned char gXmRxDataBuffer[XM_RX_DATA_BUFFER_SIZE] = "";
volatile unsigned short gXmInitIsReady = FALSE;                            // 是否已经完成初始化
volatile unsigned short gXmHeaderConstBytesIsRecv = FALSE;                 // 头部固定数据
volatile unsigned short gXmRemainRecvBytes = 0;                            // 剩余字节数
volatile unsigned short gXmIntcRxNewDataIsFin = FALSE;                     // 完成接收新数据

volatile unsigned int gXmTotalReceivedCount = 0;                           // 接收的字节数
volatile unsigned int gXmTotalSentCount = 0;                               // 发送的字节数

/****************************************************************************
 *              initialization
 ****************************************************************************/
int xmInit(void)
{
    int nRet = SEC_ERROR;

    // 初始化全局变量
    // 发送缓存
    memset(gXmTxDataBuffer, 0, XM_TX_DATA_BUFFER_SIZE);
    // 接收缓存
    memset(gXmRxDataBuffer, 0, XM_RX_DATA_BUFFER_SIZE);
    // 头部常数字节是否已经接收
    gXmHeaderConstBytesIsRecv = FALSE;
    // 还剩余的字节数
    gXmRemainRecvBytes = 0;
    // 完成接收新数据
    gXmIntcRxNewDataIsFin = FALSE;

    //初始化rs422串口
    nRet = uartLiteInit(&gIntcInst0, &gXmRs422UartLiteInst, GP_XM_UARTLITE_DEVICE_ID, GP_INTC_0_XM_UART1_INT_ID, xmRs422SendHandler, xmRs422RecvHandler);
    if (nRet != SEC_SUCCESS)
    {
        return nRet;
    }

    // 浣胯兘璁惧涓柇浠ヤ究鍙互鍑哄彂涓柇璋冪敤涓柇澶勭悊绋嬪簭
	nRet = intcEnable(&gIntcInst0, GP_INTC_0_XM_UART1_INT_ID);
	if (nRet != SEC_SUCCESS)
	{
		return nRet;
	}

    // 是否已经完成初始化
    gXmInitIsReady = TRUE;

    return SEC_SUCCESS;
}

/****************************************************************************
 *              initialization self check
 ****************************************************************************/
int xmInitChk(void)
{
    int nRet = SEC_ERROR;

    return nRet;
}

/****************************************************************************
 *              xm主loop函数
 ****************************************************************************/
int xmLoop()
{
    // 执行接收到的数据处理流程
    xmAxiIntcCanInstrYkProcess();

    // 异步发送数据
    //xmRs422SendProcess();

    // 异步接收数据
    xmRs422RecvProcess();

    return SEC_SUCCESS;
}

/****************************************************************************
 *             光纤放大器XM执行机构->通信板PS->PFGA
 * RS422串口接收到XM执行机构返回的遥测信息
 ****************************************************************************/
int xmRs422SendProcess(unsigned char* pBuffer, unsigned int unSize)
{
    int nRet = SEC_ERROR;

    // 参数检查
    if (!pBuffer || !unSize)
    {
        return nRet;
    }

    // 判断是否已经初始化
    if (!gXmInitIsReady)
    {
        return nRet;
    }

    // 异步发送数据
    nRet = uartLiteSend(&gXmRs422UartLiteInst, pBuffer, unSize);

    return nRet;
}

/****************************************************************************
 *             光纤放大器XM执行机构->通信板PS->PFGA
 * RS422串口接收到XM执行机构返回的遥测信息
 ****************************************************************************/
int xmRs422RecvProcess()
{
    int nRet = SEC_ERROR;
    //char szLogBuffer[256] = { 0 };

    // 判断是否已经初始化
    if (!gXmInitIsReady)
    {
        return nRet;
    }

    // 异步接收头部6个字节数据
    if (!gXmHeaderConstBytesIsRecv)
    {
        uartLiteRecv(&gXmRs422UartLiteInst, gXmRxDataBuffer, XM_SF_HEADER_SIZE);// 2字节:起始位-2 地址-2 指令-2 长度-2

        gXmHeaderConstBytesIsRecv = TRUE;
    }

    // 检测是否有回调接收完毕
    if (gXmTotalReceivedCount == XM_SF_HEADER_SIZE)
    {
        unsigned char ucHeaderFlag1 = *(unsigned char*)gXmRxDataBuffer;
        if (ucHeaderFlag1 != 0xEB)
        {
            return nRet;
        }

        unsigned char ucOffset = 1;
        unsigned char ucHeaderFlag2 = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
        if (ucHeaderFlag2 == XM_SF_YK_CMD_AND_ACK)
        {
            gXmRemainRecvBytes = XM_YK_ACK_REMAIN_RECV_SIZE;
        }
        else if (ucHeaderFlag2 == XM_SF_YC_CMD_AND_ACK)
        {
            gXmRemainRecvBytes = XM_YC_ACK_REMAIN_RECV_SIZE;
        }
        else if (ucHeaderFlag2 == XM_SF_SELF_TEST_DOWN_TX_AND_ACK)
        {
            gXmRemainRecvBytes = XM_SELF_TEST_ACK_REMAIN_RECV_SIZE;
        }
        else
        {
            return nRet;
        }

        uartLiteRecv(&gXmRs422UartLiteInst, gXmRxDataBuffer + XM_SF_HEADER_SIZE, gXmRemainRecvBytes);// 6:起始位-2 地址-2 指令-2 长度-2

        gXmTotalReceivedCount = 0;
    }

    // 检查剩余接收字节数
    if (gXmTotalReceivedCount == gXmRemainRecvBytes && gXmRemainRecvBytes != 0)
    {
        unsigned char ucOffset = 1;
        unsigned char ucHeaderFlag2 = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
        if (ucHeaderFlag2 == XM_SF_YK_CMD_AND_ACK)
        {
            unsigned char ucRetAck = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucCntAA = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucSum = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            // 检查校验和
            unsigned char ucCalcSum = xmCalcCheckXor(gXmRxDataBuffer + XM_SF_HEADER_SIZE, XM_YK_ACK_REMAIN_RECV_SIZE - 1);
            if (ucSum != ucCalcSum)
            {
                return nRet;
            }

            // 检查固定参数
            if (ucCntAA != XM_YK_ACK_CONST_FALG_0xAA)
            {
                return nRet;
            }

            // 处理结果
            if (ucRetAck == XM_YK_RET_ACK_SUC) // 正确
            {
                // 写入ddr，等待遥控返回？
            }
            else if (ucRetAck == XM_YK_RET_ACK_ERR) // 错误
            {
                // 写入ddr，等待遥控返回？
            }
            else
            {
                return nRet;
            }
        }
        else if (ucHeaderFlag2 == XM_SF_YC_CMD_AND_ACK)
        {
            unsigned char ucAckCnt22 = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned int unQ1 = *(unsigned int*)(gXmRxDataBuffer + ucOffset);   // 姿态四元数 星敏感器坐标系相对于J2000惯性坐标系）负数用补码表示。每个四元数占四个字节，高字节在前，数据解析式为：f(x)=x/2147483647  1~q3:矢量 q4:标量，标量为正
            ucOffset += 4;
            unsigned int unQ2 = *(unsigned int*)(gXmRxDataBuffer + ucOffset);
            ucOffset += 4;
            unsigned int unQ3 = *(unsigned int*)(gXmRxDataBuffer + ucOffset);
            ucOffset += 4;
            unsigned int unQ4 = *(unsigned int*)(gXmRxDataBuffer + ucOffset);
            ucOffset += 4;
            unsigned int unUtcTimestampSec = *(unsigned int*)(gXmRxDataBuffer + ucOffset);
            ucOffset += 4;
            unsigned int unUtcTimestampDec = *(unsigned int*)(gXmRxDataBuffer + ucOffset) >> 8;
            ucOffset += 3;
            //unsigned char ucUtcTimestampDec1 = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            //unsigned char ucUtcTimestampDec2 = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            //unsigned char ucUtcTimestampDec3 = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);

            unsigned char ucTemp = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucImgExposureVal = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucImgThreshold = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucBackgroundVal= *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucInternalParam1 = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucInternalParam2 = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucInternalParam3 = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucInternalParam4 = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucDeviceNum = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucEdacErrCount = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned int ucImgFrameNum = *(unsigned int*)(gXmRxDataBuffer + ucOffset) >> 8;
            ucOffset += 3;
            unsigned char ucInternalParam5 = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char uc4StarSearchThreshold = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucTrackThreshold = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned short usXDirAngularV = *(unsigned short*)(gXmRxDataBuffer + ucOffset);
            ucOffset += 2;
            unsigned short usYDirAngularV = *(unsigned short*)(gXmRxDataBuffer + ucOffset);
            ucOffset += 2;
            unsigned short usZDirAngularV = *(unsigned short*)(gXmRxDataBuffer + ucOffset);
            ucOffset += 2;
            unsigned char ucInternalParam6 = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucTelemetryCmdCount = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucControlCmdCorrectCount = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucControlCmdErrCount = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucAckSum = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);

            // 检查校验和
            unsigned char ucSum = xmCalcCheckXor(gXmRxDataBuffer + XM_SF_HEADER_SIZE, XM_YC_ACK_REMAIN_RECV_SIZE - 1);
            if (ucSum != ucAckSum)
            {
                return nRet;
            }

            // 检查固定参数
            if (ucAckCnt22 != XM_YC_ACK_CONST_FALG_0x22)
            {
                return nRet;
            }

            // 处理结果
            // 写入ddr，等待遥控返回？
        }
        else if (ucHeaderFlag2 == XM_SF_SELF_TEST_DOWN_TX_AND_ACK)
        {
            unsigned char ucAckCnt0F = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucDataBlockSTFlag1 = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);     // 数据块自检标志
            unsigned char ucDataBlockSTFlag2 = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucDataBlockSTFlag3 = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);

            // 非0自检结构报错
            if (ucDataBlockSTFlag1 == 0 
                || ucDataBlockSTFlag2 == 0
                || ucDataBlockSTFlag3 == 0)
            {
               //...
            }

            unsigned char ucBootVersion = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucArmVersion = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned char ucFpgaVersion = *(unsigned char*)(gXmRxDataBuffer + ucOffset++);
            unsigned short usDeviceNum = *(unsigned short*)(gXmRxDataBuffer + ucOffset);
            ucOffset += 2;
            unsigned short ucDateTime= *(unsigned short*)(gXmRxDataBuffer + ucOffset);
            ucOffset += 2;
            unsigned short ucProNum = *(unsigned short*)(gXmRxDataBuffer + ucOffset);
            ucOffset += 2;
            unsigned char ucNN = *(unsigned int*)(gXmRxDataBuffer + ucOffset++);            
            unsigned char ucAckSum = *(unsigned int*)(gXmRxDataBuffer + ucOffset++);

            // 检查校验和
            unsigned char ucSum = xmCalcCheckXor(gXmRxDataBuffer + XM_SF_HEADER_SIZE, XM_SELF_TEST_ACK_REMAIN_RECV_SIZE - 1);
            if (ucSum != ucAckSum)
            {
                return nRet;
            }

            // 检查固定参数
            if (ucAckCnt0F != XM_STDT_ACK_CONST_FALG_0x0F)
            {
                return nRet;
            }

            // 处理结果
            // 写入ddr，等待遥控返回？
        }
        else
        {
            return nRet;
        }

        gXmHeaderConstBytesIsRecv = FALSE;

        gXmTotalReceivedCount = 0;
        gXmRemainRecvBytes = 0;
    }

    return nRet;
}

/****************************************************************************
 *  卫星平台 - CAN YK -> 通信板PL CAN - AXI-REG -> 通信板PS - RS422-> XM
 * RS422串口发送遥控遥测指令到XM执行机构
 ****************************************************************************/
int xmAxiIntcCanInstrYkProcess()
{
    int nRet = SEC_ERROR;
    //char szLogBuffer[256] = { 0 };

    // 判断是否已经初始化
    if (!gXmInitIsReady)
    {
        return nRet;
    }

    // 有新的数据要处理
    if (!gXmIntcRxNewDataIsFin)
    {
        return nRet;
    }

    // 检测是否有回调接收完毕
    unsigned char ucOffset = 1;
    unsigned char ucHeader1 = *(volatile unsigned char  *)gXmRxDataBuffer;
    unsigned char ucHeader2 = *(volatile unsigned char  *)(gXmRxDataBuffer + ucOffset++);
    unsigned char ucFrameFormat1 = *(volatile unsigned char  *)(gXmRxDataBuffer + ucOffset++);
    unsigned char ucFrameFormat2 = *(volatile unsigned char  *)(gXmRxDataBuffer + ucOffset++);
    unsigned char ucFrameFormat3 = *(volatile unsigned char  *)(gXmRxDataBuffer + ucOffset++);
    unsigned char ucFrameFormat4 = *(volatile unsigned char  *)(gXmRxDataBuffer + ucOffset++);
    unsigned char ucFrameFormat5 = *(volatile unsigned char  *)(gXmRxDataBuffer + ucOffset++);
    unsigned char ucFrameFormat6 = *(volatile unsigned char  *)(gXmRxDataBuffer + ucOffset++);
    switch (ucHeader1)
    {
        case XM_SF_HEADER1:
        {
            if (ucHeader2 == XM_SF_YK_CMD_AND_ACK)
            {
                // 构造XM RS422协议
                stuXmYkFramePkg* pInfo = (stuXmYkFramePkg *)gXmTxDataBuffer;
                memset(pInfo, 0, sizeof(stuXmYkFramePkg));
                pInfo->ucHeaderFlag1 = ucHeader1;
                pInfo->ucHeaderFlag2 = ucHeader2;

                if (ucFrameFormat1 == XM_CTRL_CMD_SYSTEM_RESET
                    || ucFrameFormat1 == XM_CTRL_CMD_SET_SELF_TEST)
                {
                    pInfo->ucFrameFormat1 = ucFrameFormat1;
                    pInfo->ucFrameFormat2 = xmCalcCheckXor((volatile unsigned char *)(gXmRxDataBuffer + XM_SF_HEADER_SIZE), 1);
                    gXmTotalSentCount = 4;
                }
                else if (ucFrameFormat1 == XM_CTRL_CMD_SET_WORK_MODE
                    || ucFrameFormat1 == XM_CTRL_CMD_SET_EXPOSURE_TIME
                    || ucFrameFormat1 == XM_CTRL_CMD_SET_GAIN
                    || ucFrameFormat1 == XM_CTRL_CMD_SAA_MODE_SWITCH
                    || ucFrameFormat1 == XM_CTRL_CMD_SWITCH_H_L_DYNAMIC_MODE
                    || ucFrameFormat1 == XM_CTRL_CMD_SELECT_IMG_START)
                {
                    pInfo->ucFrameFormat1 = ucFrameFormat1;
                    pInfo->ucFrameFormat2 = ucFrameFormat2;
                    pInfo->ucFrameFormat3 = xmCalcCheckXor((volatile unsigned char *)(gXmRxDataBuffer + XM_SF_HEADER_SIZE), 2);
                    gXmTotalSentCount = 5;
                }
                else if (ucFrameFormat1 == XM_CTRL_CMD_ENTER_BOOT_INSTR
                    || ucFrameFormat1 == XM_CTRL_CMD_SEC_INT_CALI)
                {
                    pInfo->ucFrameFormat1 = ucFrameFormat1;
                    pInfo->ucFrameFormat2 = ucFrameFormat2;
                    pInfo->ucFrameFormat3 = ucFrameFormat3;
                    pInfo->ucFrameFormat4 = ucFrameFormat4;
                    pInfo->ucFrameFormat5 = ucFrameFormat5;
                    pInfo->ucFrameFormat6 = xmCalcCheckXor((volatile unsigned char *)(gXmRxDataBuffer + XM_SF_HEADER_SIZE), 5);
                    gXmTotalSentCount = 8;
                }
                else
                {
                    return nRet;
                }
            }
            else if (ucHeader2 == XM_SF_YC_CMD_AND_ACK)
            {
                // 构造XM RS422协议
                stuXmYcFramePkg* pInfo = (stuXmYcFramePkg *)gXmTxDataBuffer;
                memset(pInfo, 0, sizeof(stuXmYcFramePkg));
                pInfo->ucHeaderFlag1 = ucHeader1;
                pInfo->ucHeaderFlag2 = ucHeader2;
                pInfo->ucFrameFormat1 = 0x22;
                pInfo->ucFrameFormat2 = 0x01;
                pInfo->ucFrameFormat3 = xmCalcCheckXor((volatile unsigned char *)(gXmRxDataBuffer + XM_SF_HEADER_SIZE), 2);
                gXmTotalSentCount = 5;
            }
            else if (ucHeader2 == XM_SF_SELF_TEST_DOWN_TX_AND_ACK)
            {
                // 构造XM RS422协议
                stuXmSelfTestFramePkg* pInfo = (stuXmSelfTestFramePkg *)gXmTxDataBuffer;
                memset(pInfo, 0, sizeof(stuXmSelfTestFramePkg));
                pInfo->ucHeaderFlag1 = ucHeader1;
                pInfo->ucHeaderFlag2 = ucHeader2;
                pInfo->ucFrameFormat1 = 0x22;
                pInfo->ucFrameFormat2 = 0x01;
                pInfo->ucFrameFormat3 = xmCalcCheckXor((volatile unsigned char *)(gXmRxDataBuffer + XM_SF_HEADER_SIZE), 2);
                gXmTotalSentCount = 5;
            }
            else if (ucHeader2 == XM_SF_PROGRAM_UPLOAD_AND_ACK)
            {
                // 注意：确认好需求在对接
                return nRet;
            }
            else
            {
                return nRet;
            }

            // 发送数据
            nRet = xmRs422SendProcess(gXmTxDataBuffer, gXmTotalSentCount);

        }break;
        default:
        {
            return nRet;
        }break;
    }

    return nRet;
}

/****************************************************************************
 *                          和校验累加和
 * 从起始位开始至数据最后一位截止，所有字节进行异或运算后的值
 ****************************************************************************/
unsigned char xmCalcCheckXor(volatile unsigned char *pBuff, unsigned short usLength)
{
    unsigned char ucCheckSum = 0;

    while (usLength--)
    {
        ucCheckSum ^= *pBuff++;
    }

    return ucCheckSum;
}

/****************************************************************************
 *              xm rs422 发送中断回调函数
 * pCallbackRef:
 * unSendBytes:
 ****************************************************************************/
void xmRs422SendHandler(void* pCallbackRef, unsigned int unSendBytes)
{
    // 发送完毕
    if (gXmTotalSentCount == unSendBytes)
    {
        gXmTotalSentCount = 0;
    }
}

/****************************************************************************
 *              xm rs422 接收中断回调函数
 * pCallbackRef:
 * unRecvBytes:
 ****************************************************************************/
void xmRs422RecvHandler(void* pCallbackRef, unsigned int unRecvBytes)
{
    // 如果是6个字节则表示头部固定位数的数据抓取
    gXmTotalReceivedCount = unRecvBytes;
}
