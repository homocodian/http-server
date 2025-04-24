#ifndef SERVER_H

#define SERVER_H

#include <netinet/in.h>

class Server {
private:
  int server_fd = -1;
  struct sockaddr_in server_addr = {};
  int reuse = -1;
  int port;

public:
  Server(int port);
  int createServer();
  void bindPort();
  void listenSocket(int connection_backlog = 5);
  ~Server();
};

#endif // SERVER_H
