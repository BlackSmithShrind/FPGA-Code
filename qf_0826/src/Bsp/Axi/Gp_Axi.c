/*@file     Gp_Axi.cpp
* @brief    软核与FPGA通过AXI-REG总线接口进行数据交互，这里设置了相关的寄存器
* @details  定义相关寄存器
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/09
*/
#include "Gp_Axi.h"
/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/

// AXI写寄存器
void axiWriteReg(unsigned int unRegAddr, unsigned int offset ,unsigned int unRegValue)
{
    Xil_Out32(unRegAddr + offset, unRegValue);
}

// AXI读寄存器
unsigned int axiReadReg(unsigned int unRegAddr, unsigned int offset)
{
    return Xil_In32(unRegAddr + offset);
}
