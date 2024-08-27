#include "Gp_PatDef.h"

// 获取平近点角
double Get_M(double a, double t_now)
{
	//定义t0=0时刻的平近点角 M0=0
    double t0 = 0;
    double M0 = 0;
    double Miu = 398601.2;//引力常数
    double M;
    M = sqrt(Miu / pow(a, 3)) * (t_now - t0) + M0;
    return M;
}

