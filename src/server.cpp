#include "server.h"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

Server::Server(int port)
    : server_fd(-1), server_addr({}), reuse(-1), port(port) {};

int Server::createServer() {
  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd < 0) {
    std::cerr << "ERROR: Failed to create server\n";
    exit(1);
  }

  // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) <
      0) {
    std::cerr << "ERROR: " << strerror(errno) << "\n";
    exit(1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  return server_fd;
}

void Server::bindPort() {
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) !=
      0) {
    std::cerr << "ERROR: " << strerror(errno) << "\n";
    exit(1);
  }
}

void Server::listenSocket(int connection_backlog) {
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "ERROR: " << strerror(errno) << "\n";
    exit(1);
  }
}

Server::~Server() {
  close(server_fd);
  std::cout << "Server closed\n";
}
