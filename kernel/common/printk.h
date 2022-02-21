/**
 * @file                    printk.h
 * @author                  yyang yyangoO@outlook.com & sjtu_yangyang@sjtu.edu.cn
 * @version                 1.0.0
 * @date                    24-APR-2021
 * @section                 description
 *                          basic I/O function
 * @section                 platforn
 * -                        Raspberry Pi 3b
 * @section                 logs
 * <table>
 * <tr><th>date             <th>version     <th>author      <th>description     </tr>
 * <tr><td>2021.04.24       <td>1.0.0       <td>yyang       <td>initial version </tr>
 * </tr>
 */
#ifndef PRINTK_H
#define PRINTK_H

/**
 * @brief print
 * The print function.
 */
void printk(const char *fmt, ...);
/**
 * @brief break down
 * This function print the information that breaking down. 
 */
void break_point();


#endif // PRINTK_H