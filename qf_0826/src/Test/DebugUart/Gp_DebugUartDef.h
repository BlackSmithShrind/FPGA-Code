/*@file     Gp_DebugUartDef.h
* @brief    �������ϵ�usbתuart����
* @details  ���ڿ������ԵĴ���
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/21
*/
#ifndef _GP_DEBUG_UART_DEF_H_
#define _GP_DEBUG_UART_DEF_H_

#include "../../Base/Comm/Gp_Common.h"                // ͨ��ͷ�ļ�

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/
#define DEBUG_UART_TX_DATA_BUFFER_SIZE              256         // ���ڷ���FPGA AXI REG �����������ݻ���
#define DEBUG_UART_RX_DATA_BUFFER_SIZE              256         // ���ڽ���FPGA AXI REG �����������ݻ���
#define DEBUG_UART_SF_HEADER_SIZE                   2           // ��������ͨ��Э��ack�İ�ͷ��0xEB 0x90\0x50\0x4
#define DEBUG_UART_YK_ACK_REMAIN_RECV_SIZE          3           // ��������ͨ��Э��ң��ʣ����3������5-2�ֽڣ�
#define DEBUG_UART_YC_ACK_REMAIN_RECV_SIZE          51          // ��������ͨ��Э��ң��ʣ����51������53-2�ֽڣ�
#define DEBUG_UART_SELF_TEST_ACK_REMAIN_RECV_SIZE   17          // ��������ͨ��Э���Լ�ʣ����17������19-2�ֽڣ�

// ����XM���Դ��ں�
#ifndef XM_DEBUG_UART_TEST
//#define XM_DEBUG_UART_TEST
#endif

// ����EDFA���Դ��ں�
#ifndef EDFA_DEBUG_UART_TEST
#define EDFA_DEBUG_UART_TEST
#endif

#endif  // _GP_DEBUG_UART_DEF_H_
