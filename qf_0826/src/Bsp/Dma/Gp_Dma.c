/*@file     Gp_Dma.c
* @brief    dma模块实现文件
* @details  提供dma引擎的相关操作
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/16
*/

#include "Gp_Dma.h"
#include "../Intc/Gp_Intc.h"
#include "sleep.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
XAxiDma gAxiDmaInst0;                                                       //  DMA0全局单例
XAxiDma gAxiDmaInst1;                                                       //  DMA1全局单例

volatile unsigned int gDmaTxCompletionFlag = 0;                             // dma接收完成标志
volatile unsigned int gDmaRxCompletionFlag = 0;                             // dma发送完成标志
volatile unsigned int gDmaTxErrorCount = 0;                                 // dma发送错误计数
volatile unsigned int gDmaRxErrorCount = 0;                                 // dma接收错误计数
volatile unsigned int gDmaRstTxTimeOut = 1000;                              // dma tx reset超时时间
volatile unsigned int gDmaRstRxTimeOut = 1000;                              // dma rx reset超时时间

/****************************************************************************
 *              初始化（外部调用使用）
 * pAxiDmaInst:
 * usAxiDmaDeviceId: device id
 * usUartNs550IntrId:
 * pCbHandler:
 ****************************************************************************/
int dmaInit(XIntc* pIntcInst, XAxiDma* pAxiDmaInst,
		unsigned short usAxiDmaDeviceId, unsigned short usAxiDmaTxIntrId,
		XInterruptHandler pTxCbHandler,
		void* pTxCallBackRef, unsigned short usAxiDmaRxIntrId,
		XInterruptHandler pRxCbHandler, void* pRxCallBackRef)
{
    int nRet = SEC_ERROR;

    // 参数检查
    if (pIntcInst == NULL || pAxiDmaInst == NULL || pTxCbHandler == NULL || pRxCbHandler == NULL)
    {
        return SEC_ERROR;
    }

    // 初始化mda驱动程序
    nRet = dmaInitialize(pAxiDmaInst, usAxiDmaDeviceId);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 检测sg模式
    nRet = dmaHasSg(pAxiDmaInst);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 连接到中断子系统，以便中断可以发生
    nRet = dmaSetupInterruptSystem(pIntcInst, usAxiDmaTxIntrId, (void*)pAxiDmaInst, pTxCbHandler, usAxiDmaRxIntrId, pRxCbHandler, (void*)pAxiDmaInst);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 注意：在使用的时候请先intcStart调用
    // 使能发送中断
    nRet = intcEnable(pIntcInst, usAxiDmaTxIntrId);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 使能接收中断
    nRet = intcEnable(pIntcInst, usAxiDmaRxIntrId);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  初始化
 * pAxiDmaInst:
 * usDeviceId:
 ****************************************************************************/
int dmaInitialize(XAxiDma* pAxiDmaInst, unsigned short usDeviceId)
{
    int nRet = SEC_ERROR;
    XAxiDma_Config* pConfig = NULL;

    // 参数检查
    if (pAxiDmaInst == NULL)
    {
        return SEC_ERROR;
    }

    // 在bsp固件配置表中根据设备id查找配置信息
    pConfig = XAxiDma_LookupConfig(usDeviceId);
    if (!pConfig)
    {
        return SEC_ERROR;
    }

    // 初始化dma引擎
    nRet = XAxiDma_CfgInitialize(pAxiDmaInst, pConfig);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    if(XAxiDma_HasSg(pAxiDmaInst)){
		xil_printf("Device configured as SG mode \r\n");
		return XST_FAILURE;
	}

    return SEC_SUCCESS;
}

/****************************************************************************
 *  sg模式
 * pAxiDmaInst:
 ****************************************************************************/
int dmaHasSg(XAxiDma* pAxiDmaInst)
{
    // 参数检查
    if (!pAxiDmaInst)
    {
        return FALSE;
    }

    return XAxiDma_HasSg(pAxiDmaInst);
}

/****************************************************************************
 *              挂接中断子系统
 * pIntcInst:
 * pAxiDmaInst:
 * usAxiDmaTxIntrId:
 * usAxiDmaRxIntrId:
 ****************************************************************************/
int dmaSetupInterruptSystem(XIntc* pIntcInst, unsigned short usAxiDmaTxIntrId, XInterruptHandler pTxCbHandler, void* pTxCallBackRef, unsigned short usAxiDmaRxIntrId, XInterruptHandler pRxCbHandler, void* pRxCallBackRef)
{
    // 参数检查
    if (pIntcInst == NULL)
    {
        return SEC_ERROR;
    }

    // tx连接中断子系统
    int nRet = intcConnect(pIntcInst, usAxiDmaTxIntrId, (XInterruptHandler)pTxCbHandler, pTxCallBackRef);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // rx连接中断子系统
    nRet = intcConnect(pIntcInst, usAxiDmaRxIntrId, (XInterruptHandler)pRxCbHandler, pRxCallBackRef);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }


    return SEC_SUCCESS;
}

