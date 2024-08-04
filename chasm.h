#ifndef CHASM_H
#define CHASM_H

/* Syscall codes */
#define SYS_write 1
#define SYS_exit 60
/* ------------- */

/*      filenos      */
#define STDOUT_FILENO 1
/* ----------------- */

/* chasm error handling */
#define STDARG_ERR "ERROR: Include stdarg.h before chasm.h to use fstring.\n"
/* -------------------- */

unsigned str_len(const char *str) {
    unsigned len = 0;
    while (*str != '\0') {
        len++; str++;
    }
    return len;
}

int s_print(const char* str, int len) {
    int ret;
    asm volatile (
        "syscall"
        : "=a"(ret)
        : "a"(SYS_write), "D"(STDOUT_FILENO), "S"(str), "d"(len)
        : "rcx", "r11", "memory"
    );
    return ret;
}

int print(const char* str) {
    unsigned len = str_len(str);
    int ret;
    asm volatile (
        "syscall"
        : "=a"(ret)
        : "a"(SYS_write), "D"(STDOUT_FILENO), "S"(str), "d"(len)
        : "rcx", "r11", "memory"
    );
    return ret;
}

int fstring(char *str, const char *format, ...) {
#ifdef _STDARG_H
    va_list args;
    va_start(args, format);
    int count = 0;
    char c;

    while ((c = *format++) != '\0') {
        if (c == '%') {
            c = *format++;
            switch (c) {
                case 'd': {
                    int num = va_arg(args, int);
                    char buf[20];
                    int i = 0;
                    if (num < 0) {
                        buf[i++] = '-';
                        num = -num;
                    }
                    do {
                        buf[i++] = num % 10 + '0';
                        num /= 10;
                    } while (num);
                    while (i) {
                        *str++ = buf[--i];
                        count++;
                    }
                    break;
                }
                case 's': {
                    char *s = va_arg(args, char *);
                    while (*s) {
                        *str++ = *s++;
                        count++;
                    }
                    break;
                }
                case 'c': {
                    int ch = va_arg(args, int);
                    *str++ = (char)ch;
                    count++;
                    break;
                }
                default:
                    *str++ = c;
                    count++;
                    break;
            }
        } else {
            *str++ = c;
            count++;
        }
    }

    *str = '\0'; // Null-terminate the string
    va_end(args);
    return count;
#else
    print(STDARG_ERR);
#endif
}

void exit_program(int exit_code) {
    asm volatile(
        "syscall"
        :
        : "a"(SYS_exit), "D"(exit_code)
        : "rcx", "r11"
    );
}



#endif // CHASM_H