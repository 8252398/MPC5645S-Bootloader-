//***************************************************************** 
//* FILE        : FlexCANInitial.c
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
#include "MPC5645S.h"
#include "LEDCfg.h"
#include "ModeCtrDefine.h"
#include "FlexCAN_RxTx.h"
#include "FlashOperation.h"
#include "FlexCANInitial.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/
extern CANReceive_Type		CAN_RxBootloaderInfo;



void InitialMCU();
/*****************************************************************/ 
/*****************  Funtion definition ***************************/
/*****************************************************************/

#pragma push
#pragma section ".userApplicationInterface_flash" 
__declspec(section ".userApplicationInterface_flash")
//*****************************************************************
//函数名  : main()
//描  述  : 主函数，用户程序的入口处 
//输入参数: 无
//返回参数: 无
//*****************************************************************
int main() 
{
  volatile int i = 0;
  unsigned char cnts=0;
  static uint32_t LoopFlag=0x00010000;
  static unsigned char IntoBootFlag=0;
  
  InitialMCU();
  LED_IOCfgInitial();
  LED_Init();
  InitialFlexCANAPP();
  
  CANRxNewDatum=FALSE;
  GotBootloaderSuc=FALSE;
  /* Loop forever */
  for (;;) 
  {
		LoopFlag=(0x00000001<<cnts);
		LED_Light(LoopFlag);
		DlayFunction(1,0);
		if(cnts==17) cnts=0;
		else cnts++;
		if(LoopFlag==0x0000FFFF) LoopFlag=0x00000001;
		
		if(CANRxNewDatum)
		{
			CANRxNewDatum=FALSE;
			switch(CAN_RxBootloaderInfo.data[0])
			{
				case DOWN_LINK:
				
					if(CAN_RxBootloaderInfo.data[7]==0x80)
					{
						TransAcknowledge(UP_SUREINBOOT);
						IntoBootFlag++;
					}	

					else if(CAN_RxBootloaderInfo.data[7]==0xA0)
					{
						if(IntoBootFlag)
						{
							IntoBootFlag=0;
							//INTC.MCR.B.HVEN = 1; 				// disable software vector mode
							CAN_2.MCR.B.MDIS = 1;          		// disable FlexCAN 
				
							//DlayFunction(0,50);
							CopyFlashDataToRAM((uint32_t*)EraseEnterBootFlagFlash,(uint32_t*)RAM_ADDR_ERASE_FLASH,Length_copy_ram);//将中断函数复制到RAM区运行
							CopyFlashDataToRAM((uint32_t*)EnterBootFlagFlashProgramer,(uint32_t*)RAM_ADDR_PROG_FLASH,Length_copy_ram);//将中断函数复制到RAM区运行
							(*(APPFunction)RAM_ADDR_ERASE_FLASH)();
							(*(APPFunction)RAM_ADDR_PROG_FLASH)();
						    if(eraseEnterBootFlashFlag)
							{
								if(progEnterBootFlashFlag)
								{
									(*(APPFunction)INTO_BOOT_ADDR)();
									for(;;)
									;
								}
							}
							
						}
					}
					else if(CAN_RxBootloaderInfo.data[7]==0xF0)
					{
						TransAcknowledge(UP_CANCELINBOOT);
						IntoBootFlag=0;
					}		
					break;
				default:
					break;
			}
		}

  	}
}
#pragma pop

