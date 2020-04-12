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

    //read header.
    lseek(fd, 0, SEEK_SET);
    read(fd, header, 400);

    //read translation map
    map = new int[header->cBlocks];
    lseek(fd, header->offBlocks, SEEK_SET);
    read(fd, map, header->cBlocks);

    return true;
}

void VDIFile::vdiClose() {
    close(fd);
    delete header;
}

size_t VDIFile::vdiRead(void *buf, size_t count) {

    size_t bytesToRead = count;

    int numRead = 0;
    int totalRead = 0;


    int vpage;
    int ppage;
    int bytesInPage;

    uint16_t offset;
    if(bytesToRead + cursor > header->cbDisk){
        bytesToRead = header->cbDisk - cursor;
    }

    while(bytesToRead > 0) {
        //cout << "bytes to read " << bytesToRead << endl;
        vpage = cursor/header->cbBlock;
        offset = cursor % header->cbBlock;
        bytesInPage = header->cbBlock - offset;

        ppage = map[vpage];
        if(ppage < 0) {

            memset((char *)buf + totalRead, 0, bytesInPage);
            numRead = bytesInPage;
        }
        else {
            if(bytesToRead < bytesInPage) {
                bytesInPage = bytesToRead;
            }

            lseek(fd, ppage * header->cbBlock + offset + header->offData, SEEK_SET);
            numRead = read(fd, (char *)buf + totalRead, bytesInPage);

        }

        totalRead += numRead;
        cursor += numRead;

        bytesToRead -= numRead;
        //cout << numRead << endl;
    }

    return totalRead;


}

size_t VDIFile::vdiWrite(void *buf, size_t count) {
    size_t bytesToWrite = count;

    int numWrite;
    int totalWrite = 0;


    int vpage;
    int ppage;
    int bytesInPage;

    uint16_t offset;

    if(bytesToWrite + cursor > header->cbDisk){
        bytesToWrite = header->cbDisk - cursor;
    }

    while(bytesToWrite > 0) {

        vpage = cursor/header->cbBlock;
        offset = cursor % header->cbBlock;
        bytesInPage = header->cbBlock - offset;

        ppage = map[vpage];
        if(ppage < 0) {
            /*
             *  for writing:
             *  - *done* use lseek to go to end of file
             *  - *done* create an char array of size cbBlock, set all entries to zero
             *  - *done* write the array out to the file
             *  - *done* set map[vpage] = cBlocksAllocated
             *  - *done* then, set ppage = cBlocksAllocated
             *  - *done* increment cBlocksAllocated
             *  - *done* write the header and the map back to the file
             *  - *done*there is no else below; if ppage is < 0, do these things and then also do the writing below
             */
            lseek(fd, header->offData, SEEK_END);

            char zeroArray[header->cbBlock];
            memset(zeroArray, 0, header->cbBlock);

            write(fd, zeroArray, bytesInPage);

            map[vpage] = header->cBlocksAllocated;
            ppage = header->cBlocksAllocated;

            header->cBlocksAllocated++;

            write(fd, header, 400);

            memset((char *)buf + totalWrite, 0, bytesInPage);
            numWrite = bytesInPage;
        }


        lseek(fd, ppage * header->cbBlock + offset + header->offData, SEEK_SET);
        numWrite = write(fd, (char *)buf + totalWrite, bytesInPage);


        totalWrite += numWrite;
        cursor += numWrite;

        bytesToWrite -= numWrite;
    }

    return totalWrite;


}


int VDIFile::vdiSeek(uint64_t offset, int anchor) {
    
    int64_t newLoc;
    
    if(anchor == SEEK_SET) {
        newLoc = offset;
    }
    else if(anchor == SEEK_CUR) {
        newLoc = cursor + offset;
    }
    else if(anchor == SEEK_END) {
        newLoc = header->cbDisk + offset;
    }
    else {
        newLoc = cursor;
     }
    
    if(newLoc >= 0 && newLoc < header->cbDisk) {
        cursor = newLoc;
    }


    return cursor;

}
