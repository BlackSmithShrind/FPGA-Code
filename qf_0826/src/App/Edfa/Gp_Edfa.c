/*@file     Gp_Edfa.c
* @brief    edfaģ��Դ�ļ�
* @details  ���ͨ�Ű�FPGA�����MB֮��ʹ��axi���߼Ĵ���+io+intc
*           ���MB��EDFA֮��ʹ��RS422�ӿڣ�ʵ��ң��ָ�ң���ѯָ��ͺ����ݽ���
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/07
*/
#include "Gp_Edfa.h"
#include "../../Bsp/UartLite/Gp_UartLite.h"
#include "../../Bsp/Intc/Gp_Intc.h"
#include "../../Bsp/Axi/Gp_Axi.h"
#include "../../Test/DebugUart/Gp_DebugUart.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
unsigned char gEdfaTxDataBuffer[EDFA_TX_DATA_BUFFER_SIZE] = "";
unsigned char gEdfaRxDataBuffer[EDFA_RX_DATA_BUFFER_SIZE] = "";
volatile unsigned short gEdfaInitIsReady = FALSE;                            // �Ƿ��Ѿ���ɳ�ʼ��
volatile unsigned short gEdfaHeaderConstBytesIsRecv = FALSE;                 // ͷ���̶�����
volatile unsigned short gEdfaRemainRecvBytes = 0;                            // ʣ���ֽ���
volatile unsigned short gEdfaIntcRxNewDataIsFin = FALSE;                     // ��ɽ���������

volatile unsigned int gEdfaTotalReceivedCount = 0;                           // ��Ҫ���յ��ֽ���
volatile unsigned int gEdfaTotalSentCount = 0;                               // ��Ҫ���͵��ֽ���

/****************************************************************************
 *              initialization                                        
 ****************************************************************************/
int edfaInit(void)
{
    // ��ʼ��ȫ�ֱ���
    // ���ͻ���
    memset(gEdfaTxDataBuffer, 0, EDFA_TX_DATA_BUFFER_SIZE);
    // ���ջ���
    memset(gEdfaRxDataBuffer, 0, EDFA_RX_DATA_BUFFER_SIZE);
    // ͷ�������ֽ��Ƿ��Ѿ�����
    gEdfaHeaderConstBytesIsRecv = FALSE;
    // ��ʣ����ֽ���
    gEdfaRemainRecvBytes = 0;
    // ��ɽ���������
    gEdfaIntcRxNewDataIsFin = FALSE;

    //��ʼ��rs422����
    uartLiteInit(&gIntcInst0, &gEdfaRs422UartLiteInst, GP_EDFA_UARTLITE_DEVICE_ID, GP_INTC_0_EDFA_UART0_INT_ID, edfaRs422SendHandler, edfaRs422RecvHandler);

    // ӦΪ���ң��ң�ⶼ��CANָ��ͨ��������ֻ��ʹ��һ���жϴ���������Ӧ���ڲ��������ݽ�������
    // ����intc�жϿ�����ϵͳ In12	CAN instruction
    //intcConnect(&gIntcInstance, GP_INTC_EDFA_INT_ID, edfaAxiIntcCanInstrHandler, (void*)0);

    // �Ƿ��Ѿ���ɳ�ʼ��
    gEdfaInitIsReady = TRUE;

    return SEC_SUCCESS;
}

/****************************************************************************
 *              initialization self check                             
 ****************************************************************************/
int edfaInitChk(void)
{
    int nRet = SEC_ERROR;

    return nRet;
}

/****************************************************************************
 *              edfa��loop����
 ****************************************************************************/
int edfaLoop()
{
#ifdef EDFA_DEBUG_UART_TEST
	// debug uart test
	edfaDebugUartTestProcess();
#else
    // ִ�н��յ������ݴ�������
    edfaPl2PsCanYkProcess();
#endif

    // �첽��������
    //edfaRs422SendProcess();

    // �첽��������
    edfaRs422RecvProcess();

    return SEC_SUCCESS;
}

/****************************************************************************
 *             ���˷Ŵ���EDFAִ�л���->ͨ�Ű�PS->PFGA
 * RS422���ڽ��յ�EDFAִ�л������ص�ң����Ϣ
 ****************************************************************************/
