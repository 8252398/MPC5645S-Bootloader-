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
#define		ProFail					0	//编程失败
#define		ProSuc					1	//编程成功
#define     ProIntoBootAddress   	2	//数据写入Bootloader区域

#define     EraseFail		0		//擦除Flash失败
#define     EraseSuce		1		//擦除Flash成功


#define		INTO_BOOT_ADDR			0x00006000		//Bootloader入口地址

#define		BOOT_ENTER_ADDR			0x00004000		//Bootloader入口地址
#define		RAM_BOOT_ENTER_ADDR		0x4000D000		//Bootloader在RAM中的入口地址
#define		BOOT_LENGTH				(0x00001A00>>2)	//Bootloader区域的长度
#define 	APP_ENRTY_ADDR 			0x0000C000		//应用程序的入口地址

typedef void(*APPFunction)(void);

/*****************************************************************/ 
/****************  Funtion declaration ***************************/
/*****************************************************************/
//返回参数: 无
//*****************************************************************
void Erase_Flash(void);

//*****************************************************************
//函数名  : CopyFlashDataToRAM()
//描  述  : 将Flash中的代码拷贝到RAM
//输入参数: *Source_addr	: 源地址(开始地址)
//        : *Target_addr    : 目标地址
//		  : length			: 数据长度
//返回参数: 无
//*****************************************************************
void CopyFlashDataToRAM(uint32_t *Source_addr, uint32_t *Target_addr, uint32_t length);

//*****************************************************************
//函数名  : CopyFlashDataToRAM()
//描  述  : 读取Flash中的数据
//输入参数: *pDataBuf	: 数据BUFFER
//        : *StartAddr  : 起始地址
//		  : Len			: 数据长度
//返回参数: 无
//*****************************************************************
void FlashBlkRead( uint32_t *pDataBuf, uint32_t StartAddr, unsigned short Lenght );

//*****************************************************************
//函数名  : Erase_Flash_Except_Bootloader()
//描  述  : 擦除除bootloader以外的所有Flash区域 
//输入参数: 无
//返回参数: 无
//*****************************************************************
unsigned char EraseSetEnterBootFlagFlash(void);

#endif