/*@file     Gp_Spi.cpp
* @brief    对spi_v4_4(axi quad spi)封装的中间层模块源文件
* @details  提供spi引擎的相关接口
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/17
*/

#include "Gp_Spi.h"
#include "../Intc/Gp_Intc.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
XSpi gSpiInst;                                                              // spi全局实例

/****************************************************************************
 *              spi初始化(对外接口)
 * pIntcInst:
 * usDeviceId:
 ****************************************************************************/
int spiInit(XIntc* pIntcInst, XSpi* pSpiInst, unsigned short usDeviceId, unsigned short usSpiIntrId, XSpi_StatusHandler pStatusCbHandler, unsigned int unOptions)
{
    // 参数检查
    if (!pIntcInst || !pSpiInst || !pStatusCbHandler)
    {
        return SEC_ERROR;
    }

    // 中断初始化
    int nRet = spiInitialize(pSpiInst, usDeviceId);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 连接到中断子系统，以便中断可以发生
    nRet = spiSetupInterruptSystem(pIntcInst, usSpiIntrId, pSpiInst);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 设置状态回调函数
    nRet = spiSetStatusHandler(pSpiInst, pStatusCbHandler, pSpiInst);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }
    
    // 设置参数:spi主从属性，手动选择等
    nRet = spiSetOptions(pSpiInst, unOptions/*XSP_MASTER_OPTION | XSP_MANUAL_SSELECT_OPTION*/);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 启动
    nRet = spiStart(pSpiInst);
    if (nRet != XST_SUCCESS)
    {
    }

    // 使能中断
    nRet = spiEnableInterrupt(pIntcInst, usSpiIntrId);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  初始化
 * pSpiInst:
 * usDeviceId:
 ****************************************************************************/
int spiInitialize(XSpi* pSpiInst, unsigned short usDeviceId)
{
    // 参数检查
    if (!pSpiInst)
    {
        return SEC_ERROR;
    }

    // 实例对象初始化
    int nRet = XSpi_Initialize(pSpiInst, usDeviceId);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  连接到中断子系统
 * pSpiInst:
 ****************************************************************************/
int spiSetupInterruptSystem(XIntc* pIntcInst, unsigned short usUartNs550IntrId, void* pCallBackRef)
{
    // 参数检查
    if (!pIntcInst)
    {
        return SEC_ERROR;
    }

    // 连接中断子系统
    int nRet = intcConnect(pIntcInst, usUartNs550IntrId, (XInterruptHandler)XSpi_InterruptHandler, (void *)pCallBackRef);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  启动
 * pSpiInst:
 ****************************************************************************/
int spiStart(XSpi* pSpiInst)
{
    // 参数检查
    if (!pSpiInst)
    {
        return SEC_ERROR;
    }

    // 启动
    int nRet = XSpi_Start(pSpiInst);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  停止
 * pSpiInst:
 ****************************************************************************/
int spiStop(XSpi* pSpiInst)
{
    // 参数检查
    if (!pSpiInst)
    {
        return SEC_ERROR;
    }

    // 停止
    int nRet = XSpi_Stop(pSpiInst);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  使能中断
 * pIntcInst:
 * ucSpiIntrId:
 ****************************************************************************/
int spiEnableInterrupt(XIntc* pIntcInst, unsigned char ucSpiIntrId)
{
    // 参数检查
    if (!pIntcInst)
    {
        return SEC_ERROR;
    }

    XIntc_Enable(pIntcInst, ucSpiIntrId);

    return SEC_SUCCESS;
}

/****************************************************************************
 *  禁用中断
 * pIntcInst:
 * ucSpiIntrId:
 ****************************************************************************/
int spiDisableInterrupt(XIntc* pIntcInst, unsigned char ucSpiIntrId)
{
    // 参数检查
    if (!pIntcInst)
    {
        return SEC_ERROR;
    }

    XIntc_Disable(pIntcInst, ucSpiIntrId);

    return SEC_SUCCESS;
}

/****************************************************************************
 *  设置状态回调函数
 * pSpiInst:
 * pStatusCbHandler:
 * pCallBackRef:
 ****************************************************************************/
int spiSetStatusHandler(XSpi* pSpiInst, XSpi_StatusHandler pStatusCbHandler, void* pCallBackRef)
{
    // 参数检查
    if (!pSpiInst)
    {
        return SEC_ERROR;
    }

    // 设置状态回调函数
    XSpi_SetStatusHandler(pSpiInst, pCallBackRef, pStatusCbHandler);
    
    return SEC_SUCCESS;
}

/****************************************************************************
 *  设置参数
 * pSpiInst:
 * usOptions: spi主从属性，手动选择等
 *          - XSP_MASTER_OPTION | XSP_MANUAL_SSELECT_OPTION
 ****************************************************************************/
int spiSetOptions(XSpi* pSpiInst, unsigned int usOptions)
{
    // 参数检查
    if (!pSpiInst)
    {
        return SEC_ERROR;
    }

    // 设置参数
    int nRet = XSpi_SetOptions(pSpiInst, usOptions);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  设置从属选择寄存器
 * pSpiInst:
 * usSlaveMask: is a 32-bit mask with a 1 in the bit position of the
 *              slave being selected. Only one slave can be selected. The
 *              SlaveMask can be zero if the slave is being deselected.
 ****************************************************************************/
int spiSetSlaveSelect(XSpi* pSpiInst, unsigned int usSlaveMask)
{
    // 参数检查
    if (!pSpiInst)
    {
        return SEC_ERROR;
    }

    // 设置参数
    int nRet = XSpi_SetSlaveSelect(pSpiInst, usSlaveMask);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  状态发送接收回调函数
 * This function is the handler which performs processing for the SPI driver.
 * It is called from an interrupt context such that the amount of processing
 * performed should be minimized. It is called when a transfer of SPI data
 * completes or an error occurs.
 * This handler provides an example of how to handle SPI interrupts and
 * is application specific.
 *
 * pCallBackRef:
 * usStatusEvent:
 * unBytesCount:
 ****************************************************************************/
void spiStatusCbHandler(void* pCallBackRef, unsigned int usStatusEvent, unsigned int unBytesCount)
{
    //struct XSpi* pSpiInst = (struct XSpi*)pCallBackRef;
   
    /*
     * If the event was not transfer done, then track it as an error.
     */
    if (usStatusEvent != XST_SPI_TRANSFER_DONE)
    {
        
    }
}


/****************************************************************************
 *  设置从属选择寄存器
 * pCallBackRef:
 * usStatusEvent:
 * unBytesCount:
 ****************************************************************************/
int spiTransfer(XSpi* pSpiInst, unsigned char* pSendBuf, unsigned char* pRecvBuf, unsigned int unBytesCount)
{
    // 参数检查
    if (!pSpiInst)
    {
        return SEC_ERROR;
    }

    // 设置参数
    int nRet = XSpi_Transfer(pSpiInst, pSendBuf, pRecvBuf, unBytesCount);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}
