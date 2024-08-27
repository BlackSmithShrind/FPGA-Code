/*@file     Gp_DmaDef.h
* @brief    dma def文件
* @details  通过该文件可以定义一些define、enum、struct等
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/15
*/
#ifndef _GP_DMA_DEF_H_
#define _GP_DMA_DEF_H_

#include "xaxidma.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xdebug.h"

#include "../../Base/Comm/Gp_Common.h"

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/
// 设备ID
#define GP_AXIDMA_0_DEVICE_ID           XPAR_AXIDMA_0_DEVICE_ID         // Canonical definitions for peripheral DMA0_FIFO0_AXI_DMA_0
#define GP_AXIDMA_1_DEVICE_ID           XPAR_AXIDMA_1_DEVICE_ID         // Canonical definitions for peripheral DMA0_FIFO0_AXI_DMA_1

//发送数据包包长度 0x200=512
#define GP_DMA_TRANSFER_MAX_PKT_LEN     0x200                           // dma传输的最大数据包长度
#define GP_DMA_RST_TX_TIMEOUT           1000                            // 复位时间ms
#define GP_DMA_RST_RX_TIMEOUT           1000                            // 复位时间ms


#endif  // _GP_DMA_DEF_H_
