/*@file     Gp_DebugUartDef.h
* @brief    开发板上的usb转uart串口
* @details  用于开发调试的串口
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/21
*/
#ifndef _GP_DEBUG_UART_DEF_H_
#define _GP_DEBUG_UART_DEF_H_

#include "../../Base/Comm/Gp_Common.h"                // 通用头文件

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/
#define DEBUG_UART_TX_DATA_BUFFER_SIZE              256         // 用于发送FPGA AXI REG 给过来的数据缓存
#define DEBUG_UART_RX_DATA_BUFFER_SIZE              256         // 用于接收FPGA AXI REG 给过来的数据缓存
#define DEBUG_UART_SF_HEADER_SIZE                   2           // 星敏感器通信协议ack的包头（0xEB 0x90\0x50\0x4
#define DEBUG_UART_YK_ACK_REMAIN_RECV_SIZE          3           // 星敏感器通信协议遥控剩余字3节数（5-2字节）
#define DEBUG_UART_YC_ACK_REMAIN_RECV_SIZE          51          // 星敏感器通信协议遥测剩余字51节数（53-2字节）
#define DEBUG_UART_SELF_TEST_ACK_REMAIN_RECV_SIZE   17          // 星敏感器通信协议自检剩余字17节数（19-2字节）

// 定义XM调试串口宏
#ifndef XM_DEBUG_UART_TEST
//#define XM_DEBUG_UART_TEST
#endif

// 定义EDFA调试串口宏
#ifndef EDFA_DEBUG_UART_TEST
#define EDFA_DEBUG_UART_TEST
#endif

#endif  // _GP_DEBUG_UART_DEF_H_
