/*@file     Gp_Common.h
* @brief    閸╄櫣顢呴柅姘辨暏婢跺瓨鏋冩禒锟�
* @details  闁俺绻冪拠銉︽瀮娴犺泛褰叉禒銉ョ暰娑斿绔存禍娌漞fine閵嗕躬num閵嗕够truct缁涳拷
* @author   huyanfeng
* @date     2023/11/15
*/
#ifndef _GP_COMMON_H_
#define _GP_COMMON_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

// C 鐠囶叀鈻�
#include <stdio.h>                      // C 鐠囶叀鈻堥弽鍥у櫙鏉堟挸鍙嗘潏鎾冲毉閸戣姤鏆熸惔锟�(printf)

// C 閺嶅洤鍣惔锟�
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "math.h"                        // C 閺佹澘顒熼崙鑺ユ殶鎼达拷
#include <time.h>                        // time
#include <string.h>                      // memset
#include <stdlib.h>
#include <math.h>


/***************************** Include Files *********************************/
// Xilinx bsp 婢跺瓨鏋冩禒锟�
#include "xparameters.h"
#include "xstatus.h"
#include "../../../../qf_bsp/microblaze_0/include/xil_exception.h"
#include "xil_printf.h"
#include "xil_types.h"
#include "xintc.h"
//#include "xuartns550_l.h"
//#include "xuartns550_i.h"
#include "xuartns550.h"
#include "xspi.h"
#include "xspi_l.h"

// 婵″倹鐏夐弰鐤�++閺傚洣娆㈤敍灞藉灟鐎规矮绠焈_cplusplus鐎癸拷
#if defined(__cplusplus) && !defined(_CPP_STYLE_HEADER)
#include <iostream>
using namespace std;
#endif /* ! _CPP_STYLE_HEADER */

/****************************************************************************/
/*                                                                          */
/*              macros, enums, and structure definitions                    */
/*                                                                          */
/****************************************************************************/
// 鐎规矮绠烠CS_USE_C_GEL_SCRIPT缂傛牞鐦х�瑰骏绱濋崚娆掕泲C鐠囶叀鈻堢粙瀣碍鏉╂稖顢戦柊宥囩枂,娑撳秴鐣炬稊澶婂灟鐠х檲CS缂傛牞鐦ч悳顖氼暔閻ㄥ嚕EL閼存碍婀版潻娑滎攽PLL\PSC\DDR\EMIF缁涘娈戦崚婵嗩潗閸栨牭绱�
/****************************************************************************/
/* 濞夈劍鍓伴敍姘愁嚉鐎瑰繑绉归崣濠傚煂Gp_Emif閸楁洖鍘撻妴涓焢_PLL閸楁洖鍘撻妴涓焢_PSC閸楁洖鍘撻妴涓焢_System_MMR閸楁洖鍘�   */
/*       閻╃鍙уΟ鈥虫健閻ㄥ嫬鍨垫慨瀣瀹搞儰缍旈敍灞剧厙娴滄稑宕熼崗鍐ㄥ嚒缂佸繒些闂勵棭omm閸楁洜瀚幋鎰彌閸楁洖鍘撳Ο鈥虫健         */
/****************************************************************************/
#ifndef CCS_USE_C_GEL_SCRIPT
//#define CCS_USE_C_GEL_SCRIPT
#endif

// 濞夈劍鍓伴敍姝婨LF_DEV_PAT_CCS_DSP_UART_FPGA閸滃瓕EV_PAT_CCS_DSP_UART_FPGA娑撳秷鍏橀崥灞炬闁棄鐣炬稊锟�
// 鐎规矮绠焈SELF_DEV_PAT_CCS_DSP_UART_FPGA_缂傛牞鐦х�瑰骏绱濋崚姗瓵T鐠х殟art娑撴彃褰涚拫鍐槸濡�崇础閿涘牐鍤滈惍鏃�婢�+CCS鐠嬪啳鐦敍锟�
#ifndef SELF_DEV_PAT_CCS_DSP_UART_FPGA
//#define SELF_DEV_PAT_CCS_DSP_UART_FPGA
#endif

// 鐎规矮绠烡EV_PAT_CCS_DSP_UART_FPGA缂傛牞鐦х�瑰骏绱濋崚姗瓵T鐠х殟art娑撴彃褰涚拫鍐槸濡�崇础閿涘牆绱戦崣鎴炴緲+CCS鐠嬪啳鐦敍锟�
#ifndef DEV_PAT_CCS_DSP_UART_FPGA
#define DEV_PAT_CCS_DSP_UART_FPGA
#endif

// 鐎规矮绠烺S422_OUTPUT缂傛牞鐦х�瑰骏绱濋崚娆掕泲娑撴彃褰涙潏鎾冲毉閿涘牆绱戦崣鎴炴緲閿涳拷
#ifndef RS422_OUTPUT
#define RS422_OUTPUT
#endif

// 鐎规矮绠烢MIF_OUTPUT缂傛牞鐦х�瑰骏绱濋崚娆掕泲EMIF鏉堟挸鍤敍鍫ｅ殰閻梹婢橀敍锟�
#ifndef EMIF_OUTPUT
#define EMIF_OUTPUT
#endif

// 鐎规矮绠熼懛顏嗙埡閺夛拷+PAT+CCD+FSM
#ifndef SELF_DEV_PAT_IRCCD_FSM_TEST
#define SELF_DEV_PAT_IRCCD_FSM_TEST
#endif

