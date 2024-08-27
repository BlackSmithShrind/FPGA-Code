/*@file     Gp_Xm.h
* @brief    星敏感器外部机构头文件
* @details  通过该文件定义一些对外接口
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/12
*/
#ifndef _GP_XM_H_
#define _GP_XM_H_

#include "Gp_XmDef.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
extern unsigned char gXmTxDataBuffer[XM_TX_DATA_BUFFER_SIZE];
extern unsigned char gXmRxDataBuffer[XM_RX_DATA_BUFFER_SIZE];
extern volatile unsigned short gXmInitIsReady;                              // 是否已经完成初始化
extern volatile unsigned short gXmHeaderConstBytesIsRecv;                   // 头部固定数据
extern volatile unsigned short gXmRemainRecvBytes;                          // 剩余字节数
extern volatile unsigned short gXmIntcRxNewDataIsFin;                       // 完成接收新数据

extern volatile unsigned int gXmTotalReceivedCount;
extern volatile unsigned int gXmTotalSentCount;                             // 需要发送的字节数

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// xm模块初始化接口
int xmInit(void);
// xm模块初始化自检接口
int xmInitChk(void);
// 接收数据loop函数
int xmLoop();
// 光纤放大器EDFA执行机构->通信板PS->PFGA
int xmRs422SendProcess(unsigned char* pBuffer, unsigned int unSize);
// 光纤放大器EDFA执行机构->通信板PS->PFGA
int xmRs422RecvProcess();
// xm rs422 发送中断回调函数
void xmRs422SendHandler(void* pCallbackRef, unsigned int unSendBytes);
// xm rs422 接收中断回调函数
void xmRs422RecvHandler(void* pCallbackRef, unsigned int unRecvBytes);
// xm intc 响应函数（CAN instruction）卫星平台 -can-> 通信板PL -AXI-INTC AXI-REG->通信板PS
//void xmAxiIntcCanInstrHandler(void* pCallbackRef);
// 卫星平台 - CAN YK -> 通信板PL CAN - AXI-REG -> 通信板PS - RS422-> EDFA
int xmAxiIntcCanInstrYkProcess();
// 校验和
unsigned char xmCalcCheckXor(volatile unsigned char *pBuff, unsigned short usLength);

#endif  // _GP_XM_H_ 
