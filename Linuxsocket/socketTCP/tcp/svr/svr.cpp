#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
using namespace std;

int main()
{
  //创建套接字
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0)
  {
    perror("socket");
    return -1;
  }
  //填充结构体
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(19898);
  addr.sin_addr.s_addr = inet_addr("192.168.115.133");
  //绑定地址信息
  int ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
  if(ret < 0)
  {
    perror("bind");
    return -1;
  }
  //侦听
  int listenfd = listen(sockfd, 1);
  if(ret < 0)
  {
    perror("listen");
    return -1;
  }
  //接收新连接
  int new_sock = accept(sockfd,NULL, NULL);
  if(new_sock < 0)
  {
    perror("accept");
    return -1;
  }
  while(1)
  {
    //接收数据
    char buf[1024] = {0};
    ssize_t recv_size = recv(new_sock,buf, sizeof(buf) - 1, 0);
    if(recv_size < 0)
    {
      perror("recv");
      continue;
    }
    else if(recv_size == 0)
    {
      cout << "perr shutdown" << endl;
      close(new_sock);
      close(sockfd);
      return 0;
    }
    cout << "cli say:" << buf << endl;
    memset(buf, '\0', sizeof(buf));
    cout << "plase enter massage to cli:";
    fflush(stdout);
    cin >> buf;
    ssize_t send_size = send(new_sock, buf, sizeof(buf) - 1,0);
    if(send_size < 0)
    {
      perror("send");
      continue;
    }
    //发送数据
  }
  //关闭套接字
  
  close(sockfd);
  return 0;
}
