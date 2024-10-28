#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
int main(int argc, const char *argv[]){
  printf("Hello world. I'm the server. %d\n", getpid());
  const int fd = socket(AF_UNIX,SOCK_STREAM,0);
  if (fd == -1){
    printf("Faild to open socket");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_un add;
  add.sun_family = AF_UNIX;
  strcpy(add.sun_path, "/tmp/testpazh");
  if(bind(fd, (const struct sockaddr*) &add, sizeof(struct sockaddr_un))){
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
  char buf[1024];
  read(acceptedSock, buf, 1024);
  printf("%s", buf);
}
