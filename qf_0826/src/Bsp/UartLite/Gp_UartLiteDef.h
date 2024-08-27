/*@file     Gp_UartLiteDef.h
* @brief    UartLite def鏂囦欢
* @details  閫氳繃璇ユ枃浠跺彲浠ュ畾涔変竴浜沝efine銆乪num銆乻truct绛�,IP鏍歌繘琛岄厤缃殑鏃跺��115200kps,浣跨敤uartlite IP鍜屾潵瀹氫箟RS422涓插彛瀵硅薄
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/07
*/
#ifndef _GP_UARTLITE_DEF_H_
#define _GP_UARTLITE_DEF_H_

#include "xparameters.h"
#include "xuartlite.h"
#include "xintc.h"
#include "xil_exception.h"
#include "xil_printf.h"

#include "../../Base/Comm/Gp_Common.h"

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/
// rs422鎺ュ彛
#define GP_EDFA_UARTLITE_DEVICE_ID          XPAR_UARTLITE_0_DEVICE_ID       // 涓巈dfa澶栬RS422鎺ュ彛:婵�鍏夊櫒EDFA 鎺у埗鎸囦护
#define GP_XM_UARTLITE_DEVICE_ID            XPAR_UARTLITE_1_DEVICE_ID       // 涓巟m澶栬RS422鎺ュ彛:鏄熸晱鎺у埗鎸囦护
#define GP_CG_2_FSM0_UARTLITE_DEVICE_ID     XPAR_UARTLITE_3_DEVICE_ID       // 閲嶆瀯FSM0 RS422鎺ュ彛:CG To FSM0
#define GP_CG_2_FSM1_UARTLITE_DEVICE_ID     XPAR_UARTLITE_4_DEVICE_ID       // 閲嶆瀯FSM1 RS422鎺ュ彛:CG To FSM1
#define GP_2_ZK_YKYC_UARTLITE_DEVICE_ID     XPAR_UARTLITE_5_DEVICE_ID       // 涓庝富鎺ч仴鎺S422鎺ュ彛:涓庝富鎺ф澘 閬ユ帶閬ユ祴
#define GP_DEBUG_UARTLITE_DEVICE_ID         XPAR_UARTLITE_6_DEVICE_ID       // debug鎺ュ彛:DEBUG



#endif  // _GP_UARTLITE_DEF_H_
