/*@file     Gp_CanCmd.h
* @brief    CAN总线文件
* @details  通过该文件可以定义一些define、enum、struct等
* @author   huyanfeng
* @date     2024/1/17
*/
#ifndef _GP_CAN_CMD_DEF_H_
#define _GP_CAN_CMD_DEF_H_

#include "../../Base/Comm/Gp_Common.h"

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/
// 遥控指令汇总
// pat主机工作模式
typedef enum
{
	WM_SLEEP										= 0x03,						// 休眠模式
	WM_IDLE											= 0x07,						// 待机模式
	WM_CALIBRATION									= 0x08,						// 标校模式  待定？
	WM_RECONSTRUCTION								= 0x09,						// 重构模式
	WM_COMMUNICATION								= 0x0A,						// 通信模式
	WM_TEST											= 0x0F						// 测试模式 新增？
}emWorkMode;

// 刷新开关
typedef enum
{
	FS_OPEN											= 0x05,						// 刷新开（默认）
	FS_CLOSE										= 0x0A						// 刷新关
}emFlushState;

// 瞄准数据发射端源选择
// 0B0000：发射端姿态数据为星上数据，轨道数据为星上数据，
// 0B1100：发射端姿态数据为上注数据，轨道数据为星上数据，
// 0B0011：发射端姿态数据为星上数据，轨道数据为上注数据
// 0B1111：发射端姿态数据为上注数据，轨道数据为上注数据
typedef enum
{
	TDTSS_TR_SAT_DATA_TRA_STA_DATA					= 0x00,						// 发射端姿态数据为星上数据，轨道数据为星上数据，
	TDTSS_TR_UP_DATA_TRA_STA_DATA					= 0xC0,						// 发射端姿态数据为上注数据，轨道数据为星上数据，
	TDTSS_TR_SAT_DATA_TRA_UP_DATA					= 0x30,						// 发射端姿态数据为星上数据，轨道数据为星上数据，
	TDTSS_TR_UP_DATA_TRA_UP_DATA					= 0xF0						// 发射端姿态数据为上注数据，轨道数据为星上数据，
}emTagDataTrSrcSel;

// 瞄准数据发射端源选择
// 0B0000：接收端星上数据
// 0B0011：接收端地面上注数据
typedef enum
{
	TDTSS_RE_SAT_DATA								= 0x00,						// 接收端星上数据
	TDTSS_RE_UP_DATA								= 0x03						// 接收端地面上注数据
}emTagDataReSrcSel;

// 本地平台位置属性
// 0X00：本终端为高轨道星载平台
// 0X05：本终端为低轨道星载平台
typedef enum
{
	LPLP_HIGH_ORBIT_SPACEBORNE_PLATFORMS			= 0x00,						// 本终端为高轨道星载平台
	LPLP_LOW_ORBIT_SPACEBORNE_PLATFORMS				= 0x05						// 本终端为低轨道星载平台
}emLocalPlatformLocationProp;

// 本地平台位置类型(轨道数据格式）
// 0X00：GPS的经纬高（高轨道、地面站、飞机）
// 0X05：GPS的位置速度（飞机、低轨卫星）
// 0X0A：J2000位置速度（低轨卫星）
// 0X0C：J2000六根数（低轨卫星）（默认）
// 0X0F：J2000两行根数
typedef enum
{
	LPLT_GPS_LONGITUDE_LATITUDE_ALTITUDE			= 0x00,						// GPS的经纬高（高轨道、地面站、飞机）
	LPLT_GPS_POSITION_SPEED							= 0x05,						// GPS的位置速度（飞机、低轨卫星）
	LPLT_J2000_ALTITUDE_POSITION_SPEED				= 0x0A,						// J2000位置速度（低轨卫星）
	LPLT_J2000_SIX_ELEMENTS							= 0x0C,						// J2000六根数（低轨卫星）（默认）
	LPLT_J2000_ROWS									= 0x0F						// J2000两行根数
}emLocalPlatformLocationType;

