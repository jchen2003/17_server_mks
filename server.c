#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

void handshake_s(){
    char buffer[256];
    printf("Waiting for client connection...\n");
    mkfifo("wkp", 0666); //wkp creation
    int fd, ack;
    
    fd = open("wkp", O_RDONLY);
    read(fd, buffer, sizeof(buffer));
    printf("Client connected. Received private FIFO name: %s\n", buffer);
    remove("wkp");

    char ack_msg[256];
    sprintf(ack_msg, "%d", getpid() );
    printf("Sending acknowledgement...\n");
    ack = open(buffer, O_WRONLY);
    write(ack, ack_msg, sizeof(ack_msg));
    close(ack);

    char msg[256];
    read(fd,msg , sizeof(msg));
    printf("Handshake complete. Confirmation message: %s\n\n",msg);
    close(fd);
}

static void sighandler(int signo){
    if (signo == SIGQUIT){
        printf("Client disconnected.\n");
        handshake_s();
    }

    if (signo == SIGINT){
        remove("inpu");
        remove("outpu");
        exit(0);
    }
}


int main(){
    int fd;
    handshake_s();
    mkfifo("outpu", 0644);
    signal(SIGQUIT, sighandler);
    signal(SIGINT, sighandler);

    while(1){

        char line[100];
        fd = open("inpu", O_RDONLY);
        read(fd,line,sizeof(line));
        close(fd);

        printf("Processing Input...\n");
        sleep(.5);
        for(int i = 0; line[i]; i++){
                line[i] = tolower(line[i]);
        }

        fd = open("outpu", O_WRONLY);
        write(fd,line,sizeof(line));
        close(fd);

    }
}