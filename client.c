#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

int spid;

static void sighandler(int signo){
    if (signo == SIGINT){
        kill(spid, SIGQUIT);
        exit(0);
    }
}

void handshake_c(){
    char buffer[256];
    sprintf(buffer, "%d", getpid() );
    mkfifo(buffer, 0666); //private pipe
    int fd, ack;
    fd = open("wkp", O_WRONLY);
    printf("Sending private FIFO name to server\n");
    write(fd, buffer, sizeof(buffer));  //sending private name
    

    ack = open(buffer, O_RDONLY);
    char msg[256];
    read(ack, msg, sizeof(msg));
    printf("Acknowledgement received: %s\n\n",msg);
    spid = atoi(msg);
    remove(buffer);
    close(ack);

    char ack_r[] = "Acknowledgement received";
    write(fd, ack_r,sizeof(ack_r));
    close(fd);
}

int main(){
    signal(SIGINT, sighandler);
    int fd;
    handshake_c();
    mkfifo("inpu", 0644);

    printf("All inputs are changed to lower case.\n\n");
    while(1){
        fd = open("inpu", O_WRONLY);
        char line[100];
        printf("Input a string.\n");
        fgets(line,sizeof(line), stdin);
        write(fd,line,sizeof(line));
        close(fd);

        fd = open("outpu",O_RDONLY);
        read(fd,line,sizeof(line));
        printf("Output: %s\n", line);
        close(fd);
    }
}