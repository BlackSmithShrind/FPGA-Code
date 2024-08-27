/*@file     Gp_Edfa.c
* @brief    edfa模块源文件
* @details  针对通信板FPGA和软核MB之间使用axi总线寄存器+io+intc
*           软核MB与EDFA之间使用RS422接口，实现遥控指令、遥测查询指令发送和数据接收
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/07
*/
#include "Gp_Edfa.h"
#include "../../Bsp/UartLite/Gp_UartLite.h"
#include "../../Bsp/Intc/Gp_Intc.h"
#include "../../Bsp/Axi/Gp_Axi.h"
#include "../../Test/DebugUart/Gp_DebugUart.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
unsigned char gEdfaTxDataBuffer[EDFA_TX_DATA_BUFFER_SIZE] = "";
unsigned char gEdfaRxDataBuffer[EDFA_RX_DATA_BUFFER_SIZE] = "";
volatile unsigned short gEdfaInitIsReady = FALSE;                            // 是否已经完成初始化
volatile unsigned short gEdfaHeaderConstBytesIsRecv = FALSE;                 // 头部固定数据
volatile unsigned short gEdfaRemainRecvBytes = 0;                            // 剩余字节数
volatile unsigned short gEdfaIntcRxNewDataIsFin = FALSE;                     // 完成接收新数据

volatile unsigned int gEdfaTotalReceivedCount = 0;                           // 需要接收的字节数
volatile unsigned int gEdfaTotalSentCount = 0;                               // 需要发送的字节数

/****************************************************************************
 *              initialization                                        
 ****************************************************************************/
int edfaInit(void)
{
    // 初始化全局变量
    // 发送缓存
    memset(gEdfaTxDataBuffer, 0, EDFA_TX_DATA_BUFFER_SIZE);
    // 接收缓存
    memset(gEdfaRxDataBuffer, 0, EDFA_RX_DATA_BUFFER_SIZE);
    // 头部常数字节是否已经接收
    gEdfaHeaderConstBytesIsRecv = FALSE;
    // 还剩余的字节数
    gEdfaRemainRecvBytes = 0;
    // 完成接收新数据
    gEdfaIntcRxNewDataIsFin = FALSE;

    //初始化rs422串口
    uartLiteInit(&gIntcInst0, &gEdfaRs422UartLiteInst, GP_EDFA_UARTLITE_DEVICE_ID, GP_INTC_0_EDFA_UART0_INT_ID, edfaRs422SendHandler, edfaRs422RecvHandler);

    // 应为多个遥控遥测都走CAN指令通道，所以只能使用一个中断处理函数来响应，内部进行数据解析处理
    // 连接intc中断控制子系统 In12	CAN instruction
    //intcConnect(&gIntcInstance, GP_INTC_EDFA_INT_ID, edfaAxiIntcCanInstrHandler, (void*)0);

    // 是否已经完成初始化
    gEdfaInitIsReady = TRUE;

    return SEC_SUCCESS;
}

/****************************************************************************
 *              initialization self check                             
 ****************************************************************************/
int edfaInitChk(void)
{
    int nRet = SEC_ERROR;

    return nRet;
}

/****************************************************************************
 *              edfa主loop函数
 ****************************************************************************/
int edfaLoop()
{
#ifdef EDFA_DEBUG_UART_TEST
	// debug uart test
	edfaDebugUartTestProcess();
#else
    // 执行接收到的数据处理流程
    edfaPl2PsCanYkProcess();
#endif

    // 异步发送数据
    //edfaRs422SendProcess();

    // 异步接收数据
    edfaRs422RecvProcess();

    return SEC_SUCCESS;
}

/****************************************************************************
 *             光纤放大器EDFA执行机构->通信板PS->PFGA
 * RS422串口接收到EDFA执行机构返回的遥测信息
 ****************************************************************************/
int edfaRs422SendProcess(unsigned char* pBuffer, unsigned int unSize)
{
    int nRet = SEC_ERROR;

    // 参数检查
    if (!pBuffer || !unSize)
    {
        return nRet;
    }

    // 判断是否已经初始化
    if (!gEdfaInitIsReady)
    {
        return nRet;
    }

    // 异步发送数据
    uartLiteSend(&gEdfaRs422UartLiteInst, pBuffer, unSize);// 6:起始位-2 地址-2 指令-2 长度-2

    return nRet;
}

