#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#include <iostream>
using namespace std;


void sigcb(int signo)
{
  wait(NULL);
}
int main()
{
  //创建侦听套接字
  int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
  if(listen_sock < 0)
  {
    perror("listen_sock");
    return -1;
  }
  //绑定地址信息
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr("192.168.115.133");
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
  signal(SIGCHLD, sigcb);
  while(1)
  {
    //接收新连接
    int new_sock = accept(listen_sock, NULL, NULL);
    if(new_sock < 0)
    {
      perror("accept");
      continue;
    }
    pid_t pid = fork();
    if(pid < 0)
    {
      perror("fork");
      continue;
    }
    else if(pid == 0)
    {
      while(1)
      {

        //接收数据i
        close(listen_sock);
        char buf[1024] = {0};
        ssize_t recv_size = recv(new_sock,buf,sizeof(buf) - 1, 0);
        if(recv_size < 0)
        {
          perror("recv");
          continue;
        }
        else if(recv_size == 0)
        {
          cout << "peer shutdown!" << endl;
          close(new_sock);
          return 0;
        }
        cout << "cli say:" << buf << endl;
        //清空缓冲区
        memset(buf, '\0', sizeof(buf));
        //发送数据
        cout << "please enter massage to cli:";
        //刷新缓冲区
        fflush(stdout);
        cin >> buf;
        ssize_t send_size = send(new_sock, buf, sizeof(buf) - 1, 0);
        if(send_size < 0)
        {
          perror("send");
          continue;
        }
      }
    }

  close(new_sock);
  }
  return 0;
}
