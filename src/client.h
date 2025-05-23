#ifndef CLIENT_H

#define CLIENT_H

#include <netdb.h>
#include <string>

#define BUFFER_SIZE 1024

class Client {
private:
  struct sockaddr_in clientAddr = {};
  int clientAddrLen;
  int clientFd = -1;

public:
  Client();
  int acceptConnection(int server_fd);
  void repond(const std::string &response);
  std::string readRequest();
  ~Client();
};

#endif // CLIENT_H
