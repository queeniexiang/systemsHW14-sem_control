/* 
Queenie Xiang
Systems pd5
HW14 -- Get sem control!
2017-12-06
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h> 
#include <string.h>

#define KEY 12

// separates a string based on delimteer and makes it into an array
// returns an array
char ** parse_args(char * line, char * delimiter ) {
  char **args = calloc(100, sizeof(char *));
  int i = 0;
  char *comm;
  while (line != NULL) {
    comm = strsep(&line, delimiter);
    if (strlen(comm) != 0) { //adds to array only if it is not an empty string
      args[i] = comm;
      i++;
    }
  }
  args[i] = NULL;
  return args;
}

void print_args(char **arr) {
  int i = 0;
  while ( *(arr + i) != NULL ) {
    printf("command: %s\n", arr[i]);
    i++;
  }
}

int main() {
  char ** args; 
  char str[100];
  int i = 0;
  int sem_descriptor; 
  while (fgets(str, sizeof(str), stdin) ) {
    //Removing extraneous /n at the end 
    str[strlen(str)-1] = 0;

    //Parsing through str by blank space 
    args = parse_args(str, " ");
    
    //print_args(args);

    //Creating a new semaphore
    if (strcmp(args[0], "-c") == 0) {
      //Create a semaphore 
      sem_descriptor = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0664);
      
      if (sem_descriptor == -1) {
	printf("Error! Semaphore already exists\n");
      }
      
      else { 
	printf("Semaphore created: %d\n", sem_descriptor);
	printf("Value set: %d\n",semctl(sem_descriptor, 0, GETVAL));
	//Set semaphore value to what user specified 
	semctl(sem_descriptor, 0, SETVAL, atoi(args[1])); 
      }
    }

    //Returning value of semaphore 
    else if (strcmp(args[0], "-v") == 0) {
      sem_descriptor = semget(KEY, 1, 0664); 
      printf("Semaphore value: %d\n",semctl( sem_descriptor, 0, GETVAL ));
    }

    //Removing semaphore 
    else if (strcmp(args[0], "-r") == 0) {
      sem_descriptor = semget(KEY, 1, 0664);
      int status = semctl(sem_descriptor, 1, IPC_RMID);
      
      if (status == 0) {
	printf("Semaphore removed successfully!\n");
      }
      else {
	printf("Removing failed.\n");
      }
    }
    else {
      printf("Command not recognized, please try again.\n");
    }
  }
  return 0;
}

