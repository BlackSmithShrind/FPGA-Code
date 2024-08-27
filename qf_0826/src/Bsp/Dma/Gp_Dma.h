/*@file     Gp_Dma.h
* @brief    dma模块实现文件
* @details  提供dma引擎的相关操作
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/16
*/
#ifndef _GP_DMA_H_
#define _GP_DMA_H_

#include "Gp_DmaDef.h"


/****************DMA中断初始化顺序*******************
 * 
 * 中断Intialize--------->intcInitialize
 * 
 * 初始化dma驱动程序--------->dmaInitialize--------->连接发送中断处理函数--------->intcConnect
 *                                        -------->连接接收中断处理函数--------->intcConnect
 * 
 * 连接dma中断处理函数--------->dmaSetupInterruptSystem
 * 
 * 使能发送/接收中断--------->intcEnable
 *  
 * 使能dma发送/接收中断--------->dmaIntrEnable
 * 
 * 整体中断开始--------->intcStart
 * 
 * 使能硬件中断--------->hardwareintcEnable
 * 
 ***************************************************/

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
extern XAxiDma gAxiDmaInst0;                                                //  DMA0全局单例
extern XAxiDma gAxiDmaInst1;                                                //  DMA1全局单例

extern volatile unsigned int gDmaTxCompletionFlag;                         // dma接收完成标志
extern volatile unsigned int gDmaRxCompletionFlag;                         // dma发送完成标志
extern volatile unsigned int gDmaTxErrorCount;                             // dma发送错误计数
extern volatile unsigned int gDmaRxErrorCount;                             // dma接收错误计数
extern volatile unsigned int gDmaRstTxTimeOut;                             // dma tx reset超时时间
extern volatile unsigned int gDmaRstRxTimeOut;                             // dma rx reset超时时间

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// 初始化（外部）
int dmaInit(XIntc* pIntcInst, XAxiDma* pAxiDmaInst, unsigned short usAxiDmaDeviceId, unsigned short usAxiDmaTxIntrId, XInterruptHandler pTxCbHandler, void* pTxCallBackRef, unsigned short usAxiDmaRxIntrId, XInterruptHandler pRxCbHandler, void* pRxCallBackRef);
// 初始化（内部）
int dmaInitialize(XAxiDma* pAxiDmaInst, unsigned short usDeviceId);
// sg模式自检
int dmaHasSg(XAxiDma* pAxiDmaInst);
// 连接到中断子系统
int dmaSetupInterruptSystem(XIntc* pIntcInst, unsigned short usAxiDmaTxIntrId, XInterruptHandler pTxCbHandler, void* pTxCallBackRef, unsigned short usAxiDmaRxIntrId, XInterruptHandler pRxCbHandler, void* pRxCallBackRef);
// 从中断子系统中卸载
int dmaDisconnectIntrSystem(XIntc* pIntcInst, unsigned short usAxiDmaTxIntrId, unsigned short usAxiDmaRxIntrId);
// 中断使能
int dmaIntrEnable(XAxiDma* pAxiDmaInst, unsigned int unAxiDmaIrqMask, unsigned int unAxiDmaDir);
// 中断禁用
int dmaIntrDisable(XAxiDma* pAxiDmaInst, unsigned int unAxiDmaIrqMask, unsigned int unAxiDmaDir);
// 数据传输
int dmaTransfer(XAxiDma* pAxiDmaInst, unsigned int* pSrcBuffer, unsigned int unSrcDir,
							unsigned int* pDesBuffer, unsigned int unDesDir, unsigned int unLength);
// 重置
int dmaReset(XAxiDma* pAxiDmaInst);
// 发送中断处理函数
void dmaTxIntrHandler(void* pCallback);
// 接收中断处理函数
void dmaRxIntrHandler(void* pCallback);


#endif // _GP_UARTNS550_H_


/****************DMA中断初始化顺序*******************
 * 
 * 中断Intialize--------->intcInitialize
 * 
 * 初始化dma驱动程序--------->dmaInitialize--------->连接发送中断处理函数--------->intcConnect
 *                                        -------->连接接收中断处理函数--------->intcConnect
 * 
 * 连接dma中断处理函数--------->dmaSetupInterruptSystem
 * 
 * 使能发送/接收中断--------->intcEnable
 *  
 * 使能dma发送/接收中断--------->dmaIntrEnable
 * 
 * 整体中断开始--------->intcStart
 * 
 * 使能硬件中断--------->hardwareintcEnable
 * 
 ***************************************************/
