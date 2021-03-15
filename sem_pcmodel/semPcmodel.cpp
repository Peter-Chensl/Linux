#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <vector>
#include <iostream>
using namespace std;

#define CAPACITY 1
#define PTHREADCOUNT 1
class RingQueue
{
  public:
    RingQueue()
      :vec_(CAPACITY)
    {
      capacity_ = CAPACITY;
      sem_init(&lock_,0,1);
      sem_init(&proud, 0, capacity_);
      sem_init(&consum, 0 ,0);

      pos_read = 0;
      pos_write = 0;
    }
    ~RingQueue()
    {
      sem_destroy(&lock_);
      sem_destroy(&proud);
      sem_destroy(&consum);
    }
    void Push(int data)
    {
      sem_wait(&proud);

      sem_wait(&lock_);
      vec_[pos_write] = data;
      pos_write = (pos_write + 1) % capacity_;
      sem_post(&lock_);
      sem_post(&consum);
    }
    void Pop(int* data)
    {
      sem_wait(&consum);

      sem_wait(&lock_);
      *data = vec_[pos_read];
      pos_read = (pos_read +1) % capacity_;

      sem_post(&lock_);
      sem_post(&proud);
    }
  private:
    vector<int> vec_;
    int capacity_;

    sem_t lock_;
    sem_t proud;
    sem_t consum;

    int pos_write;
    int pos_read;
};
void* writeFun(void* arg)
{
  RingQueue *rq = (RingQueue*)arg;
  int data = 0;
  while(1)
  {
    rq->Push(data);
    printf("I am writePthread:%p,i am write:%d\n",pthread_self(),data);
    data++;
  }
  return NULL;
}
void* readFun(void* arg)
{
  RingQueue *rq = (RingQueue*)arg;
  while(1)
  {
    int data;
    rq->Pop(&data);
    printf("I am readPthread:%p,i am read:%d\n",pthread_self(),data);
  }
  return NULL;
}
int main()
{
  RingQueue* rq = new RingQueue();
  pthread_t product[PTHREADCOUNT],consume[PTHREADCOUNT];
  for(int i = 0;i < PTHREADCOUNT;i++)
  {
    int ret = pthread_create(&product[i],NULL,writeFun,(void*)rq);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }
    ret = pthread_create(&consume[i],NULL, readFun,(void*)rq);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }
  }
  for(int i = 0;i < PTHREADCOUNT;i++)
  {
    pthread_join(product[i],NULL);
    pthread_join(consume[i],NULL);
  }
  delete  rq;
  rq = NULL;
  return 0;
}
