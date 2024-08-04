#include <stdlib.h> /* <-- Just for the malloc */
#include <stdarg.h> /* <-- Required for fstring() function */
#include "chasm.h"

void _start() {
    char* _str = malloc(sizeof(char) * 20);
    fstring(_str, "13 * 2 = %d\n", (13 * 2));

    // Print "13 * 2 = 26"
    print(_str);

    free(_str);

    // Exit
    exit_program(0);
}