// 本地平台姿态属性(姿态数据格式)
// 0X00：姿态角度（默认）
// 0X05：欧拉角度
// 0X0A：卫星本体坐标系四元数
// 0X0C：WGS坐标系四元数
// 0X0F：J2000四元数
typedef enum
{
	LPPA_ATTITUDE_ANGLE								= 0x00,						// GPS的经纬高（高轨道、地面站、飞机）
	LPPA_EULER_ANGLE								= 0x05,						// GPS的位置速度（飞机、低轨卫星）
	LPPA_STAELLITE_COORDINATE_QUATERNION			= 0x0A,						// J2000位置速度（低轨卫星）
	LPPA_WGS_COORDINATE_QUATERNION					= 0x0C,						// J2000六根数（低轨卫星）（默认）
	LPPA_J2000_COORDINATE_QUATERNION				= 0x0F						// J2000两行根数
}emLocalPlatformPostureAttr;

// 目标平台位置属性
// 0X00：本终端为高轨道星载平台
// 0X05：本终端为低轨道星载平台
typedef enum
{
	TPLP_HIGH_ORBIT_SPACEBORNE_PLATFORMS			= 0x00,						// 本终端为高轨道星载平台
	TPLP_LOW_ORBIT_SPACEBORNE_PLATFORMS				= 0x05						// 本终端为低轨道星载平台
}emTagPlatformLocationProp;

// 目标平台位置类型(轨道数据格式）
// 0X00：GPS的经纬高
// 0X05：GPS的位置速度
// 0X0A：J2000位置速度
// 0X0C：J2000六根数（默认）
// 0X0F：J2000两行根数
typedef enum
{
	TPLT_GPS_LONGITUDE_LATITUDE_ALTITUDE			= 0x00,						// GPS的经纬高（高轨道、地面站、飞机）
	TPLT_GPS_POSITION_SPEED							= 0x05,						// GPS的位置速度（飞机、低轨卫星）
	TPLT_J2000_ALTITUDE_POSITION_SPEED				= 0x0A,						// J2000位置速度（低轨卫星）
	TPLT_J2000_SIX_ELEMENTS							= 0x0C,						// J2000六根数（低轨卫星）（默认）
	TPLT_J2000_ROWS									= 0x0F						// J2000两行根数
}emTagPlatformLocationType;

// 捕获角色
// 0B0000：凝视端
// 0B1111：扫描端（默认）
typedef enum
{
	CC_GAZING										= 0x00,						// 凝视端
	CC_SCANNING										= 0xF0						// 扫描端（默认）
}emCaptureCharacter;

// 发送通信数据源选择
// 0B0000：伪随机数
// 0B0111：5a5a循环
// 0B1100：卫星业务数据（默认）
typedef enum
{
	SCDSS_PSEUDORANDOMNUMBER						= 0x00,						// 伪随机数
	SCDSS_5A5A_LOOP									= 0x70,						// 5a5a循环
	SCDSS_SATELLITE_BUSINESS_DATA					= 0xC0						// 卫星业务数据（默认）
}emSndComDataSrcSel;

// 发射激光器波长设置
// 0B0000：C46
// 0B1111：C18
typedef enum
{
	ELWS_C46										= 0x00,						// C64
	ELWS_C18										= 0xF0						// C18
}emEmitLaserWaveLengthSet;

