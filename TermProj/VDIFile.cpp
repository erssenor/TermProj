//
// Created by csis on 1/25/20.
//

#include <unistd.h>
#include "VDIFile.h"

bool VDIFile::vdiOpen() {
    header = new vdiHeader;

    fd = open("/home/csis/Documents/Operating Systems/Test-fixed-1k.vdi", O_RDWR);
    if(fd == -1) {
        return false;
    }
    cursor = 0;

    lseek(fd, 0, SEEK_SET);
    read(fd, header, 400);



    return true;
}

void VDIFile::vdiClose() {
    close(fd);
}

size_t VDIFile::vdiRead(void *buf, size_t count) {

    size_t bytesToRead = count;

    int numRead;

    //keeps track where we are in buffer
    int curLoc = 0;

    long long int transCur;

    uint16_t offset;

    while(bytesToRead > 0) {
    //translate cursor

        lseek(fd, transCur + header->offData, SEEK_SET);
        numRead = read(fd, buf, header->cbBlock);
        offset += numRead;
        curLoc += numRead;
        bytesToRead -= numRead;
      }


}

size_t VDIFile::vdiWrite(void *buf, size_t count) {  }


int VDIFile::vdiSeek(uint64_t offset, int anchor) {
    
    int newLoc;
    
    if(anchor == SEEK_SET) {
        newLoc = offset;
    }
    else if(anchor == SEEK_CUR) {
        newLoc = cursor + offset;
    }
    else if(anchor == SEEK_END) {
        cursor = header->cbDisk + offset;
    }
    else {
        newLoc = cursor;
     }
    
    if(newLoc < 0 || newLoc >= header->cbDisk) {
        cursor = newLoc;
    }

    return newLoc;

}
