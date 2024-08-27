/*@file     Gp_WatchDog.h
* @brief    看门狗定时器
* @details  放置dsp跑飞了进行reset重置
* @author   huyanfeng
* @date     2024/1/9
*/
#ifndef _GP_WATCH_DOG_H_
#define _GP_WATCH_DOG_H_

#include "Gp_WatchDogDef.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// 初始化
int Gp_WatchDog_Init(void);
// 定时器 / 计数器初始化
void Gp_WatchdogReactivate(void);

#endif  /* _GP_WATCH_DOG_H_ */
