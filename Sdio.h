#ifndef __SDIO_H_
#define __SDIO_H_

#include "bsp_sd.h"

typedef SdInit() SD_Init();
typedef SdWriteBlock(address, Block, len) \
        SD_Write(address,Block, len);
typedef SdReadBlock(Block, address,len) \
        SD_Read(Block,address,len);
            

#endif /*__SDIO_H_*/
