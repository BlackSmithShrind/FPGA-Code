#include "Gp_PatDef.h"

// 获取真近点角
double Get_f(double e, double E)
{
    double f;
    f = 2 * atan(tan(E / 2) / sqrt((1 - e) / (1 + e)));
    return f;
}
