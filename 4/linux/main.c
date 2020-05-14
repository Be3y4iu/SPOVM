#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <syscall.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define THREADS_NUMBER 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void print(char* thread_id);

void* thread_print(void* args)
{
  char thread_id[256];
  int cancel_state = 0;
  
  sprintf(thread_id, "My id is %ld", syscall(SYS_gettid));
  
  while(TRUE)
    {
            
      cancel_state = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

      if(0 != cancel_state)
	{
	  perror("setcancelstate");
	  exit(EXIT_FAILURE);
	}
      
      pthread_mutex_lock(&mutex);
      print(thread_id);
      pthread_mutex_unlock(&mutex);

      cancel_state = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

      if(0 != cancel_state)
	{
	  perror("setcancelstate");
	  exit(EXIT_FAILURE);
	}
      
      usleep(100);
    }
  
  pthread_exit(NULL);
}

int main()
{
  pthread_t thread[THREADS_NUMBER]; 
  
  int position = 0;
  
  while(TRUE)
    {
      char symbol = getchar();

      switch(symbol)
	{
	case '+':
	  {
	    if(position >= 10)
	      {
		printf("Thread list is full\n");
		break;
	      }
	    
	    pthread_create(&thread[position], NULL, &thread_print, NULL);
	    
	    position++;
	    
	    break;
	  }
	case '-':
	  {
	    if(position <= 0)
	      {
		printf("Unable to terminate...nothing?");
		break;
	      }
	    
	    pthread_cancel(thread[position - 1]);
	    pthread_join(thread[position - 1], NULL);
	    
	    position--;
	    
	    break;
	  }
	case 'q':
	  {
	    for (int i = 0; i < position; i++)
	      {
		pthread_cancel(thread[i]);
		// could be used to wait for threads end printing 
		/* pthread_join(thread[i], NULL); */ 
	      }
	    
	    exit(EXIT_SUCCESS);
	  }
	  
	}
    }
  
  return 0;
}

void print(char* thread_id)
{
  for(int i = 0; i < strlen(thread_id); i++)
    {
      printf("%c", thread_id[i]);
      fflush(stdout);
      usleep(100000);
    }
  printf("\n");
  fflush(stdout);
}
