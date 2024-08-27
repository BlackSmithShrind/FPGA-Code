/*@file     Gp_Edfa.h
* @details  针对通信板FPGA和软核MB之间使用axi总线寄存器+io+intc
*           软核MB与EDFA之间使用RS422接口，实现遥控指令、遥测查询指令发送和数据接收
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/07
*/
#ifndef _GP_EDFA_H_
#define _GP_EDFA_H_

#include "Gp_EdfaDef.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
extern unsigned char gEdfaTxDataBuffer[EDFA_TX_DATA_BUFFER_SIZE];
extern unsigned char gEdfaRxDataBuffer[EDFA_RX_DATA_BUFFER_SIZE];
extern volatile unsigned short gEdfaInitIsReady;                            // 是否已经完成初始化并且准备就绪
extern volatile unsigned short gEdfaHeaderConstBytesIsRecv;                 // 头部固定数据
extern volatile unsigned short gEdfaRemainRecvBytes;                        // 剩余字节数
extern volatile unsigned short gEdfaIntcRxNewDataIsFin;                     // 完成接收新数据

extern volatile unsigned int gEdfaTotalReceivedCount;                       // 需要接收的字节数
extern volatile unsigned int gEdfaTotalSentCount;                           // 需要发送的字节数

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/

// edfa模块初始化接口
int edfaInit(void);
// edfa模块初始化自检接口
int edfaInitChk(void);
// 接收数据loop函数
int edfaLoop();
// 光纤放大器EDFA执行机构->通信板PS->PFGA
int edfaRs422SendProcess(unsigned char* pBuffer, unsigned int unSize);
// 光纤放大器EDFA执行机构->通信板PS->PFGA
int edfaRs422RecvProcess();
// edfa rs422 发送中断回调函数
void edfaRs422SendHandler(void* pCallbackRef, unsigned int unSendBytes);
// edfa rs422 接收中断回调函数
void edfaRs422RecvHandler(void* pCallbackRef, unsigned int unRecvBytes);
// edfa intc 响应函数（CAN instruction）卫星平台 -can-> 通信板PL -AXI-INTC AXI-REG->通信板PS
//void edfaAxiIntcCanInstrHandler(void* pCallbackRef);
// 卫星平台 - CAN YK -> 通信板PL CAN - AXI-REG -> 通信板PS - RS422-> EDFA
int edfaPl2PsCanYkProcess();
// edfa debug uart test处理函数
int edfaDebugUartTestProcess();
// 校验和
unsigned char edfaCalcCheckXor(volatile unsigned char *pBuff, unsigned short usLength);

#endif  // _GP_EDFA_H_
