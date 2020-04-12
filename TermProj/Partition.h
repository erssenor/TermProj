//
// Created by csis on 2/19/20.
//

#ifndef TERMPROJ_PARTITION_H
#define TERMPROJ_PARTITION_H

#include <cstdint>
#include "VDIFile.h"

struct partEntry {
    uint8_t status;
    uint8_t chsFirst[3];
    uint8_t type;
    uint8_t chsLast[3];

    uint32_t firstSector;
    uint32_t nSectors;
};

class partTable {
public:
    VDIFile vdi;
    uint32_t pStart;
    uint32_t pSize;
    partEntry *tableEntries;

    void partOpen(VDIFile f, partEntry p[]);

    void partClose();

    ssize_t partRead(void *buf, ssize_t count);

    ssize_t partWrite(void *buf, ssize_t count);

    ssize_t partSeek(uint64_t offset, int anchor);
};

#endif //TERMPROJ_PARTITION_H
