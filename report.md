## Lab2: Syscall

### Part 1: Trace




Notes

1. 系统调用依赖于硬件支持：在User Mode执行到ecall x后，硬件将模式切换至Kernel Model，并且跳转至syscall_handler执行相应命令，syscall_handler根据x对应的具体命令，再跳转至相应的命令。（syscall_handler起到）
