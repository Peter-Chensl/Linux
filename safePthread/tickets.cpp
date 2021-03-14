//改进版抢票程序
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PTHREADCOUNT 4
int g_ticket = 100;

pthread_mutex_t lock;

void* myPthread(void* arg)
{
  (void)arg;
  pthread_mutex_lock(&lock);
  while(1)
  {
    if(g_ticket > 0)
    {
      printf("I am workPthread:%p,i hava:%d\n",pthread_self(),g_ticket);
      g_ticket--;
    }
    else 
    {
      pthread_mutex_unlock(&lock);
      break;
    }
  }
  pthread_mutex_unlock(&lock);
}
int main()
{
  pthread_t tid[PTHREADCOUNT];
  pthread_mutex_init(&lock,NULL);
  for(int i = 0;i < PTHREADCOUNT;i++)
  {
   int ret =  pthread_create(&tid[i],NULL,myPthread,NULL);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }
  }
  for(int i = 0;i < PTHREADCOUNT;i++)
  {
    pthread_join(tid[i],NULL);
  }
  pthread_mutex_destroy(&lock);
  return 0;
}