int edfaRs422SendProcess(unsigned char* pBuffer, unsigned int unSize)
{
    int nRet = SEC_ERROR;

    // �������
    if (!pBuffer || !unSize)
    {
        return nRet;
    }

    // �ж��Ƿ��Ѿ���ʼ��
    if (!gEdfaInitIsReady)
    {
        return nRet;
    }

    // �첽��������
    uartLiteSend(&gEdfaRs422UartLiteInst, pBuffer, unSize);// 6:��ʼλ-2 ��ַ-2 ָ��-2 ����-2

    return nRet;
}

/****************************************************************************
 *             ���˷Ŵ���EDFAִ�л���->ͨ�Ű�PS->PFGA
 * RS422���ڽ��յ�EDFAִ�л������ص�ң����Ϣ
 ****************************************************************************/
int edfaRs422RecvProcess()
{
    int nRet = SEC_ERROR;
    //char szLogBuffer[256] = {0};

    // �ж��Ƿ��Ѿ���ʼ��
    if(!gEdfaInitIsReady)
    {
        return nRet;
    }

    // �첽����ͷ��8���ֽ�����
    if (!gEdfaHeaderConstBytesIsRecv)
    {
        uartLiteRecv(&gEdfaRs422UartLiteInst, gEdfaRxDataBuffer, EDFA_SF_HEADER_SIZE);// 6:��ʼλ-2 ��ַ-2 ָ��-2 ����-2

        gEdfaHeaderConstBytesIsRecv = TRUE;
    }
    
    // ����Ƿ��лص��������
    if (gEdfaTotalReceivedCount == EDFA_SF_HEADER_SIZE)
    {
        unsigned short usStartFlag = *(unsigned short*)gEdfaRxDataBuffer;
        if (usStartFlag != 0xEDFA)
        {
            return nRet;
        }

        unsigned short usOffset = 2;
        unsigned short usAddrFlag = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);
        usOffset += 2;
        unsigned short usCtrlCmd = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);
        usOffset += 2;
        unsigned short usDataLen = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);
        // д�����FPGA�Ĵ���
        //...
        //edfaEmifWriteRegister();

        gEdfaRemainRecvBytes = usDataLen + 1;
        uartLiteRecv(&gEdfaRs422UartLiteInst, gEdfaRxDataBuffer + EDFA_SF_HEADER_SIZE, gEdfaRemainRecvBytes);// 6:��ʼλ-2 ��ַ-2 ָ��-2 ����-2

        gEdfaTotalReceivedCount = 0;
    }

    // ���ʣ������ֽ���
    if (gEdfaTotalReceivedCount == gEdfaRemainRecvBytes && gEdfaRemainRecvBytes != 0)
    {
        // ģ���¶�
        unsigned short usOffset = 8;
        unsigned short usModeTmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ģ�������ѹ
        usOffset += 2;
        unsigned short usModuleInputVol = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ����ģʽ������
        usOffset += 1;
        unsigned char ucWorkModeAndSwitch = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ���빦��
        usOffset += 2;
        unsigned short usInputPower = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // �������
        usOffset += 2;
        unsigned short usOutputPower = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ��У����������
        usOffset += 2;
        unsigned short usCaliLaserAmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ��У�������¶�
        usOffset += 2;
        unsigned short usCaliLaserTmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ��У����������
        usOffset += 2;
        unsigned short usCaliLaserPower = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ��У������TEC����
        usOffset += 2;
        unsigned short usCaliLaserTecAmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ���� 1 ����
        usOffset += 2;
        unsigned short usPump1Amp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ���� 1 �¶�
        usOffset += 2;
        unsigned short usPump1Tmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ���� 1 �������
        usOffset += 2;
        unsigned short usPump1BacklightAmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ���� 1TEC ����
        usOffset += 2;
        unsigned short usPump1TecAmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ���� 2 ����
        usOffset += 2;
        unsigned short usPump2Amp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ���� 2 �¶�
        usOffset += 2;
        unsigned short usPump2Tmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ���� 3 ����
        usOffset += 2;
        unsigned short usPump3Amp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ���� 3 �¶�
        usOffset += 2;
        unsigned short usPump3Tmp = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ģ��澯״̬
        usOffset += 1;
        unsigned char ucModeWarningState = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ���ʸ澯״̬
        usOffset += 1;
        unsigned char ucPowerWarningState = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ��У�������澯״̬
        usOffset += 1;
        unsigned char ucCaliLaserWarningState = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ���� 1 �澯״̬
        usOffset += 1;
        unsigned char ucPump1WarningState = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ���� 2 �澯״̬
        usOffset += 1;
        unsigned char ucPump2WarningState = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ���� 3 �澯״̬
        usOffset += 1;
        unsigned char ucPump3WarningState = *(unsigned short*)(gEdfaRxDataBuffer + usOffset);

        // ����������
        usOffset += 1;
        unsigned char szManufacturerName[32] = { 0 };
        memcpy(szManufacturerName, (void*)(gEdfaRxDataBuffer + usOffset), 32);

        // ģ�����к�
        usOffset += 32;
        unsigned char szModuleSerialNumber[32] = { 0 };
        memcpy(szModuleSerialNumber, (void*)(gEdfaRxDataBuffer + usOffset), 32);

        // �̼��汾
        usOffset += 32;
        unsigned char szFirmwareVersion[32] = { 0 };
        memcpy(szFirmwareVersion, (void*)(gEdfaRxDataBuffer + usOffset), 32);

        // ģ����������
        usOffset += 32;
        unsigned char szModuleProductDate[32] = { 0 };
        memcpy(szModuleProductDate, (void*)(gEdfaRxDataBuffer + usOffset), 32);

        // д��ddr
        //...


        gEdfaHeaderConstBytesIsRecv = FALSE;

        gEdfaTotalReceivedCount = 0;
        gEdfaRemainRecvBytes = 0;
    }

    return nRet;
}

