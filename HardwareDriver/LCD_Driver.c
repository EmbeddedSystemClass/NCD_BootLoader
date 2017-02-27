/***************************************************************************************************
*FileName��LCD_Driver
*Description��Һ������һЩ���ܺ���
*Author��xsx
*Data��2016��4��29��10:13:01
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/

#include	"LCD_Driver.h"
#include 	"Usart6_Driver.h"

#include	"CRC16.h"
#include	"Define.h"
#include	"MyMem.h"

#include	"stdio.h"
#include	"string.h"

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static char *txdat = NULL;
static unsigned char tempbuf[100];
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static void WriteLCDRegister(unsigned char reg, void *data, unsigned char len);
static void ReadLCDRegister(unsigned char reg, unsigned char len);
static void WriteLCDData(unsigned short addr, void *data, unsigned char len);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName: WriteRegister
*Description: д�Ĵ���
*Input: reg -- �Ĵ�����ַ
*		data -- д������
*		datalen -- д�����ݵĳ���
*Output: ��
*Author: xsx
*Date: 2016��8��5��15:18:01
***************************************************************************************************/
static void WriteLCDRegister(unsigned char reg, void *data, unsigned char len)
{			
	char *q = NULL;
	unsigned char *p = (unsigned char *)data;
	unsigned char i=0;
	
	txdat = MyMalloc(len + 10);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, len + 10);
	q = txdat;
	
	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = len+4;
	
	*q++ = W_REGSITER;
	
	*q++ = reg;

	for(i=0; i<len; i++)
		*q++ = *p++;
	
	CalModbusCRC16Fun2(txdat+3, len + 2, q);
	
	USART6_SentData(txdat, txdat[2]+3);

	MyFree(txdat);
}

/***************************************************************************************************
*FunctionName: ReadLCDRegister
*Description: ��ȡ��Ļ�Ĵ���ֵ
*Input: reg -- �Ĵ�����ַ
*		len -- ��ȡ����
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��12��11:01:48
***************************************************************************************************/
static void ReadLCDRegister(unsigned char reg, unsigned char len)
{			
	char *q = NULL;
	
	txdat = MyMalloc(16);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, 16);
	q = txdat;
	
	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = 1 + 4;
	
	*q++ = R_REGSITER;
	
	*q++ = reg;

	*q++ = len;
	
	CalModbusCRC16Fun2(txdat+3, 1 + 2, q);
	
	USART6_SentData(txdat, txdat[2]+3);
	
	MyFree(txdat);
}

static void WriteLCDData(unsigned short addr, void *data, unsigned char len)
{
	char *q = NULL;
	unsigned char *p = (unsigned char *)data;
	unsigned char i=0;
	
	txdat = MyMalloc(len + 10);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, len + 10);
	q = txdat;
	
	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = len+5;

	*q++ = W_ADDRESS;
	
	*q++ = addr>>8;
	*q++ = addr;

	for(i=0; i<len; i++)
		*q++ = *p++;
	
	CalModbusCRC16Fun2(txdat+3, len + 3, q);
	
	USART6_SentData(txdat, txdat[2]+3);

	MyFree(txdat);
}

/***************************************************************************************************
*FunctionName��SelectPage
*Description��ѡ����ʾ��ͼƬid
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��30��17:02:39
***************************************************************************************************/
void SelectPage(unsigned short index)
{
	tempbuf[0] = index >> 8;
	tempbuf[1] = index;
	WriteLCDRegister(0x03, tempbuf, 2);
}

/***************************************************************************************************
*FunctionName: ReadCurrentPageId
*Description: ��ȡ��ǰҳ��id
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��12��11:02:58
***************************************************************************************************/
void ReadCurrentPageId(void)
{
	ReadLCDRegister(0x03, 2);
}

/***************************************************************************************************
*FunctionName: ReadLcdSoftVersion
*Description: ��ȡ��Ļ�������汾
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��19��14:40:12
***************************************************************************************************/
void ReadLcdSoftVersion(void)
{
	ReadLCDRegister(0x00, 1);
}

/***************************************************************************************************
*FunctionName��SendKeyCode
*Description�����ع���
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��1��16:46:15
***************************************************************************************************/
void SendKeyCode(unsigned char keycode)
{
	WriteLCDRegister(0x4f, &keycode, 1);
}

