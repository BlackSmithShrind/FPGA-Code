/*@file     Gp_Gpio.cpp
* @brief    gpio 实现文件
* @details  通过该文件可以定义一些define、enum、struct等
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/14
*/
#include "xgpio.h"
#include "Gp_Gpio.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
// 定义中断控制器实例
XGpio gGpioInst0;                                                       // The driver instance for GPIO Device configured as IO/P

/****************************************************************************
 *  初始化(外部调用)
 * pGpioInst:
 * usDeviceId:
 * unChannel:
 * unDirectionMask:
 * unHighLowVolLevel:
 ****************************************************************************/
int gpioInit(XGpio* pGpioInst, unsigned short usDeviceId, unsigned int unChannel, unsigned int unDirectionMask, unsigned int unHighLowVolLevel)
{
    // 参数检查
    if (!pGpioInst)
    {
        return SEC_ERROR;
    }

    // 参数检查
    if (unChannel != 1 && unChannel != 2)
    {
        return SEC_ERROR;
    }

    // 中断初始化
    int nRet = gpioInitialize(pGpioInst, usDeviceId);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 设置方向
    nRet = gpioSetDataDirection(pGpioInst, unChannel, unDirectionMask);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 写入数据 默认低电平
    nRet = gpioDiscreteWrite(pGpioInst, unChannel, unHighLowVolLevel);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  初始化
 * pGpioInst:
 * usDeviceId:
 ****************************************************************************/
int gpioInitialize(XGpio* pGpioInst, unsigned short usDeviceId)
{
    // 参数检查
    if (!pGpioInst)
    {
        return SEC_ERROR;
    }

    // 中断初始化
    int nRet = XGpio_Initialize(pGpioInst, usDeviceId);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  数据方向设置
 * pGpioInst:
 * unChannel:通道（1 or 2）
 * unDirectionMask:掩码位0~31,0:output 1:input
 ****************************************************************************/
int gpioSetDataDirection(XGpio* pGpioInst, unsigned int unChannel, unsigned int unDirectionMask)
{
    // 参数检查
    if (!pGpioInst)
    {
        return SEC_ERROR;
    }

    // 设置方向
   XGpio_SetDataDirection(pGpioInst, unChannel, unDirectionMask);
    
    return SEC_SUCCESS;
}

/****************************************************************************
 *  设置指定的GPIO通道的寄存器的数据，可以按着掩码位进行bit位拉高拉低
 * pGpioInst:
 * unChannel:
 * unData:设置后转换为掩码位0~31 0:low 1:high
 ****************************************************************************/
int gpioDiscreteWrite(XGpio* pGpioInst, unsigned int unChannel, unsigned int unData)
{
    // 参数检查
    if (!pGpioInst)
    {
        return SEC_ERROR;
    }

    // 写数据（一般拉高使用）
    XGpio_DiscreteWrite(pGpioInst, unChannel, unData);
    
    return SEC_SUCCESS;
}

/****************************************************************************
 *  清楚指定的GPIO通道的寄存器的数据
 * pGpioInst:
 * unChannel:
 * unMask:
 ****************************************************************************/
int gpioDiscreteClear(XGpio* pGpioInst, unsigned int unChannel, unsigned int unMask)
{
    // 参数检查
    if (!pGpioInst)
    {
        return SEC_ERROR;
    }

    // 写数据（一般拉低使用）
    XGpio_DiscreteClear(pGpioInst, unChannel, unMask);
    
    return SEC_SUCCESS;
}

/****************************************************************************
 *  自检
 * pGpioInst:
 * unChannel:
 * unMask:
 ****************************************************************************/
int gpioSelfTest(XGpio* pGpioInst)
{
    // 参数检查
    if (!pGpioInst)
    {
        return SEC_ERROR;
    }

    // 自测
    int nRet = XGpio_SelfTest(pGpioInst);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}