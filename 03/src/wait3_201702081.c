#include <stdio.h>
#include <stdlib.h>     // exit
#include <unistd.h>     // getpid, fork, getppid
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int ret_fork = fork();
    if (ret_fork < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (ret_fork == 0) {
        // child (new process)
        printf("안녕 ? \n");
	system("touch /tmp/foofoofoo");
    } else {
        // parent goes down this path (original process)
        while(access("/tmp/foofoofoo", 00) == -1);
	system("rm -rf /tmp/foofoofoo");
	printf("잘가 \n");
	
    }
    return 0;
}
