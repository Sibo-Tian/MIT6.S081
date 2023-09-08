## Copy-On-Write Lab

### Design of COWMemory Mechanism and riscv annotation of page fau

1. Insight: Parent and forked applications in user space can share the physical pages and pagetable. This is especially convinient for read-only and execute-only pages. The only thing we have to deal with is writable pages.
2. From the process perspective, nothing is changed if no `fork` happens. Each writable page is still with PTE_W set. However, when `fork` happens, we set both the parent and child process 's PTE_W to 0, and PTE_COW (a reserved bit in riscv, we use it here to record the status of pages) to 1, which means if you want to write to this page, a pagefault would be raised and the kernel will run into a handler function through interface in kernel/trap.c, allocate a new page, copy the COW page to it, set the bits of the new page with PTE_W=1 and PTE_COW=0. Finally, reduce the count of reference of the COW page.
3. As shown above, since one COW page may be referenced by multiple processes, we should record the number of reference, which is implemented by one extern array defined in kernel/defs.h. A lock is also needed to support concurrent visits.

### Design of test cases

Start with: When more than half of physical memory are allocated, the application fails if no COW implemented.

### Questions

1. Why uvmcopy iterates virtual address with increment=PGSIZE and no page faults (PTE=0) ?
   - The pagetable of user space application is continuous in address space.s
2. 