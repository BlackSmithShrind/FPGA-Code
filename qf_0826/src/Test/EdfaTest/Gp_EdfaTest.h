/*@file     Gp_EdfaTest.h
* @brief    ���˷Ŵ���debug uart����
* @details  ���ڶԹ��˷Ŵ������в���
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
extern unsigned char gEdfaDebugUartTxDataBuffer[EDFA_DEBUG_UART_TX_DATA_BUFFER_SIZE];		// debug uart ���ݷ��ͻ���
extern unsigned char gEdfaDebugUartRxDataBuffer[EDFA_DEBUG_UART_RX_DATA_BUFFER_SIZE];		// debug uart ���ݽ��ջ���
extern volatile unsigned short gEdfaDebugUartRxNewDataIsFin;          					    // debug uart ��ʶ�Ƿ���������
extern volatile unsigned int gEdfaDebugUartTotalReceivedCount;                              // ���յ���������

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// loop
int edfaTestLoop();
// ���յ����ݽ�������
int edfaTestRecvProcess();

#endif  // _GP_XM_TEST_H_
