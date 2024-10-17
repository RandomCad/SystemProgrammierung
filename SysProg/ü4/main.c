#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

#define FIFO_PATH "/tmp/myfifo" // Path to the named pipe
#define FIFO_PATH1 "/tmp/myfifo1" // Path to the named pipe

void activeWait(const int a){
  for(int i=0;i<a;i++){
    for(int j=0;j<a;j++){
      --j;++j;
    }
  }
}
#define ITTER 10

void writer_process() {
  int fdRead, fdWrite;
  char message[] = "Hello, named pipe!";
  char buffer[100];

   // Open the named pipe for reading
   fdRead = open(FIFO_PATH, O_RDONLY);
   if (fdRead == -1) {
      perror("open");
      exit(EXIT_FAILURE);
   }

   // Open the named pipe for writing
   fdWrite = open(FIFO_PATH1, O_WRONLY);
   if (fdWrite == -1) {
      perror("open");
      exit(EXIT_FAILURE);
   }

  int num = 0;
  srand(getpid());
  for(int i = rand()%ITTER; i > 0; --i){
    activeWait(rand()%ITTER);
    char add = rand() & 0xff;
    num += add;
    printf(".");
    write(fdWrite,&add,1);
  }

  close(fdWrite);

  char ad;
  while(read(fdRead, &ad, 1)) num += ad;
  printf("final number1 %d.\n", num);

  // Close the named pipe
  close(fdRead);
}

void reader_process() {
   int fdRead, fdWrite;
   char buffer[100];

   // Open the named pipe for writing
   fdWrite = open(FIFO_PATH, O_WRONLY);
   if (fdWrite == -1) {
      perror("open");
      exit(EXIT_FAILURE);
   }

   // Open the named pipe for reading
   fdRead = open(FIFO_PATH1, O_RDONLY);
   if (fdRead == -1) {
      perror("open");
      exit(EXIT_FAILURE);
   }

  int num = 0;
  srand(getpid());
  
  for(int i = rand()%ITTER; i > 0; --i){
    activeWait(rand()%ITTER);
    char add = rand() & 0xff;
    num += add;
    printf(".");
    //write(fdWrite,&add,1);
  }
  
  close(fdWrite);

  char ad;
  while(read(fdRead, &ad, 1)) num += ad;
  printf("final number2 %d.\n", num);

   // Close the named pipe
   close(fdRead);
}

int main() {
   pid_t pid;

   // Create the named pipe
   mkfifo(FIFO_PATH, 0666);
   mkfifo(FIFO_PATH1, 0666);

   // Fork a child process
   pid = fork();

   if (pid == -1) {
      perror("fork");
      exit(EXIT_FAILURE);
   } else if (pid == 0) {
      // Child process (writer)
      writer_process();
   } else {
      // Parent process (reader)
      reader_process();

      // Wait for the child process to finish
      wait(NULL);

      // Remove the named pipe
      unlink(FIFO_PATH);
   }
   return 0;
}
