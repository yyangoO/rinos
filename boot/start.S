/**
 * @file                    start.S
 * @author                  yyang yyangoO@outlook.com & sjtu_yangyang@sjtu.edu.cn
 * @version                 1.0.0
 * @date                    24-APR-2021
 * @section                 description
 *                          starting part of bootloader
 * @section                 platforn
 * -                        Raspberry Pi 3b
 * @section                 logs
 * <table>
 * <tr><th>date             <th>version     <th>author      <th>description     </tr>
 * <tr><td>2021.04.24       <td>1.0.0       <td>yyang       <td>initial version </tr>
 * </tr>
 */


#include <common/asm.h>


.extern arm64_elX_to_el1
.extern boot_cpu_stack
.extern secondary_boot_flag
.extern clear_bss_flag
.extern init_c


.global _start                  // define _start could be find by other program
.type _start, %function         // define the type of _start is function
_start:

    mrs     x8, mpidr_el1
    and     x8, x8, #0xff
    cbz     x8, primary

secondary_hang:                 // make all other kernel in loop
    bl      secondary_hang

primary:
    bl      arm64_elX_to_el1    // turn to el1 from other exception levels
    adr     x0, boot_cpu_stack  // prepare stack pointer and jump to C
    add     x0, x0, #0x1000
    mov     sp, x0
    bl      init_c              // jump to init_c
    b       .                   // should never be here

.size _start, .-_start