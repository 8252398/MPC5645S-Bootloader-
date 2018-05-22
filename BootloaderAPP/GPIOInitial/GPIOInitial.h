//***************************************************************** 
//* FILE        : GPIOInitial.h
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
#ifndef _GPIOINITIAL_H_
#define _GPIOINITIAL_H_

#include "MPC5645S.h"
#include "MPC55xx_PCRdef.h"
#include "FlashOperation.h"
#include "S_FileReceive.h"
#include "ModeCtrDefine.h"
#include "FlexCAN_RxTx.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/

#define		BootSelect_One			SIU.GPDI[PC7].B.PDI 
#define		BootSelect_Two			SIU.GPDI[PC8].B.PDI 

/*****************************************************************/ 
/****************  Funtion declaration ***************************/
/*****************************************************************/

//*****************************************************************
//������  : DlyS_Ms()
//��  ��  : ��ʱ����
//�������: Mills		: ��ʱʱ��
//���ز���: ��
//*****************************************************************

//*****************************************************************
//������  : ForceIntoBootKEYInitial()
//��  ��  : ��ǿ�ƽ���Bootloader�İ������г�ʼ��
//�������: ��
//���ز���: ��
//*****************************************************************
void ForceIntoBootKEYInitial(void);

//*****************************************************************
//������  : ScanKEYStatus()
//��  ��  : ��ⰴ���Ƿ���
//�������: ��
//���ز���: ��
//*****************************************************************
unsigned char ScanKEYStatus(void);

//*****************************************************************
//������  : PITInitial()
//��  ��  : ��ֹ��ʱ��
//�������: ��
//���ز���: ��
//*****************************************************************
void PITInitial(void);

//*****************************************************************
//������  : MCUConfiguration()
//��  ��  : MCUʱ�ӵ�����
//�������: ��
//���ز���: ��
//*****************************************************************
void MCUConfiguration(void);

#endif