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
//函数名  : Erase_Flash_Except_Bootloader()
//描  述  : 擦除除bootloader以外的所有Flash区域 
//输入参数: 无
//返回参数: 无
//*****************************************************************
void Erase_Flash(void)
{
	static uint32_t *address=(uint32_t*)0x00000000;
	
	if(CFLASH0.MCR.B.EER)	CFLASH0.MCR.B.EER=1;//清除错误标志位
	if(CFLASH0.MCR.B.RWE)	CFLASH0.MCR.B.RWE=1;
	while(CFLASH0.MCR.B.PGM){}					//如果正在进行编程操作，则等待编程完成
	while(CFLASH0.MCR.B.ERS){}					//如果正在进行擦写操作，则等待擦写完成
	
	CFLASH0.LML.R=0xA1A11111;				  	//保护bootloader flash，以免被误擦除
	CFLASH0.LML.R=0x80000003;
	
	CFLASH0.HBL.R=0xB2B22222;				  	//保护bootloader flash，以免被误擦除
	CFLASH0.HBL.R=0x80000000;
	
	CFLASH0.SLL.R=0xC3C33333;
	CFLASH0.SLL.R=0x80000003;

	CFLASH0.LMS.B.LSEL=LowFlashSelectFlag; //选择需要擦除的区域

    CFLASH0.LMS.B.MSEL=MidFlashSelectFlag;
    CFLASH0.HBS.B.HSEL=HigFlashSelectFlag;
    
	CFLASH0.MCR.B.ERS=1;				//置位擦写Flash的标志位		
	*address=0xFFFFFFFF;				//写入任意值到需要擦除的Flash区块中的任一地址
	CFLASH0.MCR.B.EHV=1;				//置位开始擦除标志位，Flash开始擦除操作
	while(!CFLASH0.MCR.B.DONE){}		//等待Flash擦除操作的完成	
	if(CFLASH0.MCR.B.PEG)				//检查是否擦除成功
	CFLASH0.MCR.B.EHV=0;				//如果成功则清除标志位	
	else							
	{
		CFLASH0.MCR.B.EHV=0;			//开始下一次擦除前，先清除标志位
		EraseFlagStatus = EraseFail;		
	}
		
	CFLASH0.MCR.B.ERS=0;						//退出擦写Flash
	EraseFlagStatus = EraseSuce;
}
__declspec(section ".boot_flash")	
//*****************************************************************
//函数名  : FlashProgramer()
//描  述  : 对Flash进行编程
//输入参数: *pDataPtr		: 编程的数据内容
//返回参数: ProFail			: 写入Flash失败
//*****************************************************************
unsigned char FlashProgramer(ProgramDataBuffer *pDataPtr)
{
	uint32_t *address;
	uint32_t *DataDetail;
	unsigned char cnts=0;
	
	address	=  (uint32_t *)pDataPtr->addr;
	DataDetail=(uint32_t *)&pDataPtr->data[0];
	
	CFLASH0.MCR.B.PGM=1;				//置位编程Flash的标志位
	if(pDataPtr->addr<APP_ENRTY_ADDR) return ProIntoBootAddress;
	else
	{
		for(cnts=0;cnts<2;cnts++)
		{
			*address++=*DataDetail++;
		}
		CFLASH0.MCR.B.EHV=1;			//给Flash Module 开始写入数据指令
		while(!CFLASH0.MCR.B.DONE){}	//等待数据写入完成
	//	for(cnts=100;cnts>0;cnts--) NOP();
		if(CFLASH0.MCR.B.PEG)			//检查是否成功写入数据
			CFLASH0.MCR.B.EHV=0;
//	while(!CFLASH0.MCR.B.PEG){}
		//	CFLASH0.MCR.B.EHV=0;		//写入成功，则清除标志位
		else
		{
			CFLASH0.MCR.B.EHV=0;		//清除标志位
			CFLASH0.MCR.B.PGM=0;		//退出Flash 编程
			return ProFail;				//写入不成功，则返回错误码	
		}
		CFLASH0.MCR.B.PGM=0;				//退出Flash 编程
		return ProSuc;
	}
}
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : CopyFlashDataToRAM()
//描  述  : 读取Flash中的数据
//输入参数: *pDataBuf	: 数据BUFFER
//        : *StartAddr  : 起始地址
//		  : Len			: 数据长度
//返回参数: 无
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
//函数名  : CopyFlashDataToRAM()
//描  述  : 将Flash中的代码拷贝到RAM
//输入参数: *Source_addr	: 源地址(开始地址)
//        : *Target_addr    : 目标地址
//		  : length			: 数据长度
//返回参数: 无
//*****************************************************************
void CopyFlashDataToRAM(uint32_t *Source_addr, uint32_t *Target_addr, uint32_t length)
{
	uint32_t cnts;
	for(cnts=0; cnts<length; cnts++)
		*Target_addr++ = *Source_addr++;
}
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : Erase_Flash_Except_Bootloader()
//描  述  : 擦除除bootloader以外的所有Flash区域 
//输入参数: 无
//返回参数: 无
//*****************************************************************
unsigned char EraseSetEnterBootFlagFlash(void)
{
	static uint32_t *address=(uint32_t*)0x00000000;
	
	if(CFLASH0.MCR.B.EER)	CFLASH0.MCR.B.EER=1;//清除错误标志位
	if(CFLASH0.MCR.B.RWE)	CFLASH0.MCR.B.RWE=1;
	while(CFLASH0.MCR.B.PGM){}					//如果正在进行编程操作，则等待编程完成
	while(CFLASH0.MCR.B.ERS){}					//如果正在进行擦写操作，则等待擦写完成
	
	CFLASH0.LML.R=0xA1A11111;				  	//保护bootloader flash，以免被误擦除
	CFLASH0.LML.R=0x801303FB;
	
	CFLASH0.HBL.R=0xB2B22222;				  	//保护bootloader flash，以免被误擦除
	CFLASH0.HBL.R=0x8000003F;
	
	CFLASH0.SLL.R=0xC3C33333;
	CFLASH0.SLL.R=0x801303FB;

	CFLASH0.LMS.B.LSEL=4; //选择需要擦除的区域

    CFLASH0.LMS.B.MSEL=0;
    CFLASH0.HBS.B.HSEL=0;
    
	CFLASH0.MCR.B.ERS=1;				//置位擦写Flash的标志位		
	*address=0xFFFFFFFF;				//写入任意值到需要擦除的Flash区块中的任一地址
	CFLASH0.MCR.B.EHV=1;				//置位开始擦除标志位，Flash开始擦除操作
	while(!CFLASH0.MCR.B.DONE){}		//等待Flash擦除操作的完成	
	if(CFLASH0.MCR.B.PEG)				//检查是否擦除成功
	CFLASH0.MCR.B.EHV=0;				//如果成功则清除标志位	
	else							
	{
		CFLASH0.MCR.B.EHV=0;			//开始下一次擦除前，先清除标志位
		CFLASH0.MCR.B.ERS=0;
		return 0;		
	}
		
	CFLASH0.MCR.B.ERS=0;						//退出擦写Flash
	return 1;
}

#pragma pop