// 鐠囪褰囬悧鈺冩倞鐎靛嫬鐡ㄩ崳銊ユ勾閸э拷瀵版锟斤拷
#define HWREG(x)                         (*((volatile unsigned int *)(x)))

// 閺冨爼鎸�
#define SYSCLK_1_FREQ         (456000000)
#define SYSCLK_2_FREQ         (SYSCLK_1_FREQ/2)
#define UART_1_FREQ           (SYSCLK_2_FREQ)
#define UART_2_FREQ           (SYSCLK_2_FREQ)

// WatchDog 閺冨爼鎸�
//#define SYSCLK_1_FREQ_456M     (456000000)
//#define SYSCLK_2_FREQ_288M     (SYSCLK_1_FREQ_456M/2)
//#define UART_1_FREQ_288M       (SYSCLK_2_FREQ_288M)

//1閿涘灏濋悧鍦芳閿涳拷460800bps
#define BAUD_RATE_460800    460800
#define BAUD_RATE_57600     57600
#define BAUD_RATE_115200    115200

typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;

// The following types are guaranteed to be signed and 32 bits wide.
typedef signed int LONG32, *PLONG32;

// The following types are guaranteed to be unsigned and 32 bits wide.
typedef unsigned int ULONG32, *PULONG32;
typedef unsigned int DWORD32, *PDWORD32;

// 鐎规矮绠熺亸蹇曨伂
#define SYS_LITTLE_ENDIAN

//little endian
#ifdef SYS_LITTLE_ENDIAN
#define TO_NET_UINT16(t) (UINT16)((((t)&0xFF) << 8) | (((t)&0xFF00) >> 8))
#define TO_NET_UINT32(t) (UINT32)((((t)&0xFF) << 24) | (((t)&0xFF00) << 8) | (((t)&0xFF0000) >> 8) | (((t)&0xFF000000) >> 24))
#define NET_SWITCH_UINT16(t) (t = (((t)&0xFF) << 8) | (((t)&0xFF00) >> 8))
#define NET_SWITCH_UINT32(t) (t = (((t)&0xFF) << 24) | (((t)&0xFF00) << 8) | (((t)&0xFF0000) >> 8) | (((t)&0xFF000000) >> 24))
#define NET_SWITCH_INT64(t)                                                                         \
    t = (((t)&0xFF) << 56) | (((t)&0xFF00) << 40) | (((t)&0xFF0000) << 24) | (((t)&0xFF000000) << 8) | \
        (((t)&0xFF00000000) >> 8) | (((t)&0xFF0000000000) >> 24) |                                     \
        (((t)&0xFF000000000000) >> 40) | (((t)&0xFF00000000000000) >> 56)
#define NET_SWITCH_COPY4(data, src) \
    {                                  \
        char *d = (char *)(data);      \
        const char *s = (char *)(src); \
        d[0] = s[3];                   \
        d[1] = s[2];                   \
        d[2] = s[1];                   \
        d[3] = s[0];                   \
    }
#define NET_SWITCH_COPY2(data, src) \
    {                                  \
        char *d = (char *)(data);      \
        const char *s = (char *)(src); \
        d[0] = s[1];                   \
        d[1] = s[0];                   \
    }
#else
#define TO_NET_WORD(t) t
#define TO_NET_DWORD(t) t
#define NET_SWITCH_WORD(t)
#define NET_SWITCH_DWORD(t)
#define NET_SWITCH_COPY4(data, src) safeMemCopy(data, 4, src, 4)
#define NET_SWITCH_COPY2(data, src) safeMemCopy(data, 2, src, 2)
#endif

// 鐎靛嫬鐡ㄩ崳銊ユ勾閸э拷鐠囪鍟�
#define HW_8_REG(addr)                    	*(volatile int8_t *)(addr)
#define HW_U8_REG(addr)                    	*(volatile uint8_t *)(addr)
#define HW_16_REG(addr)                    	*(volatile int16_t *)(addr)
#define HW_U16_REG(addr)                    *(volatile uint16_t *)(addr)
#define HW_32_REG(addr)                    	*(volatile int32_t *)(addr)
#define HW_U32_REG(addr)                    *(volatile uint32_t *)(addr)
#define HW_REG_BIT(reg, bitnum)             *(volatile unsigned int *)((BITBAND(reg, bitnum)))

// 鐎瑰鍙忛柌濠冩杹malloc閸愬懎鐡ㄩ崷鏉挎絻
#define SAFE_FREE(ptr)                      if(ptr) {    free(ptr); ptr=0; }

// inline鐎瑰繐鐣炬稊锟�
#if defined (__GNUC__) || defined (__ICCARM__) || defined (__MICROBLAZE__)
#define INLINE inline
#else
#define INLINE __inline
#endif

// 缁崵绮洪柨娆掝嚖閻拷
enum emSystemErorrCode
{
    SEC_ERROR = -1,                            // 闁挎瑨顕�
    SEC_SUCCESS,                            // 閹存劕濮�

};

/* Definitions for interface M14_AXI_0 */
#define XPAR_M14_AXI_0_BASEADDR 0x44A90000
#define XPAR_M14_AXI_0_HIGHADDR 0x44A9FFFF

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// 瀵ゆ儼绻�
void delayEx(unsigned int unDelay);
// 閺冨爼妫块崙鑺ユ殶
clock_t currentClock();
// 閺冨爼妫垮顔硷拷鐓庡毐閺侊拷
double diffTimeClock(clock_t start_t, clock_t end_t);

//#ifdef __cplusplus
//}
//#endif

#endif  /* _GP_COMMON_H_ */
