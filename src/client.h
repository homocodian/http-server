#ifndef CLIENT_H

#define CLIENT_H

#include <netdb.h>
#include <string>

#define BUFFER_SIZE 4096

class Client {
private:
  struct sockaddr_in client_addr;
  int client_addr_len;
  int client_fd;

public:
  Client();
  int acceptConnection(int server_fd);
  void repond(std::string response);
  std::string readRequest();
  ~Client();
};

#endif // CLIENT_H
