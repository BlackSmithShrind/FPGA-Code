/*@file     Gp_GpioDef.h
* @brief    gpio 文件
* @details  通过该文件可以定义一些define、enum、struct等
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/14
*/
#ifndef _GP_GPIO_H_
#define _GP_GPIO_H_

#include "xgpio.h"
#include "Gp_GpioDef.h"

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
// 定义中断控制器实例
extern XGpio gGpioInst0;                                                    // The driver instance for GPIO Device configured as IO/P

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// 初始化（外部调用）
int gpioInit(XGpio* pGpioInst, unsigned short usDeviceId, unsigned int unChannel, unsigned int unDirectionMask, unsigned int unHighLowVolLevel);
// 初始化（内部）
int gpioInitialize(XGpio* pGpioInst, unsigned short usDeviceId);
// 数据方向设置
int gpioSetDataDirection(XGpio* pGpioInst, unsigned int unChannel, unsigned int unDirectionMask);
// 设置指定的GPIO通道的寄存器的数据
int gpioDiscreteWrite(XGpio* pGpioInst, unsigned int unChannel, unsigned int unData);
// 清楚指定的GPIO通道的寄存器的数据
int gpioDiscreteClear(XGpio* pGpioInstr, unsigned int unChannel, unsigned int unMask);
// 自检
int gpioSelfTest(XGpio* pGpioInst);

#endif  /* _GP_GPIO_H_ */