// 通信速率
// b7~b4:TX
// 0B0100：625Mbps
// 0B0101：1.25Gbps
// 0B0110：2.5Gbps
// 0B0111：5.0Gbps
// 0B1000：10.0Gbps
// b3~b0：RX
// 0B0100：625Mbps
// 0B0101：1.25Gbps
// 0B0110：2.5Gbps
// 0B0111：5.0Gbps
// 0B1000：10.0Gbps
typedef enum
{
	CR_TX_625M										= 0x40,						// 625M
	CR_TX_1G250M									= 0x50,						// 1.25G
	CR_TX_2G500M									= 0x60,						// 2.5G
	CR_TX_5G										= 0x70,						// 5G
	CR_TX_10G										= 0x80,						// 10G
	CR_RX_625M										= 0x40,						// 625M
	CR_RX_1G250M									= 0x50,						// 1.25G
	CR_RX_2G500M									= 0x60,						// 2.5G
	CR_RX_5G										= 0x70,						// 5G
	CR_RX_10G										= 0x80						// 10G
}emCommunicationRate;

// 发射激光器波长设置
// 0B0001：RS255/223
// 0B0010：LDPC7/8
// 0B0100：Turbo
typedef enum
{
	SEF_RS255_223									= 0x10,						// RS255/223
	SEF_LDPC7_8										= 0x20,						// LDPC7/8
	SEF_TURBO										= 0x40						// Turbo
}emSendEncodeFormat;

// 接收编码格式
// 0B0001：RS255/223
// 0B0010：LDPC7/8
// 0B0100：Turbo
typedef enum
{
	REF_RS255_223									= 0x01,						// RS255/223
	REF_LDPC7_8										= 0x02,						// LDPC7/8
	REF_TURBO										= 0x04						// Turbo
}emReceiveEncodeFormat;

// 激光通道配置
// b17~b14:0B0000DFE模式，0B1111LPM模式，
// b13~b9：前加重设置值
// b8~b4：后加重设置值
// b3~b0：发射幅值设置值
typedef enum
{
	LCC_RS255_223									= 0x01,						// RS255/223
	LCC_LDPC7_8										= 0x02,						// LDPC7/8
	LCC_TURBO										= 0x04						// Turbo
}emLaserChannelCfg;

// 自动开始通信阈值
// b7b6：0B00开启；0B11关闭
// b5~b0：
// 0B000001:1E-9
// 0B000010:1E-8
// 0B000100:1E-7
// 0B000101:5E-7
// 0B001000:1E-6
// 0B010000:1E-5
// 0B100000:1E-4
// 0B110000:1E-3
typedef enum
{
	ASCT_1E_9										= 0x01,						// 1E-9
	ASCT_1E_8										= 0x02,						// 1E-8
	ASCT_1E_7										= 0x04,						// 1E-7
	ASCT_5E_9										= 0x05,						// 5E-7
	ASCT_1E_6										= 0x08,						// 1E-6
	ASCT_1E_5										= 0x10,						// 1E-5
	ASCT_1E_4										= 0x20,						// 1E-4
	ASCT_1E_3										= 0x30						// 1E-3
}emAutoStartCommunicationThr;

// 章动开关
// 0B0000：关闭
// 0B1111：开启
typedef enum
{
	NS_CLOSE										= 0x00,						// 关闭
	NS_OPEN											= 0xF0						// 开启
}emNutationSwitch;

// 寻心开关
// 0B0000：关闭
// 0B1111：开启
typedef enum
{
	SHS_CLOSE										= 0x00,						// 关闭
	SHS_OPEN										= 0xF0						// 开启
}emSeekHeartSwitch;

// 单机供电
typedef enum
{
	SMP_CLOSE										= 0x00,						// 关闭
	SMP_OPEN										= 0x01						// 开泵
}emSingleMachinePower;

