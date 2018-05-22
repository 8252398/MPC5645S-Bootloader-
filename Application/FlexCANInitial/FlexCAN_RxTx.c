//***************************************************************** 
//* FILE        : FlexCANRxTx.c
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
#include "FlexCAN_RxTx.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/
CANReceive_Type		CAN_RxBootloaderInfo;
CANReceive_Type		CAN_TxBootloaderInfo;
BOOTLOADERCANFLAG	BootloaderCANStatus;

unsigned char  CANTxDataBuffer[8]={0};
#pragma push
#pragma section code_type ".userApplication_flash"
/*****************************************************************/ 
/*****************  Funtion definition ***************************/
/*****************************************************************/
__declspec(section ".userApplication_flash")
void CAN_Tx_ISR(void)
{
	if(CAN_2.IFRL.B.BUF00I)
	{
		CAN_2.IFRL.B.BUF00I=TRUE;
		BootloaderCANTransSucFlag=TRUE;
	}
}

void CAN_Rx_ISR(void)
{
	unsigned char cnts;
	unsigned int FlagWord;
	
	if(CAN_2.IFRL.B.BUF04I)						//�жϽ����жϱ�־λ
	{
		FlagWord=CAN_2.BUF[4].CS.R;
		
		for(cnts=0;cnts<10;cnts++) NOP();
		if(!CAN_2.BUF[4].CS.B.IDE)				//�ж��Ƿ�Ϊ����Ҫ����չ֡��ʽ
		{
			CAN_RxBootloaderInfo.datalength=(unsigned char)CAN_2.BUF[4].CS.B.LENGTH; //�õ����յ������ݳ���
			CAN_RxBootloaderInfo.data=(unsigned char*)&CAN_2.BUF[4].DATA.B[0];
			
			if(CAN_2.BUF[4].ID.B.STD_ID==CANRxFilterID)				//�ж��Ƿ�����Ҫ��MessageID
			{
				for(cnts=0;cnts<CAN_RxBootloaderInfo.datalength;cnts++)  //ȡ�ؽ��յ���������Ϣ
				{
					CAN_RxBootloaderInfo.data[cnts]=*(&CAN_2.BUF[4].DATA.B[cnts]);
				}
			}
			CAN_2.BUF[4].CS.B.CODE=0x04;
		}
		CANRxNewDatum = TRUE;
		FlagWord=CAN_2.TIMER.R;
		
		CAN_2.IFRL.B.BUF04I=TRUE;
		for(cnts=0;cnts<10;cnts++) NOP();
	}
	
}
__declspec(section ".userApplication_flash")
//*****************************************************************
//������  : BootloaderCANTxData()
//��  ��  : FlexCAN���ͺ��� 
//�������: CANChannel	: ���������ݵ�CANͨ��
//		  : MBNum		: �����͵�����CAN���Ļ�����
//		  :	*pData		: ָ������͵������׵�ַ
//���ز���: TRUE		: ���ͳɹ�
//		  :	FALSE		: ����ʧ��
//*****************************************************************
static unsigned char  BootloaderCANTxData(CANReceive_Type *pCANTxDatum)
{
    unsigned char cnts;
    unsigned short TimeCounts=0;
    static unsigned char datalength;

    datalength=(unsigned char)CAN_2.BUF[0].CS.B.LENGTH;;//��ȡ���������ݵĳ���
   
    if(TxInactiveFlag==CAN_2.BUF[0].CS.B.CODE)	//�жϵ�ǰ�Ƿ���Է�������
    {
    	BootloaderCANTransSucFlag=FALSE;
    	for (cnts = 0; cnts < 8; cnts++)		//��ʼ�����ͻ�����
	    {
	        CAN_2.BUF[0].DATA.B[cnts] = 0xFF;
	    }
	    
    	for (cnts = 0; cnts < datalength; cnts++)//������װ�뷢�ͻ���������������
	    {
	        CAN_2.BUF[0].DATA.B[cnts] = pCANTxDatum->data[cnts];
	    }
	     
    }
    CAN_2.BUF[0].CS.B.CODE=TxActiveFlag;	//���÷��ͻ�����Ϊ����״̬����������������	
	while(BootloaderCANTransSucFlag==FALSE)					//�ȴ���Ӧ�����������������
	{
		TimeCounts++;						//��ʱ
		if(TimeCounts==2000)					
			return FALSE;				//��ʱʱ�䵽����Ȼû�з�����ɣ��򷵻ط���ʧ����Ϣ
	};
	CAN_2.IFRL.B.BUF00I = 1;				//��������жϱ�־λ
	return TRUE;						//���ط��ͳɹ���Ϣ
}
__declspec(section ".userApplication_flash")
//*****************************************************************
//������  : TransAcknowledge()
//��  ��  : ���ư�����λ����������Ϣ���� 
//�������: status		: ��ǰ�Ĵ���״̬
//���ز���: ��
//*****************************************************************
void TransAcknowledge(unsigned char status)
{
	unsigned char cnts,err;
	
	for(cnts=0;cnts<8;cnts++)		//�����ݻ�������ʼ��
	{
		CANTxDataBuffer[cnts] = 0xFF;
	}
	CAN_TxBootloaderInfo.datalength = 3;
	CANTxDataBuffer[1]=status;			//����Ҫ���͵����ݵ���Ӧ���ݻ�����
	
	CAN_TxBootloaderInfo.data = (unsigned char*)&CANTxDataBuffer[0];
	for(cnts=0;cnts<CAN_TxBootloaderInfo.datalength;cnts++)	//������֡���뷢�ͻ�����
	{
		CAN_TxBootloaderInfo.data[cnts]=CANTxDataBuffer[cnts];
	}
	err=BootloaderCANTxData(&CAN_TxBootloaderInfo);		//�ж��Ƿ��ͳɹ�
}
#pragma pop
