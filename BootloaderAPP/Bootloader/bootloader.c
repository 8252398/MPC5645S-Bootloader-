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
#include "bootloader.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/
extern CANReceive_Type  CANRxMsg;
extern ProgramDataBuffer ProgramDataBuf;
extern DataBuffer	ExchangeDataBuffer;

extern unsigned short LowFlashSelectFlag;
extern unsigned char  MidFlashSelectFlag;
extern unsigned char  HigFlashSelectFlag;

extern unsigned char  EraseFlag;

/*****************************************************************/ 
/*****************  Funtion definition ***************************/
/*****************************************************************/

//*****************************************************************
//������  : CopyBootloaderCodeToRAM()
//��  ��  : ��λ��Flash�е�Bootloader���뿽����RAM
//�������: ��
//���ز���: ��
//*****************************************************************
#pragma push
#pragma section ".init" 
__declspec(section ".init") 
void CopyBootloaderCodeToRAM(void)
{
	CopyFlashDataToRAM((uint32_t*)BOOT_ENTER_ADDR,(uint32_t*)RAM_BOOT_ENTER_ADDR,BOOT_LENGTH);
	(*(APPFunction)RAM_BOOT_ENTER_ADDR)();
}
#pragma pop


#pragma push
#pragma section ".BootloaderInterface_flash" 
__declspec(section ".BootloaderInterface_flash")
//*****************************************************************
//������  : Bootloader_main()
//��  ��  : Bootloader�����������
//�������: ��
//���ز���: ��
//*****************************************************************
void Bootloader_main(void)
{
	uint32_t cnts=0;
	static unsigned char KEYPressStatus;
	static unsigned short DelayTimes;
	unsigned char upDatedFlagFlash[12];
	static unsigned char upDatedFlag=1;
	unsigned char upDatedFlash[12]={0x4E,0x65,0x65,0x64,0x20,0x75,0x70,0x64,0x61,0x74,0x65,0x64};

    CANRxNewFram = FALSE;
    MCUBootldState = STA_NOT_LINKED;
    
    ProgramDataBuf.status=ProgramDataNotReady;
    
    ExchangeDataBuffer.status=DataBufferEmpty;
    ExchangeDataBuffer.addr=0;
    ExchangeDataBuffer.data=0;
    
    LowFlashSelectFlag=0;
	MidFlashSelectFlag=0;
	HigFlashSelectFlag=0;
	
	EraseFlag=0;
	
	MCUConfiguration();
	PITInitial();
    InitialFlexCAN();
    ForceIntoBootKEYInitial();
    FlashBlkRead((uint32_t*)&upDatedFlagFlash[0],0x8000,3);
    
    for (cnts = 0; cnts < 12; cnts++)  //�Ƚ��յ���������д��������Ƿ�һ��
    {
        if (upDatedFlagFlash[cnts] != upDatedFlash[cnts])
        {
        	upDatedFlag=0;
        	break;
        }
    }
    
    if(upDatedFlag)
    {
    	CAN_2.MCR.B.MDIS = 1;          		// disable FlexCAN 
    	CANRxNewFram = FALSE;
        MCUBootldState = STA_LINKED;		//�������ӳɹ���־
    	CAN_2.MCR.B.MDIS = 0;          		// enable FlexCAN 
    }
    
    else
    {
    	for(cnts=0; cnts<500; cnts++)
	     {   // wait 500ms for Bootload link request from host
	        
	         KEYPressStatus =(unsigned char)ScanKEYStatus();
	         
	         if(KEYPressStatus) DelayTimes++;
	         else DelayTimes=0;
	         
	         if(DelayTimes>100) 
	         {
	         	DelayTimes=0;
	         	CANRxNewFram = FALSE;
	            MCUBootldState = STA_LINKED;		//�������ӳɹ���־
	            break;
	         }
	         else 
	         {
	            DlyS_Ms(10);
	         }
	    }
    }
    
    if (MCUBootldState == STA_NOT_LINKED) 
    {    // no bootload link request during last 500ms
    
		//INTC.MCR.B.HVEN = 1; 				// disable software vector mode
		CAN_2.MCR.B.MDIS = 1;          		// disable FlexCAN 
        (*(APPFunction)APP_ENRTY_ADDR)();	// jump to app and should not back
        for(;;)
		;
    }
	MCUStatusAcknowledge(UP_INBOOT_SUC);
	
    for(;;) 
    {
    	if(CANRxNewFram)				//����Ƿ���յ�CAN����֡
    	{
    		CANBootloader_Analysis();	//�Խ��յ���CAN����֡���н���
    	}
    }
}
#pragma pop

#pragma push
#pragma section ".boot_flash" 
__declspec(section ".boot_flash")
//*****************************************************************
//������  : DlyS_Ms()
//��  ��  : ��ʱ����
//�������: Mills		: ��ʱʱ��
//���ز���: ��
//*****************************************************************
void DlyS_Ms(uint32_t Mills) 
{        
    uint32_t i,j;

	for (i=0;i<Mills;i++) 
    {
        for (j=0;j<700;j++){}
    }
}
#pragma pop

/*
#pragma push
#pragma section ".boot_flash" 
__declspec(section ".boot_flash")

#pragma pop
*/