// 单机供电
typedef enum
{
	SMP_CEFA_B5_b0_SHIFT							= 0,						// 00CEFA关泵，11CEFA开泵
	SMP_CEFA_B5_b1_SHIFT							= 1,						// 00CEFA关泵，11CEFA开泵
	SMP_EDFA_B5_b2_SHIFT							= 2,						// 00EDFA关泵，11EDFA开泵
	SMP_EDFA_B5_b3_SHIFT							= 3,						// 00EDFA关泵，11EDFA开泵
	SMP_SS_B5_b4_SHIFT								= 4,						// 00星敏关机，11星敏开机
	SMP_SS_B5_b5_SHIFT								= 5,						// 00星敏关机，11星敏开机
	SMP_CCD_B5_b6_SHIFT								= 6,						// 00S-CCD关机，11S-CCD开机
	SMP_CCD_B5_b7_SHIFT								= 7,						// 00S-CCD关机，11S-CCD开机

	SMP_FSM2_B6_b0_SHIFT							= 8,						// 00快反镜2关机，11快反镜2开机
	SMP_FSM2_B6_b1_SHIFT							= 9,						// 00快反镜2关机，11快反镜2开机
	SMP_FSM1_B6_b2_SHIFT							= 10,						// 00快反镜1关机，11快反镜1开机
	SMP_FSM1_B6_b3_SHIFT							= 11,						// 00快反镜1关机，11快反镜1开机
	SMP_LCPG_B6_b4_SHIFT							= 12,						// 00液晶偏转器关机，11液晶偏转器开机
	SMP_LCPG_B6_b5_SHIFT							= 13,						// 00液晶偏转器关机，11液晶偏转器开机
	SMP_MEMS_B6_b6_SHIFT							= 14,						// 00MEMS关机，11MEMS开机
	SMP_MEMS_B6_b7_SHIFT							= 15,						// 00MEMS关机，11MEMS开机

	SMP_RESERVE_B7_b0_SHIFT							= 16,						//
	SMP_RESERVE_B7_b1_SHIFT							= 17,						//
	SMP_MEMS_B7_b2_SHIFT							= 18,						// 00信号激光器关机，11信号激光器开机
	SMP_MEMS_B7_b3_SHIFT							= 19,						// 00信号激光器关机，11信号激光器开机
	SMP_SL_B7_b4_SHIFT								= 20,						// 00数据板关机，11数据板开机
	SMP_SL_B7_b5_SHIFT								= 21,						// 00数据板关机，11数据板开机
	SMP_CCD_B7_b6_SHIFT								= 22,						// 00EDFA关机，11EDFA开机
	SMP_CCD_B7_b7_SHIFT								= 23						// 00EDFA关机，11EDFA开机
}emSingleMachinePowerShift;

// PAT工作模式
// 0B0000：正常模式
// 0B1111：全域扫描模式
typedef enum
{
	PATWM_NORMAL									= 0x00,					// 正常模式
	PATWM_GLOBAL_SCANNING							= 0x0F					// 全域扫描模式
}emPATWorkMode;

// 精跟踪驱动工作模式
// 0x02绝对定位
// 0x05停止/断使能
// 0x09增量定位
typedef enum
{
	PTDWM_ABS_POSITION								= 0x02,					// 正常模式
	PTDWM_STOP_OR_ENABLE							= 0x05,					// 全域扫描模式
	PTDWM_INC_POSITION								= 0x09					// 增量定位
}emPrecisionTrackDriveWorkMode;

// 提前瞄准驱动工作模式
// 0x02绝对定位
// 0x05停止/使能
// 0x09增量定位
typedef enum
{
	ATDWM_ABS_POSITION								= 0x02,					// 正常模式
	ATDWM_STOP_OR_ENABLE							= 0x05,					// 全域扫描模式
	ATDWM_INC_POSITION								= 0x09					// 增量定位
}emAdvanceTargetDriveWorkMode;

// 热控板切换电机
// 0X55：位置1
// 0XCAA：位置2
typedef enum
{
	TCBSM_POSITION_1								= 0x55,					// 位置1
	TCBSM_POSITION_2								= 0xCAA					// 位置2
}emThermalCtrlBoardSwitchMotor;