/****************************************************************************
 *              edfa rs422 �����жϻص�����
 * pCallbackRef:
 * unSendBytes:
 ****************************************************************************/
void edfaRs422SendHandler(void* pCallbackRef, unsigned int unSendBytes)
{
    // �������
    if (gEdfaTotalSentCount == unSendBytes)
    {
        gEdfaTotalSentCount = 0;
    }
}

/****************************************************************************
 *              edfa rs422 �����жϻص�����
 * pCallbackRef:
 * unRecvBytes:
 ****************************************************************************/
void edfaRs422RecvHandler(void* pCallbackRef, unsigned int unRecvBytes)
{
    // �����6���ֽ����ʾͷ���̶�λ��������ץȡ
    gEdfaTotalReceivedCount = unRecvBytes;
}

/****************************************************************************
 *              edfa intc ��Ӧ������CAN instruction��
 * ����ƽ̨ -can-> ͨ�Ű�PL -AXI-INTC AXI-REG->ͨ�Ű�PS
 * pCallbackRef:
 * ע�⣺�ȷ��������ͳһ����
 ****************************************************************************/
void edfaAxiIntcCanInstrHandler(void* pCallbackRef)
{
    unsigned char ucDataType = *(volatile unsigned char  *)GP_AIX_CAN_BUS_YKYC_SINGLE_DATA_TYPE_REG_ADDR;
    switch (ucDataType)
    {
        case 0x00: // ң�⣺0x00��֡
        {
            
        }break;
        case 0x0A: // ң�أ�0x0A��֡
        {
            if (!gEdfaIntcRxNewDataIsFin)
            {
                // ����copy����
                memset(gEdfaRxDataBuffer, 0, EDFA_RX_DATA_BUFFER_SIZE);
                memcpy((void*)gEdfaRxDataBuffer, (void*)GP_AIX_CAN_BUS_YKYC_SINGLE_DATA_TYPE_REG_ADDR, 8);

                // ���ñ�ʶ
                gEdfaIntcRxNewDataIsFin = TRUE;
            }
        }break;
        case 0x3A: // ң�⣺0x3A����֡
        {

        }break;
        case 0x0F: // ң�أ�0x0F��֡
        {

        }break;
        default:
        {
        }break;
    }
}

/****************************************************************************
 *  ����ƽ̨ - CAN YK -> ͨ�Ű�PL CAN - AXI-REG -> ͨ�Ű�PS - RS422-> EDFA
 * RS422���ڷ���ң��ң��ָ�EDFAִ�л���
 ****************************************************************************/
