/*@file     Gp_Spi.h
* @brief    对spi_v4_4(axi quad spi)封装的中间层模块头文件
* @details  提供spi引擎的相关接口
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/17
*/
#ifndef _GP_SPI_H_
#define _GP_SPI_H_

#include "Gp_SpiDef.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
extern XSpi gSpiInst;                                                       // spi全局实例

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// 初始化（外部）
int spiInit(XIntc* pIntcInst, XSpi* pSpiInst, unsigned short usDeviceId, unsigned short usSpiIntrId, XSpi_StatusHandler pStatusCbHandler, unsigned int unOptions);
// 初始化（内部）
int spiInitialize(XSpi* pSpiInst, unsigned short usDeviceId);
// 连接到中断子系统
int spiSetupInterruptSystem(XIntc* pIntcInst, unsigned short usUartNs550IntrId, void* pCallBackRef);
// 启动
int spiStart(XSpi* pSpiInst);
// 停止
int spiStop(XSpi* pSpiInst);
// 使能中断
int spiEnableInterrupt(XIntc* pIntcInst, unsigned char ucSpiIntrId);
// 禁用中断
int spiDisableInterrupt(XIntc* pIntcInst, unsigned char ucSpiIntrId);
// 设置状态回调函数
int spiSetStatusHandler(XSpi* pSpiInst, XSpi_StatusHandler pStatusCbHandler, void* pCallBackRef);
// 设置参数
int spiSetOptions(XSpi* pSpiInst, unsigned int usOptions);
// 设置从属选择寄存器
int spiSetSlaveSelect(XSpi* pSpiInst, unsigned int usSlaveMask);
// 状态回调函数
void spiStatusCbHandler(void* pCallBackRef, unsigned int usStatusEvent, unsigned int unBytesCount);
// 设置从属选择寄存器
int spiTransfer(XSpi* pSpiInst, unsigned char* pSendBuf, unsigned char* pRecvBuf, unsigned int unBytesCount);


#endif // _GP_SPI_H_
