#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>

int main()
{
  //创建套接字
  int socketfd = socket(AF_INET, SOCK_DGRAM,0);
  if(socketfd < 0)
  {
    perror("socket");
    return -1;
  }
  //填充struct sockaddr_in结构体
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  //转换ip地址并填充
  addr.sin_addr.s_addr = inet_addr("192.168.115.133");
  //转换字节序并填充端口号
  addr.sin_port = htons(19999);
  //绑定地址信息
  int ret = bind(socketfd, (struct sockaddr*)&addr, sizeof(addr));

  if(ret < 0)
  {
    perror("bind");
    return -1;
  }
  while(1)
  {
    char buf[1024] = {0};
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);
    ssize_t size = recvfrom(socketfd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&peer_addr,&peer_addr_len );
    if(size < 0)
    {
      perror("recvfrom");
      continue;
    }
    printf("client say:%s\n",buf);

    std::string s;
    getline(std::cin, s);
   // std::cin >> s;

    ssize_t sent_size = sendto(socketfd, s.c_str(),s.size(), 0, (struct sockaddr*)&peer_addr, peer_addr_len);
    if(sent_size < 0)
    {
      perror("sendto");
      continue;
    }
  }
  close(socketfd);
  return 0;
}
