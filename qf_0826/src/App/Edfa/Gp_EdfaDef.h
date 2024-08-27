/*@file     Gp_EdfaDef.h
* @brief    edfa def文件
* @details  通过该文件可以定义一些define、enum、struct等
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/07
*/
#ifndef _GP_EDFA_DEF_H_
#define _GP_EDFA_DEF_H_

#include "../../Base/Comm/Gp_Common.h"                // 通用头文件

/****************************************************************************
 * 通信模式
 * 通讯采用主从模式通讯。
 * 放大器模块为从机(S)，外接设备为主机（M）。
 * M -> S 代表：主机下传数据至从机
 * S -> M 代表：从机上传数据至主机

 * 数据发送规则
 * 所有多字节采用 Big-endian 模块，高字节先发
****************************************************************************/

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/
// 具体参看：光纤放大器通信协议V1.0.pdf(2024.06 v1.0)
// 串口参数
#define EDFA_BAUD_RATE_115200           115200                      // 波特率：115200
#define EDFA_START_BIT                  1                           // 启动位：1bit
#define EDFA_STOP_BIT                   1                           // 停止位：1bit
#define EDFA_DATA_BIT                   8                           // 数据位：8bit
#define EDFA_CRC_BIT                    0                           // 校验位：无
#define EDFA_HARDWARE_FLOW_CONTROL      0                           // 硬件流控制：无

#define EDFA_S2M_ACK_DATA_SIZE          167                         // 读数据命令返回
#define EDFA_TX_DATA_BUFFER_SIZE        256                         // 用于发送FPGA AXI REG 给过来的数据缓存
#define EDFA_RX_DATA_BUFFER_SIZE        256                         // 用于接收FPGA AXI REG 给过来的数据缓存
#define EDFA_SF_HEADER_SIZE             8                           // edfa通信协议ack的包头（0xEB 0x90\0x50\0x40）

// 起始标识
typedef enum
{
    EDFA_M2S_SF_HEADER1                 = 0x55,                     // M2S包头
    EDFA_M2S_SF_HEADER2                 = 0xAA,                     // M2S包头
    EDFA_S2M_SF_HEADER1                 = 0xED,                     // S2M包头
    EDFA_S2M_SF_HEADER2                 = 0xFA,                     // S2M包头
}emEdfaStartFlag;

// 地址标识
typedef enum
{
    EDFA_M2S_AF                         = 0xFFFF,                     // M2S包头
}emEdfaAddrFlag;

// 指令标识
typedef enum
{
    EDFA_CTRL_CMD_QUERY_MODULE_DATA     = 0x0001,                     // 查询模块数据
    EDFA_CTRL_CMD_SET_CHANNEL_SWITCH    = 0xA001,                     // 设置通道开关
    EDFA_CTRL_CMD_SET_WORK_MODE         = 0xA002,                     // 设置工作模式
    EDFA_CTRL_CMD_SET_PUMP_AMP          = 0xA003,                     // 设置泵浦电流
    EDFA_CTRL_CMD_SET_OUTPUT_OPT_POWER  = 0xA004,                     // 设置输出光功率
}emEdfaCtrlCmd;

// S2M查询模块数据ack-工作模式及开关
typedef enum
{
    EDFA_WORK_MODE_AND_SWITCH_ACC       = 0x0000,                       // 查询模块数据
    EDFA_WORK_MODE_AND_SWITCH_APC       = 0x0001,                       // 查询模块数据
}emEdfaWorkModeAndSwitch;

// 遥测遥控单帧复合帧
typedef enum
{
    EDFA_YC_SINGLE_FRAME                = 0x00,                         // 遥测单帧
    EDFA_YC_COMP_FRAME                  = 0x3A,                         // 遥测复合帧
    EDFA_YK_SINGLE_FRAME                = 0x0A,                         // 遥控单帧
    EDFA_YK_COMP_FRAME                  = 0x0F,                         // 遥控复合帧
}emEdfaYkycSingleCompFrameFlag;

// SWIRCCD驱动模块数据帧
#pragma pack(1)
typedef struct tagEdfaYkFrameInfo
{
    unsigned short      usStartFlag;                // 起始位： M -> S：2字节 固定为：0x55AA； M -> S：2字节 固定为：0xEDFA；
    unsigned short      usAddrFlag;                 // 地址：M -> S：2字节 固定为：0xFFFF； M -> S：2字节 为：模块地址；
    unsigned short      usCtrlCmd;                  // 控制指令：2 字节，数据指令，具体指令见表 1
    unsigned short      usDataLen;                  // 数据长度：2 字节，数据长度，表示数据位数据的字节数，具体长度见表 1
    unsigned char       pData[3];                   // 数据：N 字节， 传输的数据；
    unsigned char       ucXor;                      // 校验位：1 字节， 从起始位开始至数据最后一位截止，所有字节进行异或运算后的值
}stuEdfaYkFrameInfo;
#pragma pack()

#pragma pack(1)
typedef struct tagEdfaYcFrameInfo
{
    unsigned short      usStartFlag;                // 起始位： M -> S：2字节 固定为：0x55AA； M -> S：2字节 固定为：0xEDFA；
    unsigned short      usAddrFlag;                 // 地址：M -> S：2字节 固定为：0xFFFF； M -> S：2字节 为：模块地址；
    unsigned short      usCtrlCmd;                  // 控制指令：2 字节，数据指令，具体指令见表 1
    unsigned short      usDataLen;                  // 数据长度：2 字节，数据长度，表示数据位数据的字节数，具体长度见表 1
    unsigned char       pData[167];                 // 数据：N 字节， 传输的数据；
    unsigned char       ucXor;                      // 校验位：1 字节， 从起始位开始至数据最后一位截止，所有字节进行异或运算后的值
}stuEdfaYcFrameInfo;
#pragma pack()

#endif  // _GP_EDFA_DEF_H_
