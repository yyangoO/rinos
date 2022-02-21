/**
 * @file                    init_c.c
 * @author                  yyang yyangoO@outlook.com & sjtu_yangyang@sjtu.edu.cn
 * @version                 1.0.0
 * @date                    24-APR-2021
 * @section                 description
 *                          initialization part of bootloader
 * @section                 platforn
 * -                        Raspberry Pi 3b
 * @section                 logs
 * <table>
 * <tr><th>date             <th>version     <th>author      <th>description     </tr>
 * <tr><td>2021.04.24       <td>1.0.0       <td>yyang       <td>initial version </tr>
 * </tr>
 */


#define PLAT_CPU_NUMBER		4           ///< armv8 aarch64 cpu kernel numbers
#define INIT_STACK_SIZE     0x1000      ///< initial stack size
char boot_cpu_stack[PLAT_CPU_NUMBER][INIT_STACK_SIZE] __attribute__((__aligned__(16)));


extern char _bss_start;
extern char _bss_end;

extern void el1_mmu_activate(void);
extern void init_boot_pt(void);

extern void start_kernel(void *boot_flag);


typedef unsigned long u64;              ///< unsigned long in 64 bits


/**
 * Initialize these varibles in order to make them not in .bss section.
 * So, they will have concrete initial value even on real machine.
 * Non-primary CPUs will spin until they see the secondary_boot_flag becomes
 * non-zero which is set in kernel (see enable_smp_cores).
 * The secondary_boot_flag is initilized as {NOT_BSS, 0, 0, ...}.
 */
#define NOT_BSS (0xBEEFUL)
long secondary_boot_flag[PLAT_CPU_NUMBER] = {NOT_BSS};

volatile u64 clear_bss_flag = NOT_BSS;

/**
 * @brief initialize the UART function
 * This function initialize the UART function. 
 */
void early_uart_init(void);
/**
 * @brief send string from UART
 * This function sends string by using UART.
 */
void uart_send_string(char *);
/**
 * @brief clear bss area
 * This function clear the bss area.
 */
static void clear_bss(void)
{
	u64 bss_start_addr;
	u64 bss_end_addr;
	u64 i;

	bss_start_addr = (u64) & _bss_start;
	bss_end_addr = (u64) & _bss_end;

	for (i = bss_start_addr; i < bss_end_addr; ++i)
    {
		*(char *)i = 0;
    }
	clear_bss_flag = 0;
}
/**
 * @brief initializate bootloader
 * This function initialize bootloader.
 */
void init_c(void)
{
	clear_bss();                                // clear the bss area for the kernel image

	early_uart_init();                          // initialize UART before enable MMU
	uart_send_string("boot: init_c\r\n");

	init_boot_pt();                             // initialize boot page table
    uart_send_string("[BOOT] install boot page table\r\n");

	el1_mmu_activate();                         // enable MMU
	uart_send_string("[BOOT] enable el1 MMU\r\n");

	start_kernel(secondary_boot_flag);          // call kernel main
    uart_send_string("[BOOT] Jump to kernel main\r\n");
}
