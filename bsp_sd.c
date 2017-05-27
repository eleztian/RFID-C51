#include <reg52.h>
#include <stdio.h>
#include "bsp_sd.h"
unsigned char xdata DATA[512];

void delayms(unsigned int count)
{
    int i,j;                                                                               
    for(i=0;i<count;i++)                                                                   
       {
      for(j=0;j<260;j++);
       }                                                                                    
}
//===========================================================
//?????SD?,??SPI????
void SdWrite(unsigned char n)
{

    unsigned char i;

    for(i=8;i;i--)
    {
    SD_CLK=0;
    SD_DI=(n&0x80);
    n<<=1;
    SD_CLK=1;
    }
    SD_DI=1;
}
//===========================================================
//?SD?????,??SPI????
unsigned char SdRead()
{
    unsigned char n,i;
    for(i=8;i;i--)
    {
        SD_CLK=0;
        SD_CLK=1;
        n<<=1;
        if(SD_DO) n|=1;

    }
    return n;
}
//============================================================
//??SD????
unsigned char SdResponse()
{
    unsigned char i=0,response;

    while(i<=8)
    {
    response = SdRead();
    if(response==0x00)
    break;
    if(response==0x01)
    break;
    i++;
    }
    return response;
}
//================================================================
//????SD?
void SdCommand(unsigned char command, unsigned long argument, unsigned char CRC)
{

    SdWrite(command|0x40);
    SdWrite(((unsigned char *)&argument)[0]);
    SdWrite(((unsigned char *)&argument)[1]);
    SdWrite(((unsigned char *)&argument)[2]);
    SdWrite(((unsigned char *)&argument)[3]);
    SdWrite(CRC);
}
//================================================================
//???SD?
unsigned char SdInit(void)
{
    int delay=0, trials=0;
    unsigned char i;
    unsigned char response=0x01;

    SD_CS=1;
    for(i=0;i<=9;i++)
    SdWrite(0xff);
    SD_CS=0;

    //Send Command 0 to put MMC in SPI mode
    SdCommand(0x00,0,0x95);


    response=SdResponse();

    if(response!=0x01)
    {
        return 0;
    }

    while(response==0x01)
    {
        SD_CS=1;
        SdWrite(0xff);
        SD_CS=0;
        SdCommand(0x01,0x00ffc000,0xff);
        response=SdResponse();
    }

    SD_CS=1;
    SdWrite(0xff);
    return 1;
}
//================================================================
//?SD????????,????512??
//unsigned char SdWriteBlock(unsigned char *Block, unsigned long address,int len)
unsigned char SdWriteBlock(unsigned long address, unsigned char *Block, int len)
{
    unsigned int count;
    unsigned char dataResp;
    //Block size is 512 bytes exactly
    //First Lower SS

    SD_CS=0;
    //Then send write command
    SdCommand(0x18,address,0xff);

    if(SdResponse()==00)
    {
        SdWrite(0xff);
        SdWrite(0xff);
        SdWrite(0xff);
        //command was a success - now send data
        //start with DATA TOKEN = 0xFE
        SdWrite(0xfe);
        
        //for(count=0;count<1;count++) SdRead();
        
        //now send data
        for(count=0;count<len;count++) SdWrite(*Block++);
        //for(count=0;count<len;count++) SdWrite(count);
        //for(count=0;count<len;count++) SdWrite(0xFF);
        for(;count<512;count++) SdWrite(0);
        //data block sent - now send checksum
        SdWrite(0xff); //???CRC??, ?0XFFFF ?????CRC
        SdWrite(0xff);
        //Now read in the DATA RESPONSE token
        dataResp=SdRead();
        //Following the DATA RESPONSE token
        //are a number of BUSY bytes
        //a zero byte indicates the MMC is busy

        while(SdRead()==0);

        dataResp=dataResp&0x0f; //mask the high byte of the DATA RESPONSE token
        SD_CS=1;
        SdWrite(0xff);
        if(dataResp==0x0b)
        {
            //printf("DATA WAS NOT ACCEPTED BY CARD -- CRC ERROR\n");
            return 0;
        }
        if(dataResp==0x05)
            return 1;

        //printf("Invalid data Response token.\n");
        return 0;
    }
    //printf("Command 0x18 (Write) was not received by the MMC.\n");
    return 0;
}

//=======================================================================
//?SD?????????,????512??
unsigned char SdReadBlock(unsigned char *Block, unsigned long address,int len)
{
    unsigned int count;
    //Block size is 512 bytes exactly
    //First Lower SS

     //printf("MMC_read_block\n");

    SD_CS=0;
    //Then send write command
    SdCommand(0x11,address,0xff);

    if(SdResponse()==00)
    {
        //command was a success - now send data
        //start with DATA TOKEN = 0xFE
        while(SdRead()!=0xfe);

        for(count=0;count<len;count++) *Block++=SdRead();

        for(;count<512;count++) SdRead();

        //data block sent - now send checksum
        SdRead();
        SdRead();
        //Now read in the DATA RESPONSE token
        SD_CS=1;
        SdRead();
        return 1;
    }
     //printf("Command 0x11 (Read) was not received by the MMC.\n");
    return 0;
}

void initbaud(void)
{
    TMOD=0X20;
    TH1=0XFD;
    TL1=0XFD;
    PCON=0X00;
    TR1=1;
    SCON=0X50;//8?????
    //SCON=0X52;//8????? TI???
}
//============================================================
//??? 
