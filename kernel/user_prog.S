.section ".text"
.global _start

_start:
    mov x0, 0
for:
    add x0, x0, 1
    svc 0
    cmp x0, 5
    blt for
halt:
    b halt