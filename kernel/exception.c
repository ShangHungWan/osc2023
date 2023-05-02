#include "exception.h"
#include "uart.h"

void exception_default_handler(unsigned int n)
{
    uart_puts("[exception]: ");
    uart_hex(n);
    uart_newline();
}