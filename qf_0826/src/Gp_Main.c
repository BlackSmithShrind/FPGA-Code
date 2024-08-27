/*@file     Gp_Main.c
* @brief    涓荤▼搴�
* @details  QF椤圭洰閫氫俊鏉胯蒋浠秏icroblance涓诲嚱鏁板叆鍙�
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/14
*/

#include <stdio.h>
#include "xil_printf.h"
#include "./App/Xm/Gp_Xm.h"
#include "./App/Edfa/Gp_Edfa.h"

#include "./Base/Comm/Gp_Common.h"
#include "./Bsp/Gpio/Gp_Gpio.h"
#include "./Bsp/Intc/Gp_Intc.h"
#include "./Bsp/UartLite/Gp_UartLite.h"
#include "./Bsp/UartNs550/Gp_UartNs550.h"
#include "./Bsp/Dma/Gp_Dma.h"
#include "./Bsp/Platform/Gp_Platform.h"
#include "./Bsp/Exception/Gp_Exception.h"

#include "./Test/DebugUart/Gp_DebugUart.h"
#include "./Test/XmTest/Gp_XmTest.h"
#include "./Test/EdfaTest/Gp_EdfaTest.h"

int main()
{
    initPlatform();

    int nRet = SEC_ERROR;

#if 1
    // intc瀛愮郴缁熷垵濮嬪寲
    nRet = intcInit(&gIntcInst0, GP_INTC_0_DEVICE_ID);
    if (nRet != SEC_SUCCESS)
    {
        print("main: intcInit is error...\r\n");
        return nRet;
    }

    // 鎸傚湪鍏朵粬瀛愮郴缁�...

#endif

    // debug(开发板usb->uart)初始化
#if 1
    nRet = debugUartInit();
    if (nRet != SEC_SUCCESS)
    {
        print("main: debugUartInit is error...\r\n");
        return nRet;
    }
#endif

    // xm 初始化
#if 0
    nRet = xmInit();
    if (nRet != SEC_SUCCESS)
    {
        print("main: xmInit is error...\r\n");
        return nRet;
    }
#endif

    // edfa 初始化
#if 1
    nRet = edfaInit();
    if (nRet != SEC_SUCCESS)
    {
        print("main: edfaInit is error...\r\n");
        return nRet;
    }
#endif

#if 0
    // gpio鍒濆鍖�
    // 鏂瑰悜涓鸿緭鍑�
    unsigned int unDirectionMask = 0;
    // 鍒濆璁剧疆浣嶄綆鐢靛钩
    unsigned int unHighLowVolLevel = 0;
    nRet = gpioInit(&gGpioInst0, GP_GPIO_0_DEVICE_ID, GP_GPIO_CHANNEL_0, unDirectionMask, unHighLowVolLevel);
    if (nRet != SEC_SUCCESS)
    {
        print("main: gpioInit is error...\r\n");
        return nRet;
    }

    // 璁剧疆浣嶉珮鐢靛钩
    unsigned int unAllMask = 0xFFFF;
    gpioDiscreteWrite(&gGpioInst0, GP_GPIO_CHANNEL_0, unAllMask & GP_EDFA_POWER_GPIO_BITWIDTH_MASK);

    //delay...

    // 璁剧疆浣嶄綆鐢靛钩
    unAllMask = 0x0000;
    gpioDiscreteClear(&gGpioInst0, GP_GPIO_CHANNEL_0, unAllMask & GP_EDFA_POWER_GPIO_BITWIDTH_MASK);
#endif


#if 0
    // A3PE3000 -> 涓绘帶鏉匡紙閲嶆瀯rs644鍒濆鍖栵級
    // 鏋勫缓鏁版嵁鏍煎紡
    XUartNs550Format formatInfo;
    memset(&formatInfo, 0, sizeof(XUartNs550Format));
    formatInfo.BaudRate = GP_CG_2_ZK_UARTNS550_0_RS644_BAUDRATE_921600;
    formatInfo.DataBits = XUN_LCR_8_DATA_BITS;
    formatInfo.Parity = XUN_FORMAT_NO_PARITY;
    formatInfo.StopBits = XUN_FORMAT_1_STOP_BIT;
    // 鏁版嵁涓柇 浣胯兘FIFO
    unsigned int unOptions = XUN_OPTION_DATA_INTR | XUN_OPTION_FIFOS_ENABLE;
    nRet = uartNs550Init(&gIntcInst0, &gCg2ZkRs644UartNs550Inst, GP_CG_2_ZK_UARTNS550_0_DEVICE_ID, GP_INTC_0_CG_2_ZK_UART2_INT_ID, NULL/*鏍规嵁涓嶅悓鐨勭郴缁熻繘琛岃缃甤allbackhander*/, &formatInfo, unOptions);
    if (nRet != SEC_SUCCESS)
    {
        print("main: uartNs550Init is error...\r\n");
        return nRet;
    }
#endif 

#if 0
    // dma瀛愮郴缁�
    nRet = dmaInit(&gIntcInst0, &gAxiDmaInst0, GP_AXIDMA_0_DEVICE_ID, XPAR_INTC_0_AXIDMA_0_MM2S_INTROUT_VEC_ID, dmaTxIntrHandler, &gAxiDmaInst0, XPAR_INTC_0_AXIDMA_0_S2MM_INTROUT_VEC_ID, dmaRxIntrHandler, &gAxiDmaInst0);
    if (nRet != SEC_SUCCESS)
    {
        print("main: dmaInit is error...\r\n");
        return nRet;
    }
#endif
    
    // 鍚姩涓柇鎺у埗鍣�
    nRet = intcStart(&gIntcInst0, XIN_REAL_MODE);
    if (nRet != SEC_SUCCESS)
    {
        print("main: intcStart is error...\r\n");
        return nRet;
    }

    // 注意：必须启动硬件中断，不然中断系统不好使
    // 初始化exception
    exceptionInit();
    // 注册handler
	exceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XIntc_InterruptHandler, &gIntcInst0);
	// 使能
	exceptionEnable();

    // 主循环
    while(1)
    {
    	// debug uart loop
    	debugUartLoop();

        // xm uart test
#ifdef XM_DEBUG_UART_TEST
        // xm test loop
        xmTestLoop();

        // xm rs422 loop
        xmLoop();
#endif

        // xm uart test
#ifdef EDFA_DEBUG_UART_TEST
        // edfa test loop
        edfaTestLoop();

        // edfa loop
        edfaLoop();
#endif
    }

    // 清理工作
    cleanupPlatform();

    return 0;
}