/************************************************************************
** ������:SetLEDLight
** ��  ��:num -- ��Ļ���ȣ�1 -- 1%�� 100--100%
** ��  ��:��
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��:  
** ��  �ߣ�xsx                                                 
************************************************************************/
void SetLEDLight(unsigned char num)
{
	WriteLCDRegister(0x01, &num, 1);
}
/***************************************************************************************************
*FunctionName��DisText
*Description����ʾ�ı�
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��30��17:02:53
***************************************************************************************************/
void DisText(unsigned short addr, void *data, unsigned short len)
{				
	WriteLCDData(addr, data, len);
}

void ClearText(unsigned short addr, unsigned short len)
{	
	memset(tempbuf, 0xff, len);
	DisText(addr, tempbuf, len);
}

void WriteRadioData(unsigned short addr, void *data, unsigned short len)
{
	WriteLCDData(addr, data, len);
}

void BasicUI(unsigned short addr , unsigned short cmd, unsigned short datapacknum, void *data , unsigned short len)
{
	unsigned char *p = (unsigned char *)data;
	unsigned char i=0;

	//����ͼ��ָ��
	tempbuf[0] = cmd>>8;		tempbuf[1] = cmd;
	//����ͼ�����ݰ�����
	tempbuf[2] = datapacknum>>8;		tempbuf[3] = datapacknum;
	
	//����keil�ǵ��ֽ���ǰ��������������������Ҫ���ֽ���ǰ�����������ｫ�ߵͻ���
	for(i=0; i<len/2; i++)
	{
		tempbuf[4+i*2] = p[i*2+1];
		tempbuf[5+i*2] = p[i*2];
	}
	
	WriteLCDData(addr, tempbuf, len+4);
}

/***************************************************************************************************
*FunctionName��BasicPic
*Description������ͼ��
*Input��addr -- �ؼ���ַ   �� datanum -- ���ݰ�����  �� soureid -- ͼƬԴҳ��id��  
*Output��None
*Author��xsx
*Data��
***************************************************************************************************/
void BasicPic(unsigned short addr,unsigned short datanum, unsigned short soureid ,unsigned short xs,unsigned short ys ,unsigned short xe,unsigned short ye ,
	unsigned short txs,unsigned short tys)
{
	//cmd 0x0006
	tempbuf[0] = 0;		tempbuf[1] = 6;
		
	//���ݰ�����1
	tempbuf[2] = datanum>>8;		tempbuf[3] = datanum;
		
	//ԭҳ��id
	tempbuf[4] = soureid>>8;	tempbuf[5] = soureid;
	//ԭҳ��x
	tempbuf[6] = xs>>8;	tempbuf[7] = xs;
	//ԭҳ��x
	tempbuf[8] = ys>>8;	tempbuf[9] = ys;
	//ԭҳ��x
	tempbuf[10] = xe>>8;	tempbuf[11] = xe;
	//ԭҳ��x
	tempbuf[12] = ye>>8;	tempbuf[13] = ye;
	//ԭҳ��x
	tempbuf[14] = txs>>8;	tempbuf[15] = txs;
	//ԭҳ��x
	tempbuf[16] = tys>>8;	tempbuf[17] = tys;

	WriteLCDData(addr, tempbuf, 18);
}



/************************************************************************
** ������:GetBufLen
** ��  ��:���������ص��ַ�����0xff��β
** ��  ��:��
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��:  
** ��  �ߣ�xsx                                                 
************************************************************************/
unsigned short GetBufLen(unsigned char *p ,unsigned short len)
{
	unsigned short i ;
	for(i=0; i<len; i++)
			{
				if(p[i] == 0xff)
				{
					return i;
				}
			}
		return 0;
}

void DspNum(unsigned short addr , unsigned int num, unsigned char len)
{
	unsigned char i=0;
	for(i=0; i<len; i++)
	{
		tempbuf[i] = (num >> ((len-i-1)*8));
	}

	WriteLCDData(addr, tempbuf, len);
}

void WriteVarIcoNum(unsigned short addr, unsigned short num)
{
	tempbuf[0] = (unsigned char)(num >> 8);
	tempbuf[1] = (unsigned char)num ;

	WriteLCDData(addr, tempbuf, 2);
}
	

