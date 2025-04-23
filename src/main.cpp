#include "client.h"
#include "http_request_parser.h"
#include "server.h"
#include <iostream>
#include <memory>
#include <string>

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

  const std::string httpRawRequest = client.readRequest();

  const std::unique_ptr<HttpRequestParser::HttpRequest> httpRequest =
      HttpRequestParser::parse(httpRawRequest);

  if (httpRequest->path == "/") {
    std::string httpResponse = "HTTP/1.1 200 OK\r\n\r\n";
    client.repond(httpResponse);

  } else {
    std::string httpResponse = "HTTP/1.1 404 Not Found\r\n\r\n";
    client.repond(httpResponse);
  }

  std::cout << "Client connected\n";

  return 0;
}
