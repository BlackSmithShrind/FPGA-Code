#include "GP_WM.h"

/****************************************************************************/
/*                                                                          */
/*              单元变量			                                            */
/*                                                                          */
/****************************************************************************/
/*static*/ emWorkMode gWorkMode;

// 初始化
int WM_Init(void)
{
	// 工作模式需要自己的初始化
	// TBD

	// 上电初始化后进入待机模式
	gWorkMode = WM_IDLE;

	return SEC_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////
// 模式改变接口
void ChangeWorkMode(emWorkMode unWorkMode)
{
	gWorkMode = unWorkMode;
}
