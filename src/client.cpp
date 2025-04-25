#include "client.h"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

Client::Client() : clientAddrLen(sizeof(clientAddr)) {}

int Client::acceptConnection(int server_fd) {

  clientFd = accept(server_fd, (struct sockaddr *)&clientAddr,
                    (socklen_t *)&clientAddrLen);

  if (clientFd < 0) {
    std::cerr << "ERROR: " << strerror(errno) << "\n";
    exit(1);
  }

  // Set a timeout for socket reads
  struct timeval timeout;
  timeout.tv_sec = 5; // 5 seconds timeout
  timeout.tv_usec = 0;
  setsockopt(clientFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

  return clientFd;
}

std::string Client::readRequest() {
  std::string completeRequest;

  while (true) {
    std::string buffer(BUFFER_SIZE, '\0');

    ssize_t bytesReceived = recv(clientFd, buffer.data(), buffer.size(), 0);

    if (bytesReceived < 0) {
      if (errno == EWOULDBLOCK || errno == EAGAIN) {
        std::cerr << "Socket read timeout\n";
        break;
      }
      std::cerr << "Error: Failed to read request!\n";
      return {};
    }

    if (bytesReceived == 0) {
      // Client closed the connection
      break;
    }

    completeRequest.append(buffer, 0, bytesReceived);

    // Check if the request is complete
    if (completeRequest.find("\r\n\r\n") != std::string::npos) {
      break;
    }
  }

  return completeRequest;
}

void Client::repond(const std::string &response) {
  size_t totalSent = 0;
  size_t toSend = response.length();

  while (totalSent < toSend) {
    ssize_t sent =
        send(clientFd, response.c_str() + totalSent, toSend - totalSent, 0);
    if (sent <= 0) {
      std::cerr << "Error: Failed to send response\n";
      break;
    }
    totalSent += sent;
  }
}

Client::~Client() {
  if (clientFd < 0)
    return;

  int status = close(clientFd);

  if (status != 0) {
    std::cerr << "ERROR: " << strerror(errno) << "\n";
    return;
  }

  clientFd = -1;
  std::cout << "Client closed\n";
}
