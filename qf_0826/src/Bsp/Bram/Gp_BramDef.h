/*@file     Gp_BramDef.h
* @brief    ��bram_v4.2��װ���м��ģ��ͷ�ļ�
* @details  �ṩbram�������ض���һЩdefine��enum��struct��
* @author   huyanfeng
* @email    398303171@qq.com
* @date     2024/08/22
*/
#ifndef _GP_BRAM_DEF_H_
#define _GP_BRAM_DEF_H_

#include "../../Base/Comm/Gp_Common.h"

/*****************************************************************************/
/*              macros, enums, and structure definitions                    */
/*****************************************************************************/
/*****************************************************************************/
/* bram�Ĵ�����ַ��                                                          */
/*****************************************************************************/
// XPAR_BRAM0����ַ
#define GP_XPAR_BRAM0_BASE_ADDR                             XPAR_BRAM_0_BASEADDR
// XPAR_BRAM1����ַ
#define GP_XPAR_BRAM1_BASE_ADDR                             XPAR_BRAM_1_BASEADDR
// XPAR_BRAM2����ַ
#define GP_XPAR_BRAM2_BASE_ADDR                             XPAR_BRAM_2_BASEADDR

/*****************************************************************************/
/* apd�������ݰ����bram�Ĵ���                                               */
/*****************************************************************************/
// apd power pkg���ε�ַƫ����
#define GP_XPAR_BRAM0_APD_POWER_PKG_OFFSET                  0x0002
// apd power pkg����ַ
#define GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR               (GP_XPAR_BRAM0_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_OFFSET) // ��ʼ��ַ

// apd power pkg��ʼ���ַƫ����
#define GP_XPAR_BRAM0_APD_POWER_PKG_START_FLAG_OFFSET       0x0002
// apd power pkg���ݰ���Ч���ݳ��ȵ�ַƫ����
#define GP_XPAR_BRAM0_APD_POWER_PKG_DATA_LEN_OFFSET         0x0002
// apd power pkgԴ��ַƫ����
#define GP_XPAR_BRAM0_APD_POWER_PKG_SRC_OFFSET              0x0002
// apd power pkgĿ�ĵ�ַƫ����
#define GP_XPAR_BRAM0_APD_POWER_PKG_DES_OFFSET              0x0002
// apd power pkg�Ӱ���Ŀ��ַƫ����
#define GP_XPAR_BRAM0_APD_POWER_PKG_SUB_PKG_NUM_OFFSET      0x0002
// apd power pkg�Ӱ����е�ַƫ����
#define GP_XPAR_BRAM0_APD_POWER_PKG_SUB_PKG_SEQ_OFFSET      0x0002
// apd power pkg���ݵ�ַƫ����
#define GP_XPAR_BRAM0_APD_POWER_PKG_DATA_OFFSET             0x0004
// apd power pkgУ��͵�ַƫ����
#define GP_XPAR_BRAM0_APD_POWER_PKG_SUM_OFFSET              0x0002
// apd power pkg��ʼ���ַ
#define GP_XPAR_BRAM0_APD_POWER_PKG_START_FLAG_ADDR         (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_START_FLAG_OFFSET)
// apd power pkg ���ݰ���Ч���ݳ��ȵ�ַ��ַ
#define GP_XPAR_BRAM0_APD_POWER_PKG_DATA_LEN_ADDR           (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_DATA_LEN_OFFSET)
// apd power pkg Դ��ַ
#define GP_XPAR_BRAM0_APD_POWER_PKG_SRC_ADDR                (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_SRC_OFFSET)
// apd power pkg Ŀ�ĵ�ַ
#define GP_XPAR_BRAM0_APD_POWER_PKG_DES_ADDR                (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_DES_OFFSET)
// apd power pkg �Ӱ���Ŀ��ַ
#define GP_XPAR_BRAM0_APD_POWER_PKG_SUB_PKG_NUM_ADDR        (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_SUB_PKG_NUM_OFFSET)
// apd power pkg �Ӱ����е�ַ
#define GP_XPAR_BRAM0_APD_POWER_PKG_SUB_PKG_SEQ_ADDR        (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_SUB_PKG_SEQ_OFFSET)
// apd power pkg ���ݵ�ַ
#define GP_XPAR_BRAM0_APD_POWER_PKG_DATA_ADDR               (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_DATA_OFFSET)
// apd power pkg У��͵�ַ
#define GP_XPAR_BRAM0_APD_POWER_PKG_SUM_ADDR                (GP_XPAR_BRAM0_APD_POWER_PKG_BASE_ADDR + GP_XPAR_BRAM0_APD_POWER_PKG_SUM_OFFSET)

#endif  // _GP_DDR_DEF_H_
