
#ifndef __SDCard_H__
#define __SDCard_H__

//=============================================================
//����SD����Ҫ��4���ź���
sbit SD_CLK = P3^7;
sbit SD_DI  = P3^5;
sbit SD_DO  = P3^6;
sbit SD_CS  = P3^4;
//===========================================================
//��д���ݱ������
extern unsigned char xdata DATA[512];
//===========================================================
//��������
void delayms(unsigned int count);

unsigned char SdInit(void);
unsigned char SdWriteBlock(unsigned long address, unsigned char *Block, int len);
unsigned char SdReadBlock(unsigned char *Block, unsigned long address,int len);
void initbaud(void);


#endif //__SDCard_H__