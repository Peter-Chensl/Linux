#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define PTHREADCOUNT 1
pthread_mutex_t lock_;
pthread_mutex_t lock2_;
void* lock1(void* arg)
{
  (void)arg;
  pthread_mutex_lock(&lock_);
  sleep(2);

  pthread_mutex_lock(&lock2_);
  pthread_mutex_unlock(&lock2_);
  pthread_mutex_unlock(&lock_);
  return NULL;
}
void* lock2(void* arg)
{
  (void)arg;
  pthread_mutex_lock(&lock2_);
  sleep(2);
  pthread_mutex_lock(&lock_);
  pthread_mutex_unlock(&lock_);
  pthread_mutex_unlock(&lock2_);
  return NULL;
}
int main()
{
  pthread_t tid[PTHREADCOUNT],tid2[PTHREADCOUNT];
  pthread_mutex_init(&lock_,NULL);
  pthread_mutex_init(&lock2_,NULL);

  for(int i = 0;i < PTHREADCOUNT;i++)
  {
    int ret = pthread_create(&tid[i],NULL,lock1,NULL);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }
    ret = pthread_create(&tid2[i],NULL,lock2,NULL);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }
  }
  for(int i = 0;i < PTHREADCOUNT;i++)
  {
    pthread_join(tid[i],NULL);
    pthread_join(tid2[i],NULL);
  }
  return 0;
}
