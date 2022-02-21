/**
 * @file                    kprint.h
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

#ifndef KPRINT_H
#define KPRINT_H


#include "printk.h"


#define WARNING 0
#define INFO 1
#define DEBUG 2


// LOG_LEVEL is INFO by default
#if LOG_LEVEL >= WARNING
#define kwarn(fmt, ...) printk("[WARN] "fmt, ##__VA_ARGS__)
#else
#define kwarn(fmt, ...)
#endif

#if LOG_LEVEL >= INFO
#define kinfo(fmt, ...) printk("[INFO] "fmt, ##__VA_ARGS__)
#else
#define kinfo(fmt, ...)
#endif

#if LOG_LEVEL >= DEBUG
#define kdebug(fmt, ...) printk("[DEBUG] "fmt, ##__VA_ARGS__)
#else
#define kdebug(fmt, ...)
#endif


#endif // KPRINT_H