/****************************************************************************
 *              卸载中断子系统
 * pIntcInst:
 * usAxiDmaTxIntrId:
 * usAxiDmaRxIntrId:
 ****************************************************************************/
int dmaDisconnectIntrSystem(XIntc* pIntcInst, unsigned short usAxiDmaTxIntrId, unsigned short usAxiDmaRxIntrId)
{
    // 参数检查
    if (pIntcInst == NULL)
    {
        return SEC_ERROR;
    }

    // tx卸载中断子系统
    intcDisconnect(pIntcInst, usAxiDmaTxIntrId);

    // rx连接中断子系统
    intcDisconnect(pIntcInst, usAxiDmaRxIntrId);

    return SEC_SUCCESS;

}

/****************************************************************************
 *              dma中断使能
 * pAxiDmaInst:
 * unAxiDmaIrqMask: Bitmask for interrupts, These masks are shared by XAXIDMA_CR_OFFSET register and XAXIDMA_SR_OFFSET register
 *                  XAXIDMA_IRQ_IOC_MASK\XAXIDMA_IRQ_DELAY_MASK\XAXIDMA_IRQ_ERROR_MASK\XAXIDMA_IRQ_ALL_MASK
 * unAxiDmaDir:     name DMA Transfer Direction
 *                  XAXIDMA_DMA_TO_DEVICE or XAXIDMA_DEVICE_TO_DMA
 ****************************************************************************/
int dmaIntrEnable(XAxiDma* pAxiDmaInst, unsigned int unAxiDmaIrqMask, unsigned int unAxiDmaDir)
{
    // 参数检查
    if (pAxiDmaInst == NULL)
    {
        return SEC_ERROR;
    }

    // 使能中断
    XAxiDma_IntrEnable(pAxiDmaInst, unAxiDmaIrqMask, unAxiDmaDir);

    return SEC_SUCCESS;
}

/****************************************************************************
 *              dma中断禁用
 * pAxiDmaInst:     InstancePtr is the pointer to the driver instance
 * unAxiDmaIrqMask: Bitmask for interrupts, These masks are shared by XAXIDMA_CR_OFFSET register and XAXIDMA_SR_OFFSET register
 *                  XAXIDMA_IRQ_IOC_MASK\XAXIDMA_IRQ_DELAY_MASK\XAXIDMA_IRQ_ERROR_MASK\XAXIDMA_IRQ_ALL_MASK
 * unAxiDmaDir:     Direction is DMA transfer direction, valid values are
 *                  - XAXIDMA_DMA_TO_DEVICE.
 *                  - XAXIDMA_DEVICE_TO_DMA.
 ****************************************************************************/
int dmaIntrDisable(XAxiDma* pAxiDmaInst, unsigned int unAxiDmaIrqMask, unsigned int unAxiDmaDir)
{
    // 参数检查
    if (!pAxiDmaInst)
    {
        return SEC_ERROR;
    }

    // 禁用中断
    XAxiDma_IntrDisable(pAxiDmaInst, unAxiDmaIrqMask, unAxiDmaDir);

    return SEC_SUCCESS;
}

/****************************************************************************
 *              dma数据传输
 * pAxiDmaInst:
 * pSrcBuffer:
 * unSrcDir:
 * pDesBuffer:
 * unDesDir:
 * unLength:
 ****************************************************************************/
