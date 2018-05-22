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
//函数名  : DlyS_Ms()
//描  述  : 延时函数
//输入参数: Mills		: 延时时间
//返回参数: 无
//*****************************************************************
void DlyS_Ms(uint32_t Mills);

 
#endif