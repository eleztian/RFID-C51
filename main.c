#include <reg52.h>
#include <stdio.h>
#include <string.h>
#include "bsp_sd.h"
#include "bsp_rc522.h"
#include "main.h"

#define ADDR_SD (512)

#define updateID() SdWriteBlock(ADDR_SD,DATA,512) 
#define reloadID() SdReadBlock(DATA,ADDR_SD,512)

////SD ?    512??2???????			512*4=1024 ???????		

///**
//???????:
//	???1?
//	???????1?
//	?????2?,????2?
//	?????3?,????3?
//	??k35s ??????,??????,??????4?

//*/
unsigned char g_ucTempbuf[4];								//???????
unsigned char flag = 0;
unsigned char num = 0;
void FM10ms(unsigned char i){
	while(i>0){
	FM = 0;
	delay_10ms(8);
	FM = 1;
	delay_10ms(8);
	i--;
	}

}
void Timer0Init(void)		//???0???;
{
	TMOD|=0x01;
	TH0 = (65536-50000)/256;
	TL0 = (65536-50000)%256;
	EA = 1;
	ET0 = 1;
	TR0 = 0;
}
bit checkExist(unsigned char* id){
	int i=0,j=0;
	for(i=0;i<512;i+=4){
		if (strncmp(id,DATA+i,4) == 0)
			return 1;
	}
	return 0;
}
bit deletAllUser(){
	int i;
	for(i=0;i<512;i++){
		DATA[i] = 0;
	}
    updateID();
    delay_10ms(100);
    reloadID();
	return 1;
}

void addUser(unsigned char* admin){
	 unsigned char status,zro[]={0,0,0,0};
	 unsigned int i,j,flag = 0;
	 for(i=0;i<4;i++)
			g_ucTempbuf[i]=0;
	 while(1){
        status = PcdRequest(PICC_REQALL, g_ucTempbuf);
        if (status != MI_OK)
        {
            continue;
        }
        status = PcdAnticoll(g_ucTempbuf);
        if (status != MI_OK)
        {
            continue;
        }
        FM10ms(1);
        for(i=0;i<4;i++){
            SBUF=g_ucTempbuf[i];
            while(!TI);
            TI=0;
            delay_10ms(1);
        }
            
        if(strncmp(admin,g_ucTempbuf,4)==0)
            break;

        flag = 0;
        for(i=0;i<512;i+=4){
            if(strncmp(DATA+i,g_ucTempbuf,4)==0){				//????????  ????1?,???????;
                FM10ms(1);
                flag = 1;
                break;
            }
        }
        if(flag == 1)
            continue;
        
        for(i=0;i<512;i+=4){
            if(strncmp(DATA+i,zro,4)==0){
                for(j=0;j<4;j++){
                    DATA[i+j] = g_ucTempbuf[j];
                }
                break;
            }
        }
        delay_10ms(20);
    }
    updateID();
    delay_10ms(100);
    reloadID();
}
void deletUser(unsigned char *admin){
	 unsigned char status;
	 unsigned int i,j;
	 for(i=0;i<4;i++)
        g_ucTempbuf[i]=0;
	 while(1){
         status = PcdRequest(PICC_REQALL, g_ucTempbuf);
         if (status != MI_OK)
         {
            continue;
         }
         status = PcdAnticoll(g_ucTempbuf);
         if (status != MI_OK)
         {
            continue;    
         }
         FM10ms(1);/*
         for(i=0;i<4;i++){
                SBUF=g_ucTempbuf[i];
                while(!TI);
                TI=0;
                delay_10ms(1);
            }*/
            delay_10ms(20);
            if(strncmp(admin,g_ucTempbuf,4)==0)
                break;
            for(i=0;i<512;i+=4){											//????,??????????id????
                if(strncmp(DATA+i,g_ucTempbuf,4)==0){
                    for(j=0;j<4;j++){
                        DATA[i+j] = 0;
                    }
                }
            }
        }
        updateID();
        delay_10ms(100);
        reloadID();
}

