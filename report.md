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
   2. `user/usys.pl`：指定了ecall对应到的Supervise Mode的位置；由此生成 `user/usys.S`， 即user mode process 调用 syscall 的user mode接口，来到user/usys.S；user/usys.S需要知道不同的syscall对应的数字编码，因此user/usys.S kernel/syscall.c syscall数字编码是user mode和kernel mode的桥梁；需要注意的是，用户态和内核态是不同的地址空间，因此不能在内核态中直接向得到的用户态（虚拟）地址写入数据，需要使用copyout函数。
   3. `kernel/syscall.c`：由syscall进行分发，指定具体的系统调用函数（syscall函数的作用类似rcore中的syscall_handler）
   4. `kernel/sysproc.c`: 定义了具体的sys_trace, sys_sysinfo两个系统调用；



用户态：user/

在user/user.h中声明新加入的函数和数据结构， 使得用户态程序可以调用之。

usys.pl只是一个脚本，在编译阶段被运行，但并不是最终编译得到的xv6的一部分，其唯一作用是生成usys.S，后者是用户态进入内核态的窗口；使用脚本生成汇编代码可以简化写代码的过程，也避免了编写相同的重复assembly代码；注意在usys.S中include了kernel/syscall.h（定义了syscall的数字编码（宏）），得到了syscall的编码；

* 问题：user.h和usys.S如何联系起来的 （猜测是user.h中的syscall函数都被绑定到了usys.S同名字段的汇编实现，猜测是编译阶段完成的函数声明-定义绑定）


内核态：kernel/

syscall.c是内核态接收用户态调用的窗口。通过一个函数指针数组（其中显式指定元素索引，使用了C99的特性），联系起了syscall的数字编码和相应的syscall函数，后者在proc.c中实现；
