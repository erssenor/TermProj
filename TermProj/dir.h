//
// Created by csis on 4/29/20.
//

#ifndef TERMPROJ_DIR_H
#define TERMPROJ_DIR_H

#include <cstdint>
#include "ext2.h"

struct dirent {
    uint32_t iNum;

    uint16_t recLen;

    uint8_t
        nameLen,
        fileType,
        name[1];
};

struct directory {
    uint32_t cursor;
    uint8_t *block;
    uint32_t iNodeNum;
    inode *i;
    uint8_t *pointer;
    dirent *dir;

};

directory *openDir(ext2File *ext, uint32_t iNum) {
    directory *d = new directory;
    d->block = new uint8_t [ext->blockSize];
    fetchInode(ext, iNum, d->i);
    d->iNodeNum = iNum;

    return d;
}

bool getNextDirent(ext2File ext, directory *d, uint32_t &iNum, char *name) {
    while(d->cursor < d->i->i_size) {
        int blockNum = d->cursor / ext.blockSize;
        int offset = d->cursor % ext.blockSize;
        ext.fetchBlockFromFile( d->i, blockNum, d->block);
        d->dir = (dirent*)(d->block + offset);
        if (d->dir->iNum != 0) {
            iNum = d->dir->iNum;
            for(int i = 0; i < d->dir->nameLen; i++) {
                name[i] = d->dir->name[i];
            }
            name[d->dir->nameLen] = 0;
        }
    }
    return false;
}

void rewindDir(directory *d) {
    d->cursor = 0;
}

void closeDir(directory *d) {
    delete(d);
}

uint32_t searchDir(ext2File *f, uint32_t iNum, char *target){
    return 0;
}

uint32_t traversePath(ext2File *f, char *path) {
    int start = 1;
    int len = strlen(path);
    int iNum = 2;

    while(start < len && iNum != 0) {
        int end = start + 1;
        while(path[end] != 0 && path[end] != '/') {
            end++;
        }
        path[end] = 0;
        iNum = searchDir(f, iNum, path + start);
        start = end + 1;
    }

    return iNum;
}

#endif //TERMPROJ_DIR_H