void main()
{   
    unsigned char status;
    unsigned char admin[8] = {0x32, 0xDB, 0x96, 0x75};									//???????
    unsigned char old_id[4]={0,0,0,0};
    unsigned int i;
    OPENDOOR = 1;
    
    initbaud();
    Timer0Init();
    SdInit();
    PcdReset();
    PcdAntennaOff(); 
    delay_10ms(1);
    PcdAntennaOn();  
    M500PcdConfigISOType( 'A' );
    
    //deletAllUser();
    /////////////////////////////////////////////////////////////////////////
    for(i=0;i<512;i++){
        DATA[i] = 0;
    }
    reloadID();													//????????? ???  512
    delay_10ms(10);

    for(i=0;i<512;i++){
        SBUF=DATA[i];
        while(!TI);
        TI=0;
        delay_10ms(1);
    }

    FM10ms(5);														//???????  ????5?
    //////////////////////////////////////////////////////////////////

    while ( 1 )
    { 

        status = PcdRequest(PICC_REQALL, g_ucTempbuf);	//??
        if (status != MI_OK)
        {
            continue;
        }
        status = PcdAnticoll(g_ucTempbuf);							//???,????
        if (status != MI_OK)
        {
            continue;
        }
        if(strncmp(old_id,g_ucTempbuf,4)==0&&flag==1){				//5s??????????
            continue;
        }
        else{
            flag = 1;
            TR0 = 0;
            TR0 = 1;
        }
        FM10ms(1);

        for(i=0;i<4;i++){
            SBUF=g_ucTempbuf[i];
            while(!TI);
            TI=0;
            delay_10ms(1);
        }

        if(strncmp(admin,g_ucTempbuf,4)==0){						//????
            OPENDOOR = 0;
            delay_10ms(50);
            OPENDOOR = 1;
            FM10ms(2);
            addUser(admin);
            FM10ms(2);
        }
        // 二号管理员
        else if(strncmp(admin+4,g_ucTempbuf,4)==0){						//????
            OPENDOOR = 0;
            delay_10ms(50);
            OPENDOOR = 1;
            FM10ms(2);
            addUser(admin+4);
            FM10ms(2);
        }
        
//        else if(strncmp(admin+4,g_ucTempbuf,4)==0){		//????
//            if(DELETEALL==0){
//                delay_10ms(500);												//??K3??????????5S???????
//                if(DELETEALL==0){
//                    deletAllUser();
//                    FM10ms(4);
//                    continue;
//                }
//            }
//            FM10ms(3);
//            deletUser(admin+4);
//            FM10ms(3);
//        }
        else{																					//????
            if(checkExist(g_ucTempbuf)){
                OPENDOOR = 0;
                delay_10ms(50);
                OPENDOOR = 1;
            }
            else{
                FM10ms(1);
            }
            delay_10ms(100);
        }

    PcdHalt();				//??ic?
    } /* while */
		
}


void time0() interrupt 1{
	TH0 = (65536-50000)/256;
	TL0 = (65536-50000)%256;
	num++;
	if(num == 100){
		flag = 0;
		num = 0;
		TR0 = 0;
	}
}



//void main()
//{
//    unsigned int i;
//    unsigned long AddTemp=0;//SD???????????????,???winhex??,???641????,512x641=328192,????SD?????
//    delayms(5);
//    SdInit();         //SD????
//    for(i=0;i<512;i++)
//    {
//        DATA[i] = i;
//    }
//    while(1)
//    {
//        updateID();
//        delayms(1000);
//        for(i=0;i<512;i++)
//        {
//            DATA[i] = 0;
//        }
//        AddTemp=0;
//        reloadID();//????512???????
//        initbaud();
//        for(i=0;i<512;i++)
//        {
//            SBUF=DATA[i];
//            while(!TI);
//            TI=0;
//            delayms(1);
//        }
//        while(1);                         //??????????
//    }
//}