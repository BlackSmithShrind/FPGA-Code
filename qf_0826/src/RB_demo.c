#include "xaxidma.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xintc.h"
#include "xgpio.h"
#include "Bsp/Axi/Gp_Axi.h"
#include "Bsp/Axi/Gp_AxiDef.h"
#include "Bsp/Ddr/Gp_Ddr.h"
#include "Bsp/Ddr/Gp_DdrDef.h"
#include "Bsp/Dma/Gp_Dma.h"
#include "Bsp/Dma/Gp_DmaDef.h"
#include "Bsp/Gpio/Gp_Gpio.h"
#include "Bsp/Gpio/Gp_GpioDef.h"
#include "Bsp/Intc/Gp_Intc.h"
#include "Bsp/Intc/Gp_IntcDef.h"


int unRet;

int rb_err_cnt;


u16 CG_save_package_num_ps;
u16 CG_save_package_num_pl;
u16 CG_send_package_num_ps;
u16 CG_package_total;
u16 CG_source;
u16 CG_destination;
u16 CG_Data_package_code;
u32 CG_flash_partition;
u16 CG_flash_num;
u16 CG_check_code;




#define DMASAVE_DONE_GPIO_MASK		GP_CG_RX_U5_GPIO_BITWIDTH_MASK

static int SetupIntrSystem(XIntc * IntcInstancePtr,
			   XAxiDma * AxiDmaPtr, u16 TxIntrId, u16 RxIntrId);

