//***************************************************************** 
//* FILE        : bootloader.c
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
#include "LEDCfg.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/

/*****************************************************************/ 
/*****************  Funtion definition ***************************/
/*****************************************************************/
#pragma push
#pragma section code_type ".userApplication_flash" 
__declspec(section ".userApplication_flash")
//*****************************************************************
//函数名  : LED_IOCfgInitial()
//描  述  : 对LED的IO端口进行初始化
//输入参数: 无
//返回参数: 无
//*****************************************************************
void LED_IOCfgInitial(void)
{
	
	SIU.PCR[PC3].B.PA = 0;//LEDOUT_1
	SIU.PCR[PC3].B.OBE = 1;
	SIU.PCR[PC3].B.IBE = 0;

	SIU.PCR[PC4].B.PA = 0;//LEDOUT_2
	SIU.PCR[PC4].B.OBE = 1;
	SIU.PCR[PC4].B.IBE = 0;
	
	SIU.PCR[PJ12].B.PA = 0;//LEDOUT_3
	SIU.PCR[PJ12].B.OBE = 1;
	SIU.PCR[PJ12].B.IBE = 0;
	
	SIU.PCR[PJ13].B.PA = 0;//LEDOUT_4
	SIU.PCR[PJ13].B.OBE = 1;
	SIU.PCR[PJ13].B.IBE = 0;
	
	SIU.PCR[PJ14].B.PA = 0;//LEDOUT_5
	SIU.PCR[PJ14].B.OBE = 1;
	SIU.PCR[PJ14].B.IBE = 0;
	
	SIU.PCR[PK1].B.PA = 0;//LEDOUT_6
	SIU.PCR[PK1].B.OBE = 1;
	SIU.PCR[PK1].B.IBE = 0;

	SIU.PCR[PK0].B.PA = 0;//LEDOUT_7
	SIU.PCR[PK0].B.OBE = 1;
	SIU.PCR[PK0].B.IBE = 0;
	
	SIU.PCR[PF1].B.PA = 0;//LEDOUT_8
	SIU.PCR[PF1].B.OBE = 1;
	SIU.PCR[PF1].B.IBE = 0;
	
	SIU.PCR[PF0].B.PA = 0;//LEDOUT_9
	SIU.PCR[PF0].B.OBE = 1;
	SIU.PCR[PF0].B.IBE = 0;
	
	SIU.PCR[PF3].B.PA = 0;//LEDOUT_10
	SIU.PCR[PF3].B.OBE = 1;
	SIU.PCR[PF3].B.IBE = 0;
	
	SIU.PCR[PF6].B.PA = 0;//LEDOUT_11
	SIU.PCR[PF6].B.OBE = 1;
	SIU.PCR[PF6].B.IBE = 0;
	
	SIU.PCR[PF7].B.PA = 0;//LEDOUT_12
	SIU.PCR[PF7].B.OBE = 1;
	SIU.PCR[PF7].B.IBE = 0;
	
	SIU.PCR[PF11].B.PA = 0;//LEDOUT_13
	SIU.PCR[PF11].B.OBE = 1;
	SIU.PCR[PF11].B.IBE = 0;

	SIU.PCR[PF14].B.PA = 0;//LEDOUT_14
	SIU.PCR[PF14].B.OBE = 1;
	SIU.PCR[PF14].B.IBE = 0;
	
	SIU.PCR[PF4].B.PA = 0;//LEDOUT_15
	SIU.PCR[PF4].B.OBE = 1;
	SIU.PCR[PF4].B.IBE = 0;

	SIU.PCR[PJ5].B.PA = 0;//LEDOUT_16
	SIU.PCR[PJ5].B.OBE = 1;
	SIU.PCR[PJ5].B.IBE = 0;
	
	SIU.PCR[PJ4].B.PA = 0;//LEDOUT_17
	SIU.PCR[PJ4].B.OBE = 1;
	SIU.PCR[PJ4].B.IBE = 0;
}
__declspec(section ".userApplication_flash")
//*****************************************************************
//函数名  : LED_Init()
//描  述  : 对LED的状态进行初始化
//输入参数: 无
//返回参数: 无
//*****************************************************************
void LED_Init(void)
	{	
		LEDOUT_1=LedOff;
		LEDOUT_2=LedOff;
		LEDOUT_3=LedOff;
		LEDOUT_4=LedOff;
		LEDOUT_5=LedOff;
		LEDOUT_6=LedOff;
		LEDOUT_7=LedOff;
		LEDOUT_8=LedOff;
		LEDOUT_9=LedOff;
		LEDOUT_10=LedOff;
		LEDOUT_11=LedOff;
		LEDOUT_12=LedOff;
		LEDOUT_13=LedOff;
		LEDOUT_14=LedOff;
		LEDOUT_15=LedOff;
		LEDOUT_16=LedOff;
		LEDOUT_17=LedOff;
	}