/****************************************************************************
 *             光纤放大器EDFA执行机构->通信板PS->PFGA
 * RS422串口接收到EDFA执行机构返回的遥测信息
 ****************************************************************************/
int edfaRs422RecvProcess()
{
    int nRet = SEC_ERROR;
    //char szLogBuffer[256] = {0};

    // 判断是否已经初始化
    if(!gEdfaInitIsReady)
    {
        return nRet;
    }

    // 异步接收头部8个字节数据
    if (!gEdfaHeaderConstBytesIsRecv)
    {
        uartLiteRecv(&gEdfaRs422UartLiteInst, gEdfaRxDataBuffer, EDFA_SF_HEADER_SIZE);// 6:起始位-2 地址-2 指令-2 长度-2

        gEdfaHeaderConstBytesIsRecv = TRUE;
    }
    
    // 检测是否有回调接收完毕
    if (gEdfaTotalReceivedCount == EDFA_SF_HEADER_SIZE)
    {
        unsigned short usStartFlag = *(unsigned short*)gEdfaRxDataBuffer;
        if (usStartFlag != 0xEDFA)
        {
            return nRet;
        }

        unsigned short usOffset = 2;
        unsigned short usAddrFlag = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);
        usOffset += 2;
        unsigned short usCtrlCmd = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);
        usOffset += 2;
        unsigned short usDataLen = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);
        // 写入相关FPGA寄存器
        //...
        //edfaEmifWriteRegister();

        gEdfaRemainRecvBytes = usDataLen + 1;
        uartLiteRecv(&gEdfaRs422UartLiteInst, gEdfaRxDataBuffer + EDFA_SF_HEADER_SIZE, gEdfaRemainRecvBytes);// 6:起始位-2 地址-2 指令-2 长度-2

        gEdfaTotalReceivedCount = 0;
    }

    // 检查剩余接收字节数
    if (gEdfaTotalReceivedCount == gEdfaRemainRecvBytes && gEdfaRemainRecvBytes != 0)
    {
        // 模块温度
        unsigned short usOffset = 8;
        unsigned short usModeTmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 模块输入电压
        usOffset += 2;
        unsigned short usModuleInputVol = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 工作模式及开关
        usOffset += 1;
        unsigned char ucWorkModeAndSwitch = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 输入功率
        usOffset += 2;
        unsigned short usInputPower = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 输出功率
        usOffset += 2;
        unsigned short usOutputPower = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 标校激光器电流
        usOffset += 2;
        unsigned short usCaliLaserAmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 标校激光器温度
        usOffset += 2;
        unsigned short usCaliLaserTmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 标校激光器功率
        usOffset += 2;
        unsigned short usCaliLaserPower = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 标校激光器TEC电流
        usOffset += 2;
        unsigned short usCaliLaserTecAmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 泵浦 1 电流
        usOffset += 2;
        unsigned short usPump1Amp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 泵浦 1 温度
        usOffset += 2;
        unsigned short usPump1Tmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 泵浦 1 背光电流
        usOffset += 2;
        unsigned short usPump1BacklightAmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 泵浦 1TEC 电流
        usOffset += 2;
        unsigned short usPump1TecAmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 泵浦 2 电流
        usOffset += 2;
        unsigned short usPump2Amp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 泵浦 2 温度
        usOffset += 2;
        unsigned short usPump2Tmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 泵浦 3 电流
        usOffset += 2;
        unsigned short usPump3Amp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 泵浦 3 温度
        usOffset += 2;
        unsigned short usPump3Tmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 模块告警状态
        usOffset += 1;
        unsigned char ucModeWarningState = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 功率告警状态
        usOffset += 1;
        unsigned char ucPowerWarningState = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 标校激光器告警状态
        usOffset += 1;
        unsigned char ucCaliLaserWarningState = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 泵浦 1 告警状态
        usOffset += 1;
        unsigned char ucPump1WarningState = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 泵浦 2 告警状态
        usOffset += 1;
        unsigned char ucPump2WarningState = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 泵浦 3 告警状态
        usOffset += 1;
        unsigned char ucPump3WarningState = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // 制造商名称
        usOffset += 1;
        unsigned char szManufacturerName[32] = { 0 };
        memcpy(szManufacturerName, (void*)(gEdfaRxDataBuffer + usOffset), 32);

        // 模块序列号
        usOffset += 32;
        unsigned char szModuleSerialNumber[32] = { 0 };
        memcpy(szModuleSerialNumber, (void*)(gEdfaRxDataBuffer + usOffset), 32);

        // 固件版本
        usOffset += 32;
        unsigned char szFirmwareVersion[32] = { 0 };
        memcpy(szFirmwareVersion, (void*)(gEdfaRxDataBuffer + usOffset), 32);

        // 模块生产日期
        usOffset += 32;
        unsigned char szModuleProductDate[32] = { 0 };
        memcpy(szModuleProductDate, (void*)(gEdfaRxDataBuffer + usOffset), 32);

        // 写入ddr
        //...


        gEdfaHeaderConstBytesIsRecv = FALSE;

        gEdfaTotalReceivedCount = 0;
        gEdfaRemainRecvBytes = 0;
    }

    return nRet;
}

