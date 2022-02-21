#pragma once


#define SZ_16K          0x4000                  ///< aligned size is 16K
#define SZ_64K          0x10000                 ///< aligned size is 64k

#define KERNEL_VADDR    0xffffff0000000000      ///< kernel's viturl address
#define TEXT_OFFSET     0x80000                 ///< the offset of data in ELF