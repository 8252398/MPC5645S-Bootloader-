//***************************************************************** 
//* FILE        : bootloader.h
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
#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_



#include "FlashOperation.h"
#include "S_FileReceive.h"
#include "ModeCtrDefine.h"
#include "FlexCAN_RxTx.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/


/*****************************************************************/ 
/****************  Funtion declaration ***************************/
/*****************************************************************/

//*****************************************************************
//������  : DlyS_Ms()
//��  ��  : ��ʱ����
//�������: Mills		: ��ʱʱ��
//���ز���: ��
//*****************************************************************
void DlyS_Ms(uint32_t Mills);

 
#endif