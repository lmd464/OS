#include <stdio.h>
#include <stdlib.h> // exit
#include <unistd.h> // fork, getpid

int global_int = 1000;

int main(int argc, char *argv[])
{
    int local_int = 10;

    printf("global int : %d \n", global_int);
    printf("local int : %d \n\n", local_int);

    int ret_fork = fork();
    if (ret_fork < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } 
    

    // Child
    else if (ret_fork == 0) {
        printf("Child pid : %d \n", (int) getpid());
	
   	printf("child global int before : %d \n", global_int);
    	printf("child local int before: %d \n\n", local_int);

 
	// Child's Modification
	global_int = 99999;
	local_int = 100000;
	printf("Child changed global, local var to 99999, 100000 \n");
	sleep(2);

	// Print after Parent's Modification
	printf("[After Parent modified] \n");
	printf("child global : %d \n", global_int);
	printf("child local : %d \n", local_int);
	printf("\n");
	

    // Parent
    } else {
	
    	printf("parent global int before : %d \n", global_int);
   	printf("parent local int before : %d \n\n", local_int);


	// Print after Child's Modification
	sleep(1);
	printf("[After Child modified] \n");
	printf("parent global : %d \n", global_int);
	printf("parent local : %d \n", local_int);
	printf("\n");
	
	// Parent's Modification
	global_int = 99999999;
	local_int = 100000000;
	printf("Parent changed global, local var to 99999999, 100000000 \n");
    }



    return 0;
}
