#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define PTHREADCOUNT 2

int g_bowl = 0;
pthread_mutex_t lock_;
pthread_cond_t pcond;//生产者条件变量
pthread_cond_t ccond;//消费者条件变量
  void* productFunc(void* arg)
  {
    (void)arg;
    while(1)
    {
      pthread_mutex_lock(&lock_);
      while(g_bowl > 0)
      {
        pthread_cond_wait(&pcond,&lock_);
      }
      g_bowl++;
      printf("i am productPthread:%p,i am product:%d\n",pthread_self(),g_bowl);
      pthread_mutex_unlock(&lock_);
      pthread_cond_signal(&ccond);
      sleep(1);
    }
    pthread_mutex_unlock(&lock_);
    return NULL;
  }
  void* consumFunc(void* arg)
  {
    (void)arg;
    while(1)
    {
      pthread_mutex_lock(&lock_);
      while(g_bowl <= 0)
      {
        pthread_cond_wait(&ccond,&lock_);
      }
      printf("I am consumePthread:%p,i am consume:%d\n",pthread_self(),g_bowl);
      g_bowl--;
      pthread_mutex_unlock(&lock_);
      pthread_cond_signal(&pcond);
      sleep(1);
    }
    pthread_mutex_unlock(&lock_);
    return NULL;
  }
int main()
{
  pthread_mutex_init(&lock_,NULL);//初始化互斥锁
  pthread_cond_init(&pcond,NULL);//初始化生产者条件变量
  pthread_cond_init(&ccond,NULL);//初始化消费者条件变量

  pthread_t product[PTHREADCOUNT];
  pthread_t consume[PTHREADCOUNT];

  for(int i = 0;i < PTHREADCOUNT;i++)
  {
    int ret = pthread_create(&product[i],NULL,productFunc,NULL);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }
    ret = pthread_create(&consume[i],NULL,consumFunc,NULL);
    if(ret < 0)
    {
      perror("pthead_create");
      return -1;
    }
  }
  for(int i = 0;i < PTHREADCOUNT;i++)
  {
    pthread_join(consume[i],NULL);
    pthread_join(product[i],NULL);
  }

  pthread_mutex_destroy(&lock_);//销毁互斥锁
  pthread_cond_destroy(&pcond);//销毁生产者条件变量
  pthread_cond_destroy(&ccond);//销毁消费者条件变量
  return 0;
}
