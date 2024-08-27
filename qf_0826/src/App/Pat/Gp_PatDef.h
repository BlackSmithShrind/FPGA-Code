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

// ΢�ű�����Ϣ
struct Xita
{
    double a;                       // ����볤�᣺���ǹ����������Բ����ľ���
    double e;                       // ���ƫ����e=(������aƽ��-bƽ��)/a b�������㵽����Բ�ĵľ���
    double i;                       // �����ǣ����ǹ���ͳ����ļн�
    double Omega;                   // �����㣨�ƣ��ྭ���Ƶ�X�ᴺ�ֵ��������ļн�
    double w;                       // ������Ǿ�
    double delta_t;                 // ��������ʱ��
    double roll;                    // ��ת��
    double pitch;                   // ������
    double psi;                     // ��λ��
    double roll_rate;               // ��ת���ٶ�
    double pitch_rate;              // �������ٶ�
    double psi_rate;                // ��λ���ٶ�
    double M;                       // ƽ�����
    double E;                       // ƫ�����
    double F;                       // ������
};

// GPS���������ṹ��
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

// ���ǵ�ǰ���ٶȺ�λ�ýṹ��
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

// ���6����
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
// ��ʼ��
int patInit(void);
// ͨ��Ŀ���ѧ��Ѱ�������LCPG��
double Optical2LCPG(double Opitcal_Angle);
//
double LCPG_Optical2FSM_Opitcal(double Optical_Angle, double LCPG_Optical_Angle);
// IJK����ϵתSEZ����ϵ
Matrix *IJK2SEZ(double Omega, double w, double i, double f);

int Get_Roll_Pitch_Psi(struct Xita *Xita_A, double delta_t);
// ��õ��ĳ������ϵ�µ�����λ��ʸ��
Matrix *Get_pIJK(double a, double e, double E, Matrix *T_PWQ2IJK);
// ��ȡƽ�����
double Get_M(double a, double t_now);
// ��ȡ������
double Get_f(double e,double E);
// ��ȡƫ�����
double Get_E(double M, double e);
// ������׼ʸ���������� *******�Ż�
Matrix* Get_Attitude_Compensator(double Roll, double Pitch, double Psi);
// ��������������е��
int FSM_Optical2FSM_Machenical(double pitch_out, double psi_out,double *pitch_MA,double *psi_MA);
// ������ˮƽ��������ϵSEZʸ����ʾ���������ϵ�µĸ����ǣ���λ�ǣ�б��
Matrix *Cartesian2Polar(Matrix *pSEZ_v);
// ָ��
double Pointing_SEZ(double *SatelliteA, double *SatelliteB, double t_now);
// ������ǰ��׼�ķ�λ��
Matrix *Pointing_Ahead_Simulation(double *SatelliteA, double *SatelliteB, double tA, double tB, int AB);
// ��ǰ��׼������
Matrix *Pointing_Ahead_SEZ(double *SatelliteA, double *SatelliteB, double Ro, double t_now, int AB);
// ������ϵ��ֱ������ϵ��λʸ����ʾ
Matrix *Polar2Cartesian(Matrix *pitch_psi_R);
// SEZ����ϵת��ѧ����
Matrix *SEZ2Optical(Matrix *pitch_psi_SEZ, Matrix *T_SEZ2XYW);
// ָ����
int pointing(double* SatelliteA, double* SatelliteB, double t_now, double* A_pitch_LCPG_Res, double* A_psi_LCPG_Res, double* A_pitch_Optical_Res, double* A_psi_Optical_Res, double* B_pitch_LCPG_Res, double* B_psi_LCPG_Res, double* B_pitch_Optical_Res, double* B_psi_Optical_Res);
// IJK����ʸ������
Matrix *IJK_Simulation(double *Satellite, double t_now, struct Xita *xita);
// ���Ʒ��⼫��
double Solve_Polar_Angle_Approximation(double screw_length, double screw_pitch);
// ������ݾ�����ٵ�����
Matrix *SS_SSP_SLV(double Divergence_Angle, double Overlapping_Ratio, double screw_pitch_in);
// ɨ�躯��
void scaning(int Scan_Mode, double FOU, double Divergence_Angle, double Overlapping_Ratio, double Tdewll, double Pitch_LCPG, double Psi_LCPG, double Pitch_Optical, double Psi_Optical);
// ���Ӻ���
int Staring(double *Pitch_LCPG, double *Psi_LCPG, double *Pitch_Optical, double *Psi_Optical, double Image_Pitch, double Image_Psi);
// ���ź���
int sign(double x);
// �����fh
double fhan(double x1, double x2, double r, double h);
// ����� fal
double fal(double e, double a, double delta);
// ADRC���ƺ���
int ADRC(double xd , double yd , double x_now, double y_now, double* u_lastx__ptr, double* u_last_y_ptr);
// �������ǵĹ��λ�ú��ٶ���Ϣ
int sv_state_tsv(double tsv, eph_t eph, satellite_v_p_t* satellite_vp_ptr);

//δ�����Ļ������棺warning: implicit declaration of function 'PQW2IJK' [-Wimplicit-function-declaration]
// ����������ϵPQW�����ĳ������ϵIJK�ı任����
Matrix* PQW2IJK(double Omega,double w,double i);
// pid�㷨���Խ�IRCCD
//int pid(double x1_new/*300*/, double y1_new/*400*/, double x1_tag/*100*/, double y1_tag/*200*/);
int pid(double x1_new/*300*/, double y1_new/*400*/, double x_collection_value, double y_collection_value, double* pDelta_u, double* pDelta_u2);
// ������6����
void calcTrack6Numbers();
// �����ǵ�λ��ʸ�����ٶ�ʸ��������6����
void orbit_state(double* r, double* v, stuTrack6Numbers* pInfo);
// �������Ĳ����һ�������ཻ�����Ľ����Ҵ�ֱ������������ƽ�������
void cross(double* r, double* v, double* t);
// ŷ����÷�����L2-norm��
double euclidean_norm(int n, double *x);
// �����ٷ�����L1-norm��
double manhattan_norm(int n, double *x);

// ������pointing.cģ����
//extern double A_pitch_Optical;
//extern double A_psi_Optical;
//extern double A_pitch_LCPG;
//extern double A_psi_LCPG;

//extern double B_pitch_Optical;
//extern double B_psi_Optical;
//extern double B_pitch_LCPG;
//extern double B_psi_LCPG;

#endif  /* _GP_PAT_DEF_H_ */
