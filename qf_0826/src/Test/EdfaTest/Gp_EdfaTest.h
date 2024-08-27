/*@file     Gp_EdfaTest.h
* @brief    光纤放大器debug uart测试
* @details  用于对光纤放大器进行测试
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/22
*/
#ifndef _GP_EDFA_TEST_H_
#define _GP_EDFA_TEST_H_

#include "Gp_EdfaTestDef.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
extern unsigned char gEdfaDebugUartTxDataBuffer[EDFA_DEBUG_UART_TX_DATA_BUFFER_SIZE];		// debug uart 数据发送缓存
extern unsigned char gEdfaDebugUartRxDataBuffer[EDFA_DEBUG_UART_RX_DATA_BUFFER_SIZE];		// debug uart 数据接收缓存
extern volatile unsigned short gEdfaDebugUartRxNewDataIsFin;          					    // debug uart 标识是否有新数据
extern volatile unsigned int gEdfaDebugUartTotalReceivedCount;                              // 接收到多少数据

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// loop
int edfaTestLoop();
// 接收到数据进行数据
int edfaTestRecvProcess();

#endif  // _GP_XM_TEST_H_
