#include "Gp_PatDef.h"

// ��ȡƽ�����
double Get_M(double a, double t_now)
{
	//����t0=0ʱ�̵�ƽ����� M0=0
    double t0 = 0;
    double M0 = 0;
    double Miu = 398601.2;//��������
    double M;
    M = sqrt(Miu / pow(a, 3)) * (t_now - t0) + M0;
    return M;
}

