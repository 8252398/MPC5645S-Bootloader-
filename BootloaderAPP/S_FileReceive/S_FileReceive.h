//***************************************************************** 
//* FILE        : S_FileReceive.h
//* 
//* DESCRIPTION : This is the file to Initial the FlexCAN register
//*
//*               In some places this includes alternative register
//*               nomenclature. Comment out the unused version to
//*               match code requirements
//*               The default version is used by Freescale in code
//*               examples
//* 
//* COPYRIGHT   :(c) 2014-2020, by XueLiMan
//* 
//* VERSION     : 1.00
//* DATE        : 3 apir 2014
//* AUTHOR      : lilin 
//******************************************************************
#ifndef   S_FILERECIVE_H
#define	  S_FILERECIVE_H

#include "MPC5645S.h"
#include "S_File_Proc.h"
#include "FlexCANInitial.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/
#define     NOP()				 asm("nop")	



#define		TxInactiveFlag        0x08		//��ֹ���ͱ�־
#define		TxActiveFlag          0x0C		//�������ͱ�־

#define     TxSuccessful		  1			//���ͳɹ���־
#define     TxFail				  0			//����ʧ�ܱ�־

typedef struct
{
	uint32_t MsgID;
	unsigned char  datalength;
	unsigned char  *data;
}CANReceive_Type;

typedef struct
{
	unsigned char   CANGetNewFrame		:1;
	unsigned char   BootloaderStatus	:2;
	unsigned char   FlashIsBusy			:1;
	unsigned char 	EraseFlg			:1;
	unsigned char 	TxSuccessfulFlag	:1;
	unsigned char   MCUBusyFlag			:1;
	unsigned char 						:1;
}CANBootloaderStatus;

extern CANBootloaderStatus CANBootldSts;

#define CANRxNewFram		CANBootldSts.CANGetNewFrame		//MCU���յ���CAN����֡��־λ
#define MCUBootldState		CANBootldSts.BootloaderStatus	//��ǰMCU��������״̬
#define EraseFlagStatus		CANBootldSts.EraseFlg			//�����ɹ���־
#define TxSuccFlg			CANBootldSts.TxSuccessfulFlag	//MCU��Ӧ���ݷ��ͳɹ���־
#define MCUBusyFlg			CANBootldSts.MCUBusyFlag		//MCU��ǰ�Ƿ����ڴ�����һ�������־λ

extern unsigned char  EraseFlag;


//****************************************************************
//***********    ���ư�����λ��֮���ͨѶ��������  ***************
//****************************************************************
#define  	UP_FLASH_ERR 	0x01     	//���ش���
#define  	UP_INBOOT_SUC 	0xC0		//����Bootloader�ɹ�
#define  	UP_PRGEND 		0xC1		//��̽���
#define  	UP_BUSY 		0xC2		//MCU���ڴ�����������
#define  	UP_READY 		0xC3		//MCU׼���ý���������
#define  	UP_IDLE 		0xC4		//MCU���ڿ���״̬
#define  	UP_ERASEERR 	0xC5		//Flash��д���ִ���
#define  	UP_DOWN_ERR 	0xC6		//�յ��������д���
#define  	UP_PRO_NO_END_ERR 	0xC7	//��ǰ������δ��д���
#define  	UP_OUTBL_SUC 	0xC8		//�˳�Bootloader�ɹ�
#define  	UP_PROONELINE_SUC 	0xC9	//һ�����ݱ�̳ɹ�
#define  	UP_ERASE_SUC 	0xCA		//Flash�����ɹ�
#define	    UP_PROADDR_ERR  0xCB		//��д���ݵ�ַ���󣬽���BOOTLOADER����
#define    	UP_ERASEBOOTAREA_ERR  0xCC	//����Bootloader���򱨴�
#define    	UP_CANCELINBOOT  0xCE	//����Bootloader���򱨴�
#define		UP_SUREINBOOT	0xCD		//ѯ���Ƿ���Ҫ����Bootloader	
#define		UP_GETERASECMD	0xD0		//֪ͨ��λ���յ�����Flash������

//****************************************************************
//***********     ���ư��Bootloader��״̬��־     ***************
//****************************************************************
#define  	STA_NOT_LINKED 	0x00		//��λ����MCUû�н�������
#define  	STA_LINKED 		0x01		//��λ����MCU���ӳɹ�
#define  	STA_PRGEND 		0x02		//MCU��̽���


//****************************************************************
//***********     ��λ������ư�֮�����������     ***************
//****************************************************************
#define  	DOWN_LINK 			0xFF   	//��MCU����Bootloaderģʽ����
#define  	DOWN_LINE_END 		0xFE	//����һ�����ݽ�������
#define  	DOWN_FILE_END 		0xFD	//�ļ������������
#define  	DOWN_ERASE_CMD 		0xFC	//����Flash����
#define  	DOWN_OUTBOOT_CMD 	0xFB	//�˳�Bootloaderģʽ����
#define  	DOWN_INBOOTSUC_CMD 	0xFA	//�Ƿ�ɹ�����Bootloader

/*****************************************************************/ 
/****************  Funtion declaration ***************************/
/*****************************************************************/

//*****************************************************************
//������  : CAN_SRecrod_RxISR()
//��  ��  : �����жϺ���
//�������: ��
//���ز���: ��
//*****************************************************************
void CAN_SRecrod_RxISR(void);

//*****************************************************************
//������  : CAN_0_ERR_ISR()
//��  ��  : CAN�����жϱ�־
//�������: ��
//���ز���: ��
//*****************************************************************
void CAN_0_ERR_ISR(void);

//*****************************************************************
//������  : CAN_SRecrod_TxISR()
//��  ��  : CAN�����жϴ�����
//�������: ��
//���ز���: ��
//*****************************************************************
void CAN_SRecrod_TxISR(void);

//*****************************************************************
//������  : CANStatusAcknowledge()
//��  ��  : ���ư�����λ����������Ϣ���� 
//�������: status		: ��ǰ�Ĵ���״̬
//���ز���: ��
//*****************************************************************
void CANStatusAcknowledge(unsigned char status);

//*****************************************************************
//������  : CanWorkStatusGet()
//��  ��  : ȡ���ư�����λ����CAN��״̬
//�������: ��
//���ز���: ��ǰCAN�ߵ�״̬
//*****************************************************************
unsigned char CanWorkStatusGet(void);


#endif