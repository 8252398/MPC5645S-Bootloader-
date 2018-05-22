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
//函数名  : Erase_Flash_Except_Bootloader()
//描  述  : 擦除除bootloader以外的所有Flash区域 
//输入参数: 无
//返回参数: 无
//*****************************************************************
unsigned char EraseEnterBootFlagFlash(void)
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
		return eraseEnterBootFlashFlag=0;		
	}
		
	CFLASH0.MCR.B.ERS=0;						//退出擦写Flash
	return eraseEnterBootFlashFlag=1;
}
__declspec(section ".userApplication_flash")
//*****************************************************************
//函数名  : FlashProgramer()
//描  述  : 对Flash进行编程
//输入参数: *pDataPtr		: 编程的数据内容
//返回参数: ProFail			: 写入Flash失败
//*****************************************************************
unsigned char EnterBootFlagFlashProgramer(void)
{
	static uint32_t *address;
	unsigned char cnts=0;
	uint32_t *DataDetail;
	
	address=(uint32_t*)0x8000;		//取写入数据的首地址
	
	DataDetail=(uint32_t*)&NeedUpdated[0];	

	CFLASH0.MCR.B.PGM=1;				//置位编程Flash的标志位
	
	for(cnts=0;cnts<3;cnts++)
	{
		*address++=*DataDetail++;
	}

	CFLASH0.MCR.B.EHV=1;			//给Flash Module 开始写入数据指令
	while(!CFLASH0.MCR.B.DONE){}	//等待数据写入完成
	if(CFLASH0.MCR.B.PEG)			//检查是否成功写入数据
		CFLASH0.MCR.B.EHV=0;		//写入成功，则清除标志位
	else
	{
		CFLASH0.MCR.B.EHV=0;		//清除标志位
		CFLASH0.MCR.B.PGM=0;		//退出Flash 编程
		return progEnterBootFlashFlag=0;				//写入不成功，则返回错误码	
	}
	CFLASH0.MCR.B.PGM=0;				//退出Flash 编程
	return progEnterBootFlashFlag=1;

}
#pragma pop