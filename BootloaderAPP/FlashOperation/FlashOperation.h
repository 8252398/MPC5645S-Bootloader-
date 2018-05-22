//***************************************************************** 
//* FILE        : FlashOperation.h
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
//* DATE        : 2 apir 2014
//* AUTHOR      : lilin 
//******************************************************************
#ifndef FLASHOPERRTION_H
#define FLASHOPERRTION_H

#include "MPC5645S.h"
#include "S_File_Proc.h"
#include "MPC55xx_PCRdef.h"
#include "S_FileReceive.h"


/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/
#define		ProFail					0	//���ʧ��
#define		ProSuc					1	//��̳ɹ�
#define     ProIntoBootAddress   	2	//����д��Bootloader����

#define     EraseFail		0		//����Flashʧ��
#define     EraseSuce		1		//����Flash�ɹ�


#define		INTO_BOOT_ADDR			0x00006000		//Bootloader��ڵ�ַ

#define		BOOT_ENTER_ADDR			0x00004000		//Bootloader��ڵ�ַ
#define		RAM_BOOT_ENTER_ADDR		0x4000D000		//Bootloader��RAM�е���ڵ�ַ
#define		BOOT_LENGTH				(0x00001A00>>2)	//Bootloader����ĳ���
#define 	APP_ENRTY_ADDR 			0x0000C000		//Ӧ�ó������ڵ�ַ

typedef void(*APPFunction)(void);

/*****************************************************************/ 
/****************  Funtion declaration ***************************/
/*****************************************************************/
//���ز���: ��
//*****************************************************************
void Erase_Flash(void);

//*****************************************************************
//������  : CopyFlashDataToRAM()
//��  ��  : ��Flash�еĴ��뿽����RAM
//�������: *Source_addr	: Դ��ַ(��ʼ��ַ)
//        : *Target_addr    : Ŀ���ַ
//		  : length			: ���ݳ���
//���ز���: ��
//*****************************************************************
void CopyFlashDataToRAM(uint32_t *Source_addr, uint32_t *Target_addr, uint32_t length);

//*****************************************************************
//������  : CopyFlashDataToRAM()
//��  ��  : ��ȡFlash�е�����
//�������: *pDataBuf	: ����BUFFER
//        : *StartAddr  : ��ʼ��ַ
//		  : Len			: ���ݳ���
//���ز���: ��
//*****************************************************************
void FlashBlkRead( uint32_t *pDataBuf, uint32_t StartAddr, unsigned short Lenght );

//*****************************************************************
//������  : Erase_Flash_Except_Bootloader()
//��  ��  : ������bootloader���������Flash���� 
//�������: ��
//���ز���: ��
//*****************************************************************
unsigned char EraseSetEnterBootFlagFlash(void);

#endif