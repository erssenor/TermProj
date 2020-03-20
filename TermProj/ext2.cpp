//
// Created by csis on 3/2/20.
//

#include "ext2.h"

void ext2File::ext2Open(char *fn, uint32_t pnum) {
    //vdi = new VDIFile;
    vdi.vdiOpen();
    //pTable.partOpen(vdi, )
}

void ext2File::ext2Close() {

}

uint32_t ext2File::fetchBlock(uint32_t blockNum, void *buf){

}

uint32_t ext2File::writeBlock(uint32_t blockNum, void *buf){

}

uint32_t ext2File::fetchSuperBlock(uint32_t blockNum, superBlock *sb){

}

uint32_t ext2File::writeSuperBlock(uint32_t blockNum, superBlock *sb){

}

uint32_t ext2File::fetchBGDT(uint32_t blockNum, blockGDT *bgdt){

}

uint32_t ext2File::writeBGDT(uint32_t blockNum, blockGDT *bgdt){
    
}