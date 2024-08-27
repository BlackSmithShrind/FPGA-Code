/*@file     Gp_GpioDef.h
* @brief    gpio def文件
* @details  通过该文件可以定义一些define、enum、struct等
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/13
*/
#ifndef _GP_GPIO_DEF_H_
#define _GP_GPIO_DEF_H_

#include "../../Base/Comm/Gp_Common.h"                // 通用头文件

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/
// 定义通道
#define GP_GPIO_CHANNEL_0                                  1
// 定位掩码位位宽
#define GP_GPIO_BITWIDTH                                   16 
// 设备ID
#define GP_GPIO_0_DEVICE_ID	                                XPAR_GPIO_0_DEVICE_ID
// 掩码位
#define GP_CNA_DIR1_GPIO_BITWIDTH_MASK                      0x00000001      // GPIO[0]	CAN_DIR1
#define GP_CNA_DIR2_GPIO_BITWIDTH_MASK                      0x00000002      // GPIO[1]	CAN_DIR2
#define GP_CAN_STB_GPIO_BITWIDTH_MASK                       0x00000004      // GPIO[2]	CAN_STB
#define GP_EDFA_POWER_GPIO_BITWIDTH_MASK                    0x00000008      // GPIO[3]	EDFA 供电
#define GP_XM_POWER_GPIO_BITWIDTH_MASK                      0x00000010      // GPIO[4]	星敏 供电
#define GP_FSM_POWER_GPIO_BITWIDTH_MASK                     0x00000020      // GPIO[5]	电磁驱动供电
#define GP_EML_TEST_TEMP_EN_GPIO_BITWIDTH_MASK              0x00000040      // GPIO[6]	EML测温使能
#define GP_EML_AMP_EN_GPIO_BITWIDTH_MASK                    0x00000080      // GPIO[7]	EML电流使能
#define GP_CG_RX_U5_GPIO_BITWIDTH_MASK                      0x00000100      // GPIO[8]	CG_RX_U5
#define GP_CG_TX_U13_GPIO_BITWIDTH_MASK                     0x00000200      // GPIO[9]	CG_TX_U13
#define GP_TMTC_RX_U9_GPIO_BITWIDTH_MASK                    0x00000400      // GPIO[10]	TMTC_RX_U9
#define GP_TMTC_TX_U7_GPIO_BITWIDTH_MASK                    0x00000800      // GPIO[11]	TMTC_TX_U7
#define GP_YCYK_EDFA_PPS_XM_RX_U8_GPIO_BITWIDTH_MASK        0x00001000      // GPIO[12]	YCYK_EDFA_PPS_XM_RX_U8
#define GP_YCYK_EDFA_PPS_XM_TX_U11_GPIO_BITWIDTH_MASK       0x00002000      // GPIO[13]	YCYK_EDFA_PPS_XM_TX_U11
#define GP_P_LVDS_RX_U65_GPIO_BITWIDTH_MASK                 0x00004000      // GPIO[14]	P_LVDS_RX_U65
#define GP_P_LVDS_TX_U66_GPIO_BITWIDTH_MASK                 0x00008000      // GPIO[15]	P_LVDS_TX_U66

#endif  // _GP_GPIO_DEF_H_
