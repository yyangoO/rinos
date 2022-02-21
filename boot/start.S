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


.global _start
.type _start, %function
_start:

    mrs     x8, mpidr_el1       // get current process element's cpu ID
    and     x8, x8, #0xff       // take low 8 bits
    cbz     x8, primary         // if 0, it's first process element, go to primary

secondary_hang:
    bl      secondary_hang      // if not 0, make them in dead loop

primary:
    bl      arm64_elX_to_el1    // turn to el1 from other exception levels
    adr     x0, boot_cpu_stack  // get the address of boot_cpu_stack
    add     x0, x0, #0x1000     // give the stack 4096 bytes
    mov     sp, x0              // set stack pointer registor
    bl      init_c              // jump to init_c
    b       .

.size _start, .-_start
