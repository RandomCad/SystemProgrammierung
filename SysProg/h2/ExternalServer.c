#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "commen.h"

int main(int argc, const char *argv[]){
  ///CLI Parsing:
  if(argc != 3){
    printf("Pleas give the IP in the -ip option");
    exit(EXIT_FAILURE);
  }
  const char *IP;
  for (argv++; *argv; ++argv) {
    if(!strcmp(*argv, "-ip")){
      IP = *(++argv);
    }
  }

  printf("Hello world. I'm the server. %d\n\n", getpid());

  const int fd = socket(AF_INET,SOCK_STREAM,0);
  if (fd == -1){
    printf("Faild to open socket");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in add;
  add.sin_family = AF_INET;
  inet_aton(IP, &add.sin_addr);
  add.sin_port = 5001;
  if(bind(fd, (const struct sockaddr*) &add, sizeof(struct sockaddr_in))){
    printf("Faild to bind socket. errno: %d", errno);
    exit(EXIT_FAILURE);
  }
  
  if(listen(fd, 1)){
    printf("Faild to listen socket. errno: %d", errno);
    exit(EXIT_FAILURE);
  }

  int acceptedSock = accept(fd, NULL, NULL);
  if(acceptedSock < 0){
    printf("Failed to accept the call");
    exit(EXIT_FAILURE);
  }

  write(acceptedSock, SERVER_INTRO, sizeof(SERVER_INTRO));

  char buf[1024];
  while (read(acceptedSock, buf, 1024)) printf("%s", buf);
}
