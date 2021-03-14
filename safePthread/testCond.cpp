#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define EATCOUNT 2
#define WORKCOUNT 2

int g_bowl = 0;
pthread_mutex_t lock;
pthread_cond_t wcond;
pthread_cond_t econd;

void* eatFun(void* arg)
{
  (void)arg;
  while(1)
  {
    pthread_mutex_lock(&lock);
    while(g_bowl <= 0)
    {
      pthread_cond_wait(&econd,&lock);
    }
    printf("I am eat:%p,i eat : %d\n",pthread_self(),g_bowl);
    g_bowl--;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&wcond);
  }
  return NULL;
}
void* workFun(void* arg)
{
  (void)arg;
  while(1)
  {
    pthread_mutex_lock(&lock);
    while(g_bowl > 0)
    {
      pthread_cond_wait(&wcond,&lock);
    }
    g_bowl++;
    printf("I am work:%p,i work:%d\n",pthread_self(),g_bowl);
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&econd);
  }
  return NULL;
}

int main()
{
  pthread_mutex_init(&lock,NULL);//初始花互斥锁

  pthread_cond_init(&wcond,NULL);//初始化条件变量
  pthread_cond_init(&econd,NULL);//初始化条件变量

  pthread_t eat[EATCOUNT];
  pthread_t work[WORKCOUNT];

  for(int i = 0;i< EATCOUNT;i++)
  {
    //创建消费者线程
    int ret = pthread_create(&eat[i],NULL,eatFun,NULL);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }

  }
  for(int i = 0;i < WORKCOUNT;i++)
  {
    //创建生产者线程
    int ret = pthread_create(&work[i],NULL,workFun,NULL);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }
  }

  for(int i = 0;i < EATCOUNT;i++)
  {
    pthread_join(eat[i],NULL);
  }
  for(int i = 0; i < WORKCOUNT;i++)
  {
    pthread_join(work[i],NULL);
  }
  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&wcond);
  pthread_cond_destroy(&econd);
  return 0;
}
