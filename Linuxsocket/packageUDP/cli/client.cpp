#include "../udp.hpp"

#define CHECK_RET(p) if(p < 0) {return -1;}

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    printf("using ./client [ip] [port]\n");
    return -1;
  }

  UDP us;
  CHECK_RET(us.CreateSocket());

  string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  struct sockaddr_in dest_addr;
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_addr.s_addr = inet_addr(ip.c_str());
  dest_addr.sin_port = htons(port);

  while(1)
  {
    string data;
    cout << "please enter massage to svr:";
    fflush(stdout);
    getline(cin,data);
    int ret = us.Sendto(data, &dest_addr);
    if(ret < 0)
    {
      continue;
    }
    data.clear();
    struct sockaddr_in peer_addr;
    ret = us.RecvFrom(&data,&peer_addr);
    if(ret < 0)
    {
      continue;
    }
    cout << "svr say:" << data << endl;
  }

  us.Close();//关闭套接字
  return 0;
}
