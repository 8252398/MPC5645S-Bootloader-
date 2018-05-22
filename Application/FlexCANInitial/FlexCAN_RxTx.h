//***************************************************************** 
//* FILE        : FlexCANRxTx.c
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
//* DATE        : 31 march 2014
//* AUTHOR      : lilin 
//******************************************************************
#ifndef FLEXCAN_RXTX_H
#define FLEXCAN_RXTX_H

#include "MPC5645S.h"
#include "S_FileReceive.h"
#include "FlexCANInitial.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/

typedef struct
{
	unsigned char  BOOTLOADERCANTXSUCFLAG		:1;		//BootloaderCAN����״̬��־λ
	unsigned char  CANRXNEWDATUM				:1;
	unsigned char  GOTOBOOTSUC					:1;
	unsigned char  ERASE_ENTERBOOT				:1;
	unsigned char  PROG_ENTERBOOT				:1;
}BOOTLOADERCANFLAG;

extern BOOTLOADERCANFLAG	BootloaderCANStatus;
#define		BootloaderCANTransSucFlag		BootloaderCANStatus.BOOTLOADERCANTXSUCFLAG	
#define		CANRxNewDatum					BootloaderCANStatus.CANRXNEWDATUM
#define		GotBootloaderSuc				BootloaderCANStatus.GOTOBOOTSUC
#define		eraseEnterBootFlashFlag			BootloaderCANStatus.ERASE_ENTERBOOT
#define		progEnterBootFlashFlag			BootloaderCANStatus.PROG_ENTERBOOT


#define 	RAM_ADDR_ERASE_FLASH	    0x4000CA00  	//����д�������Ƶ�RAM�е���ڵ�ַ
#define 	RAM_ADDR_PROG_FLASH	        0x4000CD00		//����̺������Ƶ�RAM�е���ڵ�ַ

#define	    Length_copy_ram				(0x00000300>>2)	//���Ƶ�RAM�еĳ���

/*****************************************************************/ 
/*****************  Funtion definition ***************************/
/*****************************************************************/

//*****************************************************************
//������  : TransAcknowledge()
//��  ��  : ���ư�����λ����������Ϣ���� 
//�������: status		: ��ǰ�Ĵ���״̬
//���ز���: ��
//*****************************************************************
void TransAcknowledge(unsigned char status);

#endif