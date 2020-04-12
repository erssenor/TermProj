#include <iostream>
#include <string>
#include <iomanip>
#include <fcntl.h>
#include <unistd.h>
#include "VDIFile.h"
#include "VDIheader.h"
#include "Partition.h"
#include "ext2.h"

using namespace std;

void displayBufPage(uint8_t *buf, uint32_t count, uint32_t start, uint64_t offset);
void displayBuf(uint8_t *buf, uint32_t count, uint64_t offset);
void dumpHeader(VDIFile vdi);
void displayPart(partEntry p[]);
void combine(VDIFile vdi, partEntry p[]);
void checkExt(ext2File ext);

int main() {
    uint8_t *buf;

    //int fd;
    //long fsize;
    //char *fileName = reinterpret_cast<char *>('/home/csis/Documents/Operating Systems/Test-fixed-1k.vdi');
    //char *fn = fileName;
    VDIFile vdi;
    cout << "What is the file name. " << endl;
    //cin >> fileName;

    //open the file
    if(!vdi.vdiOpen()) {
        cout << "File not found";
        return 0;
    };

    //partTable pTable;
    partEntry pEntry[4];

    combine(vdi, pEntry);

    //pTable.partOpen(vdi, pEntry);
    ext2File ext;
    if(!ext.ext2Open(vdi, 1, pEntry))
        return 0;

    checkExt(ext);
    blockGDT *blkgdt = new blockGDT[ext.numGroup];

    if(ext.fetchBGDT(8194, blkgdt)){
        cout << "fetch bgdt failed" << endl;
        return 0;
    }
    cout << endl << blkgdt[0].bg_inode_bitmap << endl;
    buf = new uint8_t [ext.blockSize];

    if(ext.fetchBlock(1, buf)){
        cout << "Fetch Failed" << endl;
        return 0;
    }
    displayBuf(buf, 1024, 0);

    //dumpHeader(vdi);


    vdi.vdiClose();
    ext.ext2Close();


    return 0;
}


void displayBuf(uint8_t *buf, uint32_t count, uint64_t offset) {
    //uint32_t start = 190;
    uint32_t start = 0;
    int proceed = count;

    //repeatedly print buffer pages until complete
    while(proceed > 0) {
        displayBufPage(buf+start, count, 0, offset);
        start += 256;
        offset += 256;
        proceed -= 256;
        count -= 256;

    }
}

void displayBufPage(uint8_t *buf, uint32_t count, uint32_t start, uint64_t offset) {
    string display[] = {"00", "10", "20", "30", "40", "50", "60", "70", "80", "90", "A0", "B0", "C0", "D0", "E0", "F0"};
    cout << "Offset: " << hex << offset << endl;
    //cout << "  ";

    //determines current location of buffer to be displayed
//    uint64_t curLoc = offset;
    uint64_t curLoc = 0;

    cout << "   00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f   0...4...8...c..." << endl;
    cout << "  +-----------------------------------------------+ +-----------------+" << endl;
    for(int row = 0; row < 16; row++) {

        cout << display[row] << '|';
        int tmpLoc = curLoc;
        for(int col = 0; col < 16; col++) {
            if((start  <= row*16+col) && (row*16+col < count + start )) {
                cout << hex << setfill('0') << setw(2) << static_cast<int>(buf[tmpLoc]) << ' ';
                tmpLoc++;
//                cout << curLoc << ' ';
            }
            else {
                cout << "   ";
            }
        }

        cout << '|' << display[row] << '|';
//        curLoc -= 16 ;

        for(int col = 0; col < 16; col++) {
            if((start  <= row*16+col) && (row*16+col < count + start )) {
                if (isprint(buf[curLoc])) {
                    cout << buf[curLoc];
                    //cout << setw(2) << buf[charCell] << ' ';
                }
                else {
                    cout << " ";
                }
                curLoc++;
            }
            else {
                cout << ' ';
            }
        }

        cout << '|' << endl;
    }

    cout << "  +-----------------------------------------------+ +-----------------+" << endl;

}

