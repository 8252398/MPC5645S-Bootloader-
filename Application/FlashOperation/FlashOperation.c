//***************************************************************** 
//* FILE        : FlashOperation.c
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
#include "MPC5645S.h"
#include "S_File_Proc.h"
#include "MPC55xx_PCRdef.h"


#pragma push
#pragma section sconst_type ".const_data" 
__declspec(section ".const_data")
//*******************************************
static const unsigned char NeedUpdated[12]={0x4E,0x65,0x65,0x64,0x20,0x75,0x70,0x64,0x61,0x74,0x65,0x64};
#pragma pop


#pragma push
#pragma section code_type ".userApplication_flash" 
__declspec(section ".userApplication_flash")
//*****************************************************************
//������  : Erase_Flash_Except_Bootloader()
//��  ��  : ������bootloader���������Flash���� 
//�������: ��
//���ز���: ��
//*****************************************************************
unsigned char EraseEnterBootFlagFlash(void)
{
	static uint32_t *address=(uint32_t*)0x00000000;
	
	if(CFLASH0.MCR.B.EER)	CFLASH0.MCR.B.EER=1;//��������־λ
	if(CFLASH0.MCR.B.RWE)	CFLASH0.MCR.B.RWE=1;
	while(CFLASH0.MCR.B.PGM){}					//������ڽ��б�̲�������ȴ�������
	while(CFLASH0.MCR.B.ERS){}					//������ڽ��в�д��������ȴ���д���
	
	CFLASH0.LML.R=0xA1A11111;				  	//����bootloader flash�����ⱻ�����
	CFLASH0.LML.R=0x801303FB;
	
	CFLASH0.HBL.R=0xB2B22222;				  	//����bootloader flash�����ⱻ�����
	CFLASH0.HBL.R=0x8000003F;
	
	CFLASH0.SLL.R=0xC3C33333;
	CFLASH0.SLL.R=0x801303FB;

	CFLASH0.LMS.B.LSEL=4; //ѡ����Ҫ����������

    CFLASH0.LMS.B.MSEL=0;
    CFLASH0.HBS.B.HSEL=0;
    
	CFLASH0.MCR.B.ERS=1;				//��λ��дFlash�ı�־λ		
	*address=0xFFFFFFFF;				//д������ֵ����Ҫ������Flash�����е���һ��ַ
	CFLASH0.MCR.B.EHV=1;				//��λ��ʼ������־λ��Flash��ʼ��������
	while(!CFLASH0.MCR.B.DONE){}		//�ȴ�Flash�������������	
	if(CFLASH0.MCR.B.PEG)				//����Ƿ�����ɹ�
	CFLASH0.MCR.B.EHV=0;				//����ɹ��������־λ	
	else							
	{
		CFLASH0.MCR.B.EHV=0;			//��ʼ��һ�β���ǰ���������־λ
		CFLASH0.MCR.B.ERS=0;
		return eraseEnterBootFlashFlag=0;		
	}
		
	CFLASH0.MCR.B.ERS=0;						//�˳���дFlash
	return eraseEnterBootFlashFlag=1;
}
__declspec(section ".userApplication_flash")
//*****************************************************************
//������  : FlashProgramer()
//��  ��  : ��Flash���б��
//�������: *pDataPtr		: ��̵���������
//���ز���: ProFail			: д��Flashʧ��
//*****************************************************************
unsigned char EnterBootFlagFlashProgramer(void)
{
	static uint32_t *address;
	unsigned char cnts=0;
	uint32_t *DataDetail;
	
	address=(uint32_t*)0x8000;		//ȡд�����ݵ��׵�ַ
	
	DataDetail=(uint32_t*)&NeedUpdated[0];	

	CFLASH0.MCR.B.PGM=1;				//��λ���Flash�ı�־λ
	
	for(cnts=0;cnts<3;cnts++)
	{
		*address++=*DataDetail++;
	}

	CFLASH0.MCR.B.EHV=1;			//��Flash Module ��ʼд������ָ��
	while(!CFLASH0.MCR.B.DONE){}	//�ȴ�����д�����
	if(CFLASH0.MCR.B.PEG)			//����Ƿ�ɹ�д������
		CFLASH0.MCR.B.EHV=0;		//д��ɹ����������־λ
	else
	{
		CFLASH0.MCR.B.EHV=0;		//�����־λ
		CFLASH0.MCR.B.PGM=0;		//�˳�Flash ���
		return progEnterBootFlashFlag=0;				//д�벻�ɹ����򷵻ش�����	
	}
	CFLASH0.MCR.B.PGM=0;				//�˳�Flash ���
	return progEnterBootFlashFlag=1;

}
#pragma pop