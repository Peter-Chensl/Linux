#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>
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
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(19898);
  addr.sin_addr.s_addr = inet_addr("192.168.115.133");
  //连接服务端
  int ret = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
  if(ret < 0)
  {
    perror("connect");
    return -1;
  }
  while(1)
  {
    //发送数据
    string data;
    cout << "please enter massage to server:";
    cin >> data;
    ssize_t send_size = send(sockfd, data.c_str(), data.size(), 0);
    if(send_size < 0)
    {
      perror("send");
      continue;
    }
    data.clear();
    char buf[1024] = {0};
    ssize_t recv_size = recv(sockfd, buf, sizeof(buf), 0);
    if(recv_size < 0)
    {
      perror("recv");
      continue;
    }
    else if(recv_size == 0)
    {
      cout << "perr shutdown!" << endl;
      close(sockfd);
    }
    cout << "server say:" << buf << endl;
  }
  close(sockfd);
  return 0;
}
