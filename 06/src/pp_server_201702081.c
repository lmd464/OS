    /*
       Homework
       Fill your codes here and make the correct result.
       Refer to the included header files and given variables.
       You can complete this file from this skeleton without creating another variable,
       but also modify anything as you want.
       As long as your result is correct, it doesn't matter how you write codes.
       However, you must use fifo, named semaphore.
       And based on this file, you have to create another source for the client.
       It would be similar to this.
    */

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define FIFO_PATH "fifo_temp"
#define SEM_NAME "sem_pp"
#define BUF_SIZE 8
#define TURN 5


int main()
{

    int fd;
    int cnt = 0;            // 턴 수
    char buf[BUF_SIZE];

    const char *msg = "ping\n";
    sem_t *p_sem;
    int score = 100;
    
    /* TODO */

    // FIFO : 보낸 내용을 기록할 파일을 지정 (없으면 생성)
    if(-1 == mkfifo(FIFO_PATH, 0600))
        printf("FIFO ERROR \n");
    else printf("FIFO OK \n");

    // 세마포어 초기화
    if((p_sem = sem_open("my_semaphore", O_CREAT, 0600, 1)) == SEM_FAILED) {
        perror("SEM_OPEN_ERR \n");
        exit(1);
    }
    else printf("SEM_OPEN OK \n");

    
    while(cnt < TURN) {
        
        // 보냄
        if(-1 == (fd = open(FIFO_PATH, O_WRONLY)))
            printf("OPEN ERR \n");
        printf("Your turn! : ");
        memset(buf, 0, BUF_SIZE);
        fgets(buf, BUF_SIZE, stdin);
        printf("%s \n", buf);
        write(fd, buf, strlen(buf));
        close(fd);
        if(strcmp(buf, msg) != 0) {
            score -= 20;
            printf("wrong! -20 \n");
        }
        cnt++;
        close(fd);
        sem_post(p_sem);


        // 받음
        sem_wait(p_sem);     
        if(-1 == (fd = open(FIFO_PATH, O_RDONLY)))
            printf("OPEN ERR \n");
        memset(buf, 0, BUF_SIZE);
        read(fd, buf, BUF_SIZE);
        printf("[opponent] %s \n", buf);
        close(fd);

    }


    printf("Done! Your score: %d\n", score);
    return 0;
}
