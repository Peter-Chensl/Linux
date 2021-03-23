#include "../udp.hpp"

#define CHECK_ERT(p) {if(p < 0) return -1;}

int main(int argc, char *argv[])
{
  if(argc != 3)
  {
    cout << "using ./client [ip] [port]" << endl;
    return -1;
  }
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  udpSer us;
  //创建套接字
  CHECK_ERT(us.CreateSocket());
  struct sockaddr_in dest_addr;
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(port);
  dest_addr.sin_addr.s_addr = inet_addr(ip.c_str());

  while(1)
  {
    //发送消息
    string data;
    cout << "Plase enter massage to server:";
    fflush(stdout);
    getline(cin, data);
    int ret = us.Sendto(data, &dest_addr);
    if(ret < 0)
    {
      continue;
    }
    //接收消息
    data.clear();
    struct sockaddr_in dest_addr;
    ret = us.RecvFrom(&data,&dest_addr);
    if(ret < 0)
    {
      continue;
    }
    cout << "server say:" << data << endl;
  }
  //关闭套接字
  us.Close();
  return 0;
}
