#include "Gp_Common.h"

/****************************************************************************
 *              延时
 ****************************************************************************/
void delayEx(unsigned int unDelay)
{
	unsigned int i;

	for(i = unDelay; i > 0; i--);
}

/**
 * \function startTime
 * \brief    clock function.（时间函数)
 * \param    None
 * \return   None
 */
long long startTime()
{
	return 0; //_itoll(TSCH, TSCL);
}

/**
 * \function endTime
 * \brief    clock function.（时间函数)
 * \param    None
 * \return   None
 */
long long endTime()
{
	return 0; //_itoll(TSCH, TSCL);
}

/**
 * \function diffTimeMs ms
 * \brief    clock diff function.（时间差值函数)
 * \param    None
 * \return   None
 */
long long diffTimeMs(long long i64StartTime, long long i64EndTime)
{
	long long i64Overhead = i64EndTime - i64StartTime;
	long long i64TotalTimeMs = i64Overhead / 456;
	printf("pointing Execution Cycle tOverhead=[%16lld] \n", i64Overhead);
	printf("pointing Execution Cost Time=[%lld us] (CPU Frequency:456MHz)\n", i64TotalTimeMs);
    return i64TotalTimeMs;
}

//inline long long read_time(void)
//{
//	extern cregister volatile unsigned int TSCL;
//	extern cregister volatile unsigned int TSCH;
//	long long l = (TSCH << 32) | TSCL;
//	return l;
//}
