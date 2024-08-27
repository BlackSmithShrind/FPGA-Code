/*@file     Gp_SpiDef.h
* @brief    对spi_v4_4(axi quad spi)封装的中间层模块头文件
* @details  提供spi引擎的相关定义一些define、enum、struct等
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/17
*/
#ifndef _GP_SPI_DEF_H_
#define _GP_SPI_DEF_H_

#include "../../Base/Comm/Gp_Common.h"

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/
// 设备ID
#define GP_SPI_0_DEVICE_ID              XPAR_SPI_0_DEVICE_ID   // Canonical definitions for peripheral AXI_QUAD_SPI_0

#define GP_XXXX_FLASH_TEST_ADDRESS	    0x259800                    //Address of the page to perform Erase, Write and Read operations
#endif  // _GP_SPI_DEF_H_
