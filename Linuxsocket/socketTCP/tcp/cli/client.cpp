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
  int lis_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(lis_sockfd < 0)
  {
    perror("socket");
    return -1;
  }
  //连接服务端
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr("192.168.115.133");
  addr.sin_port = htons(19898);
  int ret = connect(lis_sockfd, (struct sockaddr*)&addr,sizeof(addr));
  if(ret < 0)
  {
    perror("connect");
    return -1;
  }
  while(1)
  {
    //发送数据
    char buf[1024] = {0};
    cout << "please enter massage to server:";
    //刷新缓冲区
    fflush(stdout);
    cin >> buf;
    ssize_t send_size = send(lis_sockfd,buf,sizeof(buf) - 1,0 );
    if(send_size < 0)
    {
      perror("send");
      continue;
    }
    //清空缓冲区
    memset(buf, '\0', sizeof(buf));
    ssize_t recv_size = recv(lis_sockfd, buf, sizeof(buf) - 1,0);
    if(recv_size < 0)
    {
      perror("recv");
      continue;
    }
    else if(recv_size == 0)
    {
      //对端关闭
      cout << "peer shutdown" << endl;
      close(lis_sockfd);
    }
    cout << "sve say:" << buf << endl;
  }
  //关闭套接字
  close(lis_sockfd);
  return 0;
}
