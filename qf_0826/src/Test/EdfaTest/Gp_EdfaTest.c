/*@file     Gp_EdfaTest.c
* @brief    ���˷Ŵ���debug uart����
* @details  ���ڶԹ��˷Ŵ������в���
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/22
*/
#include "Gp_EdfaTest.h"
#include "../../App/Edfa/Gp_Edfa.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
unsigned char gEdfaDebugUartTxDataBuffer[EDFA_DEBUG_UART_TX_DATA_BUFFER_SIZE] = "";			// debug uart tx缓存
unsigned char gEdfaDebugUartRxDataBuffer[EDFA_DEBUG_UART_RX_DATA_BUFFER_SIZE] = "";			// debug uart rx缓存
volatile unsigned short gEdfaDebugUartRxNewDataIsFin = FALSE;                     		    // debug uart ��ʶ�Ƿ���������
volatile unsigned int gEdfaDebugUartTotalReceivedCount = 0;                                 // ���յ���������

/****************************************************************************
 *              edfa test loop
 ****************************************************************************/
int edfaTestLoop()
{
    // ���յ������ݽ������ݴ���
    edfaTestRecvProcess();

    return SEC_SUCCESS;
}

/****************************************************************************
 *             edfa test �������ݴ���
 * ͨ���˽ӿڴ���debug uart���յ������ݣ�copy��xmģ�������buffer��Ȼ����xmģ��process�������д���
 * �Խ�ģ��can intr �ж�fpga����
 ****************************************************************************/
int edfaTestRecvProcess()
{
    // �ж��Ƿ���յ�������
    if (!gEdfaDebugUartRxNewDataIsFin)
    {
        return SEC_SUCCESS;
    }

    // ����copy��App/Gp_Edfa.c��Ԫ�н��д���
    if (gEdfaDebugUartTotalReceivedCount != 0)
    {
        // ��������copy,Ȼ��xmģ��ȥ����
        memcpy(gEdfaRxDataBuffer, gEdfaDebugUartRxDataBuffer, gEdfaDebugUartTotalReceivedCount);

        // ģ��can instruction �ж�ָ���ȡ����
        gEdfaIntcRxNewDataIsFin = TRUE;

        // ����copy�����0
        gEdfaDebugUartTotalReceivedCount = 0;
    }

	// ����������
	gEdfaDebugUartRxNewDataIsFin = FALSE;

    return SEC_SUCCESS;
}


