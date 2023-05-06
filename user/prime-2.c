#include "kernel/types.h"
#include "user/user.h"

void new_proc(int p[2]){
    int prime;
    int flag;
    int tmp;
    close(p[1]);

    if(read(p[0], &prime, sizeof(int)) != 4){ //error
        fprintf(2, "child process failed to read\n");
		exit(1);
    }
    printf("prime %d\n", prime);

    if(read(p[0], &flag, sizeof(int)) != 4){ //error
        fprintf(2, "child process failed to read\n");
		exit(1);
    }
    else{
        int new_p[2];
        pipe(new_p);
        if(fork()==0){
            new_proc(new_p);
        }
        else{
            close(new_p[0]);
            if(flag % prime != 0){
                write(new_p[1], &flag, sizeof(int));
            }
            while(read(p[0], &tmp, sizeof(int)) != 0){
                if(tmp % prime != 0){
                    write(new_p[1], &tmp, sizeof(int));
                }
            }
            close(p[0]);
            close(new_p[1]);
        }
        exit(0);
    }
    
}

int main(int argc, char const *[])
{
    int p[2];
    pipe(p);
    if(fork()==0){
        new_proc(p);
    }
    else{
        close(p[0]);
        for(int i=2; i<=35; i++){
            if (write(p[1], &i, 4) != 4)
			{
				fprintf(2, "first process failed to write %d into the pipe\n", i);
				exit(1);
			}
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}