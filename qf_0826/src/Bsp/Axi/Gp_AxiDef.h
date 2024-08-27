/*@file     Gp_AxiDef.h
* @brief    软核与FPGA通过AXI-REG总线接口进行数据交互，这里设置了相关的寄存器
* @details  定义相关寄存器
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/09
*/
#ifndef _GP_AXI_DEF_H_
#define _GP_AXI_DEF_H_

#include "../../Base/Comm/Gp_Common.h"

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/

#define GP_AXI_BASE_REG 0x44A90000 // AXI-REG总线1基地址
/*****************************************************************************/
/* 握手信号相关寄存器设置                                              */
/*****************************************************************************/
// pl与ps之间的lvds apd pkg req和ack握手信号寄存器地址偏移量
#define GP_AIX_PL_2_PS_APD_PKG_HANDSHAKE_SIGNAL_OFFSET 0x0100
// pl与ps之间的lvds apd pkg req和ack握手信号寄存器地址
#define GP_AIX_PL_2_PS_APD_PKG_HANDSHAKE_SIGNAL_REG_ADDR (GP_AXI_BASE_REG + GP_AIX_PL_2_PS_APD_PKG_HANDSHAKE_SIGNAL_OFFSET)

/*****************************************************************************/
/* 板温、电压检测相关寄存器设置                                              */
/*****************************************************************************/
// 板温寄存器地址偏移量
#define GP_AIX_BOARD_TEMP_VOL_DET_OFFSET 0x0200
// 电压5v寄存器地址偏移量
#define GP_AIX_VOL_5V_OFFSET 0x0204
// 电压12v寄存器地址偏移量
#define GP_AIX_VOL_12V_OFFSET 0x0208
// 板温类型寄存器地址
#define GP_AIX_BOARD_TEMP_VOL_DET_REG_ADDR (GP_AXI_BASE_REG + GP_AIX_BOARD_TEMP_VOL_DET_OFFSET)
// 电压5v寄存器地址
#define GP_AIX_VOL_5V_REG_ADDR (GP_AXI_BASE_REG + GP_AIX_VOL_5V_OFFSET)
// 电压12v寄存器地址
#define GP_AIX_VOL_12V_REG_ADDR (GP_AXI_BASE_REG + GP_AIX_VOL_12V_OFFSET)

/*****************************************************************************/
/* APD相关寄存器设置                                                         */
/*****************************************************************************/
// 光功率接收寄存器地址偏移量
#define GP_AIX_APD_OPTICAL_POWER_RX_OFFSET 0x0210
// 温度采集寄存器地址偏移量
#define GP_AIX_APD_TEMP_COL_OFFSET 0x0214
// 电流采集寄存器地址偏移量
#define GP_AIX_APD_CUR_COL_OFFSET 0x0218
// 电压采集寄存器地址偏移量
#define GP_AIX_APD_VOL_COL_OFFSET 0x021C
// 光功率接收寄存器地址
#define GP_AIX_APD_OPTICAL_POWER_RX_REG_ADDR (GP_AXI_BASE_REG + GP_AIX_APD_OPTICAL_POWER_RX_OFFSET)
// 温度采集寄存器地址
#define GP_AIX_APD_TEMP_COL_REG_ADDR (GP_AXI_BASE_REG + GP_AIX_APD_TEMP_COL_OFFSET)
// 电流采集寄存器地址
#define GP_AIX_APD_CUR_COL_REG_ADDR (GP_AXI_BASE_REG + GP_AIX_APD_CUR_COL_OFFSET)
// 电压采集寄存器地址
#define GP_AIX_APD_VOL_COL_REG_ADDR (GP_AXI_BASE_REG + GP_AIX_APD_VOL_COL_OFFSET)

/*****************************************************************************/
/* EML相关寄存器设置                                                         */
/*****************************************************************************/
// 温度采集寄存器地址偏移量
#define GP_AIX_EML_CUR_COL_OFFSET 0x0220
// 电流采集寄存器地址偏移量
#define GP_AIX_EML_VOL_COL_OFFSET 0x0224
// 温度采集寄存器地址
#define GP_AIX_EML_CUR_COL_REG_ADDR (GP_AXI_BASE_REG + GP_AIX_EML_CUR_COL_OFFSET)
// 电流采集寄存器地址
#define GP_AIX_EML_VOL_COL_REG_ADDR (GP_AXI_BASE_REG + GP_AIX_EML_CUR_COL_OFFSET)

/****************************************************************************
 * 主要功能	                字段名称	        偏移地址	长度（字节）	备注
 * CAN总线-遥控指令单帧	    数据类型	        0x0050	1	                遥控：0x0A单帧 0xFH复帧
 *                          数据编号	        0x0051	1
 *                          帧序号	            0x0054	4
 *                          指令码 + 遥控内容	0x0058	1
 *                          校验和	            0x005C	1
****************************************************************************/

