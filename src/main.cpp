#include "client.h"
#include "server.h"
#include <iostream>

int main(int argc, char **argv) {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  Server server(4221);

  int server_fd = server.createServer();

  server.bindPort();

  server.listenSocket();

  Client client;

  std::cout << "Waiting for a client to connect...\n";

  int client_fd = client.acceptConnection(server_fd);

  std::string http_response = "HTTP/1.1 200 OK\r\n\r\n";

  client.repond(http_response);

  std::cout << "Client connected\n";

  return 0;
}
