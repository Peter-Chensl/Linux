//多线程版本的TCP代码
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
using namespace std;

struct sockVal
{
  int new_sock_;
};

void* TCPthread(void* arg)
{
  //分离线程
  pthread_detach(pthread_self());
  sockVal* sv = (struct sockVal*)arg;
  while(1)
  {
    //接收数据
    char buf[1024] = {0};
    ssize_t recv_size = recv(sv->new_sock_, buf, sizeof(buf) - 1, 0);
    if(recv_size < 0)
    {
      perror("recv");
      continue;
    }
    else if(recv_size == 0)
    {
      cout << "peer shutdown!" << endl;
      close(sv->new_sock_);
      delete sv;
      return NULL;
    }
    cout << "client say:" << buf << endl;
    //清空缓冲区
    memset(buf, '\0', sizeof(buf));
    //发送数据
    cout << "please enter massage to client:";
    fflush(stdout);
    cin >> buf;
    ssize_t send_size = send(sv->new_sock_, buf, sizeof(buf) - 1, 0);
    if(send_size < 0)
    {
      perror("send");
      continue;
    }

  }
  return NULL;
}

int main()
{
  //创建侦听套接字
  int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
  if(listen_sock < 0)
  {
    perror("socket");
    return -1;
  }
  //绑定地址信息
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr("0.0.0.0");
  addr.sin_port = htons(19898);
  int ret = bind(listen_sock, (struct sockaddr*)&addr, sizeof(addr));
  if(ret < 0)
  {
    perror("bind");
    return -1;
  }
  //侦听
  ret = listen(listen_sock, 5);
  if(ret < 0)
  {
    perror("listen");
    return -1;
  }
  while(1)
  {
    //接收新连接
    int new_sock = accept(listen_sock, NULL, NULL);
    if(new_sock < 0)
    {
      perror("accept");
      continue;
    }
      //创建线程
    pthread_t tid;
    sockVal* sv = new sockVal();
    sv->new_sock_ = new_sock;
    ret = pthread_create(&tid,NULL,TCPthread, (void*)sv);
    if(ret < 0)
    {
      perror("pthread_create");
      continue;
    }

    
  }
  close(listen_sock);
  return 0;
}
