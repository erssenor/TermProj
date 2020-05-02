
#ifndef TERMPROJ_INODE_H
#define TERMPROJ_INODE_H

//#include <cstdint>
//#include "VDIFile.h"
//#include "Partition.h"
#include "VDIFile.h"
#include "Partition.h"
#include <iomanip>
#include <cmath>
#include "ext2.h"

/*
struct inode{

    uint16_t
            i_mode,
            i_uid;
    uint32_t
            i_size,
            i_atime,
            i_ctime,
            i_mtime,
            i_dtime;
    uint16_t
            i_gid,
            i_links_count;
    uint32_t
            i_blocks,
            i_flags,
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
            i_osd1,
#pragma clang diagnostic pop
            i_block[15],
            i_generation,
            i_file_acl,
            i_sizeHigh,
           //
           //i_dir_acl,
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
            i_faddr;
#pragma clang diagnostic pop
    uint16_t
            i_blocksHigh,
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
            reserved16,
#pragma clang diagnostic pop
            i_uidHigh,
            i_gidHigh;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
    uint32_t
            reserved32;

#pragma clang diagnostic pop

};*/

struct Inode {
    uint16_t
            i_mode,
            i_uid;
    uint32_t
            i_size,
            i_atime,
            i_ctime,
            i_mtime,
            i_dtime;
    uint16_t
            i_gid,
            i_links_count;
    uint32_t
            i_blocks,
            i_flags,
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
            i_osd1,
#pragma clang diagnostic pop
            i_block[15],
            i_generation,
            i_file_acl,
            i_sizeHigh,
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
            i_faddr;
#pragma clang diagnostic pop
    uint16_t
            i_blocksHigh,
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
            reserved16,
#pragma clang diagnostic pop
            i_uidHigh,
            i_gidHigh;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
    uint32_t
            reserved32;
#pragma clang diagnostic pop
};

class inodeFile{
public:

    partTable pTable;
    partEntry *p;
    //Inode *buf;

    int32_t fetchInode(ext2File f, uint32_t iNum, struct Inode *buf);

    uint32_t writeInode(ext2File f, int32_t iNum, struct Inode *buf);

    int32_t inodeInUse(ext2File f, uint32_t iNum);

    int32_t allocateInode(ext2File f, uint32_t group);

    int32_t freeInode(ext2File f, uint32_t iNum);

};


#endif //TERMPROJ_INODE_H