// SWIR CCD窗口尺寸
typedef enum
{
	SWIRCCD_WS_0									= 0x00,					// 窗口0
	SWIRCCD_WS_1									= 0x01,					// 窗口1
	SWIRCCD_WS_2									= 0x02,					// 窗口2
	SWIRCCD_WS_3									= 0x03,					// 窗口3
	SWIRCCD_WS_4									= 0x04,					// 窗口4
	SWIRCCD_WS_5									= 0x05,					// 窗口5
	SWIRCCD_WS_6									= 0x06,					// 窗口6
	SWIRCCD_WS_7									= 0x07,					// 窗口7
	SWIRCCD_WS_8									= 0x08,					// 窗口8
	SWIRCCD_WS_9									= 0x09,					// 窗口9
	SWIRCCD_WS_10									= 0x0A,					// 窗口10
	SWIRCCD_WS_11									= 0x0B,					// 窗口12
	SWIRCCD_WS_12									= 0x0C 					// 窗口12
}emSWIRCCDWindowSize;

// SWIR CCD计算阈值
// 0：默认阈值
// 1~255：主机设置
typedef enum
{
	SWIRCCD_CT_0									= 0x0					// 默认值
}emSWIRCCDCalcThreshold;

// 图像处理算法设置
// 0x00：不输出
// 0x01：输出形心
// 0x02：输出重心
typedef enum
{
	SWIRCCD_TPAS_OUTPUT_NO							= 0x00,					// 不输出
	SWIRCCDTPAS_OUTPUT_CENTROID						= 0x01,					// 输出形心
	SWIRCCDTPAS_OUTPUT_CENTER_GRAVITY				= 0x02					// 输出重心
}emSWIRCCDImageProcessAlgorithmSet;

// EDFA工作模式/BA
// 0x00-关闭；
// 0x01 恒流模式；
// 0x02-恒功率模式；
// 0xfe 保持当前状态；
// 0xff-复位
typedef enum
{
	EDFA_WM_BA_CLOSE								= 0x00,					// 关闭
	EDFA_WM_BA_CONSTANT_CURRENT						= 0x01,					// 恒流模式
	EDFA_WM_BA_CONSTANT_POWER						= 0x02,					// 恒功率模式
	EDFA_WM_BA_MAINTAIN_CURRENT_STATE				= 0xFE,					// 保持当前状态
	EDFA_WM_BA_RESET								= 0xFF					// 复位
}emEDFAWorkModeBA;

// CEFA工作模式/PA
// 0x00-关闭；
// 0x01 恒流模式；
// 0x02-恒功率模式；
// 0xfe 保持当前状态；
// 0xff-复位
typedef enum
{
	CEFA_WM_BA_CLOSE								= 0x00,					// 关闭
	CEFA_WM_BA_CONSTANT_CURRENT						= 0x01,					// 恒流模式
	CEFA_WM_BA_CONSTANT_POWER						= 0x02,					// 恒功率模式
	CEFA_WM_BA_MAINTAIN_CURRENT_STATE				= 0xFE,					// 保持当前状态
	CEFA_WM_BA_RESET								= 0xFF					// 复位
}emCEFAWorkModeBA;

/****************************************************************************/
/*                                                                          */
/*              遥测返回信息汇总                                                   									*/
/*                                                                          */
/****************************************************************************/
// pat所处工作阶段
// 0X00：开机自检
// 0X11：开机等待
// 0X22：瞄准阶段
// 0X33：捕获阶段
// 0X44：粗跟踪阶段
// 0X55：精跟踪阶段
// 0X66：章动阶段
// 0X77：通信阶段
// 0X88：关机准备阶段
// 0X99：完成关机准备，可以断电
typedef enum
{
	WP_START_UP_AUTO_CHECK_PHASE					= 0x00,						// 开机自检
	WP_START_UP_WAIT_PHASE							= 0x11,						// 开机等待
	WP_POINT_PHASE									= 0x22,						// 瞄准阶段
	WP_ACQUISITION_PHASE							= 0x33,						// 捕获阶段
	WP_ROUGH_TRACK_PHASE							= 0x44,						// 粗跟踪阶段
	WP_ACCURATE_TRACK_PHASE							= 0x55,						// 精跟踪阶段
	WP_NUTATION_PHASE								= 0x66,						// 章动阶段
	WP_COMMUNICATION_PHASE							= 0x77,						// 通信阶段
	WP_SHUTDOWN_PREPARE_PHASE						= 0x88,						// 关机准备阶段
	WP_POWER_OFF_PHASE								= 0x99						// 完成关机准备，可以断电
}emWorkPhase;

