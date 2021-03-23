#include "../udp.hpp"

#define CHECK_RET(p) if(p < 0){return -1;}//检测是否成功
int main(int argc, char* argv[])
{
  //用参数的形式把IP和端口传递进来
  if(argc != 3)
  {
    printf("using ./svr [ip] [port]\n");//告诉怎么使用
    return -1;
  }

  UDP us;
  CHECK_RET(us.CreateSocket());

  string ip = argv[1];
  uint16_t port =atoi(argv[2]);
  
  CHECK_RET(us.Bind(ip,port));

  while(1)
  {
    string data;
    struct sockaddr_in peer_addr;
    int ret = us.RecvFrom(&data,&peer_addr);
    if(ret < 0)
    {
      continue;
    }
    cout << "client say:" << data << endl;

    data.clear();
    cout << "please enter msg to client:";
    fflush(stdout);
    getline(cin,data);

    ret = us.Sendto(data,&peer_addr);
    if(ret < 0)
    {
      continue;
    }
  }
  us.Close();
  return 0;
}
