/*@file     Gp_Exception.c
* @brief    exception def文件
* @details  各种与异常相关的操作（即启用异常、禁用异常、注册异常处理程序等
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/19
*/
#include "Gp_Exception.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/

/****************************************************************************
 *  初始化(外部调用)
 ****************************************************************************/
int exceptionInit()
{
    Xil_ExceptionInit();

    return SEC_SUCCESS;
}

/****************************************************************************
 *  使能
 ****************************************************************************/
int exceptionEnable()
{
    Xil_ExceptionEnable();

    return SEC_SUCCESS;
}

/****************************************************************************
 *  禁用
 ****************************************************************************/
int exceptionDisable()
{
    Xil_ExceptionDisable();

    return SEC_SUCCESS;
}

/****************************************************************************
 *  注册异常handler
 ****************************************************************************/
int exceptionRegisterHandler(unsigned int unExceptionId, Xil_ExceptionHandler pExpHandler, void* pData)
{
    Xil_ExceptionRegisterHandler(unExceptionId, pExpHandler, pData);

    return SEC_SUCCESS;
}

/****************************************************************************
 *  移除异常handler
 ****************************************************************************/
int exceptionRemoveHandler(unsigned int unExceptionId)
{
    Xil_ExceptionRemoveHandler(unExceptionId);

    return SEC_SUCCESS;
}