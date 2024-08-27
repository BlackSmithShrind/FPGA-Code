/*@file     Gp_XmTest.c
* @brief    星敏感器debug uart测试
* @details  用于对星敏感器进行测试
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/20
*/
#include "Gp_XmTest.h"
#include "../../App/Xm/Gp_Xm.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
unsigned char gXmDebugUartTxDataBuffer[XM_DEBUG_UART_TX_DATA_BUFFER_SIZE] = "";			// debug uart tx缂撳瓨
unsigned char gXmDebugUartRxDataBuffer[XM_DEBUG_UART_RX_DATA_BUFFER_SIZE] = "";			// debug uart rx缂撳瓨
volatile unsigned short gXmDebugUartRxNewDataIsFin = FALSE;                     		// debug uart 标识是否有新数据

/****************************************************************************
 *              xm test loop
 ****************************************************************************/
int xmTestLoop()
{
    // 接收到新数据进行数据处理
    xmTestRecvProcess();

    return SEC_SUCCESS;
}

/****************************************************************************
 *             xm test 接收数据处理
 * 通过此接口处理debug uart接收到的数据，copy到xm模块的数据buffer，然后由xm模块process进行自行处理
 * 对接模仿can intr 中断fpga数据
 ****************************************************************************/
int xmTestRecvProcess()
{
    // 判断是否接收到新数据
    if (!gXmDebugUartRxNewDataIsFin)
    {
        return SEC_SUCCESS;
    }

    // 进行数据copy,然后xm模块去处理
	memcpy(gXmRxDataBuffer, gXmDebugUartRxDataBuffer, 8);

	// 模拟can instruction 中断指令获取数据
	gXmIntcRxNewDataIsFin = TRUE;

	// 设置无数据
    gXmDebugUartRxNewDataIsFin = FALSE;

    return SEC_SUCCESS;
}