void dumpHeader(VDIFile vdi) {
    cout << "Image Name : ";
    for(int i = 0; i < 64; i++) {
        if(isprint(vdi.header->szFileInfo[i]))
            cout << hex << vdi.header->szFileInfo[i];
    }

    cout << endl;

    cout << "Signature: " << setw(8) << setfill('0') << vdi.header->u32Signature << endl;

    cout << "Version: " << vdi.header->u32Version << endl;

    cout << "Header Size: " << setw(8) << setfill('0') << vdi.header->cbHeader << endl;

    cout << "Image Type: " << setw(8) << setfill('0') << vdi.header->u32Type << endl;

    cout << "Flags: " << setw(8) << setfill('0') << vdi.header->fFlags << endl;

    cout << "Sector Size: " << setw(8) << setfill('0') << vdi.header->sectorSize << endl;

    cout << "Map offset: " << setw(8) << setfill('0') << vdi.header->offBlocks << endl;

    cout << "Frame offset: " << setw(8) << setfill('0') << vdi.header->offData << endl;

    cout << "Frame Size: " << setw(8) << setfill('0') << vdi.header->cbBlock << endl;

    cout << "Total frames: " << setw(8) << setfill('0') << vdi.header->cBlocks << endl;

    cout << "Frames Allocated: " << setw(8) << setfill('0') << vdi.header->cBlocksAllocated << endl;

    cout << "Disk Size: " << setw(16) << setfill('0') << vdi.header->cbDisk << endl;



}

void displayPart(partEntry p[]) {
    for(int i = 0; i < 4; i++) {
        cout << "Partition " << i+1 << endl;
        cout << "Status: " << hex <<(int)p[i].status << endl;
        cout << "First Sector CHS: ";
        for(int j = 0; j < 3; j++) {
            cout << (int)p[i].chsFirst[j];
        }
        cout << endl;
        cout << "Last Sector CHS: ";
        for(int j = 0; j < 3; j++) {
            cout  << (int)p[i].chsLast[j];
        }
        cout << endl;
        cout << "Partition Type: " << hex << (int)p[i].type << endl;
        cout << "First LBA Sector: " << p[i].firstSector << endl;
        cout << "Number of Sectors: " << p[i].nSectors << endl;
        cout << endl;


    }
}

void combine(VDIFile vdi, partEntry p[]) {
    //cout << (int)p[0].nSectors << endl;

    vdi.vdiSeek(446, SEEK_SET);
    //cout << vdi.cursor << endl;
    vdi.vdiRead( p, 64);

    //cout << (int)p[0].nSectors << endl;
}

void checkExt(ext2File ext) {
    cout << endl << "superblock contents: " << endl;
    cout << "Num of Inodes: " << ext.sb->s_inodes_count << endl;
    cout << "Num of blocks: " << ext.sb->s_blocks_count << endl;
    cout << "Num of free blocks: " << ext.sb->s_free_block_count << endl;
    cout << "Num of free inodes: " << ext.sb->s_free_inodes_count << endl;
    cout << "First data block: " << ext.sb->s_first_data_block << endl;
    cout << "Log block size: " << ext.sb->s_log_block_size << endl;
    cout << "Log frag size: " << ext.sb->s_log_frag_size << endl;
    cout << "Blocks per group: " << ext.sb->s_blocks_per_group << endl;
    cout << "Frags per group: " << ext.sb->s_frags_per_group << endl;
    cout << "Inodes per group: " << ext.sb->s_inodes_per_group << endl;

    cout << "Block GDT:" << endl;
    cout << setw(10) << left << "Block" << setw(10) << "Block" << setw(10) << "Inode" <<setw(10) << "Inode";
    cout << setw(10) << left << "Free" << setw(10) << "Free" << setw(10) << "Used" << endl;
    cout << setw(10) << left << "Number" << setw(10) << "Bitmap" << setw(10) << "Bitmap";
    cout << setw(10) << left << "Table" << setw(10) << "Blocks" << setw(10) << "Inodes" << setw(10) << "Dirs" << endl;
    cout << endl;
    for(int i = 0; i < ext.numGroup; i++) {
        cout << setw(10) << left << i << setw(10) << ext.bGDT[i].bg_block_bitmap << setw(10) << ext.bGDT[i].bg_inode_bitmap;
        cout << setw(10) << left << ext.bGDT[i].bg_inode_table <<setw(10) << ext.bGDT[i].bg_free_blocks_count <<setw(10) << ext.bGDT[i].bg_free_inodes_count;
        cout << setw(10) << left << ext.bGDT[i].bg_used_dirs_count << endl;
    }
}

