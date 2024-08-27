/*@file     GP_WMM.h
* @brief    工作模式管理单元
* @details
* @author   huyanfeng
* @date     2024/1/9
*/
#ifndef _GP_WM_H_
#define _GP_WM_H_

#include "../../Base/Comm/Gp_Common.h"
#include "../CMD/Gp_CanCmd.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// 初始化
int WM_Init(void);
// 模式改变接口
void ChangeWorkMode(emWorkMode unWorkMode);

#endif  /* _GP_WM_H_ */
