/*@file     Gp_Apd.h
* @brief    杞牳apd椹卞姩浜や簰鍗曞厓
* @details  娣诲姞apd鎺ュ彛鐩稿叧澶勭悊閫昏緫
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/21
*/
#ifndef _GP_APD_H_
#define _GP_APD_H_

#include "Gp_ApdDef.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// 初始化接口(外部接口)
int apdInit(void);
// loop函数
int apdLoop();
// axi-reg数据处理函数 APD\EML\板温电压电流数据处理函数 (APD:光功率、温度、电流、电压 EML:电流、温度 板温、电压检测：板温、电压5V、电压12V)
int apdEmlBoardDataProcess();
// intc bram中断时数据处理函数
int apdIntcInterLvdsBram1Process();
// 中断处理函数
void intcInternalLvdsBram1InstrHandler(void* pCallbackRef);
// 数据发送处理（通信板软核 <---> 主控板软核）
int apdLvdsSendProcess(unsigned char* pBuffer, unsigned int unSize);
// 数据接受处理（通信板软核 <---> 主控板软核）
int apdLvdsRecvProcess();
// 发送中断回调函数
void apdLvdsSendHandler(void* pCallbackRef, unsigned int unSendBytes);
// 接收中断回调函数
void apdLvdsRecvHandler(void* pCallbackRef, unsigned int unRecvBytes);
// 累加和
unsigned short apdCalcCheckSum(unsigned char* pBuff, unsigned int unLength);
#endif  // _GP_APD_H_
