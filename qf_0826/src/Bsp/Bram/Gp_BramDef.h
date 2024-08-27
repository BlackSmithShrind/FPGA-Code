/*@file     Gp_BramDef.h
* @brief    对bram_v4.2封装的中间层模块头文件
* @details  提供bram引擎的相关定义一些define、enum、struct等
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/22
*/
#ifndef _GP_BRAM_DEF_H_
#define _GP_BRAM_DEF_H_

#include "../../Base/Comm/Gp_Common.h"

/*****************************************************************************/
/*              macros, enums, and structure definitions                    */
/*****************************************************************************/
/*****************************************************************************/
/* bram寄存器地址段                                                          */
/*****************************************************************************/
// XPAR_BRAM0基地址
#define GP_XPAR_BRAM0_BASE_ADDR                             XPAR_BRAM_0_BASEADDR
// XPAR_BRAM1基地址
#define GP_XPAR_BRAM1_BASE_ADDR                             XPAR_BRAM_1_BASEADDR
// XPAR_BRAM2基地址
#define GP_XPAR_BRAM2_BASE_ADDR                             XPAR_BRAM_2_BASEADDR

/*****************************************************************************/
/* apd功率数据包相关bram寄存器                                               */
/*****************************************************************************/
// apd power pkg整段地址偏移量
#define GP_XPAR_BRAM0_APD_POWER_PKG_OFFSET                  0x0002
// apd power pkg基地址
#define GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR               (GP_XPAR_BRAM0_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_OFFSET) // 起始地址

// apd power pkg起始码地址偏移量
#define GP_XPAR_BRAM0_APD_POWER_PKG_START_FLAG_OFFSET       0x0002
// apd power pkg数据包有效数据长度地址偏移量
#define GP_XPAR_BRAM0_APD_POWER_PKG_DATA_LEN_OFFSET         0x0002
// apd power pkg源地址偏移量
#define GP_XPAR_BRAM0_APD_POWER_PKG_SRC_OFFSET              0x0002
// apd power pkg目的地址偏移量
#define GP_XPAR_BRAM0_APD_POWER_PKG_DES_OFFSET              0x0002
// apd power pkg子包数目地址偏移量
#define GP_XPAR_BRAM0_APD_POWER_PKG_SUB_PKG_NUM_OFFSET      0x0002
// apd power pkg子包序列地址偏移量
#define GP_XPAR_BRAM0_APD_POWER_PKG_SUB_PKG_SEQ_OFFSET      0x0002
// apd power pkg数据地址偏移量
#define GP_XPAR_BRAM0_APD_POWER_PKG_DATA_OFFSET             0x0004
// apd power pkg校验和地址偏移量
#define GP_XPAR_BRAM0_APD_POWER_PKG_SUM_OFFSET              0x0002
// apd power pkg起始码地址
#define GP_XPAR_BRAM0_APD_POWER_PKG_START_FLAG_ADDR         (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_START_FLAG_OFFSET)
// apd power pkg 数据包有效数据长度地址地址
#define GP_XPAR_BRAM0_APD_POWER_PKG_DATA_LEN_ADDR           (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_DATA_LEN_OFFSET)
// apd power pkg 源地址
#define GP_XPAR_BRAM0_APD_POWER_PKG_SRC_ADDR                (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_SRC_OFFSET)
// apd power pkg 目的地址
#define GP_XPAR_BRAM0_APD_POWER_PKG_DES_ADDR                (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_DES_OFFSET)
// apd power pkg 子包数目地址
#define GP_XPAR_BRAM0_APD_POWER_PKG_SUB_PKG_NUM_ADDR        (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_SUB_PKG_NUM_OFFSET)
// apd power pkg 子包序列地址
#define GP_XPAR_BRAM0_APD_POWER_PKG_SUB_PKG_SEQ_ADDR        (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_SUB_PKG_SEQ_OFFSET)
// apd power pkg 数据地址
#define GP_XPAR_BRAM0_APD_POWER_PKG_DATA_ADDR               (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_DATA_OFFSET)
// apd power pkg 校验和地址
#define GP_XPAR_BRAM0_APD_POWER_PKG_SUM_ADDR                (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_SUM_OFFSET)

#endif  // _GP_DDR_DEF_H_
