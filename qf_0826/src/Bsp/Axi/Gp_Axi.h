/*@file     Gp_Axi.h
* @brief    软核与FPGA通过AXI-REG总线接口进行数据交互，这里设置了相关的寄存器
* @details  定义相关寄存器
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/09
*/
#ifndef _GP_AXI_H_
#define _GP_AXI_H_

#include "Gp_AxiDef.h"
#include "../../Base/Comm/Gp_Common.h"

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/


/****************************************************************************/
/*              emif interface declaration                                  */
/****************************************************************************/

// AXI写寄存器
void axiWriteReg(unsigned int unRegAddr, unsigned int offset ,unsigned int unRegValue);
// AXI读寄存器
unsigned int axiReadReg(unsigned int unRegAddr, unsigned int offset);



#endif  // _GP_AXI_H_
