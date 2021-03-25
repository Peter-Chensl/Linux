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
    perror("sockfd");
    return -1;
  }
  //填充数据结构
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr("192.168.115.133");
  addr.sin_port = htons(19898);
  //连接服务器
  int ret = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
  if(ret < 0)
  {
    perror("connect");
    return -1;
  }
  while(1)
  {
    //发送数据
    
    char buf[1024] = {0};
    cout << "please enter massage to svr:";
    //清空缓冲区
    fflush(stdout);
    cin >> buf;
    ssize_t send_size = send(sockfd, buf, sizeof(buf) - 1, 0);
    if(send_size < 0)
    {
      perror("send");
      continue;
    }
    //清空buf
    memset(buf,'\0', sizeof(buf));
    ssize_t recv_size = recv(sockfd, buf, sizeof(buf) - 1, 0);
    if(recv_size < 0)
    {
      perror("recv");
      continue;
    }
    else if(0 == recv_size)
    {
      cout << "peer shutdown!" << endl;
      close(sockfd);
    }
    cout << "server say:" << buf << endl;
  }
  close(sockfd);
  return 0;
}