__declspec(section ".userApplication_flash")
//*****************************************************************
//函数名  : LED_Light()
//描  述  : 点亮LED灯
//输入参数: 无
//返回参数: 无
//*****************************************************************

void LED_Light(uint32_t LightFlag)
{
	if(LightFlag&0x00000001) LEDOUT_1 = LedOn;
	else  LEDOUT_1= LedOff;
	if(LightFlag&0x00000002) LEDOUT_2 = LedOn;
	else  LEDOUT_2= LedOff;
	if(LightFlag&0x00000004) LEDOUT_3 = LedOn;
	else  LEDOUT_3= LedOff;
	if(LightFlag&0x00000008) LEDOUT_4 = LedOn;
	else  LEDOUT_4= LedOff;
	if(LightFlag&0x00000010) LEDOUT_5 = LedOn;
	else  LEDOUT_5= LedOff;
	if(LightFlag&0x00000020) LEDOUT_6 = LedOn;
	else  LEDOUT_6= LedOff;
	if(LightFlag&0x00000040) LEDOUT_7 = LedOn;
	else  LEDOUT_7= LedOff;
	if(LightFlag&0x00000080) LEDOUT_8 = LedOn;
	else  LEDOUT_8= LedOff;
	if(LightFlag&0x00000100) LEDOUT_9 = LedOn;
	else  LEDOUT_9= LedOff;
	if(LightFlag&0x00000200) LEDOUT_10 = LedOn;
	else  LEDOUT_10= LedOff;
	if(LightFlag&0x00000400) LEDOUT_11 = LedOn;
	else  LEDOUT_11= LedOff;
	if(LightFlag&0x00000800) LEDOUT_12 = LedOn;
	else  LEDOUT_12= LedOff;
	if(LightFlag&0x00001000) LEDOUT_13 = LedOn;
	else  LEDOUT_13= LedOff;
	if(LightFlag&0x00002000) LEDOUT_14 = LedOn;
	else  LEDOUT_14= LedOff;
	if(LightFlag&0x00004000) LEDOUT_15 = LedOn;
	else  LEDOUT_15= LedOff;
	if(LightFlag&0x00008000) LEDOUT_16 = LedOn;
	else  LEDOUT_16= LedOff;
	if(LightFlag&0x00010000) LEDOUT_17 = LedOn;
	else  LEDOUT_17= LedOff;;	
}
__declspec(section ".userApplication_flash")
//*****************************************************************
//函数名  : DlayFunction()
//描  述  : 延时函数
//输入参数: Seconds		: 延时秒
//	      : Ms			: 延时毫秒
//返回参数: 无
//*****************************************************************
void DlayFunction(uint32_t Seconds,uint32_t Ms) 
{        
    uint32_t j=0;
    
    uint32_t msTime=(Seconds*1000)+Ms;
	do
	{
	    for (j=0;j<5000;j++){}

	}while(--msTime);

}
#pragma pop
