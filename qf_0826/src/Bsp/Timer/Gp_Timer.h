/*@file     GP_LCPG.h
* @brief    定时器
* @details  定时器
* @author   huyanfeng
* @date     2024/2/28
*/
#ifndef _GP_TIMER_H_
#define _GP_TIMER_H_

#include "Gp_TimerDef.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// 定时器 / 计数器初始化
void Gp_TimerInit(void);
// 定时器 / 计数器中断初始化
void Gp_TimerInterruptInit(void);
// 中断服务函数
void Gp_TimerIsr(void);

#endif  /* _GP_TIMER_H_ */
