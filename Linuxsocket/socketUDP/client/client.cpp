#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
                    
#include <iostream>
#include <string>     
                          
using namespace std;   
                               
int main()
{      

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockfd < 0)
  {                 
    perror("socket");
    return 0;
  }
  struct sockaddr_in peer_addr;
  peer_addr.sin_family = AF_INET;
  peer_addr.sin_port = htons(19999);
  peer_addr.sin_addr.s_addr = inet_addr("192.168.115.133");
  while(1)       
  {
    std::string s;
    getline(std::cin,s);
    ssize_t send_size = sendto(sockfd,s.c_str(),s.size(),0,(struct sockaddr*)&peer_addr,sizeof(peer_addr));
    if(send_size < 0)
    {
      perror("sendto");
      continue;
    }

    char buf[1024] = {0};

    ssize_t recv = recvfrom(sockfd, buf,sizeof(buf) - 1,0,NULL,NULL);
    if(recv < 0)
    {
      perror("recvfrom");
      continue;
    }
    printf("svr say:%s\n",buf);
  }            
  close(sockfd);
  return 0;
}        