#pragma push
#pragma section ".userApplication_flash" 
__declspec(section ".userApplication_flash")
void SetupPLL(void)
{
    ME.DRUN.B.FXOSCON=1;			        /* Switch on external osc */
    ME.DRUN.B.SYSCLK=2;			            /* Select external osc */
/* RE enter the drun mode, to update the configuration */
    //ME.MCTL.R = 0x30005AF0;                 /* Mode & Key */
    //ME.MCTL.R = 0x3000A50F;                 /* Mode & Key inverted */
    //while(ME.GS.B.S_FXOSC==0) {}            /* Wait for external osc to stabilize */
    //while(ME.GS.B.S_MTRANS==1) {}           /* Wait for mode entry to complete */
    //while(ME.GS.B.S_CURRENTMODE!=3) {}      /* Check DRUN mode has been entered */
/* Max bus 64Mhz, VCO 256-512Mhz  Voltage control osc
/* Fpll = XTAL * NDIV / IDF / ODF =248;// 64MHz */
/* Fvco = XTAL * NDIV / IDF = 512MHz */

	CGM.FMPLL[0].CR.B.IDF=0x0000; 	        /* Divide by 1 */
	CGM.FMPLL[0].CR.B.ODF=0x0;	            /* Divide by 2 */
	CGM.FMPLL[0].CR.B.NDIV=55; 	        /* Loop divide by freq */

    ME.DRUN.B.FMPLL0ON=1;                   /* enable pll */      
/* RE enter the drun mode, to update the configuration */
    ME.MCTL.R = 0x30005AF0;     	        /* Mode & Key */
    ME.MCTL.R = 0x3000A50F;                 /* Mode & Key inverted */
    while(ME.GS.B.S_MTRANS==1) {}		    /* Wait for mode entry to complete */
    while(ME.GS.B.S_CURRENTMODE!=0x3) {}    /* Check DRUN mode has been entered */	
    while(CGM.FMPLL[0].CR.B.S_LOCK==0) {}	
    ME.DRUN.B.SYSCLK=0x4;                   /* system clock is PLL */
    ME.MCTL.R = 0x30005AF0;     	        /* Mode & Key */
    ME.MCTL.R = 0x3000A50F;                 /* Mode & Key inverted */
    while(ME.GS.B.S_MTRANS==1) {}		    /* Wait for mode entry to complete */
    while(ME.GS.B.S_CURRENTMODE!=0x3) {}    /* Check DRUN mode has been entered */
/* ME_GS Poll Global status register to get current system clock
   fail if system clock is not pll
   0000 16MHz internal RC oscillator
   0001 divided 16MHz internal RC oscillator
   0010 4MHz crystal oscillator
   0011 divided 4MHz crystal oscillator
   0100 system PLL
   1111 system clock is disabled */
    while(ME.GS.B.S_SYSCLK != 4){}          /* fail is stuck here	 */
}
#pragma pop

#pragma push
#pragma section ".userApplication_flash" 
__declspec(section ".userApplication_flash")
//*****************************************************************
//函数名  : InitialMCU()
//描  述  : 对MCU初始化，初始化时钟等
//输入参数: 无
//返回参数: 无
//*****************************************************************
void InitialMCU(void)
{
	uint8_t i;
	
	SWT.SR.R = 0x0000C520; 
	SWT.SR.R = 0x0000D928; // 复位看门狗定时器 
	SWT.CR.R = 0x8000010A; // 禁止看门狗 

		/* Enable all peripheral clocks */
	for (i=0;i<4;i++) CGM.SC_DC[i].R = 0x80;
	/* Setting RUN Configuration Register ME_RUN_PC[0] */
	ME.RUNPC[0].R=0x000000FE; /* Periphe ral ON in every mode */
	CGM.OCDS_SC.B.SELCTL=0; 	   	   //Select Fast IRC (16 MHz)
	/* Enable aux peripheral clocks */
	CGM.AC0_DC.R = 0x80000000;
	CGM.AC1_DC.R = 0x80000000;
	CGM.AC2_DC.R = 0x80000000;
	CGM.AC3_DC.R = 0x80000000;
	CGM.AC4_DC.R = 0x80000000;

	CGM.AC0_SC.R = 0x03000000; // Selects the FMPLL0/2 clock for pixel clock
	CGM.AC0_DC.R = 0x80000000; // 15+1 division

	CGM.AC2_SC.R = 0x03000000; // Selects the FMPLL0/2 clock for pixel clock
	
	CGM.AC4_SC.R = 0x03000000; // Selects the FMPLL0/2 clock for pixel clock
	CGM.AC4_DC.R = 0x80000000; // No division
	CGM.AC5_SC.R = 0x01000000; // Selects the FMPLL0 cloc
	/* Use 1 of the next 2 lines depending on crystal frequency: */
	/*system clock = (Clock_In*R_Loop)/(R_Inp*R_Outp)*/

	//CGM.FMPLL[0].CR.R = 0x013C0100; /* 8 MHz xtal: Set PLL0 to 120 MHz */ 
	//CGM.FMPLL[0].CR.R = 0x02400100; /* 8 MHz xtal: Set PLL0 to 64 MHz */ 
	   	
	SetupPLL();
	#if 0

	ME.PCTL[FlexCAN_2_Control].R = 0x01;   /* MPC56xxB/S FlexCAN2 */	
	ME.PCTL[SIUL_Control].R = 0x01;        /* MPC56xxB/S SIUL:  select ME.RUNPC[1] */	

	                                /* Mode Transition to enter RUN0 mode: */
	ME.MCTL.R = 0x40005AF0;         /* Enter RUN0 Mode & Key */
	ME.MCTL.R = 0x4000A50F;         /* Enter RUN0 Mode & Inverted Key */
	  
	while (ME.GS.B.S_MTRANS) {}     /* Wait for mode transition to complete */    
	                                /* Note: could wait here using timer and/or I_TC IRQ */
	while(ME.GS.B.S_CURRENTMODE != 4) {} /* Verify RUN0 is the current mode */
	#endif

	INTC.MCR.B.HVEN = 0; 	// software vector mode enable
    INTC.CPR.B.PRI = 0;		// low the PRI field to allow other interrupt to be served
}
#pragma pop

