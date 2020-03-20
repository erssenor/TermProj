//
// Created by csis on 1/25/20.
//

#ifndef TERMPROJ_VDIFILE_H
#define TERMPROJ_VDIFILE_H


#include <cstddef>
#include <cstdint>
#include "VDIheader.h"
#include <fcntl.h>
#include <iostream>
#include <cstring>

using namespace std;



class VDIFile {
public:
    vdiHeader *header;

    int fd;

    long long int cursor;

    int *map;

    bool vdiOpen();

    void vdiClose();

    size_t vdiRead(void *buf, size_t count);

    size_t vdiWrite(void *buf, size_t count);

    int vdiSeek(uint64_t offset, int anchor);


};


#endif //TERMPROJ_VDIFILE_H
