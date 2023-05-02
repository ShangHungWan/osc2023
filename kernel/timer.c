#include "timer.h"
#include "helper.h"
#include "uart.h"

void timer_print_counter()
{
    unsigned int frequency = read_register(cntfrq_el0);
    unsigned int counter = read_register(cntpct_el0);
    uart_puts("Seconds after booted: ");
    uart_hex(counter / frequency);
    uart_newline();
}