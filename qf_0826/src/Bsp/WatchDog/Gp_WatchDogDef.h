/*@file     Gp_WatchDogDef.h
* @brief    看门狗定时器，程序异常时复位（上电复位power on reset）
* 			https://blog.51cto.com/u_16213569/9337166
* 			在创龙提供的程序中，程序都是跑在CPU默认的主频456MHz下，而所有的外设都是使用的PLL0_SYSCLK2，
* 			也就是说这些外设的输入时钟都是CPU的2分频也就是228MHz。对于我们的定时器来说，
* 			定时器0、1所使用的时候是旁路时钟也就是24MHz的时钟，
* 			而定时器2、3使用的是CPU2分频228MHz的输入时钟
* @details  定时器
* @author   huyanfeng
* @date     2024/2/26
*/
#ifndef _GP_WATCH_DOG_DEF_H_
#define _GP_WATCH_DOG_DEF_H_

#include "../../Base/Comm/Gp_Common.h"

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/
// 看门狗定时器周期
// 定时时间 5秒
// 低32位
// 这个值就是(5 * 24 * 1000 * 1000) 十六进制 0x07270E00
// 定时器1使用的是PLL旁路时钟也就是24MHz时钟
#define WD_TMR_PERIOD_LSB32_5_S  (5 * 24 * 1000 * 1000)//(0x07270E00)
// 高32位
#define WD_TMR_PERIOD_MSB32_5_S  (0x0)

#endif  /* _GP_WATCH_DOG_DEF_H_ */
