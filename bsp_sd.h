
#ifndef __SDCard_H__
#define __SDCard_H__

//=============================================================
//定义SD卡需要的4根信号线
sbit SD_CLK = P3^7;
sbit SD_DI  = P3^5;
sbit SD_DO  = P3^6;
sbit SD_CS  = P3^4;
//===========================================================
//读写数据保存对象
extern unsigned char xdata DATA[512];
//===========================================================
//函数声明
void delayms(unsigned int count);

unsigned char SdInit(void);
unsigned char SdWriteBlock(unsigned long address, unsigned char *Block, int len);
unsigned char SdReadBlock(unsigned char *Block, unsigned long address,int len);
void initbaud(void);


#endif //__SDCard_H__