#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main()
{
  //创建侦听套接字
  int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
  if(listen_sock < 0)
  {
    perror("socket");
    return -1;
  }
  //填充结构体
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(19998);
  addr.sin_addr.s_addr = inet_addr("0.0.0.0");
  //绑定地址信息
  int ret = bind(listen_sock,(struct sockaddr*)&addr, sizeof(addr));
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
  int new_sock = -1;
  while(1)
  {
    //接收新连接
    new_sock = accept(listen_sock, NULL, NULL);
    if(new_sock < 0)
    {
      perror("accept");
      return -1;
    }
    //接收数据
    char buf[1024] = {0};
    ssize_t size_recv = recv(new_sock, buf,sizeof(buf) - 1, 0);
    if(size_recv < 0)
    {
      perror("recv");
      continue;
    }
    else if(0 == size_recv)
    {
      cout << "peer shutdown!" << endl;
      close(new_sock);
      close(listen_sock);
      return 0;
    }
    cout << "client say:" << buf << endl;
    memset(buf, '\0', sizeof(buf));
    string body = "<html> hello!</html>";
    stringstream ss;
    ss << "HTTP/1.1 OK 200\r\n";
    ss << "Content-Type:text/html\r\n";
    ss << "Content-Length:" << body.size() << "\r\n";
   // ss << "\r\n";

    //发送数据
    ssize_t size_send = send(new_sock, ss.str().c_str(), ss.str().size(), 0);
    if(size_send < 0)
    {
      perror("send");
      continue;
    }
    size_send = send(new_sock, body.c_str(), body.size(),0);
    if(size_send < 0)
    {
      perror("send");
      continue;
    }
  }
  close(new_sock);
  close(listen_sock);
  return 0;
}
