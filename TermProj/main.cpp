#include <iostream>
#include <string>
#include <iomanip>
#include <fcntl.h>
#include <unistd.h>
#include "VDIFile.h"
#include "VDIheader.h"

using namespace std;

void displayBufPage(uint8_t *buf, uint32_t count, uint32_t start, uint64_t offset);
void displayBuf(uint8_t *buf, uint32_t count, uint64_t offset);
//void displayBufPage(uint8_t *buf, uint32_t count, uint32_t start, uint64_t offset);
//void displayBuf(uint8_t *buf, uint32_t count, uint64_t offset);
void dumpHeader(VDIFile vdi);

int main() {
    uint8_t *buf;

    int fd;
    long fsize;
    char *fileName = reinterpret_cast<char *>('/home/csis/Documents/Operating Systems/Test-fixed-1k.vdi');
    //char *fn = fileName;
    VDIFile vdi;
    cout << "What is the file name. " << endl;
    //cin >> fileName;

    //open the file
    if(!vdi.vdiOpen()) {
        cout << "File not found";
        return 0;
    };
    dumpHeader(vdi);

    fd = open("/home/csis/Documents/Operating Systems/Test-fixed-1k.vdi", O_RDONLY);
    if(fd == -1) {
        cout << "File not found" << endl;
        return 0;
    }

    //get size of the file to allocate space for the buffer.
//    fsize = lseek(fd, 0, SEEK_END);
    fsize = 400;
    //exmaple 1
    lseek(fd, 0, SEEK_SET);
    //example 2
    //lseek(fd, 1048576, SEEK_SET);
    //lseek(fd, 2*1048576, SEEK_SET);
    //example 3
    //lseek(fd, 2*1048576+446, SEEK_SET);
    buf = new uint8_t [fsize];
    //read into the buffer
    read(fd, buf, fsize);



    //display the buffer
    //example 1 and 2
    displayBuf(buf, 400, 0);


    //example 2 and 3
    //displayBufPage(buf, 400, 190, 256);
    vdi.vdiClose();
    close(fd);


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

    cout << "Map offset: " << setw(8) << setfill('0') << vdi.header->offBlocks << endl;

    cout << "Frame offset: " << setw(8) << setfill('0') << vdi.header->offData << endl;

    cout << "Frame Size: " << setw(8) << setfill('0') << vdi.header->cBlocks << endl;

    cout << "Total frames: " << setw(8) << setfill('0') << vdi.header->cbDisk << endl;

    cout << "Disk Size: " << setw(16) << setfill('0') << vdi.header->cbBlock << endl;

    cout << "Disk Size: " << setw(16) << setfill('0') << vdi.header->cBlocksAllocated << endl;


}