int edfaPl2PsCanYkProcess()
{
    int nRet = SEC_ERROR;
    //char szLogBuffer[256] = { 0 };

    // �ж��Ƿ��Ѿ���ʼ��
    if (!gEdfaInitIsReady)
    {
        return nRet;
    }

    // ���µ�����Ҫ����
    if (!gEdfaIntcRxNewDataIsFin)
    {
        return nRet;
    }

    // ����Ƿ��лص��������
    unsigned char usOffset = 1;
    unsigned char ucDataType = *(volatile unsigned char  *)gEdfaRxDataBuffer;
    unsigned char ucDataNum = *(volatile unsigned char  *)(gEdfaRxDataBuffer + usOffset++);
    unsigned char ucFrameNum = *(volatile unsigned char  *)(gEdfaRxDataBuffer + usOffset++);
    unsigned char ucCmdContent1 = *(volatile unsigned char  *)(gEdfaRxDataBuffer + usOffset++);
    unsigned char ucCmdContent2 = *(volatile unsigned char  *)(gEdfaRxDataBuffer + usOffset++);
    unsigned char ucCmdContent3 = *(volatile unsigned char  *)(gEdfaRxDataBuffer + usOffset++);
    unsigned char ucCmdContent4 = *(volatile unsigned char  *)(gEdfaRxDataBuffer + usOffset++);
    unsigned char ucCheckSum = *(volatile unsigned char  *)(gEdfaRxDataBuffer + usOffset++);
    switch (ucCmdContent1)
    {
        case 0x6F: // ��У����
        {
            // ����EDFA RS422Э��
            stuEdfaYkFrameInfo* pInfo = (stuEdfaYkFrameInfo *)gEdfaTxDataBuffer;
            gEdfaTotalSentCount = sizeof(stuEdfaYkFrameInfo);
            memset(pInfo, 0, gEdfaTotalSentCount);
            pInfo->usStartFlag = 0x55AA;
            pInfo->usAddrFlag = 0xFFFF;
            pInfo->usCtrlCmd = EDFA_CTRL_CMD_SET_CHANNEL_SWITCH;
            pInfo->pData[0] = 0;
            pInfo->pData[1] = 1;
            pInfo->ucXor = edfaCalcCheckXor((volatile unsigned char *)pInfo, gEdfaTotalSentCount - 1);

            // ��������
            nRet = edfaRs422SendProcess(gEdfaTxDataBuffer, gEdfaTotalSentCount);

        }break;
        default:
            break;
    }
    return nRet;
}

/****************************************************************************
 *  ����ƽ̨ - CAN YK -> ͨ�Ű�PL CAN - AXI-REG -> ͨ�Ű�PS - RS422-> EDFA
 * RS422���ڷ���ң��ң��ָ�EDFAִ�л���
 ****************************************************************************/
