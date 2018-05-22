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
#include "FlashOperation.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/
extern unsigned short LowFlashSelectFlag;
extern unsigned char  MidFlashSelectFlag;
extern unsigned char  HigFlashSelectFlag;

extern 	SFileRecord SInfroRecord;
extern  ProgramDataBuffer ProgramDataBuf;
extern	DataBuffer	ExchangeDataBuffer;
/*****************************************************************/ 
/*****************  Funtion definition ***************************/
/*****************************************************************/
#pragma push
#pragma section code_type ".boot_flash" 
__declspec(section ".boot_flash")
//*****************************************************************
//������  : Erase_Flash_Except_Bootloader()
//��  ��  : ������bootloader���������Flash���� 
//�������: ��
//���ز���: ��
//*****************************************************************
void Erase_Flash(void)
{
	static uint32_t *address=(uint32_t*)0x00000000;
	
	if(CFLASH0.MCR.B.EER)	CFLASH0.MCR.B.EER=1;//��������־λ
	if(CFLASH0.MCR.B.RWE)	CFLASH0.MCR.B.RWE=1;
	while(CFLASH0.MCR.B.PGM){}					//������ڽ��б�̲�������ȴ�������
	while(CFLASH0.MCR.B.ERS){}					//������ڽ��в�д��������ȴ���д���
	
	CFLASH0.LML.R=0xA1A11111;				  	//����bootloader flash�����ⱻ�����
	CFLASH0.LML.R=0x80000003;
	
	CFLASH0.HBL.R=0xB2B22222;				  	//����bootloader flash�����ⱻ�����
	CFLASH0.HBL.R=0x80000000;
	
	CFLASH0.SLL.R=0xC3C33333;
	CFLASH0.SLL.R=0x80000003;

	CFLASH0.LMS.B.LSEL=LowFlashSelectFlag; //ѡ����Ҫ����������

    CFLASH0.LMS.B.MSEL=MidFlashSelectFlag;
    CFLASH0.HBS.B.HSEL=HigFlashSelectFlag;
    
	CFLASH0.MCR.B.ERS=1;				//��λ��дFlash�ı�־λ		
	*address=0xFFFFFFFF;				//д������ֵ����Ҫ������Flash�����е���һ��ַ
	CFLASH0.MCR.B.EHV=1;				//��λ��ʼ������־λ��Flash��ʼ��������
	while(!CFLASH0.MCR.B.DONE){}		//�ȴ�Flash�������������	
	if(CFLASH0.MCR.B.PEG)				//����Ƿ�����ɹ�
	CFLASH0.MCR.B.EHV=0;				//����ɹ��������־λ	
	else							
	{
		CFLASH0.MCR.B.EHV=0;			//��ʼ��һ�β���ǰ���������־λ
		EraseFlagStatus = EraseFail;		
	}
		
	CFLASH0.MCR.B.ERS=0;						//�˳���дFlash
	EraseFlagStatus = EraseSuce;
}
__declspec(section ".boot_flash")	
//*****************************************************************
//������  : FlashProgramer()
//��  ��  : ��Flash���б��
//�������: *pDataPtr		: ��̵���������
//���ز���: ProFail			: д��Flashʧ��
//*****************************************************************
unsigned char FlashProgramer(ProgramDataBuffer *pDataPtr)
{
	uint32_t *address;
	uint32_t *DataDetail;
	unsigned char cnts=0;
	
	address	=  (uint32_t *)pDataPtr->addr;
	DataDetail=(uint32_t *)&pDataPtr->data[0];
	
	CFLASH0.MCR.B.PGM=1;				//��λ���Flash�ı�־λ
	if(pDataPtr->addr<APP_ENRTY_ADDR) return ProIntoBootAddress;
	else
	{
		for(cnts=0;cnts<2;cnts++)
		{
			*address++=*DataDetail++;
		}
		CFLASH0.MCR.B.EHV=1;			//��Flash Module ��ʼд������ָ��
		while(!CFLASH0.MCR.B.DONE){}	//�ȴ�����д�����
	//	for(cnts=100;cnts>0;cnts--) NOP();
		if(CFLASH0.MCR.B.PEG)			//����Ƿ�ɹ�д������
			CFLASH0.MCR.B.EHV=0;
//	while(!CFLASH0.MCR.B.PEG){}
		//	CFLASH0.MCR.B.EHV=0;		//д��ɹ����������־λ
		else
		{
			CFLASH0.MCR.B.EHV=0;		//�����־λ
			CFLASH0.MCR.B.PGM=0;		//�˳�Flash ���
			return ProFail;				//д�벻�ɹ����򷵻ش�����	
		}
		CFLASH0.MCR.B.PGM=0;				//�˳�Flash ���
		return ProSuc;
	}
}
__declspec(section ".boot_flash")
//*****************************************************************
//������  : CopyFlashDataToRAM()
//��  ��  : ��ȡFlash�е�����
//�������: *pDataBuf	: ����BUFFER
//        : *StartAddr  : ��ʼ��ַ
//		  : Len			: ���ݳ���
//���ز���: ��
//*****************************************************************
void FlashBlkRead( uint32_t *pDataBuf, uint32_t StartAddr, unsigned short Lenght )
{
    unsigned short i = 0;
    uint32_t *pStartAddr;
    
    pStartAddr=(uint32_t*)StartAddr;

    for (i = 0; i < Lenght; i++)
    {
        *pDataBuf++ = *pStartAddr++;
    }
}
__declspec(section ".boot_flash")
//*****************************************************************
//������  : CopyFlashDataToRAM()
//��  ��  : ��Flash�еĴ��뿽����RAM
//�������: *Source_addr	: Դ��ַ(��ʼ��ַ)
//        : *Target_addr    : Ŀ���ַ
//		  : length			: ���ݳ���
//���ز���: ��
//*****************************************************************
void CopyFlashDataToRAM(uint32_t *Source_addr, uint32_t *Target_addr, uint32_t length)
{
	uint32_t cnts;
	for(cnts=0; cnts<length; cnts++)
		*Target_addr++ = *Source_addr++;
}
__declspec(section ".boot_flash")
//*****************************************************************
//������  : Erase_Flash_Except_Bootloader()
//��  ��  : ������bootloader���������Flash���� 
//�������: ��
//���ز���: ��
//*****************************************************************
unsigned char EraseSetEnterBootFlagFlash(void)
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
		return 0;		
	}
		
	CFLASH0.MCR.B.ERS=0;						//�˳���дFlash
	return 1;
}

#pragma pop