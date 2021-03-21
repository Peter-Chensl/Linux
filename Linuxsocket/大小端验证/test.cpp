#include <stdio.h>
#include <iostream>
using namespace std;

int main()
{
  union Data
  {
    int a;
    char b;
  }data;
  data.a = 1;
  if(data.b == 1)
  {
    cout<<"小端"<<endl;
  }
  else 
  {
    cout<<"大端"<<endl;
  }
  return 0;
}
