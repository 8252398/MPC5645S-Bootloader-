//***************************************************************** 
//* FILE        : S_FileReceive.c
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
//* DATE        : 3 apir 2014
//* AUTHOR      : lilin 
//******************************************************************
#include "S_FileReceive.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/

CANReceive_Type  CANRxMsg;		//CAN½ÓÊÕÊý¾ÝÖ¡»º³åÇø
CANReceive_Type  CANTxMsg;		//CAN·¢ËÍÊý¾ÝÖ¡»º³åÇø

unsigned char TxDataBuffer[8]={0};		//CAN·¢ËÍÊý¾Ý»º³åÇø
unsigned char S_FileOnelineData[128] = {0};
CANBootloaderStatus CANBootldSts;
SFileRecord SInfroRecord;
ProgramDataBuffer ProgramDataBuf;
DataBuffer	ExchangeDataBuffer;

unsigned short LowFlashSelectFlag;
unsigned char  MidFlashSelectFlag;
unsigned char  HigFlashSelectFlag;
unsigned char  EraseFlag=0;
/*****************************************************************/ 
/*****************  Funtion definition ***************************/
/*****************************************************************/
#pragma push
#pragma section code_type ".boot_flash" 
__declspec(section ".boot_flash")
//*****************************************************************
//º¯ÊýÃû  : FlexCANTxData()
//Ãè  Êö  : FlexCAN·¢ËÍº¯Êý 
//ÊäÈë²ÎÊý: CANChannel	: ´ý·¢ËÍÊý¾ÝµÄCANÍ¨µÀ
//		  : MBNum		: ´ý·¢ËÍµÄÊý¾ÝCAN±¨ÎÄ»º³åÇø
//		  :	*pData		: Ö¸Ïò´ý·¢ËÍµÄÊý¾ÝÊ×µØÖ·
//·µ»Ø²ÎÊý: TRUE		: ·¢ËÍ³É¹¦
//		  :	FALSE		: ·¢ËÍÊ§°Ü
//*****************************************************************
static unsigned char  FlexCANTxData(CANReceive_Type *pCANTxDatum)
{
    unsigned char cnts;
    unsigned short TimeCounts=0;
    static unsigned char datalength;

    datalength=(unsigned char)CAN_2.BUF[0].CS.B.LENGTH;;//»ñÈ¡´ý·¢ËÍÊý¾ÝµÄ³¤¶È
   
    if(TxInactiveFlag==CAN_2.BUF[0].CS.B.CODE)	//ÅÐ¶Ïµ±Ç°ÊÇ·ñ¿ÉÒÔ·¢ËÍÊý¾Ý
    {
    	TxSuccFlg=FALSE;
    	for (cnts = 0; cnts < 8; cnts++)		//³õÊ¼»¯·¢ËÍ»º³åÇø
	    {
	        CAN_2.BUF[0].DATA.B[cnts] = 0xFF;
	    }
	    
    	for (cnts = 0; cnts < datalength; cnts++)//½«Êý¾Ý×°Èë·¢ËÍ»º³åÇøµÄÊý¾ÝÇøÓò
	    {
	        CAN_2.BUF[0].DATA.B[cnts] = pCANTxDatum->data[cnts];
	    }
	     
    }
    CAN_2.BUF[0].CS.B.CODE=TxActiveFlag;	//ÉèÖÃ·¢ËÍ»º³åÇøÎª¼¤»î×´Ì¬£¬ÎÞÌõ¼þ·¢ËÍÊý¾Ý	
	while(TxSuccFlg==FALSE)					//µÈ´ýÏàÓ¦»º³åÇø·¢ËÍÊý¾ÝÍê³É
	{
		TimeCounts++;						//ÑÓÊ±
		if(TimeCounts==2000)					
			return TxSuccFlg;				//ÑÓÊ±Ê±¼äµ½£¬ÒÀÈ»Ã»ÓÐ·¢ËÍÍê³É£¬Ôò·µ»Ø·¢ËÍÊ§°ÜÐÅÏ¢
	};
	CAN_2.IFRL.B.BUF00I = 1;				//Çå³ý·¢ËÍÖÐ¶Ï±êÖ¾Î»
	return TxSuccFlg;						//·µ»Ø·¢ËÍ³É¹¦ÐÅÏ¢
}
__declspec(section ".boot_flash")
//*****************************************************************
//º¯ÊýÃû  : CANStatusAcknowledge()
//Ãè  Êö  : ¿ØÖÆ°åÓëÉÏÎ»»úµÄÎÕÊÖÐÅÏ¢·¢ËÍ 
//ÊäÈë²ÎÊý: status		: µ±Ç°µÄ´«Êä×´Ì¬
//·µ»Ø²ÎÊý: ÎÞ
//*****************************************************************
void MCUStatusAcknowledge(unsigned char status)
{
	unsigned char cnts,err;
	
	for(cnts=0;cnts<8;cnts++)		//¶ÔÊý¾Ý»º³åÇø³õÊ¼»¯
	{
		TxDataBuffer[cnts] = 0xFF;
	}
	CANTxMsg.datalength = 3;
	TxDataBuffer[1]=status;			//ÌîÈëÒª·¢ËÍµÄÊý¾Ýµ½ÏàÓ¦Êý¾Ý»º³åÇø
	
	CANTxMsg.data = (unsigned char*)&TxDataBuffer[0];
	for(cnts=0;cnts<CANTxMsg.datalength;cnts++)	//½«Êý¾ÝÖ¡·ÅÈë·¢ËÍ»º³åÇø
	{
		CANTxMsg.data[cnts]=*(&TxDataBuffer[cnts]);
	}
	err=FlexCANTxData(&CANTxMsg);		//ÅÐ¶ÏÊÇ·ñ·¢ËÍ³É¹¦
}
__declspec(section ".boot_flash")
//*****************************************************************
//º¯ÊýÃû  : CANWorkStatusSet()
//Ãè  Êö  : ¿ØÖÆ°åÓëÉÏÎ»»úµÄÎÕÊÖÐÅÏ¢ÉèÖÃ
//ÊäÈë²ÎÊý: status		: ÐèÒªÉèÖÃµÄ×´Ì¬
//·µ»Ø²ÎÊý: ÎÞ
//*****************************************************************
static void CANWorkStatusSet(unsigned char status)
{
	MCUBootldState=status;
}
__declspec(section ".boot_flash")
//*****************************************************************
//º¯ÊýÃû  : CanWorkStatusGet()
//Ãè  Êö  : È¡¿ØÖÆ°åÓëÉÏÎ»»úµÄCANÏß×´Ì¬
//ÊäÈë²ÎÊý: ÎÞ
//·µ»Ø²ÎÊý: µ±Ç°CANÏßµÄ×´Ì¬
//*****************************************************************
unsigned char CanWorkStatusGet(void)
{
    return MCUBootldState;
}
__declspec(section ".boot_flash")
//*****************************************************************
//º¯ÊýÃû  : CAN_SRecrod_RxISR()
//Ãè  Êö  : CAN½ÓÊÕºÍ·¢ËÍ´¦ÀíÖÐ¶Ïº¯Êý
//ÊäÈë²ÎÊý: ÎÞ
//·µ»Ø²ÎÊý: ÎÞ
//*****************************************************************
void CAN_SRecrod_RxISR(void)
{
	unsigned char cnts;
	uint32_t FlagWord;
	
		
	if(CAN_2.IFRL.B.BUF04I)						//ÅÐ¶Ï½ÓÊÕÖÐ¶Ï±êÖ¾Î»
	{
		FlagWord=CAN_2.BUF[04].CS.R;
		
		for(cnts=0;cnts<10;cnts++) NOP();
		if(!CAN_2.BUF[04].CS.B.IDE)				//ÅÐ¶ÏÊÇ·ñÎªËùÐèÒªµÄÀ©Õ¹Ö¡¸ñÊ½
		{
			CANRxMsg.datalength=(unsigned char)CAN_2.BUF[04].CS.B.LENGTH; //µÃµ½½ÓÊÕµ½µÄÊý¾Ý³¤¶È
			CANRxMsg.data=(unsigned char*)&CAN_2.BUF[04].DATA.B[0];		  //½«½ÓÊÕÊý¾ÝµÄ´æ·ÅÖ¸ÕëÖ¸Ïò½ÓÊÕ»º³åÇøµÄÊý¾ÝÊ×µØÖ·

			if(CANRxMsg.MsgID==0x200)				//ÅÐ¶ÏÊÇ·ñÊÇÐèÒªµÄMessageID
			{
				for(cnts=0;cnts<CANRxMsg.datalength;cnts++)  //È¡»Ø½ÓÊÕµ½µÄÊý¾ÝÐÅÏ¢
				{
					CANRxMsg.data[cnts]=CAN_2.BUF[04].DATA.B[cnts];
				}
			}
			CAN_2.BUF[04].CS.B.CODE=0x04;
		}
		CANRxNewFram = TRUE;
		FlagWord=CAN_2.TIMER.R;
		CAN_2.IFRL.B.BUF04I=TRUE;
		for(cnts=0;cnts<10;cnts++) NOP();
	}
}
__declspec(section ".boot_flash")
//*****************************************************************
//º¯ÊýÃû  : CAN_0_ERR_ISR()
//Ãè  Êö  : CAN´íÎóÖÐ¶Ï´¦Àíº¯Êý
//ÊäÈë²ÎÊý: ÎÞ
//·µ»Ø²ÎÊý: ÎÞ
//*****************************************************************
void CAN_2_ERR_ISR(void)
{
	CAN_2.ESR.R=0XFFFF;
}
__declspec(section ".boot_flash")
//*****************************************************************
//º¯ÊýÃû  : CAN_SRecrod_TxISR()
//Ãè  Êö  : CAN·¢ËÍÖÐ¶Ï´¦Àíº¯Êý
//ÊäÈë²ÎÊý: ÎÞ
//·µ»Ø²ÎÊý: ÎÞ
//*****************************************************************
void CAN_SRecrod_TxISR(void)
{
	if(CAN_2.IFRL.B.BUF00I == 1)
	{
		CAN_2.IFRL.B.BUF00I = 1;	//Çå³þÖÐ¶Ï±êÖ¾Î»
		TxSuccFlg=TRUE;				//ÖÃÎ»·¢ËÍ³É¹¦±êÖ¾		
	}
	
}
__declspec(section ".boot_flash")
//*****************************************************************
//º¯ÊýÃû  : CANBootloader_Analysis()
//Ãè  Êö  : ¶Ô½ÓÊÕµ½µÄÊý¾Ý½øÐÐ·ÖÎö´¦Àí
//ÊäÈë²ÎÊý: ÎÞ
//·µ»Ø²ÎÊý: ÎÞ
//*****************************************************************
void CANBootloader_Analysis(void)
{
	unsigned char cnts,err;
	unsigned char ProcFlag;
	static unsigned char OutBootloaderFlag=0;
	static unsigned char pSDataNum=0;
	static SFileRecord SrcdDatum;
	
	CANRxNewFram = FALSE;						//Çå³ýCAN½ÓÊÕµ½ÐÂÊý¾ÝµÄ±êÖ¾Î»
	if(MCUBusyFlg)
	{
		if(CANRxMsg.data[0]!=DOWN_OUTBOOT_CMD)	//Èç¹ûMCU»¹ÔÚ´¦ÀíÉÏÒ»´ÎµÄÃüÁî£¬Ôò·µ»ØMCUÃ¦×´Ì¬
		{
			MCUStatusAcknowledge(UP_BUSY);
			return;
		}		
	}
	switch(CANRxMsg.data[0])					//½âÎöÊÕµ½µÄÊý¾ÝÖ¡µÚÒ»Ö¡
	{
		MCUBusyFlg=TRUE;
		case DOWN_INBOOTSUC_CMD:				//ÊÇ·ñÎªÉÏÎ»»úÁ¬½ÓÖ¸Áî
		{
			NOP();
			NOP();
			pSDataNum=0;

			MCUStatusAcknowledge(UP_INBOOT_SUC);//ÉèÖÃCANÁ¬½Ó×´Ì¬
			MCUBusyFlg=FALSE;					//Çå³ýµ±Ç°MCUÃ¦×´Ì¬
		
			CANWorkStatusSet(STA_LINKED);		//ÉèÖÃMCUÍ¬ÉÏÎ»»úµÄÁ¬½Ó×´Ì¬ÎªÁ¬½Ó³É¹¦±êÖ¾
			break;
		}
		case DOWN_ERASE_CMD:					//ÊÕµ½ÉÏÎ»»úµÄ²Á³ýFlashÖ¸Áî
		{
			 MCUStatusAcknowledge(UP_GETERASECMD);
			 if(!EraseFlag) 
			 {
			 	LowFlashSelectFlag=CANRxMsg.data[6];
			 	LowFlashSelectFlag=(unsigned short)((LowFlashSelectFlag<<8)|CANRxMsg.data[7]);
			 	MidFlashSelectFlag=CANRxMsg.data[5];
				HigFlashSelectFlag=CANRxMsg.data[4];
				if((CANRxMsg.data[7]&0x01)||(CANRxMsg.data[7]&0x02))
				{
					EraseFlag=0;
					MCUStatusAcknowledge(UP_ERASEBOOTAREA_ERR);
				}
				else
				{
					EraseFlag++;
					Erase_Flash();   //²Á³ýFlash
					ProgramDataBuf.status=ProgramDataNotReady;
    
				    ExchangeDataBuffer.status=DataBufferEmpty;
				    ExchangeDataBuffer.addr=0;
				    ExchangeDataBuffer.data=0;
				   
					if(EraseFlagStatus==EraseSuce)//ÅÐ¶ÏMCUÊÇ·ñÒÑ¾­¶ÔFlash½øÐÐ¹ý²Á³ý²Ù×÷£¬Ã»ÓÐÔò½øÐÐFlash²Á³ý
	                {
	                	MCUStatusAcknowledge(UP_ERASE_SUC);
	                	EraseFlag=0;
	                }
	                	
	                else if(EraseFlagStatus==EraseFail)
	                {
	                	MCUStatusAcknowledge(UP_ERASEERR);
	                	EraseFlag=0;
	                }	
				}			 	
			 }
			 else
			 {
			    MCUStatusAcknowledge(UP_BUSY);
			 }
			 break;
		}
		case DOWN_LINE_END:						//ÊÕµ½ÉÏÎ»»ú´«ËÍÍêÒ»ÐÐÊý¾ÝµÄÖ¸Áî
		{
			ProcFlag=S19DataConvert(&S_FileOnelineData[0], &SInfroRecord);//¶ÔÊÕµ½µÄÕûÐÐÊý¾Ý½øÐÐ´¦Àí
			pSDataNum=0;
			
			 
			if (ProcFlag == SPASS)			//¼ìÑéÐ£ÑéÊÇ·ñ³É¹¦£
			{
				while(SInfroRecord.status!=LineDataEnd)
                {
                	DoubleWordProgramSytleProcess(&ExchangeDataBuffer,&SInfroRecord,&ProgramDataBuf);
                	if(ProgramDataBuf.status==ProgramDataReady)
                	{
                		if(ProgramDataBuf.addr>=APP_ENRTY_ADDR)
                		{
			                CAN_2.MCR.B.MDIS = 1;
			                for(cnts=0;cnts<20;cnts++)
			                {
			                	NOP();
			                }
			                err=(unsigned char)FlashProgramer(&ProgramDataBuf);
			                for(cnts=0;cnts<20;cnts++)
			                {
			                	NOP();
			                }
			                CAN_2.MCR.B.MDIS = 0;
			                if(err==ProFail)  
			                {
				            	MCUStatusAcknowledge(UP_FLASH_ERR);
				            	EraseFlag=0;
				            	return;
			                }
                		}
                		else
                		{
				            if(SInfroRecord.status==LineDataEnd)
				            	MCUStatusAcknowledge(UP_PROONELINE_SUC);
				            return;
                		}
                	}
                }
                FlashBlkRead(SrcdDatum.data, SInfroRecord.addr, SInfroRecord.datalen);//¶ÁÈ¡Ð´ÈëFlashÖÐµÄÊý¾Ý
                
                for (cnts = 0; cnts < SInfroRecord.datalen; cnts++)  //±È½ÏÊÕµ½µÄÊý¾ÝÓëÐ´ÈëµÄÊý¾ÝÊÇ·ñÒ»ÖÂ
	            {
	                if (SrcdDatum.data[cnts] != SInfroRecord.data[cnts])
	                {
						MCUStatusAcknowledge(UP_FLASH_ERR);
						EraseFlag=0;
	                    return;
	                }
	            }
	        	MCUStatusAcknowledge(UP_PROONELINE_SUC);
			}
     
            else if (ProcFlag == SERR)
            {
            	MCUStatusAcknowledge(UP_DOWN_ERR); //Ð£Ñé²»³É¹¦£¬Ôò·µ»Ø´íÎóÐÅÏ¢
            	MCUBusyFlg=FALSE;				   //Çå³ýµ±Ç°MCUÃ¦×´Ì¬	
            } 
            else if (ProcFlag == SInvalid)
            {
            	MCUStatusAcknowledge(UP_DOWN_ERR); //Ð£Ñé²»³É¹¦£¬Ôò·µ»Ø´íÎóÐÅÏ¢
            	MCUBusyFlg=FALSE;				  //Çå³ýµ±Ç°MCUÃ¦×´Ì¬
            }  
            else
            {
            	MCUStatusAcknowledge(UP_PROONELINE_SUC); //ÉèÖÃÍ¨ÐÅ×´Ì¬
            	MCUBusyFlg=FALSE;						//Çå³ýµ±Ç°MCUÃ¦×´Ì¬
            }  
            break;
		}
		case DOWN_FILE_END:    					//ÊÕµ½ÎÄ¼þ´«ÊäÍê³ÉÖ¸Áî
        {
            if(ExchangeDataBuffer.status==DataBufferFull)
            {
            	ProgramDataBuf.status=ProgramDataReady;
            	ProgramDataBuf.addr=ExchangeDataBuffer.addr;
            	ProgramDataBuf.data[0]=ExchangeDataBuffer.data;
            	ProgramDataBuf.data[1]=0x00;
            	
            	if(ProgramDataBuf.status==ProgramDataReady)
            	{
            		if(ProgramDataBuf.addr>=APP_ENRTY_ADDR)
            		{
            			err=(unsigned char)FlashProgramer(&ProgramDataBuf);
		                NOP();
		                if(err==ProFail)  
		                {
			            	MCUStatusAcknowledge(UP_FLASH_ERR);
			            	EraseFlag=0;
			            	return;
		                }
            		}
            	}
            }
            MCUStatusAcknowledge(UP_PRGEND);	//ÉèÖÃÍ¨ÐÅ×´Ì¬
            MCUBusyFlg=FALSE;					//Çå³ýµ±Ç°MCUÃ¦×´Ì¬
            CANWorkStatusSet(STA_PRGEND);		
            break;
        }
        case DOWN_OUTBOOT_CMD:
        {
        	if((CANRxMsg.data[7]==0x80)&&(OutBootloaderFlag==0))
        	{
        		if(CanWorkStatusGet()==STA_PRGEND)
	        	{
	        		MCUStatusAcknowledge(UP_OUTBL_SUC);	 //ÉèÖÃÍ¨ÐÅ×´Ì¬
	        		CopyFlashDataToRAM((uint32_t*)EraseSetEnterBootFlagFlash,(uint32_t*)RAM_ADDR_ERASE_FLASH,Length_copy_ram);//½«ÖÐ¶Ïº¯Êý¸´ÖÆµ½RAMÇøÔËÐÐ
        			(*(APPFunction)RAM_ADDR_ERASE_FLASH)();
	        		//INTC.MCR.B.HVEN = 1; 			 	 //½ûÖ¹ÖÐ¶Ï
					CAN_2.MCR.B.MDIS = 1;          		 //½ûÖ¹CAN
					DlyS_Ms(100);
			        (*(APPFunction)APP_ENRTY_ADDR)();    //Ìø×ªµ½Ó¦ÓÃ³ÌÐò
			        for(;;)
			        ;
	        	}
	        	else
	        	{
	        		MCUStatusAcknowledge(UP_PRO_NO_END_ERR);
	        		OutBootloaderFlag++;
	        	}
        	}
        	else if((CANRxMsg.data[7]==0xA0)&&(OutBootloaderFlag==1))
        	{
        		if(OutBootloaderFlag)
        		{
        			OutBootloaderFlag=0;
        			MCUStatusAcknowledge(UP_OUTBL_SUC);	 //ÉèÖÃÍ¨ÐÅ×´Ì¬
        			CopyFlashDataToRAM((uint32_t*)EraseSetEnterBootFlagFlash,(uint32_t*)RAM_ADDR_ERASE_FLASH,Length_copy_ram);//½«ÖÐ¶Ïº¯Êý¸´ÖÆµ½RAMÇøÔËÐÐ
        			(*(APPFunction)RAM_ADDR_ERASE_FLASH)();
	        		//INTC.MCR.B.HVEN = 1; 			 	 //½ûÖ¹ÖÐ¶Ï
					CAN_2.MCR.B.MDIS = 1;          		 //½ûÖ¹CAN
					DlyS_Ms(100);
			        (*(APPFunction)APP_ENRTY_ADDR)();    //Ìø×ªµ½Ó¦ÓÃ³ÌÐò
			        for(;;)
			        ;
        		}	
        	}
        	else if(CANRxMsg.data[7]==0xF0)
        	{
        		OutBootloaderFlag=0;
        		MCUStatusAcknowledge(UP_CANCELINBOOT);
        	}
        	
        	break;
        }
        default:               
        {
            if (CanWorkStatusGet() == STA_LINKED)//½ÓÊÕµ½µÄÎª´¿Êý¾ÝÊ±
            {
                for (cnts = 0; cnts < CANRxMsg.datalength; cnts++) 
                {
                    S_FileOnelineData[pSDataNum++] = CANRxMsg.data[cnts];  //½«Êý¾Ý´æÈë»º³åÇø
                }
                MCUBusyFlg=FALSE;				//Çå³ýµ±Ç°MCUÃ¦×´Ì¬
            }
            break;
        }
	}
}
#pragma pop