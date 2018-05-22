//***************************************************************** 
//* FILE        : bootloader.c
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
#ifndef LEDCFG_H
#define	LEDCFG_H

#include "MPC5645S.h"
#include "MPC55xx_PCRdef.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/
#define 	LedOff		0
#define 	LedOn		1

#define LEDOUT_1      SIU.GPDO[PC3].B.PDO	
#define LEDOUT_2      SIU.GPDO[PC4].B.PDO	
#define LEDOUT_3      SIU.GPDO[PJ12].B.PDO	
#define LEDOUT_4      SIU.GPDO[PJ13].B.PDO
#define LEDOUT_5      SIU.GPDO[PJ14].B.PDO	
#define LEDOUT_6      SIU.GPDO[PK0].B.PDO
#define LEDOUT_7      SIU.GPDO[PK1].B.PDO
#define LEDOUT_8      SIU.GPDO[PF1].B.PDO		
#define LEDOUT_9      SIU.GPDO[PF0].B.PDO
#define LEDOUT_10     SIU.GPDO[PF3].B.PDO
#define LEDOUT_11     SIU.GPDO[PF6].B.PDO
#define LEDOUT_12     SIU.GPDO[PF7].B.PDO
#define LEDOUT_13     SIU.GPDO[PF11].B.PDO
#define LEDOUT_14     SIU.GPDO[PF14].B.PDO
#define LEDOUT_15     SIU.GPDO[PF4].B.PDO
#define LEDOUT_16     SIU.GPDO[PJ5].B.PDO
#define LEDOUT_17     SIU.GPDO[PJ4].B.PDO

/*****************************************************************/ 
/*****************  Funtion definition ***************************/
/*****************************************************************/

//*****************************************************************
//������  : LED_IOCfgInitial()
//��  ��  : ��LED��IO�˿ڽ��г�ʼ��
//�������: ��
//���ز���: ��
//*****************************************************************
void LED_IOCfgInitial(void);

//*****************************************************************
//������  : LED_Init()
//��  ��  : ��LED��״̬���г�ʼ��
//�������: ��
//���ز���: ��
//*****************************************************************
void LED_Init(void);

//*****************************************************************
//������  : LED_Light()
//��  ��  : ����LED��
//�������: ��
//���ز���: ��
//*****************************************************************
void LED_Light(uint32_t LightFlag);

//*****************************************************************
//������  : DlayFunction()
//��  ��  : ��ʱ����
//�������: Seconds		: ��ʱ��
//	      : Ms			: ��ʱ����
//���ز���: ��
//*****************************************************************
void DlayFunction(uint32_t Seconds,uint32_t Ms); 

//*****************************************************************
//������  : Erase_Flash_Except_Bootloader()
//��  ��  : ������bootloader���������Flash���� 
//�������: ��
//���ز���: ��
//*****************************************************************
unsigned char EraseEnterBootFlagFlash(void);

//*****************************************************************
//������  : FlashProgramer()
//��  ��  : ��Flash���б��
//�������: *pDataPtr		: ��̵���������
//���ز���: ProFail			: д��Flashʧ��
//*****************************************************************
unsigned char EnterBootFlagFlashProgramer(void);

#endif