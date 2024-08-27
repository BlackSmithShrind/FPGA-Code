/*@file     Gp_UartNs550.h
* @brief   uartns550串口模块头文件
* @details  提供uartns550串口类的相关操作
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/14
*/
#ifndef _GP_UARTNS550_H_
#define _GP_UARTNS550_H_

#include "Gp_UartNs550Def.h"
#include "../Intc/Gp_Intc.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
extern XUartNs550 gCg2ZkRs644UartNs550Inst;                                            //  CG  To 主控板全局单例
extern XUartNs550 gCg2TxRs644UartNs550Inst;                                            //  CG  To 通信板全局单例

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// 初始化（外部）
int uartNs550Init(XIntc* pIntcInst, XUartNs550* pUartNs550Inst, unsigned short usUartNs550DeviceId, unsigned short usUartNs550IntrId, XUartNs550_Handler pCbHandler, XUartNs550Format* pDataFormat, unsigned int unOptions);
// 初始化（内部）
int uartNs550Initialize(XUartNs550* pUartNs550Inst, unsigned short usDeviceId);
// 自检
int uartNs550SelfTest(XUartNs550* pUartNs550Inst);
// 连接到中断子系统
int uartNs550SetupInterruptSystem(XIntc* pIntcInst, unsigned short usUartNs550IntrId, void* pCallBackRef);
// 设置中断响应函数
int uartNs550SetHandler(XUartNs550* pUartNs550Inst, XUartNs550_Handler pCbHandler, void* pCallBackRef);
// 使能中断
int uartNs550EnableInterrupt(XIntc* pIntcInst, unsigned short usUartNs550IntrId);
// 设置数据格式
int uartNs550SetDataFormat(XUartNs550* pUartNs550Inst, XUartNs550Format* pDataFormat);
// 设置波特率
int uartNs550SetBaudRate(XUartNs550* pUartNs550Inst, unsigned int unBaudRate);
// 设置数据位
int uartNs550SetDataBits(XUartNs550* pUartNs550Inst, unsigned int unDataBits);
// 设置属性参数
int uartNs550SetOption(XUartNs550* pUartNs550Inst, unsigned short usOptions);
// 异步发送
int uartNs550Send(XUartNs550* pUartNs550Inst, unsigned char* pDataBuffer, unsigned int unNumBytes);
// 异步接收
int uartNs550Recv(XUartNs550* pUartNs550Inst, unsigned char* pDataBuffer, unsigned int unNumBytes);
// 中断响应函数(可以根据实际情况各个单元模块自行创建自己的中断响应函数，这里只是例子)
void UartNs550IntrHandler(void* pCallBackRef, unsigned int unEvent, unsigned int unEventData);


#endif // _GP_UARTNS550_H_