int edfaDebugUartTestProcess()
{
    int nRet = SEC_ERROR;
    //char szLogBuffer[256] = { 0 };

    // �ж��Ƿ��Ѿ���ʼ��
    if (!gEdfaInitIsReady)
    {
        return nRet;
    }

    // ���µ�����Ҫ����
    if (!gEdfaIntcRxNewDataIsFin)
    {
        return nRet;
    }

    // ����Ƿ��лص��������
    unsigned short usOffset = 0;
    unsigned short usStartFlag = *(volatile unsigned short *)gEdfaRxDataBuffer;
    usOffset += 2;
    unsigned short usConstFlag = *(volatile unsigned short *)(gEdfaRxDataBuffer + usOffset);
    usOffset += 2;
    unsigned short usCmd = *(volatile unsigned short *)(gEdfaRxDataBuffer + usOffset);
    usOffset += 2;
    unsigned short usDataLen = *(volatile unsigned short *)(gEdfaRxDataBuffer + usOffset);
    usOffset += 2 + usDataLen;
    unsigned char ucCheckSum = *(volatile unsigned char *)(gEdfaRxDataBuffer + usOffset);
    switch (usCmd)
    {
        case EDFA_CTRL_CMD_SET_CHANNEL_SWITCH: // ��У����
        {
            // ����EDFA RS422Э��
        	gEdfaTotalSentCount = 11;
            stuEdfaYkFrameInfo* pInfo = (stuEdfaYkFrameInfo *)gEdfaTxDataBuffer;
            memset(pInfo, 0, gEdfaTotalSentCount);
            pInfo->usStartFlag = 0x55AA;
            pInfo->usAddrFlag = 0xFFFF;
            pInfo->usCtrlCmd = EDFA_CTRL_CMD_SET_CHANNEL_SWITCH;
            pInfo->usDataLen = usDataLen;
            pInfo->pData[0] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 8);
            pInfo->pData[1] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 9);
            pInfo->ucXor = edfaCalcCheckXor((volatile unsigned char *)pInfo, gEdfaTotalSentCount - 1);
            gEdfaTxDataBuffer[gEdfaTotalSentCount - 1] = pInfo->ucXor;

            // ��������
            nRet = edfaRs422SendProcess(gEdfaTxDataBuffer, gEdfaTotalSentCount);
        }break;
        case EDFA_CTRL_CMD_SET_WORK_MODE: // ���ù���ģʽ
        {
            // ����EDFA RS422Э��
        	gEdfaTotalSentCount = 11;
            stuEdfaYkFrameInfo* pInfo = (stuEdfaYkFrameInfo *)gEdfaTxDataBuffer;
            memset(pInfo, 0, gEdfaTotalSentCount);
            pInfo->usStartFlag = 0x55AA;
            pInfo->usAddrFlag = 0xFFFF;
            pInfo->usCtrlCmd = EDFA_CTRL_CMD_SET_WORK_MODE;
            pInfo->usDataLen = usDataLen;
            pInfo->pData[0] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 8);
            pInfo->pData[1] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 9);
            pInfo->ucXor = edfaCalcCheckXor((volatile unsigned char *)pInfo, gEdfaTotalSentCount - 1);
            gEdfaTxDataBuffer[gEdfaTotalSentCount - 1] = pInfo->ucXor;

            // ��������
            nRet = edfaRs422SendProcess(gEdfaTxDataBuffer, gEdfaTotalSentCount);
        }break;
        case EDFA_CTRL_CMD_SET_PUMP_AMP: // ���ñ��ֵ���
        {
            // ����EDFA RS422Э��
        	gEdfaTotalSentCount = 12;
            stuEdfaYkFrameInfo* pInfo = (stuEdfaYkFrameInfo *)gEdfaTxDataBuffer;
            memset(pInfo, 0, gEdfaTotalSentCount);
            pInfo->usStartFlag = 0x55AA;
            pInfo->usAddrFlag = 0xFFFF;
            pInfo->usCtrlCmd = EDFA_CTRL_CMD_SET_PUMP_AMP;
            pInfo->usDataLen = usDataLen;
            pInfo->pData[0] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 8);
            pInfo->pData[1] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 9);
            pInfo->pData[2] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 10);
            pInfo->ucXor = edfaCalcCheckXor((volatile unsigned char *)pInfo, gEdfaTotalSentCount - 1);
            gEdfaTxDataBuffer[gEdfaTotalSentCount - 1] = pInfo->ucXor;

            // ��������
            nRet = edfaRs422SendProcess(gEdfaTxDataBuffer, gEdfaTotalSentCount);
        }break;
        case EDFA_CTRL_CMD_SET_OUTPUT_OPT_POWER: // ��������⹦��
        {
            // ����EDFA RS422Э��
        	gEdfaTotalSentCount = 12;
            stuEdfaYkFrameInfo* pInfo = (stuEdfaYkFrameInfo *)gEdfaTxDataBuffer;
            memset(pInfo, 0, gEdfaTotalSentCount);
            pInfo->usStartFlag = 0x55AA;
            pInfo->usAddrFlag = 0xFFFF;
            pInfo->usCtrlCmd = EDFA_CTRL_CMD_SET_OUTPUT_OPT_POWER;
            pInfo->usDataLen = usDataLen;
            pInfo->pData[0] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 8);
            pInfo->pData[1] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 9);
            pInfo->pData[2] = *(volatile unsigned char *)(gEdfaTxDataBuffer + 10);
            pInfo->ucXor = edfaCalcCheckXor((volatile unsigned char *)pInfo, gEdfaTotalSentCount - 1);
            gEdfaTxDataBuffer[gEdfaTotalSentCount - 1] = pInfo->ucXor;

            // ��������
            nRet = edfaRs422SendProcess(gEdfaTxDataBuffer, gEdfaTotalSentCount);
        }break;
        default:
        {

        }break;
    }

    // ���ñ�ʶ
    gEdfaIntcRxNewDataIsFin = FALSE;

    return nRet;
}

/****************************************************************************
 *                          ��У���ۼӺ�
 * У��λ�� 1 �ֽڣ� ����ʼλ��ʼ���������һλ��ֹ�������ֽڽ������������ֵ
 ****************************************************************************/
unsigned char edfaCalcCheckXor(volatile unsigned char* pBuff, unsigned short usLength)
{
    unsigned char ucCheckSum = 0;

    while (usLength--)
    {
        ucCheckSum ^= *pBuff++;
    }

    return ucCheckSum;
}


