/*@file     Gp_DebugUart.h
* @brief    开发板的debug uart
* @details  开发板的uart转usb串口，用于串口通信和调试
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/21
*/
#ifndef _GP_DEBUG_UART_H_
#define _GP_DEBUG_UART_H_

#include "Gp_DebugUartDef.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
extern unsigned char gDebugUartTxDataBuffer[DEBUG_UART_TX_DATA_BUFFER_SIZE];
extern unsigned char gDebugUartRxDataBuffer[DEBUG_UART_RX_DATA_BUFFER_SIZE];
extern volatile unsigned short gDebugUartInitIsReady;                       	// 初始化准备就绪
extern volatile unsigned short gDebugUartRecvIsReady;                       	// 已经接收到数据

extern volatile unsigned int gDebugUartTotalReceivedCount;                           // 接收到多少数据
extern volatile unsigned int gDebugUartTotalSentCount;                               // 发送到多少数据

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// debug uart 初始化（外部接口）
int debugUartInit(void);
// DebugUart loop
int debugUartLoop();
// debug uart 发送中断回调函数
void debugUartSendHandler(void* pCallbackRef, unsigned int unSendBytes);
// debug uart 接收中断回调函数
void debugUartRecvHandler(void* pCallbackRef, unsigned int unRecvBytes);
// xm debug uart 数据发送处理
int xmDebugUartSendProcess(unsigned char* pBuffer, unsigned int unSize);
// xm debug uart 数据接收处理
int xmDebugUartRecvProcess();
// xm debug uart 发送中断回调函数
void xmDebugUartSendHandler(void* pCallbackRef, unsigned int unSendBytes);
// xm debug uart 接收中断回调函数
void xmDebugUartRecvHandler(void* pCallbackRef, unsigned int unRecvBytes);
// edfa debug uart 数据发送处理
int edfaDebugUartSendProcess(unsigned char* pBuffer, unsigned int unSize);
// edfa debug uart 数据接收处理
int edfaDebugUartRecvProcess();
// edfa debug uart 发送中断回调函数
void edfaDebugUartSendHandler(void* pCallbackRef, unsigned int unSendBytes);
// edfa debug uart 接收中断回调函数
void edfaDebugUartRecvHandler(void* pCallbackRef, unsigned int unRecvBytes);

#endif  // _GP_DEBUG_UART_H_
