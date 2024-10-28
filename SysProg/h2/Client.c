#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
int main(int argc, const char *argv[]){
  printf("Hello world im a client. %d\n", getpid());
  const int fd = socket(AF_UNIX,SOCK_STREAM,0);
  if (fd == -1){
    printf("Faild to open socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_un add;
  add.sun_family = AF_UNIX;
  strcpy(add.sun_path, "/tmp/testpazh");
  if(connect(fd, (const struct sockaddr*) &add, sizeof(struct sockaddr_un))){
    printf("Faild to connect socket. errno: %d", errno);
    exit(EXIT_FAILURE);
  }

  char buf[1024];
  scanf("%s", buf);
  write(fd, buf, strlen(buf));
}
