#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <queue>
#include <iostream>
using namespace std;

#define PTHREADCOUNT 2

class blackQueue 
{
  public:
    blackQueue()
    {
      capacity_ = 1;
      pthread_mutex_init(&lock_,NULL);
      pthread_cond_init(&pcond,NULL);
      pthread_cond_init(&ccond,NULL);
    }
    ~blackQueue()
    {
      pthread_mutex_destroy(&lock_);
      pthread_cond_destroy(&pcond);
      pthread_cond_destroy(&ccond);
    }
    void Push(int data)
    {
      pthread_mutex_lock(&lock_);
      while(que_.size() >= capacity_)
      {
        pthread_cond_wait(&pcond,&lock_);
      }
      que_.push(data);
      pthread_mutex_unlock(&lock_);
      pthread_cond_signal(&ccond);
    }

    void Pop(int *data)
    {
      pthread_mutex_lock(&lock_);
      while(que_.empty())
      {
        pthread_cond_wait(&ccond, &lock_);
      }
      *data = que_.front();
      que_.pop();
      pthread_mutex_unlock(&lock_);

      pthread_cond_signal(&pcond);

    }
  private:
    queue<int> que_;
    size_t capacity_;

    pthread_mutex_t lock_;
    pthread_cond_t pcond;
    pthread_cond_t ccond;

};

void* productFun(void* arg)
{
   blackQueue* bq = (blackQueue*)arg;
   int data = 0;
   while(1)
   {
      bq->Push(data);
      printf("I am productPTHREAD:%p,i paoduct:%d\n",pthread_self(),data);
      data++;
   }
   return NULL;

}

void* consumeFun(void* arg)
{
  blackQueue* bq = (blackQueue*)arg;
  while(1)
  {
    int data;
    bq->Pop(&data);
    printf("I am conusmePthread:%p,i conusme:%d\n",pthread_self(),data);
  }
  return NULL;
}

int main()
{
  blackQueue* bq = new blackQueue();
  pthread_t product[PTHREADCOUNT],conusme[PTHREADCOUNT];
  for(int i = 0;i < PTHREADCOUNT;i++)
  {
    int ret = pthread_create(&product[i],NULL,productFun,(void*)bq);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }
    ret = pthread_create(&conusme[i],NULL,consumeFun,(void*)bq);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }
  }
  for(int i = 0;i < PTHREADCOUNT;i++)
  {
    pthread_join(product[i],NULL);
    pthread_join(conusme[i],NULL);
  }
  return 0;
}
