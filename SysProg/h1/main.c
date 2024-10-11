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

#include			<sys/types.h>
#include			<sys/ipc.h>
#include			<sys/sem.h>

int main(int argc, const char * argv[]){
  srand(time(NULL));
  const int am = 100;

 	int		semflg;			/* semflg to pass to semget() */
 	int		nsems;			/* nsems to pass to semget() */
 	int		semid;			/* return value from semget() */
  semid = semget(ftok("/home/prog/test", 1), 1, IPC_CREAT | 0777);
  
  semctl(semid, 0, SETVAL, 1);

  struct sembuf buf;
  buf.sem_num = 0;
  buf.sem_op = -1;
  buf.sem_flg = 0;

  if(fork()){
    srand(getpid());
    for (int i = 0; i < am; i++){
      semop(semid,&buf,1);
      activeWait(rand()%1000);
      printf("V");
      fflush(stdout);
      buf.sem_op = 1;
      semop(semid,&buf,1);
    }
  }
  else{;
    for (int i = 0; i < am; i++){
      semop(semid,&buf,1);
      activeWait(rand()%1000);
      printf("s");
      fflush(stdout);
      buf.sem_op = 1;
      semop(semid,&buf,1);
    }
  }
}
