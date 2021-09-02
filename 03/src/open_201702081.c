#include <stdio.h>
#include <stdlib.h>     // exit
#include <unistd.h>     // fork, close
#include <string.h>     // strdup
#include <fcntl.h>      // open(flags, modes)
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    close(STDOUT_FILENO);
    open("./p4.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
    
    int ret_fork = fork();
    
    if (ret_fork < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);

    } else if (ret_fork == 0) {
        // child: redirect standard output to a file
	printf("자식 왔다감 \n");
	sleep(3);
	
    } else {
        // parent goes down this path (original process)
	
	printf("부모 왔다감 \n");
    		    

    }
    return 0;
}
