## Lab2: Syscall

### Part 1: Trace

1. 通过在User Mode和Supervise Mode之前传参的寄存器中读取mask，并且存储到当前process的mask中；（code中命名为trace，含义相同）
2. 每次执行fork时，使用parent process的mask覆盖child process的mask；
3. 每次由User Mode进入Supervise Mode的syscall时，判断是否是需要trace的ecall.

### Part 2：Sysinfo

1. 通过在User Mode和Supervise Mode之前传参的寄存器中读取返回结果写入地址；
2. 在 `kernel/kalloc.c`, `kernel/proc.c`中添加获取free memory和num_porc的函数；
3. 在 `kernel/sysproc.c`中调用上述函数，获取sysinfo，使用copyout写出结果.

### Notes

1. 系统调用依赖于硬件支持：在User Mode执行到ecall x后，硬件将模式切换至Kernel Model，并且跳转至syscall_handler执行相应命令，syscall_handler根据x对应的具体命令，再跳转至相应的命令。（syscall_handler起到）
2. 具体的接口调用顺序：
   1. `user/user.h`：定义了User Mode的接口；
   2. `user/usys.pl`：指定了ecall对应到的Supervise Mode的位置；
   3. `kernel/syscall.c`：由syscall进行分发，指定具体的系统调用函数（syscall函数的作用类似rcore中的syscall_handler）
   4. `kernel/sysproc.c`: 定义了具体的sys_trace, sys_sysinfo两个系统调用；
