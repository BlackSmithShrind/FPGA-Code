/*@file     Gp_XmTest.h
* @brief    星敏感器debug uart测试
* @details  用于对星敏感器进行测试
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/20
*/
#ifndef _GP_XM_TEST_H_
#define _GP_XM_TEST_H_

#include "Gp_XmTestDef.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
extern unsigned char gXmDebugUartTxDataBuffer[XM_DEBUG_UART_TX_DATA_BUFFER_SIZE];		// debug uart tx缂撳瓨
extern unsigned char gXmDebugUartRxDataBuffer[XM_DEBUG_UART_RX_DATA_BUFFER_SIZE];		// debug uart rx缂撳瓨
extern volatile unsigned short gXmDebugUartRxNewDataIsFin;          					// debug uart 标识是否有新数据

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// loop
int xmTestLoop();
// 接收到数据进行数据
int xmTestRecvProcess();

#endif  // _GP_XM_TEST_H_
