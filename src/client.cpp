#include "client.h"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

Client::Client()
    : client_addr({}), client_addr_len(sizeof(client_addr)), client_fd(-1) {}

int Client::acceptConnection(int server_fd) {

  client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                     (socklen_t *)&client_addr_len);

  if (client_fd < 0) {
    std::cerr << "ERROR: " << strerror(errno) << "\n";
    exit(1);
  }

  return client_fd;
}

std::string Client::readRequest() {
  char buffer[BUFFER_SIZE];
  read(client_fd, buffer, BUFFER_SIZE);

  return buffer;
}

void Client::repond(std::string response) {
  send(client_fd, response.c_str(), response.length(), 0);
}

Client::~Client() {
  close(client_fd);
  // std::cout << "Client closed\n";
}
