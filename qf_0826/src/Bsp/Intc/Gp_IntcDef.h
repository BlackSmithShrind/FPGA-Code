/*@file     Gp_IntcDef.h
* @brief    Interrupt Controller (Intc) 模块def文件，一个中断实例最多控制32路中断
* @details  通过该文件可以定义一些define、enum、struct等
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/07
*/
#ifndef _GP_INTC_DEF_H_
#define _GP_INTC_DEF_H_

#include "../../Base/Comm/Gp_Common.h"

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/
// define XIN_CONTROLLER_MAX_INTRS	32  /* Each Controller has 32 interrupt pins */
/*
 * 定义2个中断设备ID
 */
#define GP_INTC_0_DEVICE_ID                         	XPAR_INTC_0_DEVICE_ID

 /*
  *  定义中断号(注意：等BSP库下来，更新下面枚举)
  */
#define GP_INTC_0_DMA0_FIFO0_MM2S_INTROUT_INT_ID        XPAR_INTC_0_AXIDMA_0_MM2S_INTROUT_VEC_ID              // EDFA中断号4
#define GP_INTC_0_DMA0_FIFO0_S2MM_INTROUT_INT_ID        XPAR_INTC_0_AXIDMA_0_S2MM_INTROUT_VEC_ID              // EDFA中断号4
#define GP_INTC_0_DMA0_FIFO1_MM2S_INTROUT_INT_ID        XPAR_INTC_0_AXIDMA_1_MM2S_INTROUT_VEC_ID              // EDFA中断号4
#define GP_INTC_0_DMA0_FIFO1_S2MM_INTROUT_INT_ID        XPAR_INTC_0_AXIDMA_1_S2MM_INTROUT_VEC_ID              // EDFA中断号4
#define GP_INTC_0_EDFA_UART0_INT_ID                     XPAR_INTC_0_UARTLITE_5_VEC_ID                         // EDFA 控制指令
#define GP_INTC_0_XM_UART1_INT_ID                       XPAR_INTC_0_UARTLITE_2_VEC_ID                         // 星敏控制指令
#define GP_INTC_0_CG_2_ZK_UART2_INT_ID                  XPAR_INTC_0_UARTLITE_4_VEC_ID                         // CG  To 主控板
#define GP_INTC_0_CG_2_TX_UART3_INT_ID                  XPAR_INTC_0_UARTNS550_0_VEC_ID                        // CG  To 通信板
#define GP_INTC_0_CG_2_FSM0_UART4_INT_ID                XPAR_INTC_0_UARTLITE_0_VEC_ID                         // CG To FSM0
#define GP_INTC_0_CG_2_FSM1_UART5_INT_ID                XPAR_INTC_0_UARTLITE_1_VEC_ID                         // CG To FSM1
#define GP_INTC_0_2_ZK_YKYC_UART6_INT_ID                XPAR_INTC_0_UARTLITE_3_VEC_ID                         // 与主控板 遥控遥测
#define GP_INTC_0_SPI_INT_ID                            XPAR_INTC_0_SPI_0_VEC_ID                              // SPI 中断
#define GP_INTC_0_CAN_INSTRUCTION_INT_ID                XPAR_INTC_0_UARTLITE_6_VEC_ID                         // CAN instruction
#define GP_INTC_13_CAN_BRAM_INT_ID                      XPAR_AXI_INTC_0_SYSTEM_IN13_CAN_BRAM_INTR             // CAN BRAM
#define GP_INTC_14_LVDS_BRAM0_INT_ID                    XPAR_AXI_INTC_0_SYSTEM_IN14_LVDS_DMA_INTR             // 平台LVDS BRAM0
#define GP_INTC_15_LVDS_DMA0_INT_ID                     XPAR_AXI_INTC_0_SYSTEM_IN15_TVV_AT_INTR               // 板温，5v,12v,apd 温度
#define GP_INTC_16_APD_EML_BOARD_INT_ID                 XPAR_AXI_INTC_0_SYSTEM_IN16_APD_CV_EML_TC_INTR        // APD\EML\板温电压电流中断 (APD:光功率、温度、电流、电压 EML:电流、温度 板温、电压检测：板温、电压5V、电压12V)
#define GP_INTC_17_INTERNAL_LVDS_BRAM1_INT_ID           XPAR_AXI_INTC_0_SYSTEM_IN17_ZKLVDS_DMA_INTR           // 主控req请求->lvds->通信fpga->intc17->通信ps，通信构造apd数据包，写入bram
#define GP_INTC_DEBUG_UART_INT_ID                  	    XPAR_AXI_INTC_0_UART_AXI_UARTLITE_6_INTERRUPT_INTR    // UART_7 中断


#endif  /* _GP_UART_DEF_H_ */
