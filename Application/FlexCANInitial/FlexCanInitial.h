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
#include "FlexCAN_RxTx.h"
#include "MPC55xx_PCRdef.h"
#include "InterruptsIRQDef.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/


/*****************************************************************/ 
/****************  Funtion declaration ***************************/
/*****************************************************************/


//*****************************************************************
//������  : InitialFlexCANAPP()
//��  ��  : ��CANͨ�����г�ʼ�� 
//�������: ��
//���ز���: ��
//*****************************************************************
void InitialFlexCANAPP(void);


#endif