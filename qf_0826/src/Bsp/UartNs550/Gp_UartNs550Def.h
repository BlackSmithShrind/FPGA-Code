/*@file     Gp_UartNs550.h
* @brief    uartns550 def文件
* @details  通过该文件可以定义一些define、enum、struct等
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/14
*/
#ifndef _GP_UARTNS550_DEF_H_
#define _GP_UARTNS550_DEF_H_

#include "xparameters.h"
#include "xuartns550_l.h"
#include "xuartns550.h"

#include "../../Base/Comm/Gp_Common.h"

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/
// rs644接口
#define GP_CG_2_ZK_UARTNS550_0_DEVICE_ID       XPAR_UARTNS550_0_DEVICE_ID   // A3PE3000(FPGA)重构主控板程序（fpga+软核）的重构数据使用RS644接口:A3PE3000 -> RS644 -> 主控板
#define GP_CG_2_TX_UARTNS550_1_DEVICE_ID       XPAR_UARTNS550_1_DEVICE_ID   // A3PE3000(FPGA)重构通信板程序（fpga+软核）的重构数据使用RS644接口:A3PE3000 -> RS644 -> 通信板

// 波特率
#define GP_CG_2_ZK_UARTNS550_0_RS644_BAUDRATE_921600    921600              // 921,600bps

#endif  // _GP_UARTNS550_DEF_H_
