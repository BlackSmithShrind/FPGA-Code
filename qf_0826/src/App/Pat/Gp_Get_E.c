#include "Gp_PatDef.h"

#define EPSILON 1e-10

// 获取偏近点角
double Get_E(double M, double e)
{
    double E0 = M;
    double E1 = M + e * sin(E0);
    while (fabs(E1 - E0) > EPSILON)
    {
        E0 = E1;
        E1 = M + e * sin(E0);
    }
    return E1;
}
