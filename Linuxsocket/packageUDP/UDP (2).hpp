#pragma once 
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
using namespace std;

class UDP
{
  public:
    UDP()
    {
      sockfd_ = -1;
    }
    ~UDP()
    {}
    //创建套接字
    int CreateSocket()
    {
      sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
      if(sockfd_ < 0)
      {
        perror("socket");
        return -1;
      }
      return 0;
    }
    //建立连接
    int Bind(string ip,uint16_t port)
    {
      //填充数据结构
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());

      //绑定地址信息
      int ret = bind(sockfd_, (struct sockaddr*)&addr,sizeof(addr));
      if(ret < 0)
      {
        perror("bind");
        return -1;
      }
      return 0;
    }
    //接受数据
    int RecvFrom(string* data, struct sockaddr_in* peer_addr)
    {
      char buf[1024] = {0};
      socklen_t peer_addr_len = sizeof(struct sockaddr_in);
      ssize_t recv = recvfrom(sockfd_,buf,sizeof(buf)-1,0,(struct sockaddr*)peer_addr,&peer_addr_len);
      if(recv < 0)
      {
        perror("recvfrom");
        return -1;
      }
      data->assign(buf,strlen(buf));//把数据拷贝到data中，带出函数
      return recv;
    }
    //发送数据
    int Sendto(string data,struct sockaddr_in* dest_add)
    {
      ssize_t send_size = sendto(sockfd_, data.c_str(),data.size(),0,(struct sockaddr*)dest_add,sizeof(struct sockaddr_in));
      if(send_size < 0)
      {
        perror("sendto");
        return -1;
      }
      return send_size;
    }
    //关闭套接字
    void Close()
    {
      close(sockfd_);
    }
  private:
    int sockfd_;
};