//XGpio gGpioInst1;
int main(void)
{

    int Status;
	XAxiDma_Config *Config;
	int flag;
	CG_save_package_num_ps = 0;

    //设定储存、发送地址
	u8 *Save_TxBufferPtr;
	u8 *Save_RxBufferPtr;

	// Save_TxBufferPtr = (u8 *)PRJDATA_BRAM_SOURCE; //
	// Save_RxBufferPtr = (u8 *)PRJDATA_BASE_ADDR; //

	u8 *Send_TxBufferPtr;
	u8 *Send_RxBufferPtr;

	// Send_TxBufferPtr = (u8 *)PRJDATA_BASE_ADDR; //
	// Send_RxBufferPtr = (u8 *)PRJDATA_BRAM_DESTINATION; //



	xil_printf("\r\n--- Entering main() --- \r\n");


	//-----------------------------初始化中断-----------------------------------//
	Status = intcInitialize(&gIntcInst0, GP_INTC_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Failed Intr Initialize\r\n");
		return XST_FAILURE;
	}

	//-----------------------------连接中断处理函数-----------------------------------//

	//external
	Status =  intcConnect(&gIntcInst0, PRJ_WR_DONE_INTR_ID, intcCG_saveInstrHandler, (void*)0);

	Status =  intcConnect(&gIntcInst0, PRJ_RD_DONE_INTR_ID, intcCG_sendInstrHandler, (void*)0);


	//-------------------------------使能单个中断-------------------------------------//
	//external
	Status = intcEnable(&gIntcInst0, PRJ_WR_DONE_INTR_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("intc enable failed %d\r\n", Status);
		return XST_FAILURE;
	}

	Status = intcEnable(&gIntcInst0, PRJ_RD_DONE_INTR_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("intc enable failed %d\r\n", Status);
		return XST_FAILURE;
	}

	//初始化DMA
	Status = dmaInitialize(&gAxiDmaInst0, GP_AXIDMA_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Failed DMA Initialize\r\n");
		return XST_FAILURE;
	}

	//dma
	Status = dmaSetupInterruptSystem(&gIntcInst0,
						GP_INTC_0_DMA0_FIFO0_MM2S_INTROUT_INT_ID , dmaTxIntrHandler,  &gAxiDmaInst0,
						GP_INTC_0_DMA0_FIFO0_S2MM_INTROUT_INT_ID , dmaRxIntrHandler,  &gAxiDmaInst0);


	//dma
	Status = intcEnable(&gIntcInst0, GP_INTC_0_DMA0_FIFO0_MM2S_INTROUT_INT_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("intc enable failed %d\r\n", Status);
		return XST_FAILURE;
	}

	Status = intcEnable(&gIntcInst0, GP_INTC_0_DMA0_FIFO0_S2MM_INTROUT_INT_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("intc enable failed %d\r\n", Status);
		return XST_FAILURE;
	}

	//-------------------------------DMA中断使能-------------------------------------//
	Status = dmaIntrDisable(&gAxiDmaInst0, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
	if (Status != XST_SUCCESS) {
		xil_printf("dmaIntrDisable failed %d\r\n", Status);
		return XST_FAILURE;
	}

	Status = dmaIntrDisable(&gAxiDmaInst0, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	if (Status != XST_SUCCESS) {
		xil_printf("dmaIntrDisable failed %d\r\n", Status);
		return XST_FAILURE;
	}

	Status = dmaIntrEnable(&gAxiDmaInst0, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
	if (Status != XST_SUCCESS) {
		xil_printf("dmaIntrEnable failed %d\r\n", Status);
		return XST_FAILURE;
	}

	Status = dmaIntrEnable(&gAxiDmaInst0, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	if (Status != XST_SUCCESS) {
		xil_printf("dmaIntrEnable failed %d\r\n", Status);
		return XST_FAILURE;
	}


	//-------------------------------整体中断开始-------------------------------------//
	Status = intcStart(&gIntcInst0, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		xil_printf("wholeintc enable failed %d\r\n", Status);
		return XST_FAILURE;
	}


	//-------------------------------使能硬件中断-------------------------------------//
	Status = hardwareintcEnable(&gIntcInst0);
	if (Status != XST_SUCCESS) {
		xil_printf("hardwareintc enable failed %d\r\n", Status);
		return XST_FAILURE;
	}



	//GPIO初始化
	Status = gpioInitialize(&gGpioInst0, GP_GPIO_0_DEVICE_ID);
	Status = gpioInitialize(&gGpioInst1, GP_GPIO_1_DEVICE_TD);
	
	gpioSetDataDirection(&gGpioInst0, 1, 0 );
	gpioSetDataDirection(&gGpioInst0, 2, 0 );
	gpioSetDataDirection(&gGpioInst1, 1, 0 );
	// 
	gpioDiscreteWrite(&gGpioInst0, 1, 0 );
	gpioDiscreteWrite(&gGpioInst0, 2, 0 );
	gpioDiscreteWrite(&gGpioInst1, 1, 0 );

	gDmaRxCompletionFlag = 0;
	gDmaTxCompletionFlag = 0;
	unRet = 0;
	
	//
	gpioDiscreteWrite(&gGpioInst0, 1, 1 );
	gpioDiscreteWrite(&gGpioInst0, 1, 0 );


	//这里记录一个想法：
	//可以用状态的方式给每个中断拉高的信号也做一个类似case的多语句判断，C里面是switch？
	//不同的状态进入不同的操作方式


	while(1){
		if(CG_save_start == 1){
			//初始化参数
			gDmaRxCompletionFlag = 0;
			gDmaTxCompletionFlag = 0;
			unRet = 0;
			//正在传输
			axiWriteReg(GP_AXI_BASE_REG, (u32)GP_AIX_CG_FIXED_FRAME_FIXED_FINISH_FLAG_OFFSET, 0x00);

			CG_save_ready = 0;	//save传输完成标志

			//读取寄存器控制数据
			CG_source = axiReadReg(GP_AXI_BASE_REG, (u32)GP_AIX_CG_FIXED_FRAME_SOURCE_ADDR_OFFSET) && 0x0000ffff;
			CG_destination = axiReadReg(GP_AXI_BASE_REG, (u32)GP_AIX_CG_FIXED_FRAME_DEST_ADDR_OFFSET) && 0x0000ffff;
			CG_save_package_num_pl = axiReadReg(GP_AXI_BASE_REG, (u32)GP_AIX_CG_FIXED_FRAME_SUB_PACKAGE_SEQ_OFFSET) && 0x0000ffff;
			CG_package_total = axiReadReg(GP_AXI_BASE_REG, (u32)GP_AIX_CG_FIXED_FRAME_SUB_PACKAGE_NUM_OFFSET) && 0x0000ffff;
			CG_Data_package_code = axiReadReg(GP_AXI_BASE_REG, (u32)GP_AIX_CG_FIXED_FRAME_DATA_PACKAGE_CODE_OFFSET) && 0x0000ffff;
			CG_flash_partition = axiReadReg(GP_AXI_BASE_REG, (u32)GP_AIX_CG_FIXED_FRAME_FLASH_PARTITION_NUM_OFFSET) && 0xffffffff;
			CG_flash_num = axiReadReg(GP_AXI_BASE_REG, (u32)GP_AIX_CG_FIXED_FRAME_FLASH_NUM_OFFSET) && 0x0000ffff;
			CG_check_code = axiReadReg(GP_AXI_BASE_REG, (u32)GP_AIX_CG_FIXED_FRAME_CHECK_CODE_OFFSET) && 0x0000ffff;

			memcpy(Save_RxBufferPtr + CG_DATA_FRAME_PACKAGE + GP_AIX_CG_FIXED_FRAME_DEST_ADDR_OFFSET, &CG_destination, sizeof(CG_destination));
			memcpy(Save_RxBufferPtr + CG_DATA_FRAME_PACKAGE + GP_AIX_CG_FIXED_FRAME_SUB_PACKAGE_SEQ_OFFSET, &CG_send_package_num_ps, sizeof(CG_send_package_num_ps));
			memcpy(Save_RxBufferPtr + CG_DATA_FRAME_PACKAGE + GP_AIX_CG_FIXED_FRAME_SUB_PACKAGE_NUM_OFFSET, &CG_package_total, sizeof(CG_package_total));
			memcpy(Save_RxBufferPtr + CG_DATA_FRAME_PACKAGE + GP_AIX_CG_FIXED_FRAME_DATA_PACKAGE_CODE_OFFSET, &CG_Data_package_code, sizeof(CG_Data_package_code));
			memcpy(Save_RxBufferPtr + CG_DATA_FRAME_PACKAGE + GP_AIX_CG_FIXED_FRAME_FLASH_PARTITION_NUM_OFFSET, &CG_flash_partition, sizeof(CG_flash_partition));
			memcpy(Save_RxBufferPtr + CG_DATA_FRAME_PACKAGE + GP_AIX_CG_FIXED_FRAME_FLASH_NUM_OFFSET, &CG_flash_num, sizeof(CG_flash_num));
			memcpy(Save_RxBufferPtr + CG_DATA_FRAME_PACKAGE + GP_AIX_CG_FIXED_FRAME_CHECK_CODE_OFFSET, &CG_check_code, sizeof(CG_check_code));
			memcpy(Save_RxBufferPtr + TBD , (CGDATA_BASE_ADDR + destination * DESTINATION_PART + CG_save_package_num_pl * CGDATA_PACKAGE);, 536);
			 



			
			//根据寄存器数据分配DDR地址
			if(CG_save_package_num_pl == CG_save_package_num_ps){	//子包序号正确，传输数据
				Save_TxBufferPtr = (u8 *)CG_DATA_BRAM_SOURCE; 	//重构数据Bram源地址
				//开始传输
				Save_RxBufferPtr = (u8 *)(GP_CG_DATA_BASE_ADDR + CG_destination * CG_DATA_DESTINATION_PART + CG_save_package_num_pl * CG_DATA_FRAME_OFFSET); 	//重构数据DDR储存地址
				Status = dmaTransfer(&gAxiDmaInst0, Save_TxBufferPtr ,XAXIDMA_DMA_TO_DEVICE,
													Save_RxBufferPtr,  XAXIDMA_DEVICE_TO_DMA, 
																		GP_DMA_TRANSFER_MAX_PKT_LEN);
				//检验传输是否正常完成
				if (unRet) {				//输出报错
					xil_printf("Failed test transmit%s done, "
					"receive%s done\r\n", gDmaRxCompletionFlag? "":" not",
							gDmaTxCompletionFlag? "":" not");
					return SEC_ERROR;
				}
				CG_save_package_num_ps = CG_save_package_num_ps + 1;		//子包序号+1
				if (CG_save_package_num_pl > CG_package_total)	//传输完成
				{
						CG_save_all = 1;	//一组重构数据包save传输完成标志
						CG_save_package_num_ps = 0;	//子包序号清零
						axiWriteReg(GP_AXI_BASE_REG, (u32)GP_AIX_CG_FIXED_FRAME_FIXED_FINISH_FLAG_OFFSET, 0x02);
				}
				else{
						CG_save_all = 0;
						//用寄存器发送完成信号
						axiWriteReg(GP_AXI_BASE_REG, (u32)GP_AIX_CG_FIXED_FRAME_FIXED_FINISH_FLAG_OFFSET, 0x01);
				}
				
			}	
			else	//子包序号有误，跳过传输
			{	
				//直接跳过传输，发送完成信号
				//用寄存器发送完成信号
				axiWriteReg(GP_AXI_BASE_REG, (u32)GP_AIX_CG_FIXED_FRAME_FIXED_FINISH_FLAG_OFFSET, 0x01);
			}	

			CG_save_start = 0;	//save传输完成，状态清零
			CG_save_ready = 1;	//save传输完成标志
		}


		if(CG_send_start || CG_save_all){ //当save全部传输完成时开启第一次传输或者send中断时传输下一帧
			//初始化参数
			CG_send_all = 0;
			CG_send_ready = 0;	//send传输完成标志
			//正在传输

			//输出重构数据固化帧到A3000

			//储存到临时地址
			u8 temp_buffer[512];

			//发送到A3000
			if (CG_destination == ZHUKONG)	//主控
			{
				Status = uartNs550Send(&gCg2ZkRs644UartNs550Inst, temp_buffer, 536);
				if (Status != XST_SUCCESS) {
					xil_printf("zhongkong send failed %d\r\n", Status);
					return XST_FAILURE;
				}
			}
			else if (CG_destination == TONGXIN)	//通信
			{
				Status = uartNs550Send(&gCg2TxRs644UartNs550Inst, temp_buffer, 536);
				if (Status != XST_SUCCESS) {
					xil_printf("tongxin send failed %d\r\n", Status);
					return XST_FAILURE;
				}
			}
			else if (CG_destination == FSM0)	//从控
			{
				Status = uartLiteSend(&gCg2Fsm0Rs422UartLiteInst, temp_buffer, 536);
				if (Status != XST_SUCCESS) {
					xil_printf("FSM0 send failed %d\r\n", Status);
					return XST_FAILURE;
				}
			}
			else if (CG_destination == FSM1)
			{
				Status = uartLiteSend(&gCg2Fsm1Rs422UartLiteInst, temp_buffer, 536);
				if (Status != XST_SUCCESS) {
					xil_printf("FSM1 send failed %d\r\n", Status);
					return XST_FAILURE;
				}
			}
			
			
			Status = uartNs550Send(&gUartNs550Inst0, temp_buffer, 536);


			CG_save_start = 0;	//save传输完成，状态清零
			CG_save_ready = 1;	//save传输完成标志
		}

		
		










		if(PRJ_save_start == 1){   		//工程遥测数据传输
			//初始化参数
			gDmaRxCompletionFlag = 0;
			gDmaTxCompletionFlag = 0;
			unRet = 0;

			PRJ_save_ready = 0;	//save传输完成标志

			//读取寄存器控制数据
			CGDATA_ctrl = DATA_in(DATA_SAVE_BASEADDR, (u32)CGDATA_CTRL_OFFSET);
			CG_save_package_num_pl = CGDATA_ctrl & 0xff000000;
			CG_package_total = CGDATA_ctrl & 0x00ff0000;
			source = CGDATA_ctrl & 0x0000ff00;
			destination = CGDATA_ctrl & 0x000000ff;
			
			//根据寄存器数据分配DDR地址
			if(CG_save_package_num_pl == CG_save_package_num_ps){	//子包序号正确，传输数据
				Save_TxBufferPtr = (u8 *)PRJDATA_BRAM_SOURCE; 	//工程遥测数据Bram源地址
				//开始传输
				Save_RxBufferPtr = (u8 *)(PRJDATA_BASE_ADDR + destination * DESTINATION_PART + CG_save_package_num_pl * PRJDATA_PACKAGE); 	//工程遥测数据DDR储存地址
				Status = dmaTransfer(&gAxiDmaInst0, Save_TxBufferPtr ,XAXIDMA_DMA_TO_DEVICE,
													Save_RxBufferPtr,  XAXIDMA_DEVICE_TO_DMA, 
																		GP_DMA_TRANSFER_MAX_PKT_LEN);
				//检验传输是否正常完成
				if (unRet) {				//输出报错
					xil_printf("Failed test transmit%s done, "
					"receive%s done\r\n", gDmaRxCompletionFlag? "":" not",
							gDmaTxCompletionFlag? "":" not");
					return SEC_ERROR;
				}
				CG_save_package_num_ps = CG_save_package_num_ps + 1;	//子包序号+1
				//TBD 用寄存器发送完成信号

				
			}	
			else	//子包序号有误，跳过传输
			{	
				//直接跳过传输，发送完成信号
				//TBD 用寄存器发送完成信号
			}	

			if (CG_save_package_num_pl > CG_package_total)	//传输完成
			{
				PRJ_save_all = 1;	//一组工程遥测数据包save传输完成标志
			}

			PRJ_save_start = 0;	//save传输完成，状态清零
			PRJ_save_ready = 1;	//save传输完成标志
		}

		if(PRJ_send_start == 1){  		//工程遥测数据发送
			//初始化参数
			gDmaRxCompletionFlag = 0;
			gDmaTxCompletionFlag = 0;
			unRet = 0;

			PRJ_send_ready = 0;	//send传输完成标志

			//读取寄存器控制数据
			CGDATA_ctrl = DATA_in(DATA_send_BASEADDR, (u32)CGDATA_CTRL_OFFSET);
			CG_save_package_num_pl = CGDATA_ctrl & 0xff000000;
			CG_package_total = CGDATA_ctrl & 0x00ff0000;
			source = CGDATA_ctrl & 0x0000ff00;
			destination = CGDATA_ctrl & 0x000000ff;
			
			//根据寄存器数据分配DDR地址
			if(CG_save_package_num_pl == CG_save_package_num_ps){	//子包序号正确，传输数据
				Save_TxBufferPtr = (u8 *)(PRJDATA_BASE_ADDR + destination * DESTINATION_PART + CG_save_package_num_pl * PRJDATA_PACKAGE); 	//工程遥测数据Bram源地址
				//开始传输
				Save_RxBufferPtr = (u8 *)PRJDATA_BRAM_DESTINATION; 	//工程遥测数据DDR储存地址
				Status = dmaTransfer(&gAxiDmaInst0, Save_TxBufferPtr ,XAXIDMA_DMA_TO_DEVICE,
													Save_RxBufferPtr,  XAXIDMA_DEVICE_TO_DMA, 
																		GP_DMA_TRANSFER_MAX_PKT_LEN);
				//检验传输是否正常完成
				if (unRet) {				//输出报错
					xil_printf("Failed test transmit%s done, "
					"receive%s done\r\n", gDmaRxCompletionFlag? "":" not",
							gDmaTxCompletionFlag? "":" not");
					return SEC_ERROR;
				}
				CG_save_package_num_ps = CG_save_package_num_ps + 1;	//子包序号+1
				//TBD 用寄存器发送完成信号

				
			}	
			else	//子包序号有误，跳过传输
			{	
				//直接跳过传输，发送完成信号
				//TBD 用寄存器发送完成信号
			}	

			if (CG_save_package_num_pl > CG_package_total)	//传输完成
			{
				PRJ_send_all = 1;	//一组工程遥测数据包send传输完成标志
			}

			PRJ_send_start = 0;	//send传输完成，状态清零
			PRJ_send_ready = 1;	//send传输完成标志
		}

		


	}



}



void intcCG_saveInstrHandler()
{
	if(CG_save_ready == 1){
		CG_save_start = 1;
		CG_save_ready = 0;
	}
	else{
		CG_save_start = 0;
		CG_save_ready = 1;
	}
}

void intcPRJ_sendInstrHandler()
{


}

	static int SetupIntrSystem(XIntc * IntcInstancePtr,
				   XAxiDma * AxiDmaPtr, u16 TxIntrId, u16 RxIntrId)
	{
		int Status;


		/* Initialize the interrupt controller and connect the ISRs */
		Status = XIntc_Initialize(IntcInstancePtr, 0);
		if (Status != XST_SUCCESS) {

			xil_printf("Failed init intc\r\n");
			return XST_FAILURE;
		}

		Status = XIntc_Connect(IntcInstancePtr, TxIntrId,
				       (XInterruptHandler) dmaTxIntrHandler, AxiDmaPtr);
		if (Status != XST_SUCCESS) {

			xil_printf("Failed tx connect intc\r\n");
			return XST_FAILURE;
		}

		Status = XIntc_Connect(IntcInstancePtr, RxIntrId,
				       (XInterruptHandler) dmaRxIntrHandler, AxiDmaPtr);
		if (Status != XST_SUCCESS) {

			xil_printf("Failed rx connect intc\r\n");
			return XST_FAILURE;
		}

		//	// 涓柇杩炴帴
		Status = intcConnect(&gIntcInst0, PRJ_WR_DONE_INTR_ID, intcCG_saveInstrHandler ,  (void*)0);
		if (Status != XST_SUCCESS) {
			xil_printf("Initialization failed %d\r\n", Status);
			return XST_FAILURE;
		}

		// 涓柇杩炴帴
		Status = intcConnect(&gIntcInst0, PRJ_RD_DONE_INTR_ID, intcCG_sendInstrHandler ,  (void*)0);
		if (Status != XST_SUCCESS) {
			xil_printf("Initialization failed %d\r\n", Status);
			return XST_FAILURE;
		}


		/* Start the interrupt controller */
		Status = XIntc_Start(IntcInstancePtr, XIN_REAL_MODE);
		if (Status != XST_SUCCESS) {

			xil_printf("Failed to start intc\r\n");
			return XST_FAILURE;
		}

		XIntc_Enable(IntcInstancePtr, TxIntrId);
		XIntc_Enable(IntcInstancePtr, RxIntrId);

		Status = intcEnable(&gIntcInst0, PRJ_WR_DONE_INTR_ID);
		if (Status != XST_SUCCESS) {
			xil_printf("Initialization failed %d\r\n", Status);
			return XST_FAILURE;
		}
		Status = intcEnable(&gIntcInst0, PRJ_RD_DONE_INTR_ID);
		if (Status != XST_SUCCESS) {
			xil_printf("Initialization failed %d\r\n", Status);
			return XST_FAILURE;
		}

		/* Enable interrupts from the hardware */

		Xil_ExceptionInit();
		Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler)XIntc_InterruptHandler,
				(void *)IntcInstancePtr);

		Xil_ExceptionEnable();

		return XST_SUCCESS;
	}



