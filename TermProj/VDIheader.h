//
// Created by csis on 1/28/20.
//

#ifndef TERMPROJ_VDIHEADER_H
#define TERMPROJ_VDIHEADER_H

#include <iostream>
struct UUID {
    uint32_t
            timeLow;
    uint16_t
            timeMid,
            timeHigh,
            clock;
    uint8_t
            node[6];
};

static char
        uuidStr[40];

struct vdiHeader {
    /** Just text info about image type, for eyes only. */
    char          szFileInfo[64];
    /** The image signature (VDI_IMAGE_SIGNATURE). */
    uint32_t        u32Signature;
    /** The image version (VDI_IMAGE_VERSION). */
    uint32_t        u32Version;
    /** Size of this structure in bytes. */
    uint32_t        cbHeader;
    /** The image type (VDI_IMAGE_TYPE_*). */
    uint32_t        u32Type;
    /** Image flags (VDI_IMAGE_FLAGS_*). */
    uint32_t        fFlags;
    /** Image comment. (UTF-8) */
    char            szComment[256];
    /** Offset of blocks array from the beginning of image file.
     * Should be sector-aligned for HDD access optimization. */
    uint32_t        offBlocks;
    /** Offset of image data from the beginning of image file.
     * Should be sector-aligned for HDD access optimization. */
    uint32_t        offData;
    /** Legacy image geometry (previous code stored PCHS there). */
    uint32_t LegacyGeometry;
    /** Was BIOS HDD translation mode, now unused. */
    uint32_t        u32Dummy;
    /** Size of disk (in bytes). */
    uint64_t        cbDisk;
    /** Block size. (For instance VDI_IMAGE_BLOCK_SIZE.) Should be a power of 2! */
    uint32_t        cbBlock;
    /** Size of additional service information of every data block.
     * Prepended before block data. May be 0.
     * Should be a power of 2 and sector-aligned for optimization reasons. */
    uint32_t        cbBlockExtra;
    /** Number of blocks. */
    uint32_t        cBlocks;
    /** Number of allocated blocks. */
    uint32_t        cBlocksAllocated;
    /** UUID of image. */
    char          uuidCreate[16];
    /** UUID of image's last modification. */
    char          uuidModify[16];
    /** Only for secondary images - UUID of previous image. */
    char          uuidLinkage[16];
    /** Only for secondary images - UUID of previous image's last modification. */
    char          uuidParentModify[16];
    /** LCHS image geometry (new field in VDI1.2 version. */
    //VDIDISKGEOMETRY LCHSGeometry;


};

#endif //TERMPROJ_VDIHEADER_H
