.section ".text.boot"

.global _start

_start:
    // only use core 0
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbnz    x1, hang

    // save x0~x3
    mov     x20, x0
    mov     x21, x1
    mov     x22, x2
    mov     x23, x3

    bl      from_el2_to_el1

init_stack:
    ldr     x28, =_start
    mov     sp, x28

clear_bss:
    ldr     x4, =__bss_start
    ldr     x5, =__bss_size
    cbz     x5, run_main
clear_bss_loop:
    str     xzr, [x4], #8
    sub     x5, x5, #1
    cbnz    x5, clear_bss_loop

run_main:
    mov     x0, x20
    mov     x1, x21
    mov     x2, x22
    mov     x3, x23
    b       main

from_el2_to_el1:
    mov     x0, (1 << 31) // EL1 uses aarch64
    msr     hcr_el2, x0
    mov     x0, 0x3c5 // EL1h (SPSel = 1) with interrupt disabled
    msr     spsr_el2, x0
    msr     elr_el2, lr
    eret    // return to EL1

hang:
    wfe
    b       hang