/****************************************************************************
 *              edfa rs422 发送中断回调函数
 * pCallbackRef:
 * unSendBytes:
 ****************************************************************************/
void edfaRs422SendHandler(void* pCallbackRef, unsigned int unSendBytes)
{
    // 发送完毕
    if (gEdfaTotalSentCount == unSendBytes)
    {
        gEdfaTotalSentCount = 0;
    }
}

/****************************************************************************
 *              edfa rs422 接收中断回调函数
 * pCallbackRef:
 * unRecvBytes:
 ****************************************************************************/
void edfaRs422RecvHandler(void* pCallbackRef, unsigned int unRecvBytes)
{
    // 如果是6个字节则表示头部固定位数的数据抓取
    gEdfaTotalReceivedCount = unRecvBytes;
}

/****************************************************************************
 *              edfa intc 响应函数（CAN instruction）
 * 卫星平台 -can-> 通信板PL -AXI-INTC AXI-REG->通信板PS
 * pCallbackRef:
 * 注意：先放这里后面统一调整
 ****************************************************************************/
void edfaAxiIntcCanInstrHandler(void* pCallbackRef)
{
    unsigned char ucDataType = *(volatile unsigned char  *)GP_AIX_CAN_BUS_YKYC_SINGLE_DATA_TYPE_REG_ADDR;
    switch (ucDataType)
    {
        case 0x00: // 遥测：0x00单帧
        {
            
        }break;
        case 0x0A: // 遥控：0x0A单帧
        {
            if (!gEdfaIntcRxNewDataIsFin)
            {
                // 数据copy出来
                memset(gEdfaRxDataBuffer, 0, EDFA_RX_DATA_BUFFER_SIZE);
                memcpy((void*)gEdfaRxDataBuffer, (void*)GP_AIX_CAN_BUS_YKYC_SINGLE_DATA_TYPE_REG_ADDR, 8);

                // 设置标识
                gEdfaIntcRxNewDataIsFin = TRUE;
            }
        }break;
        case 0x3A: // 遥测：0x3A复合帧
        {

        }break;
        case 0x0F: // 遥控：0x0F复帧
        {

        }break;
        default:
        {
        }break;
    }
}

/****************************************************************************
 *  卫星平台 - CAN YK -> 通信板PL CAN - AXI-REG -> 通信板PS - RS422-> EDFA
 * RS422串口发送遥控遥测指令到EDFA执行机构
 ****************************************************************************/
