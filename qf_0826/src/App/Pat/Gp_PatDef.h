#ifndef _GP_PAT_DEF_H_
#define _GP_PAT_DEF_H_

#include <stdio.h> // printf
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "Gp_MatrixOperation.h"

/****************************************************************************/
/*              macros, enums, and structure definitions                    */
/****************************************************************************/
#define PI 3.1415926535897931159979634685

// 微信本体信息
struct Xita
{
    double a;                       // 轨道半长轴：卫星轨道距离赤道面圆心最长的距离
    double e;                       // 轨道偏心率e=(根号下a平方-b平方)/a b：升交点到地球圆心的距离
    double i;                       // 轨道倾角：卫星轨道和赤道面的夹角
    double Omega;                   // 升交点（黄）赤经：黄道X轴春分点和升交点的夹角
    double w;                       // 近拱点角距
    double delta_t;                 // 过近拱点时刻
    double roll;                    // 滚转角
    double pitch;                   // 俯仰角
    double psi;                     // 方位角
    double roll_rate;               // 滚转角速度
    double pitch_rate;              // 俯仰角速度
    double psi_rate;                // 方位角速度
    double M;                       // 平近点角
    double E;                       // 偏近点角
    double F;                       // 真近点角
};

// GPS星历参数结构体
typedef struct eph_tag
{
	double toc;
	double af2;
	double af1;
	double af0;
	double TGD;
	double sqrtA;
	double toe;
	double deltan;
	double M0;
	double e;
	double i0;
	double IDOT;
	double omega;
	double Cuc;
	double Cus;
	double Crc;
	double Crs;
	double Cic;
	double Cis;
	double omega0;
	double omegaDot;
}eph_t;

// 卫星当前的速度和位置结构体
typedef struct satellite_v_p_tag
{
	double positionX;
	double positionY;
	double positionZ;
	double Vx;
	double Vy;
	double Vz;
	double sat_Clk_error;
}satellite_v_p_t;

// 轨道6根数
typedef struct
{
double a;
double e;
double i;
double omega;
double Omega;
double M;
}stuTrack6Numbers;

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/
// 初始化
int patInit(void);
// 通过目标光学角寻找最近的LCPG角
double Optical2LCPG(double Opitcal_Angle);
//
double LCPG_Optical2FSM_Opitcal(double Optical_Angle, double LCPG_Optical_Angle);
// IJK坐标系转SEZ坐标系
Matrix *IJK2SEZ(double Omega, double w, double i, double f);

int Get_Roll_Pitch_Psi(struct Xita *Xita_A, double delta_t);
// 获得地心赤道坐标系下的卫星位置矢量
Matrix *Get_pIJK(double a, double e, double E, Matrix *T_PWQ2IJK);
// 获取平近点角
double Get_M(double a, double t_now);
// 获取真近点角
double Get_f(double e,double E);
// 获取偏近点角
double Get_E(double M, double e);
// 卫星瞄准矢量补偿矩阵 *******优化
Matrix* Get_Attitude_Compensator(double Roll, double Pitch, double Psi);
// 根据输出光线求机械角
int FSM_Optical2FSM_Machenical(double pitch_out, double psi_out,double *pitch_MA,double *psi_MA);
// 由星上水平俯仰坐标系SEZ矢量表示计算该坐标系下的俯仰角，方位角，斜距
Matrix *Cartesian2Polar(Matrix *pSEZ_v);
// 指向
double Pointing_SEZ(double *SatelliteA, double *SatelliteB, double t_now);
// 计算提前瞄准的方位角
Matrix *Pointing_Ahead_Simulation(double *SatelliteA, double *SatelliteB, double tA, double tB, int AB);
// 提前瞄准补偿量
Matrix *Pointing_Ahead_SEZ(double *SatelliteA, double *SatelliteB, double Ro, double t_now, int AB);
// 极坐标系至直角坐标系方位矢量表示
Matrix *Polar2Cartesian(Matrix *pitch_psi_R);
// SEZ坐标系转光学坐标
Matrix *SEZ2Optical(Matrix *pitch_psi_SEZ, Matrix *T_SEZ2XYW);
// 指向函数
int pointing(double* SatelliteA, double* SatelliteB, double t_now, double* A_pitch_LCPG_Res, double* A_psi_LCPG_Res, double* A_pitch_Optical_Res, double* A_psi_Optical_Res, double* B_pitch_LCPG_Res, double* B_psi_LCPG_Res, double* B_pitch_Optical_Res, double* B_psi_Optical_Res);
// IJK坐标矢量计算
Matrix *IJK_Simulation(double *Satellite, double t_now, struct Xita *xita);
// 近似法解极角
double Solve_Polar_Angle_Approximation(double screw_length, double screw_pitch);
// 计算等螺距等线速的坐标
Matrix *SS_SSP_SLV(double Divergence_Angle, double Overlapping_Ratio, double screw_pitch_in);
// 扫描函数
void scaning(int Scan_Mode, double FOU, double Divergence_Angle, double Overlapping_Ratio, double Tdewll, double Pitch_LCPG, double Psi_LCPG, double Pitch_Optical, double Psi_Optical);
// 凝视函数
int Staring(double *Pitch_LCPG, double *Psi_LCPG, double *Pitch_Optical, double *Psi_Optical, double Image_Pitch, double Image_Psi);
// 符号函数
int sign(double x);
// 求参数fh
double fhan(double x1, double x2, double r, double h);
// 求参数 fal
double fal(double e, double a, double delta);
// ADRC控制函数
int ADRC(double xd , double yd , double x_now, double y_now, double* u_lastx__ptr, double* u_last_y_ptr);
// 计算卫星的轨道位置和速度信息
int sv_state_tsv(double tsv, eph_t eph, satellite_v_p_t* satellite_vp_ptr);

//未生命的话报警告：warning: implicit declaration of function 'PQW2IJK' [-Wimplicit-function-declaration]
// 近焦点坐标系PQW到地心赤道坐标系IJK的变换矩阵
Matrix* PQW2IJK(double Omega,double w,double i);
// pid算法，对接IRCCD
//int pid(double x1_new/*300*/, double y1_new/*400*/, double x1_tag/*100*/, double y1_tag/*200*/);
int pid(double x1_new/*300*/, double y1_new/*400*/, double x_collection_value, double y_collection_value, double* pDelta_u, double* pDelta_u2);
// 计算轨道6根数
void calcTrack6Numbers();
// 由卫星的位置矢量和速度矢量计算轨道6根数
void orbit_state(double* r, double* v, stuTrack6Numbers* pInfo);
// 两向量的叉乘是一个过两相交向量的交点且垂直于两向量所在平面的向量
void cross(double* r, double* v, double* t);
// 欧几里得范数（L2-norm）
double euclidean_norm(int n, double *x);
// 曼哈顿范数（L1-norm）
double manhattan_norm(int n, double *x);

// 定义在pointing.c模块中
//extern double A_pitch_Optical;
//extern double A_psi_Optical;
//extern double A_pitch_LCPG;
//extern double A_psi_LCPG;

//extern double B_pitch_Optical;
//extern double B_psi_Optical;
//extern double B_pitch_LCPG;
//extern double B_psi_LCPG;

#endif  /* _GP_PAT_DEF_H_ */
