#include "client.h"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

Client::Client() : client_addr_len(sizeof(client_addr)) {}

int Client::acceptConnection(int server_fd) {

  client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                     (socklen_t *)&client_addr_len);

  if (client_fd < 0) {
    std::cerr << "ERROR: " << strerror(errno) << "\n";
    exit(1);
  }

  // Set a timeout for socket reads
  struct timeval timeout;
  timeout.tv_sec = 5; // 5 seconds timeout
  timeout.tv_usec = 0;
  setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

  return client_fd;
}

std::string Client::readRequest() {
  std::string complete_request;

  while (true) {
    std::string buffer(BUFFER_SIZE, '\0');

    ssize_t bytes_received = recv(client_fd, buffer.data(), buffer.size(), 0);

    if (bytes_received < 0) {
      if (errno == EWOULDBLOCK || errno == EAGAIN) {
        std::cerr << "Socket read timeout\n";
        break;
      }
      std::cerr << "Error: Failed to read request!\n";
      return {};
    }

    if (bytes_received == 0) {
      // Client closed the connection
      break;
    }

    complete_request.append(buffer, 0, bytes_received);

    // Check if the request is complete
    if (complete_request.find("\r\n\r\n") != std::string::npos) {
      break;
    }
  }

  return complete_request;
}

void Client::repond(const std::string response) {
  send(client_fd, response.c_str(), response.length(), 0);
}

Client::~Client() {
  close(client_fd);
  // std::cout << "Client closed\n";
}
