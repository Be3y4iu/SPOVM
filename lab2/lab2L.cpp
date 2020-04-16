#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
 
#define MAX_COUNT 11
 
char name[10][30] =
{
    {"\r\n|Number 1"},
    {"\r\n|Number 2"},
    {"\r\n|Number 3"},
    {"\r\n|Number 4"},
    {"\r\n|Number 5"} ,
    {"\r\n|Number 6"},
    {"\r\n|Number 7"},
    {"\r\n|Number 8"},
    {"\r\n|Number 9"},
    {"\r\n|Number 10"},
};
pid_t pid0fChild[MAX_COUNT];
int flagofPrint = 0; 
int flagofEnd = 1;
int current = 1;
int size = 0;
struct sigaction printSignal, endSignal;
 
void checkEnd()
{
    if(flagofEnd)
    {
          kill(getppid(),SIGUSR2);
          exit(0);
    }
}
 
int kbhit()
{
    fd_set readfds;
    struct timeval timevalue;
 
    FD_ZERO(&readfds);
    FD_SET(0,&readfds);
    timevalue.tv_sec=0;
    timevalue.tv_usec=100;
 
    return select(1,&readfds,NULL,NULL,&timevalue);
}
 
void ifcanPrint(int signo)
{
  flagofPrint = 1;
}
 
void setEndFlag(int signo)
{
  flagofEnd = 1;
}
 
int main(void)
{
      initscr();
      clear();
      noecho();
      refresh();
 
      printSignal.sa_handler = ifcanPrint;
      sigaction(SIGUSR1,&printSignal,NULL);
 
      endSignal.sa_handler = setEndFlag;
      sigaction(SIGUSR2,&endSignal,NULL);
 
      char c = 0;
      int i = 0;
 
     pid0fChild[0]=getpid();
      while(c!='q')
      {
          if(kbhit>0)
          {
              switch(c=getchar())
              {
                  case '+':
                  {
                      if(size==10){break;}
                      if(size < MAX_COUNT)
                      {
                          size++;
                          pid0fChild[size] = fork();
                      }
                      switch(pid0fChild[size])
                      {
                          case 0: 
                          {
                              flagofEnd = 0;
                              while(!flagofEnd)
                              {
                                    usleep(20000);
                                    if(flagofPrint)
                                    {
                                        for(i=0; i<strlen(name[size-1]); i++)
                                        {
                                            checkEnd();
                                            printf("%c",name[size-1][i]);
                                            refresh();
                                            usleep(40000);
                                        }
                                        checkEnd();
                                        refresh();
                                        flagofPrint = 0;
                                        kill(getppid(),SIGUSR2); 
                                    }
                                    checkEnd();
                              }
                              return 0;
                           }
                           break;
 
                           case -1:
                           {
                               printf("Child process[%d] failed!\n",size);
                           }
                           break;
                      }
                  }
                  break;
 
                  case '-':
                  {
                        if(size==0)
                           break;
                        kill(pid0fChild[size],SIGUSR2); 
                        if(current == size) current = 1;
                        size--;
                  }
                  break;
              }
          }
          if(flagofEnd && size>0)      
          {
              flagofEnd = 0;
              kill(pid0fChild[current++],SIGUSR1); 
              if(current > size)    
                  current = 1;                  
          }
          refresh();
      }
      if(pid0fChild[size]!=0)
      {
         for(;size>0;size--)
         {
             kill(pid0fChild[size],SIGUSR2);
             waitpid(pid0fChild[size],NULL,0);
         }
      }
      clear();
      endwin();
      return 0;
}
