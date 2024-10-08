#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
void activeWait(const int a){
  for(int i=0;i<a;i++){
    for(int j=0;j<a;j++){
      --j;++j;
    }
  }
}

int main(int argc, const char * argv[]){
  srand(time(NULL));
  int ran=rand();
  for(int i=10;i<ran;i++){
    activeWait(0xffffffff^0x10000000);
  }
  
  printf("Hi i'm process: %d\n", getpid());
  for(int i=10;i<(0xffffffff^0x10000000)-ran;i++){
    activeWait(0xffffffff^0x10000000);
  }
}
