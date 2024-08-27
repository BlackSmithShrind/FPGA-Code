/*@file     Gp_Intc.h
* @brief    对xintc_v3.8(Interrupt Controller (Intc) )封装的中间层模块头文件
* @details  提供中断相关的操作
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/07
*/
#ifndef _GP_INTC_H_
#define _GP_INTC_H_

#include "Gp_IntcDef.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/

extern volatile unsigned int CG_save_start;         // 储存指令开始
extern volatile unsigned int CG_save_ready;         // 储存指令准备好
extern volatile unsigned int CG_save_all;           // 储存指令完成
extern volatile unsigned int CG_send_start;         // 发送指令开始
extern volatile unsigned int CG_send_ready;          // 发送指令完成
extern volatile unsigned int CG_send_all;           // 发送指令完成    


/************************** Variable Definitions ****************************/
// 定义中断控制器实例
extern XIntc gIntcInst0;                                                       // The instance of the Interrupt Controller 0

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// 中断实例接口初始化（对外接口）
int intcInit(XIntc* pIntcInst, unsigned short usDeviceId);
// 中断初始化
int intcInitialize(XIntc* pIntcInst, unsigned short usDeviceId);
// 自检测硬件正确性
int intcSelfTest(XIntc* pIntcInst);
// 中断连接
int intcConnect(XIntc* pIntcInst, unsigned short usIntrId, XInterruptHandler pCbHandler, void* pCallBack);
// 断开连接
int intcDisconnect(XIntc* pIntcInst, unsigned short usIntrId);
// 中断启用
int intcStart(XIntc* pIntcInst, unsigned char cuMode);
// 中断停止
int intcStop(XIntc* pIntcInst);
// 中断使能
int intcEnable(XIntc* pIntcInst, unsigned char ucIntrId);
// 中断禁止
int intcDisable(XIntc* pIntcInst, unsigned char ucIntrId);
//硬件中断
int hardwareintcEnable(XIntc* pIntcInst);
// 定义In12	CAN instruction中断函数
void intcCanInstrHandler(void* pCallbackRef);

/****************************************************************************/

void intcCG_saveInstrHandler();
u32 intcCG_sendInstrHandler();




#endif // _GP_INTC_H_