int edfaPl2PsCanYkProcess()
{
    int nRet = SEC_ERROR;
    //char szLogBuffer[256] = { 0 };

    // 判断是否已经初始化
    if (!gEdfaInitIsReady)
    {
        return nRet;
    }

    // 有新的数据要处理
    if (!gEdfaIntcRxNewDataIsFin)
    {
        return nRet;
    }

    // 检测是否有回调接收完毕
    unsigned char usOffset = 1;
    unsigned char ucDataType = *(volatile unsigned char  *)gEdfaRxDataBuffer;
    unsigned char ucDataNum = *(volatile unsigned char  *)(gEdfaRxDataBuffer + usOffset++);
    unsigned char ucFrameNum = *(volatile unsigned char  *)(gEdfaRxDataBuffer + usOffset++);
    unsigned char ucCmdContent1 = *(volatile unsigned char  *)(gEdfaRxDataBuffer + usOffset++);
    unsigned char ucCmdContent2 = *(volatile unsigned char  *)(gEdfaRxDataBuffer + usOffset++);
    unsigned char ucCmdContent3 = *(volatile unsigned char  *)(gEdfaRxDataBuffer + usOffset++);
    unsigned char ucCmdContent4 = *(volatile unsigned char  *)(gEdfaRxDataBuffer + usOffset++);
    unsigned char ucCheckSum = *(volatile unsigned char  *)(gEdfaRxDataBuffer + usOffset++);
    switch (ucCmdContent1)
    {
        case 0x6F: // 标校控制
        {
            // 构造EDFA RS422协议
            stuEdfaYkFrameInfo* pInfo = (stuEdfaYkFrameInfo *)gEdfaTxDataBuffer;
            gEdfaTotalSentCount = sizeof(stuEdfaYkFrameInfo);
            memset(pInfo, 0, gEdfaTotalSentCount);
            pInfo->usStartFlag = 0x55AA;
            pInfo->usAddrFlag = 0xFFFF;
            pInfo->usCtrlCmd = EDFA_CTRL_CMD_SET_CHANNEL_SWITCH;
            pInfo->pData[0] = 0;
            pInfo->pData[1] = 1;
            pInfo->ucXor = edfaCalcCheckXor((volatile unsigned char *)pInfo, gEdfaTotalSentCount - 1);

            // 发送数据
            nRet = edfaRs422SendProcess(gEdfaTxDataBuffer, gEdfaTotalSentCount);

        }break;
        default:
            break;
    }
    return nRet;
}

/****************************************************************************
 *  卫星平台 - CAN YK -> 通信板PL CAN - AXI-REG -> 通信板PS - RS422-> EDFA
 * RS422串口发送遥控遥测指令到EDFA执行机构
 ****************************************************************************/
