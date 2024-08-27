/*@file     Gp_ApdDef.h
* @brief    apd def文件
* @details  通过该文件可以定义一些define、enum、struct等
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/21
*/
#ifndef _GP_APD_DEF_H_
#define _GP_APD_DEF_H_

#include "../../Base/Comm/Gp_Common.h"                // 通用头文件

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/
#define APD_TX_DATA_BUFFER_SIZE                                 256         // 用于发送数据给主控板软核的数据缓存
#define APD_RX_DATA_BUFFER_SIZE                                 256         // 用于接收主控板软核给过来的数据缓存
#define APD_POWER_FRAME_BUFFER_SIZE                             32          // 用于存储apd光功率数据帧缓存
#define APD_EML_BOARD_TEMP_VOL_DET_TIME_INTERVAL                1000        // ps从pl axi-reg中读取apd相关数据的时间间隔ms

typedef struct tagAdpEmlBoardTempVolDet
{
    // 板温、电压检测
    int nBoardTemp;                 // 板温
    int n5Vol;                      // 电压5v
    int n12Vol;                     // 电压12v

    //apd
    int nApdOpticalPowerRx;         // 光功率接收
    int nApdTempCol;                // 温度采集
    int nApdCurCol;                 // 电流采集
    int nApdVolCol;                 // 电流采集

    //eml
    int nEmlCurCol;                 // 电流采集
    int nEmlVolCol;                 // 电压采集

}stuAdpEmlBoardTempVolDet;

// apd光功率数据帧格式（lvds->pl->bram->intc->ps->read bram data->create pkg->bram->pl）
typedef struct tagAdpPowerFrame
{
    unsigned short  usStartFlag;    // 起始码（固定0x90EB）
    unsigned short  usDataLen;      // 数据包有效数据长度
    unsigned short  usSrcAddr;      // 源地址
    unsigned short  usDesAddr;      // 目的地址
    unsigned short  usSubPkgNum;    // 子包数目
    unsigned short  usSubPkgSeq;    // 子包序列
    unsigned int    unData;         // 数据
    unsigned short  usSum;          // 校验和
    unsigned short  usEndFlag;      // 结束码（固定为0x0A0D）
}stuAdpPowerFrame;
#endif  // _GP_APD_DEF_H_
