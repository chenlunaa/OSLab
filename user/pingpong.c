#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    int p_dad [2];
    int p_son [2];
    pipe(p_dad);
    pipe(p_son);

    if (fork() == 0)
    {
        close(p_son[0]);
        close(p_dad[1]);
        
        int pid_dad = 0;
        int pid_son = getpid();
        read(p_dad[0], &pid_dad, sizeof(pid_dad));
        printf("%d: received ping from pid %d\n", pid_son, pid_dad);
        close(p_dad[0]);
        
        write(p_son[1], &pid_son, sizeof(pid_son));
        close(p_son[1]);
    }else
    {
        close(p_son[1]);
        close(p_dad[0]);

        int pid_dad = getpid();
        write(p_dad[1], &pid_dad, sizeof(pid_dad));
        close(p_dad[1]);

        int pid_son = 0;
        read(p_son[0], &pid_son, sizeof(pid_son));
        printf("%d: received pong from pid %d\n", pid_dad, pid_son);
        close(p_son[0]);
    }
    exit(0);
}