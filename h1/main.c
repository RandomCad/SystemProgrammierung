//includes für Standardelemente
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>


///Eine Funktion, die, wenn sie nicht optimiert ist, etwas Zeit verschwendet activ.
///@param a Gibt an, wie lange gewartet werden soll. 
/// Ist der Wert größer als 1_000_000, dauert es  eine merkliche Zeit.
/// Verhält sich quadratisch!
void activeWait(const int a){
  //Externe Warteschlange
  for(int i=0;i<a;i++){
    //Die innere Warteschleife dient zusammen mit der äußeren Schleife dazu, genügend Schleifenumläufe zu haben.
    for(int j=0;j<a;j++){
      //Inkrementieren und Dekrementieren des Zählers für maximale Auslastung und unnötige Verschwendung von CPU-Zeit
      --j;++j;
    }
  }
}

//Inkluds für Semaphore
#include			<sys/types.h>
#include			<sys/ipc.h>
#include			<sys/sem.h>

//prüft, ob ERWARTUNG_ANZAHL vom Compiler gesetzt wurde
#ifndef ERWARTUNG_ANZAHL
#define ERWARTUNG_ANZAHL 1 // Setzt ERWARTUNG_ANZAHL auf 1, falls nicht vom Compiler gesetzt.
#endif // Beendet den Bereich, auf den sich der IFNDEF bezieht.
//prüft, ob ZWISCHENSTOP vom Compiler gesetzt wurde
#ifndef ZWISCHENSTOP
#define ZWISCHENSTOP 0 // Setzt ZWISCHENSTOP auf 0, falls nicht vom Compiler gesetzt.
#endif // Beendet den Bereich, auf den sich der IFNDEF bezieht.

//Erstellung eines Makros zur weiteren Verwendung
#define PRINTING(VALUE) for (int i = 0; i < am; i++){ /*Schleife zur Ausgabe eines Zeichens (am heufig)*/ \
        activeWait((rand()%1000)*ZWISCHENSTOP); /*wartet eine zufällige Zeit. Mit Zwischenstopp wird ein Verlängerungs- oder Verkürzungsfaktor angegeben.*/\
        semop(semid,&buf,1); /*Erhalte die Semaphore. Wenn sie gerade verwendet wird, wird gewartet */\
        for (int rand_Name=0; rand_Name < ERWARTUNG_ANZAHL; rand_Name++){ /*führt zur Ausgabe von ERWARTET_ANZAHL mit vieler Zeichen*/\
        activeWait(rand()%1000); /*wartet eine zufällige Zeit */\
        printf(VALUE); }/*VALUE nach stdout ausgeben */\
        fflush(stdout); /*Ausgabe von stdout auf der Kommandozeile erzwingen */\
        buf.sem_op = 1; /*Buf so ändern, dass die Semaphore inkrementiert wird -> kann von einem anderen verwendet werden.*/ \
        semop(semid,&buf,1); /*Operation auf die Semaphore anwenden -> sie wird freigegeben*/\
        buf.sem_op = -1; /*Rücksetzen der Semaphoren Operation zur Belegung der Semaphoren*/\
      } 

///Hauptaufruf des Programms
///Parameter werden nicht verwendet
int main(int argc, const char * argv[]){
  //Seeding der Zufallsfunktion, um bei mehrfacher Ausführung verschiedene Zufallszahlen zu erhalten.
  //Diese werden verwendet, um die Wartezeiten zufällig zu gestalten.
  srand(time(NULL));
  //Anzahl der auszugebenden Zeichen. Beeinflusst die Programmdauer mit
  const int am = 100;

  //Erstellung oder Beschaffung der Semaphore. Nete Umgebung wird angenommen -> keine Prüfung
 	int		semid = semget(ftok("/dev/smh", 1), 1, IPC_CREAT | 0777);;			/* return value from semget() */
  
  //Initialisiere die Semaphore mit dem Wert 1.
  //Damit kann die Semaphore beim ersten Aufruf betreten werden.
  semctl(semid, 0, SETVAL, 1);

  //Generierung der Struktur zur Änderung der Semaphore
  struct sembuf buf;
  //Es wird die Semaphore 0 (die einzige) unserer Semaphorengruppe verwendet.
  buf.sem_num = 0;
  //Es wird versucht, die Semaphore zu erhalten.
  //Dazu wird geprüft, ob der interne Wert um 1 vermindert werden kann und kein negatives Ergebnis entsteht.
  //Die Semaphore kann verwendet werden, wenn sie zu Beginn einen Wert größer 1 hat -> deshalb Z41.
  buf.sem_op = -1;
  //Keine weiteren Flags erforderlich
  buf.sem_flg = 0;

  //forken und erkennen, in welche Richtung man forkt(aus fehlerfall)
  switch (fork()) {
    case 0:
      //Erneutes seeding, so dass die beiden Prozesse unterschiedliche Zufallszahlen haben.
      srand(getpid() + rand());
      //Aufruf des Ausgabemakros
      PRINTING("V");
      break;
    case -1:
      //Fork wurde nicht durchgeführt Programm ist sinnlos!
      exit(EXIT_FAILURE);
    default:
      //Aufruf des Ausgabemakros
      PRINTING("s");
      break;
  }
}
