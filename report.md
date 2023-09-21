## Lab:thread

### 1. Uthread

A thread has a kernel stack to store context, a user stack to store context. (Note that in this lab, multi-threading is a feature xv6 not supported, and we implement a toy, user-designed multi-threading in user space.

We use `struct uthread_context` to behave like a kernel stack, and `char*` to serve as a user stack.

When creating a new thread, we allocate a thread cell for it in `ALL_Threads`, and set the `ra` `sp`, therefore it can run with correct PC and "user space". 

### 2. Using threads

Very easy, just use lock to avoid conflict of concurrency.

### 3. Barrier

Implement barrier for multi threading. If condition is not satisfied, use `pthread_cond_wait` to wait; When the condition is satisfied, use `pthread_cond_broadcast` to wake all sleep threads. Note that threads sleep to wait, instead of spinning, which is more efficient.

-  `pthread_barrier_t` can not be compiled on my laptop.

`pthread_cond_t` is used to sleep the threads, and wake up all sleep threads by one thread.
 
