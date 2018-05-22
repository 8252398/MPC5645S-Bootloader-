//***************************************************************** 
//* FILE        : FlexCANInitial.h
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
#ifndef FLEXCANINITIAL_H
#define FLEXCANINITIAL_H

#include "MPC5645S.h"
#include "MPC55xx_PCRdef.h"
#include "S_FileReceive.h"
#include "InterruptsIRQDef.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/

#define   	CAN_Tx    	0		//����֡
#define 	CAN_Rx		1		//����֡
#define		Std_Frame	0		//��׼֡
#define		Ext_Frame	1		//��չ֡

#define		CAN0		0		//��0ͨ��
#define		CAN1		1		//��1ͨ��
#define		CAN2		2		//��2ͨ��


typedef struct 
{
	unsigned char 	CAN_Num		  ;   //ѡ��CANͨ��
	unsigned short	BaudRate	  ;	  //���ò�����
	uint32_t	IntMaskL	  ;	  //��32λ�ж�����λ 
	uint32_t	IntMaskH	  ;	  //��32λ�ж�����λ 
}FlexCANCfg;

#define 	RAM_ADDR_ISR_FLEXCAN0003	0x4000EA00  	//�����жϺ�����RAMZ�е���ڵ�ַ
#define 	RAM_ADDR_ISR_FLEXCAN0407	0x4000F000		//�����жϺ�����RAMZ�е���ڵ�ַ

#define 	Length_ISR_FLEXCAN0003		(0x00000600>>2)	//�����жϺ����ĵ�ַ����
#define 	Length_ISR_FLEXCAN0407		(0x00000600>>2) //�����жϺ����ĵ�ַ����

#define		CANRxFilterID				0x00000200		//����CAN��Ϣ�ı���ID
#define		CANTxFilterID				0x00000100		//����CAN��Ϣ�ı���ID

/*****************************************************************/ 
/****************  Funtion declaration ***************************/
/*****************************************************************/


//*****************************************************************
//������  : InitialFlexCAN()
//��  ��  : ��CANͨ�����г�ʼ�� 
//�������: ��
//���ز���: ��
//*****************************************************************
void InitialFlexCAN(void);


#endif