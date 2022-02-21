/**
 * @file                    image.h
 * @author                  yyang yyangoO@outlook.com & sjtu_yangyang@sjtu.edu.cn
 * @version                 1.0.0
 * @date                    24-APR-2021
 * @section                 description
 *                          header of os's image
 * @section                 platforn
 * -                        Raspberry Pi 3b
 * @section                 logs
 * <table>
 * <tr><th>date             <th>version     <th>author      <th>description     </tr>
 * <tr><td>2021.04.24       <td>1.0.0       <td>yyang       <td>initial version </tr>
 * </tr>
 */
#ifndef IMAGE_H
#define IMAGE_H


#define SZ_16K          0x4000                  ///< aligned size is 16K
#define SZ_64K          0x10000                 ///< aligned size is 64k

#define KERNEL_VADDR    0xffffff0000000000      ///< kernel's viturl address
#define TEXT_OFFSET     0x80000                 ///< the offset of data in ELF

#endif // IMAGE_H