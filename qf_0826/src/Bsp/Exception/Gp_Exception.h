/*@file     Gp_Exception.h
* @brief    exception def文件
* @details  各种与异常相关的操作（即启用异常、禁用异常、注册异常处理程序等
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/19
*/
#ifndef _GP_EXCEPTION_H_
#define _GP_EXCEPTION_H_

#include "Gp_ExceptionDef.h"

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// 初始化（对外）
int exceptionInit();
// 使能
int exceptionEnable();
// 禁用
int exceptionDisable();
// 注册异常handler
int exceptionRegisterHandler(unsigned int unExceptionId, Xil_ExceptionHandler pExpHandler, void* pData);
// 移除异常handler
int exceptionRemoveHandler(unsigned int unExceptionId);

#endif  // _GP_EXCEPTION_H_
