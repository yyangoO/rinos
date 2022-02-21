/**
 * @file                    printk.c
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


#include <common/uart.h>


#define PRINT_BUF_LEN       64      ///< the length of print buffer


typedef __builtin_va_list va_list;

#define va_start(v, l)   __builtin_va_start(v, l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v, l)     __builtin_va_arg(v, l)
#define va_copy(d, s)    __builtin_va_copy(d, s)

/**
 * @brief flag of padding
 * The flag of padding when we print.
 */
enum flags 
{
	PAD_ZERO = 1,       ///< padding with zero
	PAD_RIGHT = 2       ///< padding in right
};

/**
 * @brief simply output char
 * Simply output a char instead of the string
 * @param str       the string
 * @param c         the char
 */
static void simple_outputchar(char **str, char c)
{
	if (str) 
    {
		**str = c;
		++(*str);
	} 
    else 
    {
		uart_send(c);
	}
}

/**
 * @brief print
 * Print the string in the specified format.
 * @param out       the output string
 * @param string    the input string
 * @param width     the width
 * @param flags     the flags of padding
 * @return          result (length)
 */
static int prints(char **out, const char *string, int width, int flags)
{
	int pc = 0;             // the point cursor
    int padchar = ' ';      // the padding char

    // the width must be bigger than 1
	if (width > 0)
    {
		int len = 0;
		const char *ptr;

        // get the length of string
		for (ptr = string; *ptr; ++ptr)
        {
			++len;
        }

        // check and set the wdith
		if (len >= width)
        {
			width = 0;
        }
		else
        {
			width -= len;
        }

        // check the padding type
		if (flags & PAD_ZERO)
        {
			padchar = '0';
        }
	}

    // print the padding
	if (!(flags & PAD_RIGHT)) 
    {
		for (; width > 0; --width) 
        {
			simple_outputchar(out, padchar);
			++pc;
		}
	}
    // then print each char of string
	for (; *string; ++string) 
    {
		simple_outputchar(out, *string);
		++pc;
	}
    // then print the padding
	for (; width > 0; --width) 
    {
		simple_outputchar(out, padchar);
		++pc;
	}

	return pc;
}

/**
 * @brief print number
 * Print number `i` in the base of `base`
 * @param out       the output string
 * @param i         the print number
 * @param base      the base
 * @param sgin      the flag of print signed number or unsigned number
 * @param width     the length of printed number
 * @param flags     the length of printed number at least `width`
 * @param letbase   the uppercase or lowercase when using hex
 * @return          result
 */
static int printk_write_num(char **out, long long i, int base, int sign, int width, int flags, int letbase)
{
	char print_buf[PRINT_BUF_LEN];      // the print buffer
	char *s;
	int t, neg = 0, pc = 0;
	unsigned long long u = i;

	if (i == 0) 
    {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints(out, print_buf, width, flags);
	}

	if (sign && base == 10 && i < 0) 
    {
		neg = 1;
		u = -i;
	}

    s = print_buf + PRINT_BUF_LEN - 1;
	*s = '\0';

	while(u > 0) 
    {
		s--;
		t = u % base;

		if(t <= 9)
        {
			*s = t + '0';
        }
		else
        {
			*s = t - 10 + (letbase ? 'a' : 'A');
        }
		u /= base;
	}

	if (neg) 
    {
		if (width && (flags & PAD_ZERO)) 
        {
			simple_outputchar(out, '-');
			++pc;
			--width;
		} 
        else 
        {
			*--s = '-';
		}
	}

	return pc + prints(out, s, width, flags);
}

/**
 * @brief simple variable list print
 * Simple print function by variable list.
 * @param out       the output pointer, normal be NULL
 * @param format    the format we print
 * @param ap        the variable list
 * @return          result
 */
