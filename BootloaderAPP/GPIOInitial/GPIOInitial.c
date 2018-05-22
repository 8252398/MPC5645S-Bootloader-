//***************************************************************** 
//* FILE        : GPIOInitial.c
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
#include "GPIOInitial.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/

/*****************************************************************/ 
/*****************  Funtion definition ***************************/
/*****************************************************************/
#pragma push
#pragma section code_type ".boot_flash"
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : ForceIntoBootKEYInitial()
//描  述  : 对强制进入Bootloader的按键进行初始化
//输入参数: 无
//返回参数: 无
//*****************************************************************
void ForceIntoBootKEYInitial(void)
{
	SIU.PCR[PC7].B.PA = 0;//LEDOUT_1
	SIU.PCR[PC7].B.OBE = 0;
	SIU.PCR[PC7].B.IBE = 1;
	
	SIU.PCR[PC8].B.PA = 0;//LEDOUT_1
	SIU.PCR[PC8].B.OBE = 0;
	SIU.PCR[PC8].B.IBE = 1;
}
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : ScanKEYStatus()
//描  述  : 检测按键是否按下
//输入参数: 无
//返回参数: 无
//*****************************************************************
unsigned char ScanKEYStatus(void)
{
	if(!BootSelect_One)
	{
		DlyS_Ms(10);
		if(!BootSelect_One)
		{
			if(!BootSelect_Two)
			{
				DlyS_Ms(10);
				if(!BootSelect_Two)
				{
					return TRUE;
				}
				else return FALSE;
			}
			else return FALSE;
		}
		else return FALSE;
	}
	else return FALSE;
}
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : PITInitial()
//描  述  : 禁止定时器
//输入参数: 无
//返回参数: 无
//*****************************************************************
void PITInitial(void)
{
	  PIT.MCR.B.MDIS = 1;  // Module enable
	  PIT.MCR.B.MDIS = 1;  // Module enable
	  PIT.MCR.B.MDIS = 1;  // Module enable
}
__declspec(section ".boot_flash")
//*****************************************************************
//函数名  : MCUConfiguration()
//描  述  : MCU时钟等配置
//输入参数: 无
//返回参数: 无
//*****************************************************************
void MCUConfiguration(void)
{
	SWT.SR.R = 0x0000C520; 
	SWT.SR.R = 0x0000D928; // 复位看门狗定时器 
	SWT.CR.R = 0x8000010A; // 禁止看门狗 

  	ME.MER.R = 0x0000001D;        /* Enable DRUN, RUN0, SAFE, RESET modes */
                                  /* Initialize PLL before turning it on: */
	// Use 1 of the next 2 lines depending on crystal frequency: */
	//system clock = (Clock_In*R_Loop)/(R_Inp*R_Outp)*/
	
	//CGM.FMPLL[0].CR.R = 0x013C0100; /* 8 MHz xtal: Set PLL0 to 120 MHz */ 
	CGM.FMPLL[0].CR.R = 0x013E0100; /* 8 MHz xtal: Set PLL0 to 124 MHz */ 
	//CGM.FMPLL[0].CR.R = 0x02400100; /* 8 MHz xtal: Set PLL0 to 64 MHz */  
	   	
    ME.RUN[0].R = 0x001F0074;       /* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL */
	ME.RUNPC[1].R = 0x000000FE; 	/* Peri. Cfg. 1 settings: only run in RUN0 mode */

	ME.PCTL[FlexCAN_2_Control].R = 0x01;   /* MPC56xxB/S FlexCAN2 */	
	ME.PCTL[SIUL_Control].R = 0x01;        /* MPC56xxB/S SIUL:  select ME.RUNPC[1] */	

	                                /* Mode Transition to enter RUN0 mode: */
	ME.MCTL.R = 0x40005AF0;         /* Enter RUN0 Mode & Key */
	ME.MCTL.R = 0x4000A50F;         /* Enter RUN0 Mode & Inverted Key */
	  
	while (ME.GS.B.S_MTRANS) {}     /* Wait for mode transition to complete */    
	                                /* Note: could wait here using timer and/or I_TC IRQ */
	while(ME.GS.B.S_CURRENTMODE != 4) {} /* Verify RUN0 is the current mode */
	
	INTC.MCR.B.HVEN = 0; 	// software vector mode enable
    INTC.CPR.B.PRI = 0;		// low the PRI field to allow other interrupt to be served
}
#pragma pop