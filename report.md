## Lab3: Page Table



#### Notes

1. Virtual Address一般为39bits（27+12），27分为L0，L1，L2三级索引；每一级的Page Directory / Page Table都与Physical Page对齐，因此由L0索引到L1，只需要PPN+000000000000 （12bits * 0）即可。Page Directory / Page Table不需要在物理地址上连续；同时，注意到Page Directory / Page Table基于物理地址索引，因此在处理不同Process的Page Table时要注意隔离性；
2. Page Table的三级索引，都是由硬件（MMU）完成；同时，xv6中的walk函数提供了模拟硬件完成三级索引的功能；
3. 0x80000000及以后的物理内存对应主板上的内存，更低的地址对应其他IO设备 / boot ROM等等；
4. 设置satp寄存器后，执行下一条指令时就开始需要进行地址翻译；
5.
