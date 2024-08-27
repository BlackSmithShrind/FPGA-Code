/*@file     Gp_WatchDog.c
* @brief    看门狗定时器
* @details  放置dsp跑飞了进行reset重置
* @author   huyanfeng
* @date     2024/1/9
*/
#include "Gp_WatchDog.h"

/****************************************************************************
  * 注意：I2C0, Timer64P0/P1, RTC, USB2.0 PHY, McASP0 serial clock 这些外设
  *       包括看门狗定时器（定时器 / 计数器 1）的时钟来源于 PLL 旁路时钟
  *       即 晶体振荡器的时钟 24MHz
  ****************************************************************************/

 /****************************************************************************
  *              watch dog timer reactivate
  * void
  * return
  ****************************************************************************/
void Gp_WatchdogReactivate(void)
{
	// 复位看门狗定时器 “喂狗”
	//TimerWatchdogReactivate(SOC_TMR_1_REGS);
}

 /****************************************************************************
  *              watch dog timer init
  * void
  * return
  ****************************************************************************/
int Gp_WatchDog_Init(void)
{
	int nRet = SEC_ERROR;

    // 配置 定时器 / 计数器 1 为 看门狗模式
	//TimerConfigure(SOC_TMR_1_REGS, TMR_CFG_64BIT_WATCHDOG);

    // 设置周期 64位
    //TimerPeriodSet(SOC_TMR_1_REGS, TMR_TIMER12, WD_TMR_PERIOD_LSB32_5_S);
    //TimerPeriodSet(SOC_TMR_1_REGS, TMR_TIMER34, WD_TMR_PERIOD_MSB32_5_S);

    // 使能看门狗定时器
    //TimerWatchdogActivate(SOC_TMR_1_REGS);

    nRet = SEC_SUCCESS;

    return nRet;
}
