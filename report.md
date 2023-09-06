## Lab traps

### Takeaway

1. Registers: User registers and control registers. In user mode, only user registers are available, but in superviser mode, both of them are available.

2. PageTable: In user mode, PTEs with PTE_U=1 are available, otherwise are not; But in superviser mode, PTEs with PTE_U=1 are NOT available, otherwise are available.

3. ALL difference between user mode and supervisor mode: The latter one can use control registers and PTEs with PTE_U=0;

### Design for trap

1. Trampoline should be PTE_U=0, otherwise user applications can write to it and then trap to it.

2. "Context" that should to be saved: 32 user registers, SEPC, SATP, 

3. | Control Registers | Usage |
   | --- | --- |
   | SEPC | Save the PC value before trap (point to "ecall" instruction)| 
   | SATP | Save the address of pagetable |
   | STVEC | Save the address of trampoline |
   | SSCRATCH | Supervisor Sscratch Register, save the address of trapframe|
   | SCAUSE  | Save the reason for trap |
   | SSTATUS | Save the status like SPP, SPIE etc bits|
4. 1. ecall : mode from user to supervisor; PC -> SPEC, SPVEC -> PC;
   2. trampoline.S/uservec : Save 32 user registers, load kernel stack to sp, load kernel pagetable to satp; run trap.c/usertrap (Note that in trampoline.S, pc increases evenly, so the virtual address of trampoline.S should be the same in both user mode and kernel mode, and considering the one-to-one mapping of kernel pagetable, the vitrual address and physical address of trampoline.S should be identical in user pagetable.
   3. trap.c/usertrap : Save SEPC and increase it by 4, and this is the ONLY control register containing user mode information that we saved in trapframe.
   4. trap.c/usertrapret: Set SPEC, SSTAUS. Note that must NOT set SATP to user pagetable now, because trap.c is not loaded in trampoline, and no mapping for this virtual address in user pagetable. (An interesting observation is that when we jump from trampoline to trap.c, we call trap.c/usertrap (an address stored in trapframe) by `jr` assembly instruction, and we call trampoline.S/userret by using function pointer, and we stored the address `userret` in extern char)
   5. trampoline.S : load user pagetable (which is necessary to access trapframe in user mode, instead in kernel mode we use wrapped functions), load 32 user registers, load user sp;

### Alarm

This is one mechanism implemented by syscall.

The propose is run handler function as the user function has used n ticks of CPU time.

First, use a syscall `sys_sigalarm` to set ticks and handler function address to the process, the kernel will check and use these informaion during each interrupt.

Second, recall that each interrupt is processed by the kernel in kernel/trap.c, so we just rewrite usertrap() to track the number of ticks used by current process; If n ticks are used up, we jump to handler function in user space by setting SEPC, this is done by setting
myproc()->trapframe->epc = myproc()->handler;

Third, remember that there are 3 players : original application in user space, handler function in user space, usertrap in kernel space; when you return from kernel space to user space due to an interrupt but return to handler instead of original application, you have to save the context of original application, that's why we need another field in proc.h to save these registers. To return from handler function to original application, we use a syscall `sys_sigreturn` to restore the registers in trapframe from the field mentioned above. (Also note that we should not call a handler function when we are in the handler funtion, because this will overwrite the registers of original function we saved, thus we should turn alarm down when we are in handler funtion, this is done in trap.c)