/*****************************************************************************/
/* CAN遥控遥测相关寄存器设置                                                 */
/*****************************************************************************/
// 数据类型寄存器地址偏移量
#define GP_AIX_CAN_BUS_YKYC_SINGLE_DATA_TYPE_OFFSET 0x0050
// 数据编号地址偏移量
#define GP_AIX_CAN_BUS_YKYC_SINGLE_DATA_NUM_OFFSET 0x0051
// 帧序号寄存器地址偏移量
#define GP_AIX_CAN_BUS_YKYC_SINGLE_FRAME_NUM_OFFSET 0x0054
// 指令码+遥控内容寄存器地址偏移量
#define GP_AIX_CAN_BUS_YKYC_SINGLE_CMD_CONTENT_OFFSET 0x0058
// 校验和寄存器地址偏移量
#define GP_AIX_CAN_BUS_YKYC_SINGLE_CHECK_SUM_OFFSET 0x005C

// 数据类型寄存器地址
#define GP_AIX_CAN_BUS_YKYC_SINGLE_DATA_TYPE_REG_ADDR (GP_AXI_BASE_REG + GP_AIX_CAN_BUS_YKYC_SINGLE_DATA_TYPE_OFFSET)
// 数据编号寄存器地址
#define GP_AIX_CAN_BUS_YKYC_SINGLE_DATA_NUM_REG_ADDR (GP_AXI_BASE_REG + GP_AIX_CAN_BUS_YKYC_SINGLE_DATA_NUM_OFFSET)
// 帧序号寄存器地址
#define GP_AIX_CAN_BUS_YKYC_SINGLE_FRAME_NUM_REG_ADDR (GP_AXI_BASE_REG + GP_AIX_CAN_BUS_YKYC_SINGLE_FRAME_NUM_OFFSET)
// 指令码+遥控内容寄存器地址
#define GP_AIX_CAN_BUS_YKYC_SINGLE_CMD_CONTENT_REG_ADDR (GP_AXI_BASE_REG + GP_AIX_CAN_BUS_YKYC_SINGLE_CMD_CONTENT_OFFSET)
// 校验和寄存器地址
#define GP_AIX_CAN_BUS_YKYC_SINGLE_CHECK_SUM_REG_ADDR (GP_AXI_BASE_REG + GP_AIX_CAN_BUS_YKYC_SINGLE_CHECK_SUM_OFFSET)






/****************************************************************************
 * 主要功能	                字段名称	        偏移地址	长度（字节）	备注
 * 重构固化帧                 源地址	     0x0000	        2
 *                         目的地址	        0x0004	        2
 *                        子包数目	        0x0008	        2
 *                       子包序号	        0x000C	        2
 *                    数据包报文编码	    0x0010	        2
 *                      flash内分区号	    0x0014	        2
 *                          Flash编号	    0x0018	        2
 *                          校验码	        0x001C	        2
*                       固化完成标志	    0x0020	        2
****************************************************************************/

// 重构固化帧源地址寄存器地址偏移量
#define GP_AIX_CG_FIXED_FRAME_SOURCE_ADDR_OFFSET 0x0000
// 重构固化帧目的地址寄存器地址偏移量
#define GP_AIX_CG_FIXED_FRAME_DEST_ADDR_OFFSET 0x0004
// 重构固化帧子包数目寄存器地址偏移量
#define GP_AIX_CG_FIXED_FRAME_SUB_PACKAGE_NUM_OFFSET 0x0008
// 重构固化帧子包序号寄存器地址偏移量
#define GP_AIX_CG_FIXED_FRAME_SUB_PACKAGE_SEQ_OFFSET 0x000C
// 重构固化帧数据包报文编码寄存器地址偏移量
#define GP_AIX_CG_FIXED_FRAME_DATA_PACKAGE_CODE_OFFSET 0x0010
// 重构固化帧flash内分区号寄存器地址偏移量
#define GP_AIX_CG_FIXED_FRAME_FLASH_PARTITION_NUM_OFFSET 0x0014
// 重构固化帧Flash编号寄存器地址偏移量
#define GP_AIX_CG_FIXED_FRAME_FLASH_NUM_OFFSET 0x0018
// 重构固化帧校验码寄存器地址偏移量
#define GP_AIX_CG_FIXED_FRAME_CHECK_CODE_OFFSET 0x001C
// 重构固化帧固化完成标志寄存器地址偏移量
#define GP_AIX_CG_FIXED_FRAME_FIXED_FINISH_FLAG_OFFSET 0x0020 //输出












