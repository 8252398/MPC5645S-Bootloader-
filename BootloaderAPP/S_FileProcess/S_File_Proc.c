//***************************************************************** 
//* FILE        : S_File_Proc.c
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
#include "S_File_Proc.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/

/*****************************************************************/ 
/****************  Funtion declaration ***************************/
/*****************************************************************/
#pragma push
#pragma section code_type ".boot_flash" 
__declspec(section ".boot_flash")
//*****************************************************************
//������  : Hex2Value()
//��  ��  : ���յ���16��������ת����ʮ��������
//�������: ��
//���ز���: ��
//*****************************************************************
static unsigned char Hex2Value(unsigned char *p)
{   
    unsigned char k,i;

    k=*p;
    p++;
    i=*p;
    p++;

    if(k>='0' && k<='9')
        k=k-'0';
    else
        k=k-'A'+10;
    
    if(i>='0' && i<='9')
        i=i-'0';
    else
        i=i-'A'+10;
    
    return k*16+i;
}
__declspec(section ".boot_flash")
//*****************************************************************
//������  : S19DataConvert()
//��  ��  : ���յ���һ��S���Ľ��н���
//�������: *OneLineDatum	: ���յ���һ�б���
//        : *pSDataRcd		: �洢���ĵĻ�����Ϣ
//���ز���: SERR			: ����У�����
//		  : SPASS			: ����У��ɹ�
//		  : SFileStart		: ���Ĵ��俪ʼ��־
//		  : SFileEnd		: ���Ĵ��������־
//		  : SInvalid		: ������Ч��־
//*****************************************************************
unsigned char S19DataConvert(unsigned char *OneLineDatum, SFileRecord *pSDataRcd) 
{   
    unsigned char  SFrametype,AddressLen,SFramelength,Sdatum,SDatalen,oddflag = 0;
    unsigned char  checksum = 0;
    unsigned char  *pLineData;
    uint32_t  temp;
    unsigned short  i,j,k;
    
    pLineData=OneLineDatum;
    
    for(i=0;i<5;i++) pSDataRcd->data[i]=0;
  
    if(*pLineData != 'S') 				//�жϱ��Ŀ�ʼ��־
    {
        pSDataRcd->datalen = 0;      
		return SInvalid;					//������Ŀ�ͷ��Ϊ'S'���򷵻ر�����Ч��־
    }
		
    pLineData++;							//�ۼӱ��ĵ�ַ
    SFrametype = *pLineData - '0';		//�õ����ĵĸ�ʽ

	if(SFrametype == 0) 					//�жϱ��ĵĸ�ʽ��Ϣ
	{
        pSDataRcd->datalen = 0;
        return SFileStart;					//���Ϊ'S0'��ʽ���򷵻��ļ���ʼ��־
	}
	else if(SFrametype > 6) 
	{
        pSDataRcd->datalen = 0;
        return SFileEnd;					//������ĸ�ʽΪ'S7','S8','S9',�򷵻��ļ�������־
	}
	else 
	{
        AddressLen = SFrametype + 1;        //�õ�������������ĵ�ַ������Ϣ  

        pLineData++;						//�ۼӱ��ĵ�ַ
        SFramelength = Hex2Value(pLineData);  //�õ����Ĵ����֡���ݳ�����Ϣ

        SDatalen = SFramelength-AddressLen-1;//�õ����Ĵ�������ݳ�����Ϣ   
        if(SDatalen&0x01) 					 //�ж��Ƿ�Ϊż����������
        {           
            oddflag = TRUE;             	//���Ϊ����������λ������־λΪ1
        }
        else oddflag = FALSE; 
        pSDataRcd->datalen = (unsigned short)((SFramelength-AddressLen-1)/4); //���汨�Ĵ�������ݳ�����Ϣ 
                                                      
        checksum += SFramelength;			//У����

        pLineData += 2;					//��ַ�ۼ�
        
        for(pSDataRcd->addr = 0, i = 0; i < AddressLen; i++, pLineData += 2) //ȡ���Ĵ���ĵ�ַ��Ϣ
        {
            Sdatum = Hex2Value(pLineData);
            pSDataRcd->addr = (pSDataRcd->addr<<8) | Sdatum;
            checksum += Sdatum;
        }
		
        j=k=0;
        for(i = 0; i < SDatalen; i++) 
        {
            Sdatum = Hex2Value(pLineData);	//ȡ�����������������Ϣ��������
        	pLineData += 2;
        	checksum += Sdatum;
        	pSDataRcd->data[k] = pSDataRcd->data[k]<<8;
        	pSDataRcd->data[k] = pSDataRcd->data[k]|Sdatum;
        	j++;
        	if(j>3)
    		{
    			j=0;
    			k++;
    		}
        }
    }
    
    for(i=0;i<pSDataRcd->datalen;i++)
    	temp=pSDataRcd->data[i];
    temp=pSDataRcd->addr;
    temp=pSDataRcd->datalen;
    
    pSDataRcd->status=LineDataEffect;
    
    if ( (checksum + Hex2Value(pLineData)) == 0xFF )  //У�鱨��
    {
        return SPASS;					//���У��ɹ����򷵻ر���У��ɹ���Ϣ
    }
    else
    {
        return SERR;					//���У�鲻�ɹ����򷵻ر���У�������Ϣ
    }
}
__declspec(section ".boot_flash")
//*****************************************************************
//������  : DoubleWordProgramSytleProcess()
//��  ��  : ���յ�S19����д��Flash
//�������: *pDataBuf	:�����ݴ滺����
//		  : *pLineRec	:�յ�����������Ϣ
//		  :	*pProDataBuf:��������ݴ滺����
//���ز���: ��
//*****************************************************************
void DoubleWordProgramSytleProcess(DataBuffer *pDataBuf,SFileRecord *pLineRec,ProgramDataBuffer *pProDataBuf)
{
	unsigned char cnts;
	uint32_t  checkData=0;
	
	if(pLineRec->status!=LineDataEnd)			//�жϵ�ǰһ�������Ƿ����
	{
		if(pDataBuf->status==DataBufferEmpty)	//�ж������ݴ滺�����Ƿ�Ϊ��
		{
			pLineRec->datalen--;				//�����ݸ�����һ
			if(pLineRec->datalen==0)			//�ж��������Ƿ��Ѿ�ȡ��
			{
				pDataBuf->addr=pLineRec->addr;	//��������Ѿ�ȡ�꣬�򽫵�ǰ���ݵ�ַ���浽�����ݴ滺����
				pDataBuf->data=pLineRec->data[0];//�����ݱ��浽�����ݴ滺����
				pDataBuf->status=DataBufferFull; //��λ�ݴ滺������������
				pLineRec->status=LineDataEnd;	 //��λ�����ݱ��滺���������Ѿ�����
				pProDataBuf->status=ProgramDataNotReady;//��λ��̻���������û��׼����
			}
			else
			{
				pProDataBuf->addr=pLineRec->addr;		//����ǰ�����ݵĵ�ַ���浽��������ݴ滺����
				pProDataBuf->data[0]=pLineRec->data[0];	//��Ҫ��̵����ݱ��浽��������ݴ滺����
				pProDataBuf->data[1]=pLineRec->data[1];	
				pProDataBuf->status=ProgramDataReady;	//��λ��������Ѿ�׼����
				pLineRec->datalen--;					//�����ݸ�����һ
				if(pLineRec->datalen==0)				//�ж��������Ƿ��Ѿ���̽���
				{
					pLineRec->status=LineDataEnd;		//�������꣬����λ�����ݱ�̽�����־
				}
				else
				{
					for(cnts=0;cnts<pLineRec->datalen;cnts++)	//������������
					{
						pLineRec->data[cnts]=pLineRec->data[cnts+2];
					}
					pLineRec->addr=pLineRec->addr+8;		//���ݵ�ַ��8
				}
			}
		}
		else
		{
			if((pDataBuf->addr+4)==pLineRec->addr)	  //��������ݴ滺������ַ���½������ݵ�ַһ��
			{
				pProDataBuf->addr=pDataBuf->addr;	  //�������ݴ滺�����ĵ�ַ���浽��������ݴ滺����
				pProDataBuf->data[0]=pDataBuf->data;  //�����������������ݴ滺����
				pProDataBuf->data[1]=pLineRec->data[0]; //���½��ܵ����ݵĵ�һ�����������������ݴ滺����
				pDataBuf->status=DataBufferEmpty;		//��λ����������������
				pProDataBuf->status=ProgramDataReady;   //��λ��������Ѿ�׼����
				pLineRec->datalen--;
				if(pLineRec->datalen==0)				//�ж��������Ƿ��Ѿ���̽���
				{
					pLineRec->status=LineDataEnd;		//�������꣬����λ�����ݱ�̽�����־
				}
				else
				{
					for(cnts=0;cnts<pLineRec->datalen;cnts++)	//������������
					{
						pLineRec->data[cnts]=pLineRec->data[cnts+1];
					}
					pLineRec->addr=pLineRec->addr+4;	//���ݵ�ַ��4
				}
			}
			else  //��������ݴ滺������ַ���½������ݵ�ַ��һ��
			{
				pProDataBuf->addr=pDataBuf->addr; 	//�������ݴ滺�����ĵ�ַ���浽��������ݴ滺����
				pProDataBuf->data[0]=pDataBuf->data;	//�����������������ݴ滺����
				pProDataBuf->data[1]=0x00000000;	//������ַ����0
				pProDataBuf->status=ProgramDataReady; //��λ��������Ѿ�׼����
				pDataBuf->status=DataBufferEmpty;   //��λ����������������
			}
		}
	}
	checkData = pProDataBuf->addr;
	checkData = pProDataBuf->data[0];
	checkData = pProDataBuf->data[1];
	if(pDataBuf->status==DataBufferFull)
	{
		checkData = pProDataBuf->addr;
		checkData = pDataBuf->data;		
	}
}
#pragma pop
