//
// Created by csis on 2/19/20.
//

#include <unistd.h>
#include "Partition.h"

void partTable::partOpen(VDIFile f, partEntry p[]) {
    vdi = f;
    pSize = 64;
    pStart = 446;
    tableEntries = new partEntry[4];
    for(int i = 0; i < 4; i++)
        tableEntries[i] = p[i];



}

void partTable::partClose() {
    delete tableEntries;
    vdi.vdiClose();
}

ssize_t partTable::partRead(void *buf, ssize_t count) {

    int numRead = 0;
    if(count > pSize) {
        count = pSize;
    }
    vdi.vdiSeek(446, SEEK_SET);
    numRead = vdi.vdiRead(buf, count);

    return numRead;

}

ssize_t partTable::partWrite(void *buf, ssize_t count) {

    int numWrite = 0;

    if(count > pSize) {
        count = pSize;
    }

    vdi.vdiSeek(446, SEEK_SET);
    numWrite = vdi.vdiWrite(buf, count);

    return numWrite;
}

ssize_t partTable::partSeek(uint64_t offset, int anchor) {

    uint64_t pOff = offset + pStart;

    if(pOff > pStart + pSize) {
        return vdi.cursor;
    }
    vdi.cursor = vdi.vdiSeek(pOff, anchor);

    return vdi.cursor;
}