// EDFA/PA报警
// 0x0003：报警状态，位定义如下：
// bit0：泵浦1电流报警
// bit1：泵浦1温度报警
// bit2：泵浦2电流报警
// bit3：泵浦2温度报警
// bit4：保留
// bit5：保留
// bit6：入光低报警
// bit7：出光低报警
// bit8：出光偏离报警
// bit9：壳体温度报警
// bit10：光纤温度报警
typedef enum
{
	EDFAPAA_BIT0_PUMP_1_CURRENT_ALARM				= 0,						// 泵浦1电流报警
	EDFAPAA_BIT1_PUMP_1_TEMP_ALARM					= 1,						// 泵浦1温度报警
	EDFAPAA_BIT2_PUMP_2_CURRENT_ALARM				= 2,						// 泵浦2电流报警
	EDFAPAA_BIT3_PUMP_2_TEMP_ALARM					= 3,						// 泵浦1温度报警
	EDFAPAA_BIT4_RESERVE							= 4,						// 保留
	EDFAPAA_BIT5_RESERVE							= 5,						// 保留
	EDFAPAA_BIT6_INCOMING_LIGHT_LOW_ALARM			= 6,						// 入光低报警
	EDFAPAA_BIT7_OUTPUT_LIGHT_LOW_ALARM				= 7,						// 出光低报警
	EDFAPAA_BIT8_DEVIATION_LIGHT_LOW_ALARM			= 8,						// 出光偏离报警
	EDFAPAA_BIT9_DEVIATION_LIGHT_LOW_ALARM			= 9,						// 壳体温度报警
	EDFAPAA_BIT10_FIBER_TEMP_ALARM_ALARM			= 10						// 光纤温度报警
}emEDFAPAAlarm;

// EDFA/BA报警
// 0x002A：报警状态，位定义如下：
// bit0：泵浦1电流报警
// bit1：泵浦1温度报警
// bit2：泵浦2电流报警
// bit3：泵浦2温度报警
// bit4：泵浦3电流报警
// bit5：泵浦3温度报警
// bit6：入光低报警
// bit7：出光低报警
// bit8：出光偏离报警
// bit9：壳体温度报警
// bit10：光纤温度报警
typedef enum
{
	CEFABAA_BIT0_PUMP_1_CURRENT_ALARM				= 0,						// 泵浦1电流报警
	CEFABAA_BIT1_PUMP_1_TEMP_ALARM					= 1,						// 泵浦1温度报警
	CEFABAA_BIT2_PUMP_2_CURRENT_ALARM				= 2,						// 泵浦2电流报警
	CEFABAA_BIT3_PUMP_2_TEMP_ALARM					= 3,						// 泵浦2温度报警
	CEFABAA_BIT4_RESERVE							= 4,						// 保留
	CEFABAA_BIT5_RESERVE							= 5,						// 保留
	CEFABAA_BIT6_INCOMING_LIGHT_LOW_ALARM			= 6,						// 入光低报警
	CEFABAA_BIT7_OUTPUT_LIGHT_LOW_ALARM				= 7,						// 出光低报警
	CEFABAA_BIT8_DEVIATION_LIGHT_LOW_ALARM			= 8,						// 出光偏离报警
	CEFABAA_BIT9_DEVIATION_LIGHT_LOW_ALARM			= 8,						// 壳体温度报警
	CEFABAA_BIT10_FIBER_TEMP_ALARM_ALARM			= 10						// 光纤温度报警
}emCEFABAAlarm;
#endif  /* _GP_CAN_CMD_DEF_H_ */
