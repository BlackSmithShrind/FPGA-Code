/*@file     Gp_Intc.c
* @brief    对xintc_v3.8(Interrupt Controller (Intc) )封装的中间层模块源文件
* @details  提供中断相关的操作
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/07
*/
#include "Gp_Intc.h"
#include "../../App/EDFA/Gp_Edfa.h"
#include "../../Bsp/Axi/Gp_Axi.h"
#include "../../App/XM/Gp_Xm.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
/************************** Variable Definitions ****************************/
// 定义中断控制器实例
XIntc gIntcInst0;                                                       // The instance of the Interrupt Controller 0

/****************************************************************************
 *              intc初始化(对外接口)
 * pIntcInst:
 * usDeviceId:
 ****************************************************************************/
int intcInit(XIntc* pIntcInst, unsigned short usDeviceId)
{
    // 参数检查
    if (!pIntcInst)
    {
        return SEC_ERROR;
    }

    // 中断初始化
    int nRet = intcInitialize(pIntcInst, usDeviceId);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 自检测硬件正确性
    nRet = intcSelfTest(pIntcInst);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 连接intc中断控制子系统 In12	CAN instruction
    nRet = intcConnect(pIntcInst, GP_INTC_0_CAN_INSTRUCTION_INT_ID, intcCanInstrHandler, (void*)0);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    // 连接intc中断控制子系统 In12	CAN instruction
    nRet = intcConnect(pIntcInst, GP_INTC_0_CAN_INSTRUCTION_INT_ID, intcCanInstrHandler, (void*)0);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *              中断初始化
 * pIntcInst:
 * usInstanceCount:                                            
 * usDeviceId:                                            
 * usDeviceIdCount: 
 ****************************************************************************/
int intcInitialize(XIntc* pIntcInst, unsigned short usDeviceId)
{
    // 参数检查
    if (pIntcInst == NULL)
    {
        return SEC_ERROR;
    }

    // 初始化
    int nRet = XIntc_Initialize(pIntcInst, usDeviceId);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *              自检测硬件正确性
 * pIntcInst:
 ****************************************************************************/
int intcSelfTest(XIntc* pIntcInst)
{
    // 参数检查
    if (pIntcInst == NULL)
    {
        return SEC_ERROR;
    }

    int nRet = XIntc_SelfTest(pIntcInst);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *              中断连接
 * pIntcInst:
 * usIntrId:
 * pHandler:
 * pCallBack:
 ****************************************************************************/
int intcConnect(XIntc* pIntcInst, unsigned short usIntrId, XInterruptHandler pHandler, void* pCallBack)
{
    // 参数检查
    if (pIntcInst == NULL)
    {
        return SEC_ERROR;
    }

    int nRet = XIntc_Connect(pIntcInst, usIntrId, pHandler, pCallBack);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *              中断断开
 * pIntcInst:
 * usIntrId:
 ****************************************************************************/
int intcDisconnect(XIntc* pIntcInst, unsigned short usIntrId)
{
    // 参数检查
    if (pIntcInst == NULL)
    {
        return SEC_ERROR;
    }

    XIntc_Disconnect(pIntcInst, usIntrId);
    
    return SEC_SUCCESS;
}

/****************************************************************************
 *              中断启用
 * pIntcInst:
 * ucMode:
 ****************************************************************************/
int intcStart(XIntc* pIntcInst, unsigned char ucMode)
{
    // 参数检查
    if (pIntcInst == NULL)
    {
        return SEC_ERROR;
    }

    int nRet = XIntc_Start(pIntcInst, ucMode);
    if (nRet != XST_SUCCESS)
    {
        return SEC_ERROR;
    }

    return SEC_SUCCESS;
}

/****************************************************************************
 *              中断停止
 * pIntcInst:
 * ucMode:
 ****************************************************************************/
int intcStop(XIntc* pIntcInst)
{
    // 参数检查
    if (pIntcInst == NULL)
    {
        return SEC_ERROR;
    }

    XIntc_Stop(pIntcInst);
    
    return SEC_SUCCESS;
}

/****************************************************************************
 *              中断使能
 * pIntcInst:
 * usIntrId:
 ****************************************************************************/
int intcEnable(XIntc* pIntcInst, unsigned char ucIntrId)
{
    // 参数检查
    if (pIntcInst == NULL)
    {
        return SEC_ERROR;
    }

    XIntc_Enable(pIntcInst, ucIntrId);
    
    return SEC_SUCCESS;
}

/****************************************************************************
 *              中断禁止
 * pIntcInst:
 * usIntrId:
 ****************************************************************************/
int intcDisable(XIntc* pIntcInst, unsigned char ucIntrId)
{
    // 参数检查
    if (pIntcInst == NULL)
    {
        return SEC_ERROR;
    }

    XIntc_Disable(pIntcInst, ucIntrId);
    
    return SEC_SUCCESS;
}


int hardwareintcEnable(XIntc* pIntcInst){
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)XIntc_InterruptHandler,
			(void *)pIntcInst);

	Xil_ExceptionEnable();
}




/****************************************************************************
 *              ss intc 响应函数（CAN instruction）
 * 卫星平台 -can-> 通信板PL -AXI-INTC AXI-REG->通信板PS
 * pCallbackRef:
 * 注意：先放这里后面统一调整
 ****************************************************************************/
void intcCanInstrHandler(void* pCallbackRef)
{
    unsigned char ucData1B = *(volatile unsigned char  *)GP_AIX_CAN_BUS_YKYC_SINGLE_DATA_TYPE_REG_ADDR;
    switch (ucData1B)
    {
        //---------------------------edfa 设备协议----------------------------------//
        case EDFA_YC_SINGLE_FRAME: // 遥测：0x00单帧
        {

        }break;
        case EDFA_YK_SINGLE_FRAME: // 遥控：0x0A单帧
        {
            if (!gEdfaIntcRxNewDataIsFin)
            {
                // 数据copy出来
                memset(gEdfaRxDataBuffer, 0, EDFA_RX_DATA_BUFFER_SIZE);
                memcpy((void*)gEdfaRxDataBuffer, (void*)GP_AIX_CAN_BUS_YKYC_SINGLE_DATA_TYPE_REG_ADDR, 8);

                // 设置标识
                gEdfaIntcRxNewDataIsFin = TRUE;
            }
        }break;
        case EDFA_YC_COMP_FRAME: // 遥测：0x3A复合帧
        {

        }break;
        case EDFA_YK_COMP_FRAME: // 遥控：0x0F复帧
        {

        }break;
        //---------------------------ss 设备协议----------------------------------//
        case XM_SF_HEADER1: // 遥控：0x0A单帧
        {
            unsigned char ucData2B = *(volatile unsigned char  *)GP_AIX_CAN_BUS_YKYC_SINGLE_DATA_TYPE_REG_ADDR;
            switch (ucData2B)
            {
                case XM_SF_YK_CMD_AND_ACK:
                {
                    if (!gXmIntcRxNewDataIsFin)
                    {
                        // 数据copy出来
                        memset(gXmRxDataBuffer, 0, XM_RX_DATA_BUFFER_SIZE);
                        memcpy((void*)gXmRxDataBuffer, (void*)GP_AIX_CAN_BUS_YKYC_SINGLE_DATA_TYPE_REG_ADDR, 8);

                        // 设置标识
                        gXmIntcRxNewDataIsFin = TRUE;
                    }
                }break;
                case XM_SF_YC_CMD_AND_ACK:
                {
                }break;
                case XM_SF_SELF_TEST_DOWN_TX_AND_ACK:
                {
                }break;
                case XM_SF_PROGRAM_UPLOAD_AND_ACK:
                {
                }break;
                default:
                {
                }break;
            }
            
        }break;
        default:
        {
        }break;
    }
}

/****************************************************************************
 *              CG_DMA intc 响应函数（CG instruction）
 * 卫星平台 -LVDS-> 通信板PL -AXI-INTC ->通信板PS
 * pCallbackRef:
 * 注意：先放这里后面统一调整
 ****************************************************************************/

void intcCG_saveInstrHandler()
{
	if(CG_save_ready == 1){
		CG_save_start = 1;
		CG_save_ready = 0;
	}
	else{
		CG_save_start = 0;
		CG_save_ready = 1;
	}
}