int dmaTransfer(XAxiDma* pAxiDmaInst, unsigned int* pSrcBuffer, unsigned int unSrcDir,
							unsigned int* pDesBuffer, unsigned int unDesDir, unsigned int unLength)
{
    // 参数检查
	if (!pAxiDmaInst|| !pSrcBuffer|| !pDesBuffer)
    {
        return SEC_ERROR;
    }

//    // 接收数据
//	int nRet = XAxiDma_SimpleTransfer(pAxiDmaInst, (UINTPTR)pDesBuffer, unLength, unDesDir);
//    if (nRet != XST_SUCCESS)
//    {
//        return SEC_ERROR;
//    }


    // 发送数据
    int  nRet = XAxiDma_SimpleTransfer(pAxiDmaInst, (UINTPTR)pSrcBuffer, unLength, unSrcDir);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    while(!gDmaTxCompletionFlag){

    }

    // 接收数据
    nRet = XAxiDma_SimpleTransfer(pAxiDmaInst, (UINTPTR)pDesBuffer, unLength, unDesDir);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    while(!gDmaRxCompletionFlag){

    }

    return XST_SUCCESS;
}

/****************************************************************************
 *              dma通道重置
 * pAxiDmaInst:
 ****************************************************************************/
int dmaReset(XAxiDma* pAxiDmaInst)
{
    // 参数检查
    if (!pAxiDmaInst)
    {
        return SEC_ERROR;
    }

    //重置DMA引擎的发送和接收通道
    XAxiDma_Reset(pAxiDmaInst);

    unsigned int unTimeOut = gDmaRstTxTimeOut;
    while (unTimeOut)
    {
        if (XAxiDma_ResetIsDone(pAxiDmaInst))
        {
            return SEC_SUCCESS;
        }

        unTimeOut--;
    }

    return SEC_ERROR;
}

/****************************************************************************
 *              dma发送中断处理函数
 * pCallback:
 ****************************************************************************/
void dmaTxIntrHandler(void* pCallback)
{
    // 参数检查
    if (!pCallback)
    {
        return;
    }

    //复位时间
    XAxiDma* pAxiDmaInst = (XAxiDma *)pCallback;

    //读取待处理的中断
    unsigned int unIrqStatus = XAxiDma_IntrGetIrq(pAxiDmaInst, XAXIDMA_DMA_TO_DEVICE);
    //确认待处理的中断
    XAxiDma_IntrAckIrq(pAxiDmaInst, unIrqStatus, XAXIDMA_DMA_TO_DEVICE);

    //如果中断没有发生，什么都不做
    if (!(unIrqStatus & XAXIDMA_IRQ_ALL_MASK)) 
    {
        return;
    }

    //发送出错
    if ((unIrqStatus & XAXIDMA_IRQ_ERROR_MASK)) 
    {
        // 错误计数
        gDmaTxErrorCount++;

        //重置DMA引擎的发送和接收通道
        unsigned int unRet = dmaReset(pAxiDmaInst);
        if (unRet == SEC_ERROR)
        {
            return;
        }
    }

    //发送完成的中断
    if (unIrqStatus & XAXIDMA_IRQ_IOC_MASK)
    {
        gDmaTxCompletionFlag = 1;
    }

    return;
}

/****************************************************************************
 *              dma接收中断处理函数
 * pCallback:
 ****************************************************************************/
void dmaRxIntrHandler(void* pCallback)
{
    // 参数检查
    if (!pCallback)
    {
        return;
    }

    //复位时间
    XAxiDma* pAxiDmaInst = (XAxiDma *)pCallback;

    //读取待处理的中断
    unsigned int unIrqStatus = XAxiDma_IntrGetIrq(pAxiDmaInst, XAXIDMA_DEVICE_TO_DMA);
    //确认待处理的中断
    XAxiDma_IntrAckIrq(pAxiDmaInst, unIrqStatus, XAXIDMA_DEVICE_TO_DMA);
    
    //如果中断没有发生，什么都不做
    if (!(unIrqStatus & XAXIDMA_IRQ_ALL_MASK)) 
    {
        return;
    }

    //接收出错
    if ((unIrqStatus & XAXIDMA_IRQ_ERROR_MASK)) 
    {
        // 错误计数
        gDmaRxErrorCount++;

        //重置DMA引擎的发送和接收通道
        unsigned int unRet = dmaReset(pAxiDmaInst);
        if (unRet == SEC_ERROR)
        {
            return;
        }
    }

    //接收完成
    if ((unIrqStatus & XAXIDMA_IRQ_IOC_MASK)) 
    {
        gDmaRxCompletionFlag = 1;
    }

    return;
}
