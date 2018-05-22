//***************************************************************** 
//* FILE        : S_File_Proc.h
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
#ifndef  S_FILE_PROC_H
#define  S_FILE_PROC_H

#include "MPC5645S.h"
#include "MPC55xx_PCRdef.h"
#include "FlashOperation.h"

/*****************************************************************/ 
/*******************  Marco define  ******************************/
/*****************************************************************/
#define  	SPASS       0		//��������У��ͨ��
#define  	SERR        1		//��������У�鲻ͨ��
#define  	SFileStart  2		//S�ļ����ݿ�ʼ��־
#define  	SFileEnd    3		//S�ļ����ݽ�����־
#define  	SInvalid    0xFF	//S�ļ���Ч

#define	 	AddressContinue		0//������ַ
#define		AddressBreak		1//��ַ������

#define		FileTransEnd		2 	//�ļ��������
#define		FileTransNotEnd		3	//�ļ�����δ����

#define		DataBufferEmpty		4	//���ݽ�������
#define		DataBufferFull		5	//���ݽ�������

#define		LineDataEnd			6	//�����ݽ���
#define		LineDataEffect		7	//������δ����

#define		ProgramDataReady	8	//�������׼����
#define		ProgramDataNotReady	9	//�������δ׼����



typedef struct 
{
    unsigned char	status;		//��ǰ����������״̬
    uint32_t 	addr;		//���ݱ�����ʼ��ַ
    unsigned short  datalen;	//���ݳ���
    uint32_t  data[5];	//���ݱ��滺����
} SFileRecord;

typedef struct 
{
    unsigned char	status;		//��ǰ���ݽ���������״̬
    uint32_t 	addr;		//���ݱ�����ʼ��ַ
    uint32_t  data;		//����
} DataBuffer;

typedef struct 
{
    unsigned char	status;		//��ǰ����������״̬
    uint32_t 	addr;		//���ݱ�����ʼ��ַ
    uint32_t  data[2];	//����
} ProgramDataBuffer;
/*****************************************************************/ 
/****************  Funtion declaration ***************************/
/*****************************************************************/

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
unsigned char S19DataConvert(unsigned char *OneLineDatum, SFileRecord *pSDataRcd);

//*****************************************************************
//������  : DoubleWordProgramSytleProcess()
//��  ��  : ���յ�S19����д��Flash
//�������: *pDataBuf	:�����ݴ滺����
//		  : *pLineRec	:�յ�����������Ϣ
//		  :	*pProDataBuf:��������ݴ滺����
//���ز���: ��
//*****************************************************************
void DoubleWordProgramSytleProcess(DataBuffer *pDataBuf,SFileRecord *pLineRec,ProgramDataBuffer *pProDataBuf);

#endif