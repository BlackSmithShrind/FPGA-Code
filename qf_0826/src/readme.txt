1、 qf软核工程框架目录结构        
    App:应用层单元
        |_Apd：apd操作模块
        |_Cg：重构模块
        |_Edfa：光纤放大器操作模块
        |_Eml：eml操作模块
        |_Fsm：电磁驱动操作模块
        |_IntiChk：初始化自检模块
        |_Pat：初始指向计算模块
        |_Pp：电源管理操作模块
        |_Sa：同轴度标校操作模块（待定）
        |_Sc：日凌规避操作模块（待定）
        |_Tc：遥控模块
        |_Tm：遥测模块
        |_Wm：工作模式管理模块
        |_Xm：星敏感器操作模块

    Base:基础单元
        |_Comm：通用模块

    Bsp:板级库单元
        |_Axi：Axi-reg模块
        |_Bram:  bram模块
        |_Ddr：ddr模块
        |_Dma：dma模块
        |_Exception：异常中断模块（启动硬中断）
        |_Gpio：gpio模块
        |_Intc：中断模块
        |_Platform：平台相关模块（i\dcache相关设置）
        |_Spi：spi接口模块
        |_Timer：定时器模块
        |_Uartlite：uart接口模块(固定波特率)
        |_Uartns550：uartns550接口模块(参数可设)

    Test:测试单元
        |_DebugUart：debug uart串口模块（用于测试）
        |_XmTest：星敏感器测试模块
        |_EdfaTest：Edfa测试模块

2、地址分配
     Available Memory Regions
    	name												base address	size
    microblaze_0_local_memory_ilmb_bram_if_cntlr_Mem_microblaze_0_local_memory_dlmb_bram_if_cntlr_Mem			0x50		0xFFB0
	BRAM_1_axi_bram_ctrl_3_Mem0									0x10000		0x8000
	BRAM_0_axi_bram_ctrl_1_Mem0									0x20000		0x8000
	BRAM_2_axi_bram_ctrl_3_Mem0									0x10000000	0x2000
	mig_7series_0_memaddr										0x80000000	0x80000000

    Stack and Heap Sizes
	stack size														0x400
	heap size														0x800

Section to Memory Region Mapping
	.heap        microblaze_0_local_memory_ilmb_bram_if_cntlr_Mem_microblaze_0_local_memory_dlmb_bram_if_cntlr_Mem
	.stack	microblaze_0_local_memory_ilmb_bram_if_cntlr_Mem_microblaze_0_local_memory_dlmb_bram_if_cntlr_Mem



        