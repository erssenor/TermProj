//
// Created by csis on 3/2/20.
//

#ifndef TERMPROJ_EXT2_H
#define TERMPROJ_EXT2_H
#include "VDIFile.h"
#include "Partition.h"

struct superBlock{
    uint32_t
      s_inodes_count,
      s_blocks_count,
      s_r_blocks_count,
      s_free_inodes_count,
      s_first_data_block,
      s_log_block_size,
      s_log_frag_size,
      s_blocks_per_group,
      s_frags_per_group,
      s_inodes_per_group,
      s_mtime,
      s_wtime;

    uint16_t
      s_mnt_count,
      s_max_mnt_count,
      s_magic,
      s_state,
      s_errors,
      s_minor_rev_level;

    uint32_t
      s_lastcheck,
      s_checkinterval,
      s_creator_os,
      s_rev_level;

    uint16_t
      s_def_resuid,
      s_def_resgid;

    uint32_t s_first_ino;

    uint16_t
      s_inode_size,
      s_block_group_nr;

    uint32_t
      s_feature_compat,
      s_feature_incompat,
      s_feature_ro_compat;

    uint32_t
      s_uuid[4],
      s_volume_name[4];

    uint64_t s_last_mounted[8];

    uint32_t s_algo_bitmap;

    uint8_t
      s_prealloc_blocks,
      s_prealloc_dir_blocks;

    uint16_t alignment;

    uint32_t s_journal_uuid[4];

    uint32_t
      s_journal_inum,
      s_journal_dev,
      s_last_orphan,
      s_hash_seed[4];

    uint8_t s_def_hash_version;

    uint8_t padding[3];

    uint32_t
      s_default_mount_options,
      s_first_meta_bg;

};

struct blockGDT{

    uint32_t
      bg_block_bitmap,
      bg_inode_bitmap,
      bg_inode_table;

    uint16_t
      bg_free_blocks_count,
      bg_free_inodes_count,
      bg_used_dirs_count,
      bg_pad;

    uint8_t
      bg_reserved[12];

};

class ext2File {
public:
    VDIFile vdi;
    partTable pTable;

    superBlock sb;
    blockGDT bGDT;

    void ext2Open(char *fn, uint32_t pnum);

    void ext2Close();

    uint32_t fetchBlock(uint32_t blockNum, void *buf);

    uint32_t writeBlock(uint32_t blockNum, void *buf);

    uint32_t fetchSuperBlock(uint32_t blockNum, superBlock *sb);

    uint32_t writeSuperBlock(uint32_t blockNum, superBlock *sb);

    uint32_t fetchBGDT(uint32_t blockNum, blockGDT *bgdt);

    uint32_t writeBGDT(uint32_t blockNum, blockGDT *bgdt);




};


#endif //TERMPROJ_EXT2_H