int edfaDebugUartTestProcess()
{
    int nRet = SEC_ERROR;
    //char szLogBuffer[256] = { 0 };

    // 判断是否已经初始化
    if (!gEdfaInitIsReady)
    {
        return nRet;
    }

    // 有新的数据要处理
    if (!gEdfaIntcRxNewDataIsFin)
    {
        return nRet;
    }

    // 检测是否有回调接收完毕
    unsigned short usOffset = 0;
    unsigned short usStartFlag = *(volatile unsigned short *)gEdfaRxDataBuffer;
    usOffset += 2;
    unsigned short usConstFlag = *(volatile unsigned short *)(gEdfaRxDataBuffer + usOffset);
    usOffset += 2;
    unsigned short usCmd = *(volatile unsigned short *)(gEdfaRxDataBuffer + usOffset);
    usOffset += 2;
    unsigned short usDataLen = *(volatile unsigned short *)(gEdfaRxDataBuffer + usOffset);
    usOffset += 2 + usDataLen;
    unsigned char ucCheckSum = *(volatile unsigned char *)(gEdfaRxDataBuffer + usOffset);
    switch (usCmd)
    {
        case EDFA_CTRL_CMD_SET_CHANNEL_SWITCH: // 标校控制
        {
            // 构造EDFA RS422协议
        	gEdfaTotalSentCount = 11;
            stuEdfaYkFrameInfo* pInfo = (stuEdfaYkFrameInfo *)gEdfaTxDataBuffer;
            memset(pInfo, 0, gEdfaTotalSentCount);
            pInfo->usStartFlag = 0x55AA;
            pInfo->usAddrFlag = 0xFFFF;
            pInfo->usCtrlCmd = EDFA_CTRL_CMD_SET_CHANNEL_SWITCH;
            pInfo->usDataLen = usDataLen;
            pInfo->pData[0] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 8);
            pInfo->pData[1] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 9);
            pInfo->ucXor = edfaCalcCheckXor((volatile unsigned char *)pInfo, gEdfaTotalSentCount - 1);
            gEdfaTxDataBuffer[gEdfaTotalSentCount - 1] = pInfo->ucXor;

            // 发送数据
            nRet = edfaRs422SendProcess(gEdfaTxDataBuffer, gEdfaTotalSentCount);
        }break;
        case EDFA_CTRL_CMD_SET_WORK_MODE: // 设置工作模式
        {
            // 构造EDFA RS422协议
        	gEdfaTotalSentCount = 11;
            stuEdfaYkFrameInfo* pInfo = (stuEdfaYkFrameInfo *)gEdfaTxDataBuffer;
            memset(pInfo, 0, gEdfaTotalSentCount);
            pInfo->usStartFlag = 0x55AA;
            pInfo->usAddrFlag = 0xFFFF;
            pInfo->usCtrlCmd = EDFA_CTRL_CMD_SET_WORK_MODE;
            pInfo->usDataLen = usDataLen;
            pInfo->pData[0] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 8);
            pInfo->pData[1] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 9);
            pInfo->ucXor = edfaCalcCheckXor((volatile unsigned char *)pInfo, gEdfaTotalSentCount - 1);
            gEdfaTxDataBuffer[gEdfaTotalSentCount - 1] = pInfo->ucXor;

            // 发送数据
            nRet = edfaRs422SendProcess(gEdfaTxDataBuffer, gEdfaTotalSentCount);
        }break;
        case EDFA_CTRL_CMD_SET_PUMP_AMP: // 设置泵浦电流
        {
            // 构造EDFA RS422协议
        	gEdfaTotalSentCount = 12;
            stuEdfaYkFrameInfo* pInfo = (stuEdfaYkFrameInfo *)gEdfaTxDataBuffer;
            memset(pInfo, 0, gEdfaTotalSentCount);
            pInfo->usStartFlag = 0x55AA;
            pInfo->usAddrFlag = 0xFFFF;
            pInfo->usCtrlCmd = EDFA_CTRL_CMD_SET_PUMP_AMP;
            pInfo->usDataLen = usDataLen;
            pInfo->pData[0] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 8);
            pInfo->pData[1] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 9);
            pInfo->pData[2] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 10);
            pInfo->ucXor = edfaCalcCheckXor((volatile unsigned char *)pInfo, gEdfaTotalSentCount - 1);
            gEdfaTxDataBuffer[gEdfaTotalSentCount - 1] = pInfo->ucXor;

            // 发送数据
            nRet = edfaRs422SendProcess(gEdfaTxDataBuffer, gEdfaTotalSentCount);
        }break;
        case EDFA_CTRL_CMD_SET_OUTPUT_OPT_POWER: // 设置输出光功率
        {
            // 构造EDFA RS422协议
        	gEdfaTotalSentCount = 12;
            stuEdfaYkFrameInfo* pInfo = (stuEdfaYkFrameInfo *)gEdfaTxDataBuffer;
            memset(pInfo, 0, gEdfaTotalSentCount);
            pInfo->usStartFlag = 0x55AA;
            pInfo->usAddrFlag = 0xFFFF;
            pInfo->usCtrlCmd = EDFA_CTRL_CMD_SET_OUTPUT_OPT_POWER;
            pInfo->usDataLen = usDataLen;
            pInfo->pData[0] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 8);
            pInfo->pData[1] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 9);
            pInfo->pData[2] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 10);
            pInfo->ucXor = edfaCalcCheckXor((volatile unsigned char *)pInfo, gEdfaTotalSentCount - 1);
            gEdfaTxDataBuffer[gEdfaTotalSentCount - 1] = pInfo->ucXor;

            // 发送数据
            nRet = edfaRs422SendProcess(gEdfaTxDataBuffer, gEdfaTotalSentCount);
        }break;
        default:
        {

        }break;
    }

    // 重置标识
    gEdfaIntcRxNewDataIsFin = FALSE;

    return nRet;
}

/****************************************************************************
 *                          和校验累加和
 * 校验位： 1 字节， 从起始位开始至数据最后一位截止，所有字节进行异或运算后的值
 ****************************************************************************/
unsigned char edfaCalcCheckXor(volatile unsigned char* pBuff, unsigned short usLength)
{
    unsigned char ucCheckSum = 0;

    while (usLength--)
    {
        ucCheckSum ^= *pBuff++;
    }

    return ucCheckSum;
}


