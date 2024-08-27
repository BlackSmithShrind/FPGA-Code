/*@file     GP_InitCheck.h
* @brief    上电初始化模块
* @details  dsp通过emif接口进行数据通信
* @author   huyanfeng
* @date     2024/1/10
*/
#ifndef _GP_INITCHECK_H_
#define _GP_INITCHECK_H_

#include "../../Base/Comm/Gp_Common.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// 初始化
int allInitCheck(void);

#endif  /* _GP_INITCHECK_H_ */
