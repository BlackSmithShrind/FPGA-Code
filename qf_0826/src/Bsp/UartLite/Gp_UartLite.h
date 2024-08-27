/*@file     Gp_UartLite.h
* @brief    UartLite涓插彛妯″潡澶存枃浠�
* @details  鎻愪緵UartLite涓插彛绫荤殑鐩稿叧鎿嶄綔
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/07
*/
#ifndef _GP_UARTLITE_H_
#define _GP_UARTLITE_H_

#include "Gp_UartLiteDef.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
extern XUartLite gEdfaRs422UartLiteInst;                                    // 婵�鍏夊櫒EDFA 鎺у埗鎸囦护
extern XUartLite gXmRs422UartLiteInst;                                      // 鏄熸晱鎺у埗鎸囦护
extern XUartLite gCg2Fsm0Rs422UartLiteInst;                                 // CG To FSM0
extern XUartLite gCg2Fsm1Rs422UartLiteInst;                                 // CG To FSM1
extern XUartLite gYkYc2ZkRs422UartLiteInst;                                 // 涓庝富鎺ф澘 閬ユ帶閬ユ祴
extern XUartLite gDebugUartLiteInst;                                   		// DEBUG uart

extern volatile unsigned int gTotalSentCount;
extern volatile unsigned int gTotalRecvCount;

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// 鍒濆鍖�(澶栭儴璋冪敤)
int uartLiteInit(XIntc* pIntcInst, XUartLite* pUartLiteInst, unsigned short usUartLiteDeviceId, unsigned short UartLiteIntrId, XUartLite_Handler pSendCbHandler, XUartLite_Handler pRecvCbHandler);
// 鍒濆鍖�
int uartLiteInitialize(XUartLite* pUartLiteInst, unsigned short usDeviceId);
// 鑷
int uartLiteSelfTest(XUartLite* pUartLiteInst);
// 杩炴帴鍒颁腑鏂瓙绯荤粺
int uartLiteSetupInterruptSystem(XIntc* pIntcInst, unsigned short usUartLiteIntrId, void* pCallBackRef);
// 璁剧疆鍙戦�佸洖璋冨嚱鏁�
int uartLiteSetSendHandler(XUartLite* pUartLiteInst, XUartLite_Handler pCbHandler, void* pCallBackRef);
// 璁剧疆鎺ユ敹鍥炶皟鍑芥暟
int uartLiteSetRecvHandler(XUartLite* pUartLiteInst, XUartLite_Handler pCbHandler, void* pCallBackRef);
// 浣胯兘涓柇
int uartLiteEnableInterrupt(XUartLite* pUartLiteInst);
// 绂佺敤涓柇
int uartLiteDisableInterrupt(XUartLite* pUartLiteInst);
// 寮傛鍙戦��
int uartLiteSend(XUartLite* pUartLiteInst, unsigned char* pDataBuffer, unsigned int unNumBytes);
// 寮傛鎺ユ敹
int uartLiteRecv(XUartLite* pUartLiteInst, unsigned char* pDataBuffer, unsigned int unNumBytes);


#endif // _GP_UARTLITE_H_
