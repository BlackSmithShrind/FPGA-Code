#include "Gp_PatDef.h"

// ��ȡ������
double Get_f(double e, double E)
{
    double f;
    f = 2 * atan(tan(E / 2) / sqrt((1 - e) / (1 + e)));
    return f;
}
