/*@file     Gp_Adp.c
* @brief    软核apd驱动交互单元
* @details  添加apd接口相关处理逻辑
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/21
*/
#include "Gp_Apd.h"
#include "../../Bsp/Axi/Gp_Axi.h"
#include "../../Bsp/UartNs550/Gp_UartNs550.h"
#include "../../Bsp/Bram/Gp_Bram.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
unsigned char gApdEmlBoardTempVolDetTxDataBuffer[APD_TX_DATA_BUFFER_SIZE] = "";     // 存储一下数据:
                                                                                    // APD：光功率接收、温度、电流、电压;
                                                                                    // EML：温度、电流;
                                                                                    // 板温、电压检测：板温、5v电压 12v电压;
unsigned char gApdRxDataBuffer[APD_RX_DATA_BUFFER_SIZE] = "";
unsigned char gApdPowerFrameBuffer[APD_POWER_FRAME_BUFFER_SIZE] = "";               // 用于存储apd光功率数据帧缓存
volatile unsigned short gApdInitIsReady = FALSE;                                    // 是否已经完成初始化
volatile unsigned short gApdNewReqPkgIsArrived = FALSE;                             // 从中断响应中接收新的一轮数据
volatile unsigned int gApdTotalReceivedCount = 0;                                   // 接收的字节数
volatile unsigned int gApdTotalSentCount = 0;                                       // 发送的字节数
volatile unsigned int gApdEmlBoardTempVolDetTimestemp = 0;                          // 读取时间戳

/****************************************************************************
 *              初始化（对外接口）
 ****************************************************************************/
int apdInit(void)
{
    int nRet = SEC_ERROR;

    // 初始化全局变量
    // 发送缓存
    memset(gApdEmlBoardTempVolDetTxDataBuffer, 0, APD_TX_DATA_BUFFER_SIZE);
    // 接收缓存
    memset(gApdRxDataBuffer, 0, APD_RX_DATA_BUFFER_SIZE);
    // 用于存储apd光功率数据帧缓存
    memset(gApdPowerFrameBuffer, 0, APD_POWER_FRAME_BUFFER_SIZE);
    // 从中断响应中接收新的一轮数据
    gApdNewReqPkgIsArrived = FALSE;

    // 连接intc17中断控制子系统处理apd的req和ack数据包
    nRet = intcConnect(&gIntcInst0, GP_INTC_17_INTERNAL_LVDS_BRAM1_INT_ID, intcInternalLvdsBram1InstrHandler, (void*)0);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 是否已经完成初始化
    gApdInitIsReady = TRUE;

    return SEC_SUCCESS;
}

/****************************************************************************
 *              apd主loop函数
 ****************************************************************************/
int apdLoop()
{
    // 状态检测
    if (!gApdInitIsReady)
    {
        return SEC_ERROR;
    }

    // 从相关寄存器中读取数据
    apdEmlBoardDataProcess();

    // 执行emif接收到的数据处理流程
    apdIntcInterLvdsBram1Process();

    return SEC_SUCCESS;
}

/****************************************************************************
 *              周期性去读取apd\eml\borad先关数据信息
 * 1.ps侧周期性的去读取寄存器数据
 * 2.将读取到的遥测数据写入ddr
 * 3.pl和ps不做任何握手动作
 ****************************************************************************/