static int simple_vsprintf(char **out, const char *format, va_list ap)
{
	int width, flags;
	int pc = 0;
	char scr[2];

    // all types
	union 
    {
		char c;
		char *s;
		int i;
		unsigned int u;
		long li;
		unsigned long lu;
		long long lli;
		unsigned long long llu;
		short hi;
		unsigned short hu;
		signed char hhi;
		unsigned char hhu;
		void *p;
	} u;

    // check and get every format
	for (; *format != 0; ++format) 
    {
		if (*format == '%') 
        {
			++format;
			width = flags = 0;

            // check format
			if (*format == '\0')
            {
				break;
            }
			if (*format == '%')
            {
				goto out;
            }
			if (*format == '-') 
            {
				++format;
				flags = PAD_RIGHT;
			}
			while (*format == '0') 
            {
				++format;
				flags |= PAD_ZERO;
			}
			if (*format == '*') 
            {
				width = va_arg(ap, int);
				format++;
			} 
            else 
            {
				for (; *format >= '0' && *format <= '9'; ++format) 
                {
					width *= 10;
					width += *format - '0';
				}
			}
			switch (*format) 
            {
			case ('d'):
				u.i = va_arg(ap, int);
				pc += printk_write_num(out, u.i, 10, 1, width, flags, 'a');
				break;

			case ('u'):
				u.u = va_arg(ap, unsigned int);
				pc += printk_write_num(out, u.u, 10, 0, width, flags, 'a');
				break;

			case ('o'):
				u.u = va_arg(ap, unsigned int);
				pc += printk_write_num(out, u.u, 8, 0, width, flags, 'a');
				break;

			case ('x'):
				u.u = va_arg(ap, unsigned int);
				pc += printk_write_num(out, u.u, 16, 0, width, flags, 'a');
				break;

			case ('X'):
				u.u = va_arg(ap, unsigned int);
				pc += printk_write_num(out, u.u, 16, 0, width, flags, 'A');
				break;

			case ('p'):
				u.lu = va_arg(ap, unsigned long);
				pc += printk_write_num(out, u.lu, 16, 0, width, flags, 'a');
				break;

			case ('c'):
				u.c = va_arg(ap, int);
				scr[0] = u.c;
				scr[1] = '\0';
				pc += prints(out, scr, width, flags);
				break;

			case ('s'):
				u.s = va_arg(ap, char *);
				pc += prints(out, u.s ? u.s : "(null)", width, flags);
				break;
			case ('l'):
				++format;
				switch (*format) 
                {
				case ('d'):
					u.li = va_arg(ap, long);
					pc += printk_write_num(out, u.li, 10, 1, width, flags, 'a');
					break;

				case ('u'):
					u.lu = va_arg(ap, unsigned long);
					pc += printk_write_num(out, u.lu, 10, 0, width, flags, 'a');
					break;

				case ('o'):
					u.lu = va_arg(ap, unsigned long);
					pc += printk_write_num(out, u.lu, 8, 0, width, flags, 'a');
					break;

				case ('x'):
					u.lu = va_arg(ap, unsigned long);
					pc += printk_write_num(out, u.lu, 16, 0, width, flags, 'a');
					break;

				case ('X'):
					u.lu = va_arg(ap, unsigned long);
					pc += printk_write_num(out, u.lu, 16, 0, width, flags, 'A');
					break;

				case ('l'):
					++format;
					switch (*format) 
                    {
					case ('d'):
						u.lli = va_arg(ap, long long);
						pc += printk_write_num(out, u.lli, 10, 1, width, flags, 'a');
						break;

					case ('u'):
						u.llu = va_arg(ap, unsigned long long);
						pc += printk_write_num(out, u.llu, 10, 0, width, flags, 'a');
						break;

					case ('o'):
						u.llu = va_arg(ap, unsigned long long);
						pc += printk_write_num(out, u.llu, 8, 0, width, flags, 'a');
						break;

					case ('x'):
						u.llu = va_arg(ap, unsigned long long);
						pc += printk_write_num(out, u.llu, 16, 0, width, flags, 'a');
						break;

					case ('X'):
						u.llu = va_arg(ap, unsigned long long);
						pc += printk_write_num(out, u.llu, 16, 0, width, flags, 'A');
						break;

					default:
						break;
					}
					break;
				default:
					break;
				}
				break;
			case ('h'):
				++format;
				switch (*format) 
                {
				case ('d'):
					u.hi = va_arg(ap, int);
					pc += printk_write_num(out, u.hi, 10, 1, width, flags, 'a');
					break;

				case ('u'):
					u.hu = va_arg(ap, unsigned int);
					pc += printk_write_num(out, u.lli, 10, 0, width, flags, 'a');
					break;

				case ('o'):
					u.hu = va_arg(ap, unsigned int);
					pc += printk_write_num(out, u.lli, 8, 0, width, flags, 'a');
					break;

				case ('x'):
					u.hu = va_arg(ap, unsigned int);
					pc += printk_write_num(out, u.lli, 16, 0, width, flags, 'a');
					break;

				case ('X'):
					u.hu = va_arg(ap, unsigned int);
					pc += printk_write_num(out, u.lli, 16, 0, width, flags, 'A');
					break;

				case ('h'):
					++format;
					switch (*format) {
					case ('d'):
						u.hhi = va_arg(ap, int);
						pc += printk_write_num(out, u.hhi, 10, 1, width, flags, 'a');
						break;

					case ('u'):
						u.hhu = a_arg(ap, unsigned int);
						pc += rintk_write_num(out, u.lli, 10, 0, width, flags, 'a');
						break;

					case ('o'):
						u.hhu = a_arg(ap, unsigned int);
						pc += printk_write_num(out, u.lli, 8, 0, width, flags, 'a');
						break;

					case ('x'):
						u.hhu = va_arg(ap, unsigned int);
						pc += printk_write_num(out, u.lli, 16, 0, width, flags, 'a');
						break;

					case ('X'):
						u.hhu =
						    va_arg(ap, unsigned int);
						pc +=
						    printk_write_num(out, u.lli,
								     16, 0,
								     width,
								     flags,
								     'A');
						break;

					default:
						break;
					}
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		} 
        else 
        {
out:
			simple_outputchar(out, *format);
			++pc;
		}
	}
	if (out)
    {
		**out = '\0';
    }
	return pc;
}

void printk(const char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	simple_vsprintf(NULL, fmt, va);
	va_end(va);
}

void break_point()
{
	printk("[rinos] os lost ... \n");
}