/*****************************************************************************/
/* 中断合成接口模块相关寄存器设置                                                         */
/*****************************************************************************/
// 中断信号输出使能地址偏移量
#define IOER_OFFSET 0x800
// 中断有效寄存器地址偏移量
#define IER_OFFSET 0x802
// 中断标志寄存器地址偏移量
#define IFR_OFFSET 0x804
// 中断输出使能寄存器（中断使能0-1位，对应中断输出信号level_o_1和level_o_2）
#define SOC_EMIFA_CS2_IOER_REG_ADDR (SOC_EMIFA_CS2_ADDR + IOER_OFFSET)
// 中断有效寄存器
#define SOC_EMIFA_CS2_IER_REG_ADDR (SOC_EMIFA_CS2_ADDR + IER_OFFSET)
// 中断标志寄存器
#define SOC_EMIFA_CS2_IFR_REG_ADDR (SOC_EMIFA_CS2_ADDR + IFR_OFFSET)
// bit:0 中断输出使能标志位(中断使能0-1位，对应中断输出信号level_o_1和level_o_2)
#define SOC_EMIFA_CS2_IOER_0_EN_MASK 0x0000
// bit:1 中断输出使能标志位
#define SOC_EMIFA_CS2_IOER_1_EN_MASK 0x0001
// 中断输出使能位0~7bit 中断使能掩码位
// bit:0 cana总线读中断标志位
#define SOC_EMIFA_CS2_IER_CAN_A_R_MASK 0x0001
// bit:1 cana总线写中断标志位
#define SOC_EMIFA_CS2_IER_CAN_A_W_MASK 0x0002
// bit:0 canb总线读中断标志位
#define SOC_EMIFA_CS2_IER_CAN_B_R_MASK 0x0004
// bit:1 canb总线写中断标志位
#define SOC_EMIFA_CS2_IER_CAN_B_W_MASK 0x0008
// bit:2 rs422读中断标志位
#define SOC_EMIFA_CS2_IER_RS422_R_MASK 0x0010
// bit:3 rs422写中断标志位
#define SOC_EMIFA_CS2_IER_RS422_W_MASK 0x0020
// bit:4 irccd rs422读中断标志位
#define SOC_EMIFA_CS2_IER_IRCCD_RS422_R_MASK 0x0040
// bit:5 irccd rs422写中断标志位
#define SOC_EMIFA_CS2_IER_IRCCD_RS422_W_MASK 0x0080
// bit:6 fpa rs422读中断标志位
#define SOC_EMIFA_CS2_IER_FPA_RS422_R_MASK 0x0100
// bit:7 fpa rs422写中断标志位
#define SOC_EMIFA_CS2_IER_FPA_RS422_W_MASK 0x0200
// bit:8 cpa rs422读中断标志位
#define SOC_EMIFA_CS2_IER_CPA_RS422_R_MASK 0x0400
// bit:9 cpa rs422写中断标志位
#define SOC_EMIFA_CS2_IER_CPA_RS422_W_MASK 0x0800
// 中断输出标志位0~7bit 中断标识掩码位
// bit:0 cana总线读中断标志位
#define SOC_EMIFA_CS2_IFR_CAN_A_R_MASK 0x0001
// bit:1 cana总线写中断标志位
#define SOC_EMIFA_CS2_IFR_CAN_A_W_MASK 0x0002
// bit:0 canb总线读中断标志位
#define SOC_EMIFA_CS2_IFR_CAN_B_R_MASK 0x0004
// bit:1 canb总线写中断标志位
#define SOC_EMIFA_CS2_IFR_CAN_B_W_MASK 0x0008
// bit:2 rs422读中断标志位
#define SOC_EMIFA_CS2_IFR_RS422_R_MASK 0x0010
// bit:3 rs422写中断标志位
#define SOC_EMIFA_CS2_IFR_RS422_W_MASK 0x0020
// bit:4 irccd rs422读中断标志位
#define SOC_EMIFA_CS2_IFR_IRCCD_RS422_R_MASK 0x0040
// bit:5 irccd rs422写中断标志位
#define SOC_EMIFA_CS2_IFR_IRCCD_RS422_W_MASK 0x0080
// bit:6 fpa rs422读中断标志位
#define SOC_EMIFA_CS2_IFR_FPA_RS422_R_MASK 0x0100
// bit:7 fpa rs422写中断标志位
#define SOC_EMIFA_CS2_IFR_FPA_RS422_W_MASK 0x0200
// bit:8 cpa rs422读中断标志位
#define SOC_EMIFA_CS2_IFR_CPA_RS422_R_MASK 0x0400
// bit:9 cpa rs422写中断标志位
#define SOC_EMIFA_CS2_IFR_CPA_RS422_W_MASK 0x0800

/*****************************************************************************/
/* SDRAM寄存器地址                                                                                                                                                              */
/*****************************************************************************/
// SDRAM寄存器地址
#define SOC_EMIFA_CS2_SDRAM_MEMORY_ADDR 0x40000100
#define SOC_EMIFA_CS2_SDRAM_MEMORY_ADDR_TEST 0x40000300

/*****************************************************************************/
/* 其他宏                                                                                                                                                                                                                  */
/*****************************************************************************/
// 半字拼接
#define HALF_WORD_CONSTRUCT(x, y) (x | y << 8)
// SDRAM寄存器地址读写
#define SOC_EMIFA_CS2_SDRAM_MEMORY_U8(x) *(volatile unsigned char *)(SOC_EMIFA_CS2_SDRAM_MEMORY_ADDR + x)
// SDRAM寄存器地址读写
#define SOC_EMIFA_CS2_SDRAM_MEMORY_U16(x) *(volatile unsigned short *)(SOC_EMIFA_CS2_SDRAM_MEMORY_ADDR + x)

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
//unsigned short emif_wbuffer[EMIF_BUFFER_LENGTH];
//unsigned short emif_rbuffer[EMIF_BUFFER_LENGTH];

#endif  // _GP_AXI_DEF_H_
