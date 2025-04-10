#ifndef SERVER_H

#define SERVER_H

#include <netinet/in.h>

class Server {
private:
  int server_fd;
  struct sockaddr_in server_addr;
  int reuse;
  int port;

public:
  Server(int port);
  int createServer();
  void bindPort();
  void listenSocket(int connection_backlog = 5);
  ~Server();
};

#endif // SERVER_H
