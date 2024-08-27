/*@file     Gp_XmDef.h
* @brief    星敏感器外部机构def文件
* @details  通过该文件可以定义一些define、enum、struct等
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/12
*/
#ifndef _GP_XM_DEF_H_
#define _GP_XM_DEF_H_

#include "../../Base/Comm/Gp_Common.h"                // 通用头文件

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/
// 具体参看：光纤放大器通信协议V1.0.pdf(2024.06 v1.0)
// 串口参数
#define XM_BAUD_RATE_115200                 115200      // 波特率：115200
#define XM_START_BIT                        1           // 启动位：1bit
#define XM_STOP_BIT                         1           // 停止位：1bit
#define XM_DATA_BIT                         8           // 数据位：8bit
#define XM_CRC_BIT                          0           // 校验位：无
#define XM_HARDWARE_FLOW_CONTROL            0           // 硬件流控制：无

#define XM_TX_DATA_BUFFER_SIZE              256         // 用于发送FPGA AXI REG 给过来的数据缓存
#define XM_RX_DATA_BUFFER_SIZE              256         // 用于接收FPGA AXI REG 给过来的数据缓存
#define XM_SF_HEADER_SIZE                   2           // 星敏感器通信协议ack的包头（0xEB 0x90\0x50\0x40）
#define XM_YK_ACK_REMAIN_RECV_SIZE          3           // 星敏感器通信协议遥控剩余字3节数（5-2字节）
#define XM_YC_ACK_REMAIN_RECV_SIZE          51          // 星敏感器通信协议遥测剩余字51节数（53-2字节）
#define XM_SELF_TEST_ACK_REMAIN_RECV_SIZE   17          // 星敏感器通信协议自检剩余字17节数（19-2字节）

// 遥控指令起始标识
typedef enum
{
    XM_SF_HEADER1                           = 0xEB,     // 固定头部第一个字节
    XM_SF_YK_CMD_AND_ACK                    = 0x90,     // 遥控指令和应答第二个字节
    XM_SF_YC_CMD_AND_ACK                    = 0x50,     // 遥测指令和应道第二个字节
    XM_SF_SELF_TEST_DOWN_TX_AND_ACK         = 0x40,     // 自检结果下传第二个字节
    XM_SF_PROGRAM_UPLOAD_AND_ACK            = 0x80      // 程序上注第二个字节
}emXmYkStartFlag;

// 遥控指令返回成功失败标识
typedef enum
{
    XM_YK_RET_ACK_SUC                       = 0x55,     // 成功
    XM_YK_RET_ACK_ERR                       = 0xAA      // 失败
}emXmYkRetAck;

// 协议固定标识
typedef enum
{
    XM_YK_ACK_CONST_FALG_0xAA               = 0xAA,     // 遥控应答
    XM_YC_ACK_CONST_FALG_0x22               = 0x22,     // 遥测应答
    XM_YC_STDT_REQ_CONST_FALG_0x01          = 0x01,     // 遥测、自检结果下传请求
    XM_STDT_ACK_CONST_FALG_0x0F             = 0x01      // 自检结果下传应答
}emXmYkYcRetAck;

// 遥控指令标识
typedef enum
{
    XM_CTRL_CMD_SET_WORK_MODE               = 0x00,     // 设置工作模式
    XM_CTRL_CMD_SET_EXPOSURE_TIME           = 0x02,     // 设置曝光时间
    XM_CTRL_CMD_SET_GAIN                    = 0x05,     // 设置增益
    XM_CTRL_CMD_SAA_MODE_SWITCH             = 0x06,     // SAA模式切换
    XM_CTRL_CMD_ENTER_BOOT_INSTR            = 0x09,     // 进入BOOT指令
    XM_CTRL_CMD_SEC_INT_CALI                = 0x0A,     // 秒整数校准
    XM_CTRL_CMD_SWITCH_H_L_DYNAMIC_MODE     = 0x0C,     // 切换高/低动态模式
    XM_CTRL_CMD_SELECT_IMG_START            = 0x0F,     // 选择镜像文件启动
    XM_CTRL_CMD_SYSTEM_RESET                = 0x10,     // 系统复位
    XM_CTRL_CMD_SET_SELF_TEST               = 0x11,     // 置星敏感器自检
    XM_CTRL_CMD_INTERNAL_PARAM_SHORT_INSTR  = 0xFF,     // 内部调参短指令
    XM_CTRL_CMD_INTERNAL_PARAM_LONG_INSTR   = 0xFF,     // 内部调参长指令
}emXmYkCtrlCmd;

// 遥控指令数据帧格式
#pragma pack(1)
typedef struct tagXmYkFramePkg
{
    unsigned char      ucHeaderFlag1;                   // 包头 0xEB
    unsigned char      ucHeaderFlag2;                   // 包头 0x90
    unsigned char      ucFrameFormat1;                  // 帧格式1
    unsigned char      ucFrameFormat2;                  // 帧格式2
    unsigned char      ucFrameFormat3;                  // 帧格式3
    unsigned char      ucFrameFormat4;                  // 帧格式4
    unsigned char      ucFrameFormat5;                  // 帧格式5
    unsigned char      ucFrameFormat6;                  // 帧格式6
}stuXmYkFramePkg;
#pragma pack()

// 遥测指令数据帧格式
#pragma pack(1)
typedef struct tagXmYcFramePkg
{
    unsigned char      ucHeaderFlag1;                   // 包头 0xEB
    unsigned char      ucHeaderFlag2;                   // 包头 0x50
    unsigned char      ucFrameFormat1;                  // 帧格式1 0x22
    unsigned char      ucFrameFormat2;                  // 帧格式2 0x01
    unsigned char      ucFrameFormat3;                  // 帧格式3 SUM
}stuXmYcFramePkg;
#pragma pack()

// 自检指令数据帧格式
#pragma pack(1)
typedef struct tagXmSelfTestFramePkg
{
    unsigned char      ucHeaderFlag1;                   // 包头 0xEB
    unsigned char      ucHeaderFlag2;                   // 包头 0x40
    unsigned char      ucFrameFormat1;                  // 帧格式1 0x22
    unsigned char      ucFrameFormat2;                  // 帧格式2 0x01
    unsigned char      ucFrameFormat3;                  // 帧格式3 SUM
}stuXmSelfTestFramePkg;
#pragma pack()

#endif  // _GP_XM_DEF_H_
