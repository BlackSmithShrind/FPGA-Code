/*@file     Gp_UartLite.c
* @brief    UartLite涓插彛妯″潡瀹炵幇鏂囦欢
* @details  鎻愪緵UartLite涓插彛绫荤殑鐩稿叧鎿嶄綔
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/07
*/

#include "Gp_UartLite.h"
#include "../Intc/Gp_Intc.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
XUartLite gEdfaRs422UartLiteInst;                                            // 婵�鍏夊櫒EDFA 鎺у埗鎸囦护
XUartLite gXmRs422UartLiteInst;                                              // 鏄熸晱鎺у埗鎸囦护
XUartLite gCg2Fsm0Rs422UartLiteInst;                                         // CG To FSM0
XUartLite gCg2Fsm1Rs422UartLiteInst;                                         // CG To FSM1
XUartLite gYkYc2ZkRs422UartLiteInst;                                         // 涓庝富鎺ф澘 閬ユ帶閬ユ祴
XUartLite gDebugUartLiteInst;                                           	 // DEBUG uart
                                                                            
volatile unsigned int gTotalSentCount = 0;
volatile unsigned int gTotalRecvCount = 0;

/****************************************************************************
 *             鍒濆鍖栵紙澶栭儴鎺ュ彛璋冪敤锛�
 * pUartLiteInst:
 * usUartLiteDeviceId:device id
 * pSendCbHandler:
 * pRecvCbHandler:
 ****************************************************************************/
int uartLiteInit(XIntc* pIntcInst, XUartLite* pUartLiteInst, unsigned short usUartLiteDeviceId, unsigned short usUartLiteIntrId, XUartLite_Handler pSendCbHandler, XUartLite_Handler pRecvCbHandler)
{
    int nRet = SEC_ERROR;

    // 鍙傛暟妫�鏌�
    if (!pUartLiteInst || !pIntcInst)
    {
        return SEC_ERROR;
    }

    // 鍒濆鍖朥artLiteInst椹卞姩绋嬪簭
    nRet = uartLiteInitialize(pUartLiteInst, usUartLiteDeviceId);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 鎵ц鑷浠ョ‘淇濈‖浠舵瀯寤烘纭�
    nRet = uartLiteSelfTest(pUartLiteInst);
    if (nRet != XST_SUCCESS) 
    {
        return SEC_ERROR;
    }

    // 杩炴帴鍒颁腑鏂瓙绯荤粺锛屼互渚夸腑鏂彲浠ュ彂鐢�
    nRet = uartLiteSetupInterruptSystem(pIntcInst, usUartLiteIntrId, pUartLiteInst);
    if (nRet != XST_SUCCESS) 
    {
        return SEC_ERROR;
    }

    // 璁剧疆鍙戦�佹暟鎹洖璋冨嚱鏁版帴鍙�
    nRet = uartLiteSetSendHandler(pUartLiteInst, pSendCbHandler, pUartLiteInst);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 璁剧疆鎺ユ敹鏁版嵁鍥炶皟鍑芥暟鎺ュ彛
    nRet = uartLiteSetRecvHandler(pUartLiteInst, pRecvCbHandler, pUartLiteInst);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 使能中断
    nRet = uartLiteEnableInterrupt(pUartLiteInst);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}


/****************************************************************************
 *  鍒濆鍖�
 * pUartLiteInst:
 * usDeviceId:
 ****************************************************************************/
