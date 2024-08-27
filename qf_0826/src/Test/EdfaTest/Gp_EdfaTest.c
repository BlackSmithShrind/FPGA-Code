/*@file     Gp_EdfaTest.c
* @brief    光纤放大器debug uart测试
* @details  用于对光纤放大器进行测试
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/22
*/
#include "Gp_EdfaTest.h"
#include "../../App/Edfa/Gp_Edfa.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
unsigned char gEdfaDebugUartTxDataBuffer[EDFA_DEBUG_UART_TX_DATA_BUFFER_SIZE] = "";			// debug uart tx缂撳瓨
unsigned char gEdfaDebugUartRxDataBuffer[EDFA_DEBUG_UART_RX_DATA_BUFFER_SIZE] = "";			// debug uart rx缂撳瓨
volatile unsigned short gEdfaDebugUartRxNewDataIsFin = FALSE;                     		    // debug uart 标识是否有新数据
volatile unsigned int gEdfaDebugUartTotalReceivedCount = 0;                                 // 接收到多少数据

/****************************************************************************
 *              edfa test loop
 ****************************************************************************/
int edfaTestLoop()
{
    // 接收到新数据进行数据处理
    edfaTestRecvProcess();

    return SEC_SUCCESS;
}

/****************************************************************************
 *             edfa test 接收数据处理
 * 通过此接口处理debug uart接收到的数据，copy到xm模块的数据buffer，然后由xm模块process进行自行处理
 * 对接模仿can intr 中断fpga数据
 ****************************************************************************/
int edfaTestRecvProcess()
{
    // 判断是否接收到新数据
    if (!gEdfaDebugUartRxNewDataIsFin)
    {
        return SEC_SUCCESS;
    }

    // 数据copy到App/Gp_Edfa.c单元中进行处理
    if (gEdfaDebugUartTotalReceivedCount != 0)
    {
        // 进行数据copy,然后xm模块去处理
        memcpy(gEdfaRxDataBuffer, gEdfaDebugUartRxDataBuffer, gEdfaDebugUartTotalReceivedCount);

        // 模拟can instruction 中断指令获取数据
        gEdfaIntcRxNewDataIsFin = TRUE;

        // 数据copy完成清0
        gEdfaDebugUartTotalReceivedCount = 0;
    }

	// 设置无数据
	gEdfaDebugUartRxNewDataIsFin = FALSE;

    return SEC_SUCCESS;
}


