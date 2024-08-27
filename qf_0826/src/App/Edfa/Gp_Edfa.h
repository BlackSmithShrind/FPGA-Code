/*@file     Gp_Edfa.h
* @details  ���ͨ�Ű�FPGA�����MB֮��ʹ��axi���߼Ĵ���+io+intc
*           ���MB��EDFA֮��ʹ��RS422�ӿڣ�ʵ��ң��ָ�ң���ѯָ��ͺ����ݽ���
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/07
*/
#ifndef _GP_EDFA_H_
#define _GP_EDFA_H_

#include "Gp_EdfaDef.h"

/****************************************************************************/
/*              global variable                                             */
/****************************************************************************/
extern unsigned char gEdfaTxDataBuffer[EDFA_TX_DATA_BUFFER_SIZE];
extern unsigned char gEdfaRxDataBuffer[EDFA_RX_DATA_BUFFER_SIZE];
extern volatile unsigned short gEdfaInitIsReady;                            // �Ƿ��Ѿ���ɳ�ʼ������׼������
extern volatile unsigned short gEdfaHeaderConstBytesIsRecv;                 // ͷ���̶�����
extern volatile unsigned short gEdfaRemainRecvBytes;                        // ʣ���ֽ���
extern volatile unsigned short gEdfaIntcRxNewDataIsFin;                     // ��ɽ���������

extern volatile unsigned int gEdfaTotalReceivedCount;                       // ��Ҫ���յ��ֽ���
extern volatile unsigned int gEdfaTotalSentCount;                           // ��Ҫ���͵��ֽ���

/****************************************************************************/
/*              interface declaration                                       */
/****************************************************************************/

// edfaģ���ʼ���ӿ�
int edfaInit(void);
// edfaģ���ʼ���Լ�ӿ�
int edfaInitChk(void);
// ��������loop����
int edfaLoop();
// ���˷Ŵ���EDFAִ�л���->ͨ�Ű�PS->PFGA
int edfaRs422SendProcess(unsigned char* pBuffer, unsigned int unSize);
// ���˷Ŵ���EDFAִ�л���->ͨ�Ű�PS->PFGA
int edfaRs422RecvProcess();
// edfa rs422 �����жϻص�����
void edfaRs422SendHandler(void* pCallbackRef, unsigned int unSendBytes);
// edfa rs422 �����жϻص�����
void edfaRs422RecvHandler(void* pCallbackRef, unsigned int unRecvBytes);
// edfa intc ��Ӧ������CAN instruction������ƽ̨ -can-> ͨ�Ű�PL -AXI-INTC AXI-REG->ͨ�Ű�PS
//void edfaAxiIntcCanInstrHandler(void* pCallbackRef);
// ����ƽ̨ - CAN YK -> ͨ�Ű�PL CAN - AXI-REG -> ͨ�Ű�PS - RS422-> EDFA
int edfaPl2PsCanYkProcess();
// edfa debug uart test������
int edfaDebugUartTestProcess();
// У���
unsigned char edfaCalcCheckXor(volatile unsigned char *pBuff, unsigned short usLength);

#endif  // _GP_EDFA_H_
