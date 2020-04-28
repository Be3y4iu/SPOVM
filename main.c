#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

int main()
{
  sem_t* mutex = NULL;

  int32_t file_descriptor[2];
  int8_t write_buffer[256], read_buffer[256];

  mutex = sem_open("sem", O_CREAT, 0644, 1);

  if(SEM_FAILED == mutex)
    {
      perror("sem_open");
      exit(-1);
    }
  
  if(-1 == sem_init(mutex, 0, 0))
    {
      perror("sem_init");
      exit(-1);
    }
    
  if(-1 == pipe(file_descriptor))
    {
      perror("pipe");
      exit(-1);
    }
  
  pid_t p = fork();

  switch(p)
    {
    case -1:
      {
	perror("fork");
	exit(-1);
      }
    case 0:
      {
	int32_t child_mutex;
	close(file_descriptor[1]);
	while(TRUE)
	  {
	    sem_getvalue(mutex, &child_mutex);
	    
	    
		read(file_descriptor[0], read_buffer, sizeof(read_buffer));
		printf("The transmitted string: %s", read_buffer);
		sem_wait(mutex);
	    
	  }
	close(file_descriptor[0]);  	
	exit(0);
	break;
      }
      
    default:
      {
	int32_t parent_mutex;
	close(file_descriptor[0]);
	while(TRUE)
	  {
	    sem_getvalue(mutex, &parent_mutex);
	    
	    
		printf("Enter the string\n");

		fgets(write_buffer, sizeof(write_buffer), stdin);

		printf("Ready to transmit\n");

		write(file_descriptor[1], write_buffer, strlen(write_buffer) + 1);

		printf("The string has been sent\n");

		sem_post(mutex);
	    
	  }
	waitpid(p, NULL, 0);
	exit(0);
	
	break;
      }
    }
  
  return 0;
}




