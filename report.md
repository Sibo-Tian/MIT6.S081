## Lab3: Page Table

#### Notes

1. Virtual Address一般为39bits（27+12），27分为L0，L1，L2三级索引；每一级的Page Directory / Page Table都与Physical Page对齐，因此由L0索引到L1，只需要PPN+000000000000 （12bits * 0）即可。Page Directory / Page Table不需要在物理地址上连续；同时，注意到Page Directory / Page Table基于物理地址索引，因此在处理不同Process的Page Table时要注意隔离性；
2. Page Table的三级索引，都是由硬件（MMU）完成；同时，xv6中的walk函数提供了模拟硬件完成三级索引的功能；
3. 0x80000000及以后的物理内存对应主板上的内存，更低的地址对应其他IO设备 / boot ROM等等；
4. 设置satp寄存器后，执行下一条指令时就开始需要进行地址翻译；
5. kernel mode使用direct-mapping的原因：kernel存在大量对physical address使用的函数（walk，mappages），如果indirec mapping，会无法在kernel mode层面对物理地址进行操作；因此direct-mapping的核心意义在于保证内核态可以操作物理地址。

`kernel/vm.c`: 内核态&用户态pagetable初始化以及page mapping

1. walk用来从va获取pa，同时在尚未生成多级页表的情况下生成多级页表，在前两层页表中，对于已经有次级页表的PTE，在PTE中PPN保存次级页表的PPN，以及设置PTE_V. 对于最后一级页表，即不需要对某个PTE保存PPN（这一步通过mappages实现），也不需要设置PTE_V（尚未map物理内存）。
2. mappages将va以页为单位map到pa，连续页分配。（这里是简化的页式内存分配，仍然会存在外碎片；TODO：这一问题可以通过kalloc改进，kalloc对应的是由list维护的freeMemory，不一定连续; Update：这在内核态中并非问题，因为内核态direct-mapping，每次mapping必然分配连续的物理内存；在用户态这是一个问题，因此在xv6中可以注意到用户态采取了上述alloc分配方法）
3. copyin/copyout时，由于内核态连续分配，因此直接使用了一个char*作为dst，不需要显式使用分页机制。

Problem 1: Speed up syscall

1. 流程为：内核先分配一个pid，使用kalloc分配“1个"物理页，将包含刚刚得到的pid的usyscall类型数据写入这个物理页中，将物理页的首地址保存到struct proc中的structusyscall *usyscallpage;此后才可以为proc生成pagetable，并且将USYSCALL的VA map到刚刚写入数据的物理地址处。此处要注意顺序，不能尚未分配物理页，就开始分配页表。
