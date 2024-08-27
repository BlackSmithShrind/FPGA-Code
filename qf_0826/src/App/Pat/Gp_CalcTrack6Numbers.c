#include "Gp_PatDef.h"

//欧几里得范数（L2-norm）
double euclidean_norm(int n, double *x)
{
    double norm = 0.0;
    int i = 0;
    for (i = 0; i < n; i++)
    {
        norm += x[i] * x[i];
    }
    return sqrt(norm);
}

//曼哈顿范数（L1-norm）
double manhattan_norm(int n, double *x)
{
    double norm = 0.0;
    int i = 0;
    for (i = 0; i < n; i++)
    {
        norm += fabs(x[i]);
    }
    return norm;
}

void calcTrack6Numbers()
{
	// A星位置矢量、速度矢量
	double r_a[]={1.0e+03*3.425946672442576, 1.0e+03*1.563855388788973, 1.0e+03*6.249761195147854};
	double v_a[]={-6.400945054573246, -0.567694273157523, 3.652857274285882};

	// B星位置矢量、速度矢量
	double r_b[]={1.0e+03*4.687677407793589, 1.0e+03*0.836168023774815, 1.0e+03*5.528903402332030};
	double v_b[]={-5.663489310120919, 0.710294919441006, 4.696606286171664};

	// 计算轨道根数
	stuTrack6Numbers infoA;
	stuTrack6Numbers infoB;
	orbit_state(r_a, v_a, &infoA);

	orbit_state(r_b, v_b, &infoB);

	Get_E(infoA.M, infoA.e);
	Get_E(infoB.M, infoB.e);
	/*
	// Kepler方程求解器
	double dK = infoA.E_A - infoA.e_A*sin(infoA.E_A) - infoA.M_A;
	options = optimset('Display','off');
		E_A = fsolve(@(E_A)fun(E_A,M_A,e_A), M_A, options); % A星偏近点角
		F_A = 2*atan(sqrt((1+e_A)/(1-e_A))*tan(E_A/2)); % A星真近点角
	*/
	Get_f(infoA.M, infoA.e);
	Get_f(infoB.M, infoB.e);
	/*
	%% Kepler方程求解器
	fun1 = @(E_B,M_B,e_B) E_B - e_B*sin(E_B) - M_B;
	options1 = optimset('Display','off');
		E_B = fsolve(@(E_B)fun1(E_B,M_B,e_B), M_B, options); % B星偏近点角
		F_B = 2*atan(sqrt((1+e_B)/(1-e_B))*tan(E_B/2)) ;% B星真近点角
	*/
}

// cross函数
//	A=[x1,y1,z1]
//	B=[x2,y2,z2]
//	cross(A, B)=[y1∗z2−y2∗z1, z1∗x2−z2∗x1, x1∗y2−x2∗y1]
// 原文链接：https://blog.csdn.net/Vertira/article/details/131827450
void cross(double* r, double* v, double* t)
{
	t[0] = r[1]*v[2]-v[1]*r[2];
	t[1] = r[2]*v[0]-v[2]*r[0];
	t[2] = r[0]*v[1]-v[0]*r[1];
}

// 由卫星的位置矢量和速度矢量计算轨道6根数
//
// 输入:
//   r - 位置矢量 [x, y, z] (km)
//   v - 速度矢量 [vx, vy, vz] (km/s)
//
// 输出:
//   a - 半长轴 (km)
//   e - 离心率 (无量纲)
//   i - 轨道倾角 (rad)
//   omega - 近地点距角 (rad)
//   Omega - 升交点赤经 (rad)
//   nu - 真近点角 (rad)
void orbit_state(double* r, double* v, stuTrack6Numbers* pInfo)
{
	// 计算角动量矢量 h
	double h[3] = {0, 0, 0};
	cross(r, v, h);
	double h_mag = euclidean_norm(3, h);

	// 计算轨道面法线向量 n
	double uv[3] = {0, 0, 1};
	double crs[3] = {0, 0, 1};
	cross(uv, h, crs);
	double norm_crs = euclidean_norm(3, crs);
	double n[3] = {0,0,0};
	n[0] = crs[0]/norm_crs;
	n[1] = crs[1]/norm_crs;
	n[2] = crs[2]/norm_crs;

	// 计算半长轴 a
	int mu = 398600;
	double norm_v= euclidean_norm(3, v);
	double norm_r= euclidean_norm(3, r);
	double E = pow(norm_v, 2)/2 - mu/norm_r; //能量
	pInfo->a = -mu/(2*E);               // 半长轴

	// 计算偏心率 e
	cross(v, h, crs);
	norm_r= euclidean_norm(3, r);
	double e[3] = {0, 0, 0}; // 偏心率矢量
	e[0] = crs[0]/mu - r[0]/norm_r;
	e[1] = crs[1]/mu - r[1]/norm_r;
	e[2] = crs[2]/mu - r[2]/norm_r;

	// 计算轨道倾角 i
	pInfo->i = acos(h[2] / h_mag);

	// 计算升交点赤经 Omega
	pInfo->Omega = 0.0;
	pInfo->omega = 0.0;
	if (n[1] >= 0)
	{
		pInfo->Omega = atan2(n[1], n[0]);
	}
	else
	{
		pInfo->Omega = 2*PI - atan2(-n[1], n[0]);
	}

	// 计算近地点距角 omega
	if (n[2] >= 0)
	{
		pInfo->omega = atan2(n[1], n[0]);
	}
	else
	{
		pInfo->omega = 2*PI - atan2(-n[1], n[0]);
	}

	// 计算平近点角 M
	double norm_e = euclidean_norm(3, e);
	norm_r = euclidean_norm(3, r);
	//dot:内积对应位置相乘再相加（标量）
	double dot_e_r = e[0]*r[0] + e[1]*r[1] + e[2]*r[2];
	pInfo->M = acos(dot_e_r/(norm_e * norm_r)); // 平近点角

	// 显示角度值结果
    pInfo->e = euclidean_norm(3, e);
    pInfo->i = pInfo->i * 180 / PI;
    pInfo->Omega = pInfo->Omega * 180 / PI;
    pInfo->omega = pInfo->omega * 180 / PI;
    pInfo->M = pInfo->M * 180 / PI;
}
