#include <stdio.h>
#include <iostream>

#include "httplib.h"
using namespace httplib;

void http_callback(const Request& req, Response& resp)
{
  (void)req;
  std::string body = "<html>linux so easy</html>";
  resp.set_content(body.c_str(),body.size(),"text/html");
}
int main()
{
  Server svr;
  svr.Get("/aaa",http_callback);

  svr.listen("0.0.0.0",19998);
  return 0;
}
