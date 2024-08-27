/*@file     Gp_DdrDef.h
* @brief    对mig_7series_v2.1(ddr)封装的中间层模块头文件
* @details  提供ddr引擎的相关定义一些define、enum、struct等
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/16
*/
#ifndef _GP_DDR_DEF_H_
#define _GP_DDR_DEF_H_

#include "../../Base/Comm/Gp_Common.h"

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/
// 设备ID
#define GP_MIG7SERIES_0_DEVICE_ID                   XPAR_MIG7SERIES_0_DEVICE_ID   // Canonical definitions for peripheral MIG_7SERIES_0
// DDR基地址
#define GP_MEM_DDR_BASE_ADDR                        XPAR_MIG7SERIES_0_BASEADDR
// 设置发送和传输buff地址
#define GP_TX_BUFFER_BASE_ADDR                     (GP_MEM_DDR_BASE_ADDR + 0x00100000)
#define GP_RX_BUFFER_BASE_ADDR                     (GP_MEM_DDR_BASE_ADDR + 0x00300000)
#define GP_RX_BUFFER_HIGH_ADDR                     (GP_MEM_DDR_BASE_ADDR + 0x004FFFFF)
#endif  // _GP_DDR_DEF_H_



/********************************测试************************************/
//定义重构数据地址
#define GP_MEM_DDR_BASE_ADDR	        XPAR_MIG7SERIES_0_BASEADDR   //0x80000000
#define GP_CG_DATA_BASE_ADDR	        (GP_MEM_DDR_BASE_ADDR)
#define CG_DATA_FRAME_PACKAGE 	        0x00001000 //TBD 512byte
#define CG_DATA_FRAME_OFFSET 	        0x00002000 //TBD 512byte
#define CG_DATA_DESTINATION_PART		0x05000000 //TBD

#define CG_DATA_BRAM_SOURCE		        XPAR_BRAM_0_AXI_BRAM_CTRL_1_S_AXI_BASEADDR //0x00020000U


//定义工程遥测数据
#define GCYC_DATA_BASE_ADDR		        (GP_MEM_DDR_BASE_ADDR + 0x20000000)   //0xA0000000
#define GCYC_DATA_FRAME_PACKAGE			0x00002000  //TBD

#define GCYC_DATA_BRAM_SOURCE 		    XPAR_BRAM_1_AXI_BRAM_CTRL_3_S_AXI_BASEADDR  //0x00010000
#define GCYC_DATA_BRAM_DESTINATION 	    (XPAR_BRAM_1_AXI_BRAM_CTRL_3_S_AXI_BASEADDR + 0x3fff) //0x00013fff

//定义CAN遥测数据地址
#define CAN_YC_DATA_BASE_ADDR		    (GP_MEM_DDR_BASE_ADDR + 0x40000000)   //0xC0000000
#define CAN_YC_DATA_FRAME_PACKAGE		0x00000008  //TBD

#define CAN_YC_DATA_BRAM_SOURCE 		XPAR_BRAM_2_AXI_BRAM_CTRL_3_S_AXI_BASEADDR  //0x10000000
#define CAN_YC_DATA_BRAM_DESTINATION	XPAR_BRAM_2_AXI_BRAM_CTRL_3_S_AXI_BASEADDR + 0x00000fff  //0x10000fff