int uartLiteInitialize(XUartLite* pUartLiteInst, unsigned short usDeviceId)
{
    // 鍙傛暟妫�鏌�
    if (!pUartLiteInst)
    {
        return SEC_ERROR;
    }

    // 瀹炰緥瀵硅薄鍒濆鍖�
    int nRet = XUartLite_Initialize(pUartLiteInst, usDeviceId);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *  鑷
 * pUartLiteInst:
 ****************************************************************************/
int uartLiteSelfTest(XUartLite* pUartLiteInst)
{
    // 鍙傛暟妫�鏌�
    if (!pUartLiteInst)
    {
        return SEC_ERROR;
    }

    // 鑷
    int nRet = XUartLite_SelfTest(pUartLiteInst);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *              鎸傛帴涓柇瀛愮郴缁�
 * pIntcInst:
 * usUartLiteIntrId:
 * pCallBackRef:
 ****************************************************************************/
int uartLiteSetupInterruptSystem(XIntc* pIntcInst, unsigned short usUartLiteIntrId, void* pCallBackRef)
{
    // 鍙傛暟妫�鏌�
    if (pIntcInst == NULL)
    {
        return SEC_ERROR;
    }

    // 杩炴帴涓柇瀛愮郴缁�
    int nRet = intcConnect(pIntcInst, usUartLiteIntrId, (XInterruptHandler)XUartLite_InterruptHandler, (void *)pCallBackRef);
    if (nRet != XST_SUCCESS) 
    {
        return SEC_ERROR;
    }

    //intcEnable(pIntcInst, usUartLiteIntrId);

    return SEC_SUCCESS;
}

/****************************************************************************
 *              璁剧疆鍙戦�佸洖璋冨嚱鏁�
 * pUartLiteInst:
 * pCbHandler:
 * pCallBackRef:
 ****************************************************************************/
int uartLiteSetSendHandler(XUartLite* pUartLiteInst, XUartLite_Handler pCbHandler, void* pCallBackRef)
{
    // 鍙傛暟妫�鏌�
    if (pUartLiteInst == NULL)
    {
        return SEC_ERROR;
    }

    XUartLite_SetSendHandler(pUartLiteInst, pCbHandler, pCallBackRef);

    return SEC_SUCCESS;
}

/****************************************************************************
 *              璁剧疆鎺ユ敹鍥炶皟鍑芥暟
 * pUartLiteInst:
 * pCbHandler:
 * pCallBackRef:
 ****************************************************************************/
int uartLiteSetRecvHandler(XUartLite* pUartLiteInst, XUartLite_Handler pCbHandler, void* pCallBackRef)
{
    // 鍙傛暟妫�鏌�
    if (pUartLiteInst == NULL)
    {
        return SEC_ERROR;
    }

    XUartLite_SetRecvHandler(pUartLiteInst, pCbHandler, pCallBackRef);

    return SEC_SUCCESS;
}

/****************************************************************************
 *              浣胯兘涓柇
 * pUartLiteInst:
 ****************************************************************************/
int uartLiteEnableInterrupt(XUartLite* pUartLiteInst)
{
    // 鍙傛暟妫�鏌�
    if (pUartLiteInst == NULL)
    {
        return SEC_ERROR;
    }

    XUartLite_EnableInterrupt(pUartLiteInst);

    return SEC_SUCCESS;
}

/****************************************************************************
 *              绂佺敤涓柇
 * pUartLiteInst:
 ****************************************************************************/
int uartLiteDisableInterrupt(XUartLite* pUartLiteInst)
{
    // 鍙傛暟妫�鏌�
    if (pUartLiteInst == NULL)
    {
        return SEC_ERROR;
    }

    XUartLite_DisableInterrupt(pUartLiteInst);

    return SEC_SUCCESS;
}

/****************************************************************************
 *              寮傛鍙戦��
 * pUartLiteInst:
 * pDataBuffer:
 * unNumBytes:
 ****************************************************************************/
int uartLiteSend(XUartLite* pUartLiteInst, unsigned char* pDataBuffer, unsigned int unNumBytes)
{
    // 鍙傛暟妫�鏌�
    if (pUartLiteInst == NULL || pDataBuffer == NULL)
    {
        return SEC_ERROR;;
    }

    // 鍙戦�佹暟鎹�
    int unBytes = XUartLite_Send(pUartLiteInst, pDataBuffer, unNumBytes);

    return unBytes;
}

/****************************************************************************
 *              寮傛鎺ユ敹
 * pUartLiteInst:
 * pDataBuffer:
 * unNumBytes:
 ****************************************************************************/
int uartLiteRecv(XUartLite* pUartLiteInst, unsigned char* pDataBuffer, unsigned int unNumBytes)
{
    // 鍙傛暟妫�鏌�
    if (pUartLiteInst == NULL || pDataBuffer == NULL)
    {
        return SEC_ERROR;
    }

    // 鎺ユ敹鏁版嵁
    int unBytes = XUartLite_Recv(pUartLiteInst, pDataBuffer, unNumBytes);

    return unBytes;
}
