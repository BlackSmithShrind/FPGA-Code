/*@file     Gp_TimerDef.h
* @brief	定时器
* @details  支持64位模式定时器，2个定时器实例，一个用于获取时间戳，一个用于中断
* @author   huyanfeng
* @date     2024/2/28
*/
#ifndef _GP_TIMER_DEF_H_
#define _GP_TIMER_DEF_H_

#include "../../Base/Comm/Gp_Common.h"

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/

#define TMR_PERIOD_LSB32_1_S  (0x0D970100)
// 高32位 0
#define TMR_PERIOD_MSB32_1_S  (0)

//这里需要说一下PSC，由于我们的定时器比较特殊，属于CPU系统内部相关控制模块，准确意义讲它并不是一个挂在总线上的外设，所以定时器不需要使能

#endif  /* _GP_TIMER_DEF_H_ */
