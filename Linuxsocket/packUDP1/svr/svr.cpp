#include "../udp.hpp"

#define CHECK_RET(p) {if(p < 0) return -1;}
int main(int argc , char *argv[])
{
  if(argc != 3)
  {
    cout << "using ./svr [ip] [port]" << endl;
    return -1;
  }
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  udpSer us;
  //创建套接字
  CHECK_RET(us.CreateSocket());
  //绑定地址信息
  int ret = us.Bind(ip, port);
  if(ret < 0)
  {
    return 0;
  }
  while(1)
  {
    //接收消息
    string data;
    struct sockaddr_in peer_addr;
    int ret = us.RecvFrom(&data, &peer_addr);
    if(ret < 0)
    {
      continue;
    }
    cout << "client say:" << data << endl;
    //发送数据
    data.clear();//清空内容
    cout << "Please enter massage to client:";
    //刷新缓冲区
    fflush(stdout);
    getline(cin, data);
    ret = us.Sendto(data, &peer_addr);
    if(ret < 0)
    {
      continue;
    }
  }
  //关闭套接字
  us.Close();
  return -1;
}
