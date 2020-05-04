

#include "inode.h"

int32_t inodeFile::fetchInode(ext2File f, uint32_t iNum, struct Inode *buf){

    cout << "Inode called";
    iNum -=1;

    //int ipg = f.sb->s_inodes_per_group;
    //uint32_t blockGroup;
    int blockGroup = iNum / f.sb->s_inodes_per_group;
    int *block = new int[f.sb->s_blocks_per_group];

        int index = iNum % f.sb->s_inodes_per_group;

        int blockSize = 1024 << f.sb->s_log_block_size;
        int inodesPerBlock = blockSize / f.sb->s_inode_size;

    uint32_t blockNum =index / inodesPerBlock;

    blockGDT *bGDT;

    f.fetchBGDT(blockGroup, bGDT);

    block[index] = f.fetchBlock(bGDT[blockGroup].bg_inode_table + blockNum, bGDT);

    //still need to actually read the inode into the bufer

    VDIFile vdi;
    pTable.partOpen(vdi, p);

    uint64_t location = (blockGroup / blockNum) * blockSize;
    location = location + (inodesPerBlock / index) * f.sb->s_inode_size;

    //pTable.partSeek(location, SEEK_SET);
    //pTable.partRead(buf, f.sb->s_inode_size);

    pTable.partClose();
    cout << "Inode";

    return 0;
}

uint32_t inodeFile::writeInode(ext2File f, int32_t iNum, struct Inode *buf){
    iNum -= 1;

    if(inodeInUse(f, iNum)==false)
    {
        int blockGroup = iNum / f.sb->s_inodes_per_group;
            int *block = new int[f.sb->s_blocks_per_group];
            int index = iNum % f.sb->s_inodes_per_group;

        int blockSize = 1024 << f.sb->s_log_block_size;
        int inodesPerBlock = blockSize / f.sb->s_inode_size;

        uint32_t blockNum =index / inodesPerBlock;

        blockGDT *bGDT;

        f.fetchBGDT(blockGroup, bGDT);



    }else{
        return 0;
    }

    return 1;
}

int32_t inodeFile::inodeInUse(ext2File f, uint32_t iNum){

    iNum = iNum / f.sb->s_inodes_per_group;

    uint32_t inodeL = iNum % f.sb->s_inodes_per_group;
        blockGDT *bGDT = new blockGDT[iNum];
        f.fetchBGDT(iNum, bGDT);
    //^^fetch block group descriptor table with bitmap

    uint32_t bitMap = bGDT->bg_inode_bitmap;

    int bitNum = inodeL * f.sb->s_inodes_per_group;
    //int byte = bitNum / 8;
    int bit = bitNum % 8;

    int mask = 1;


//    if(((mask <<bit) && byte) == true )
  //      return 1;
    //else
      //  return 0;




}

int32_t inodeFile::allocateInode(ext2File f, uint32_t group){
    //find empty inode

    //bitmap &= 1 << bit;

}

int32_t inodeFile::freeInode(ext2File f, uint32_t iNum) {

    iNum = iNum / f.sb->s_inodes_per_group;

    uint32_t inodeL = iNum % f.sb->s_inodes_per_group;
        blockGDT *bGDT = new blockGDT[iNum];
        f.fetchBGDT(iNum, bGDT);
        //^^fetch block group descriptor table with bitmap

    uint32_t bitMap = bGDT->bg_inode_bitmap;

    int bitNum = inodeL * f.sb->s_inodes_per_group;
    //int byte = bitNum / 8;
    int bit = bitNum % 8;

    //to turn the bit off

    //bitmap |= 1 << bit;

}
