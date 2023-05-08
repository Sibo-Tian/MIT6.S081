## Lab 1: Utils

1. pingpong.c
   Create a pipe between parent and child process;
   Note that parent process need to wait until child process has sent the byte to it.
2. primes.c
   For each stage (one column in the illstration), use a process to read from its former and writes to its successor; Use pipe for data communication.
3. find.c
   Like `ls`, open the seaching path, resursively search for the file, use `stat` to get the info of file;
4. xargs.c
   TODO
   Naive inplementation: Read new arguments from standard input, and add them to chat * new_argv[];
