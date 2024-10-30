#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
  printf("Hello world im a client. %d\n", getpid());
  const int fd = socket(AF_INET,SOCK_STREAM,0);
  if (fd == -1){
    printf("Faild to open socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in add;
  add.sin_family = AF_INET;
  inet_aton(IP, &add.sin_addr);
  add.sin_port = 5001;
  if(connect(fd, (const struct sockaddr*) &add, sizeof(struct sockaddr_in))){
    printf("Faild to connect socket. errno: %d", errno);
    exit(EXIT_FAILURE);
  }

  char buf[1024];
  read(fd, buf, 1024);
  printf("%s", buf);

  for (int inLen = read(STDIN_FILENO, buf, 1023); inLen; inLen = read(STDIN_FILENO, buf, 1023)) {
    buf[inLen] = 0;
    write(fd, buf, inLen + 1);
  }
}