int apdEmlBoardDataProcess()
{
    // 状态检测
    if (!gApdInitIsReady)
    {
        return SEC_ERROR;
    }

    int nTimeout = 0;
    if (nTimeout)
    {
        stuAdpEmlBoardTempVolDet* pInfo = (stuAdpEmlBoardTempVolDet*)gApdEmlBoardTempVolDetTxDataBuffer;
        memset(gApdEmlBoardTempVolDetTxDataBuffer, 0, APD_TX_DATA_BUFFER_SIZE);
        // 板温、电压检测
        // 板温
        pInfo->nBoardTemp = *(int*)(GP_AIX_BOARD_TEMP_VOL_DET_REG_ADDR);
        // 电压5v
        pInfo->n5Vol = *(int*)(GP_AIX_VOL_5V_REG_ADDR);
        // 电压12v
        pInfo->n12Vol = *(int*)(GP_AIX_VOL_12V_REG_ADDR);

        // apd
        // 光功率接收
        pInfo->nApdOpticalPowerRx = *(int*)(GP_AIX_APD_OPTICAL_POWER_RX_REG_ADDR);
        // 温度采集
        pInfo->nApdTempCol = *(int*)(GP_AIX_APD_TEMP_COL_REG_ADDR);
        // 电流采集
        pInfo->nApdCurCol = *(int*)(GP_AIX_APD_CUR_COL_REG_ADDR);
        // 电流采集
        pInfo->nApdVolCol = *(int*)(GP_AIX_APD_VOL_COL_REG_ADDR);

        // eml
        // 电流采集
        pInfo->nEmlCurCol = *(int*)(GP_AIX_EML_CUR_COL_REG_ADDR);
        // 电压采集
        pInfo->nEmlVolCol = *(int*)(GP_AIX_EML_VOL_COL_REG_ADDR);

        // 遥测数据放入ddr中，地址待分配
        //...TBD
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *              intc internal levd bram1中断获取到数据进行处理
 * 1.主控req req数据包->同步lvds->通信fpga->intc17->通信ps端intcFun->从bram中获取数据包->构造apd ack数据包写入bram
 *
 ****************************************************************************/
int apdIntcInterLvdsBram1Process()
{
    // 状态检测
    if (!gApdInitIsReady)
    {
        return SEC_ERROR;
    }

    // 从中断响应中接收新的一轮数据
    if (gApdNewReqPkgIsArrived)
    {
        stuAdpPowerFrame* pInfo = (stuAdpPowerFrame*)GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR;
        memset(pInfo, 0, sizeof(stuAdpPowerFrame));
        // 数据有效性检查
        if (pInfo->usStartFlag != 0x90EB || pInfo->usEndFlag != 0x0A0D)
        {
            return SEC_ERROR;
        }

        // 数据有效性检查
        if (pInfo->usSrcAddr != 0x92 || pInfo->usDesAddr != 0x91 || pInfo->usSubPkgNum != 0x0001 || pInfo->usSubPkgSeq != 0x0001)
        {
            return SEC_ERROR;
        }

        // 检查累加和
        unsigned short usSum = apdCalcCheckSum((unsigned char*)pInfo + 2, sizeof(*pInfo) - 6);
        if (usSum != pInfo->usSum)
        {
            return SEC_ERROR;
        }

        // 获取apd接收光功率
        int nApdOpticalPowerRx = *(int*)(GP_AIX_APD_OPTICAL_POWER_RX_REG_ADDR);
        // 更新bram ack中数据
        pInfo->unData = nApdOpticalPowerRx;

        // 更新bram ack中的累计和
        pInfo->usSum = apdCalcCheckSum((unsigned char*)pInfo + 2, sizeof(*pInfo) - 6);

        // 更新数据状态标识
        gApdNewReqPkgIsArrived = FALSE;

        // bram更新后设置握手标识寄存器置0x01
        *(int*)(GP_AIX_PL_2_PS_APD_PKG_HANDSHAKE_SIGNAL_REG_ADDR) = 0x01;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *              INTC14号中断响应函数
 * APD/EML/板温/电压 -> FPGA寄存器写入数据 -> 触发软核INTC4中断 -> 软核读取寄存器数据 -> 写某个寄存区标识位
 ****************************************************************************/
void intcInternalLvdsBram1InstrHandler(void* pCallbackRef)
{
    // 状态检测
    if (!gApdInitIsReady)
    {
        return;
    }

    gApdNewReqPkgIsArrived = TRUE;
}

/****************************************************************************
 *                          和校验累加和
 ****************************************************************************/
unsigned short apdCalcCheckSum(unsigned char* pBuff, unsigned int unLength)
{
    unsigned short usCheckSum = 0;

    while (unLength--)
    {
        usCheckSum += *pBuff++;
    }

    return usCheckSum;
}

