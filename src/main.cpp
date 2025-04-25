#include "client.h"
#include "http_request_parser.h"
#include "http_response_builder.h"
#include "server.h"
#include "thread_pool.h"
#include <iostream>
#include <memory>
#include <regex>
#include <string>

int main(int argc, char **argv) {
  Server server(4221);
  int server_fd = server.createServer();
  server.bindPort();
  server.listenSocket();

  ThreadPool threadPool(std::thread::hardware_concurrency() * 2);

  while (true) {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    auto client = std::make_shared<Client>();

    std::cout << "Waiting for a client to connect...\n";

    int client_fd = client->acceptConnection(server_fd);

    threadPool.enqueue([client]() -> void {
      const std::string httpRawRequest = client->readRequest();

      const auto httpRequest = HttpRequestParser::parse(httpRawRequest);

      std::cout << "\nREQUEST: \n" << *httpRequest << "\nEND\n";

      std::smatch match;

      if (httpRequest->path == "/") {
        std::string httpResponse = HttpResponseBuilder().build().toString();
        client->repond(httpResponse);

      } else if (std::regex_search(httpRequest->path, match,
                                   std::regex("^/echo/([^/]+)$"))) {

        const std::string httpResponse =
            HttpResponseBuilder()
                .addHeader("Content-Type", "text/plain")
                .setBody(match[1])
                .build()
                .toString();

        client->repond(httpResponse);
      } else if (std::regex_search(httpRequest->path, match,
                                   std::regex("/user-agent"))) {

        const std::string &userAgent = httpRequest->headers["User-Agent"];

        const std::string httpResponse =
            HttpResponseBuilder()
                .addHeader("Content-Type", "text/plain")
                .setBody(userAgent)
                .build()
                .toString();

        client->repond(httpResponse);
      } else {
        const std::string httpResponse = HttpResponseBuilder()
                                             .setStatus(404, "Not Found")
                                             .build()
                                             .toString();
        client->repond(httpResponse);
      }

      std::cout << "Client connected\n\n";
    });
  }

  return 0;
}
