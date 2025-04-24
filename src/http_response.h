#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <sstream>
#include <string>
#include <unordered_map>

class HttpResponse {
private:
  std::string version = "HTTP/1.1";
  int statusCode = 200;
  std::string statusMessage = "OK";
  std::unordered_map<std::string, std::string> headers;
  std::string body;

public:
  std::string toString() const {
    std::ostringstream response;
    response << version << " " << statusCode << " " << statusMessage << "\r\n";

    for (const auto &[key, value] : headers) {
      response << key << ": " << value << "\r\n";
    }

    response << "\r\n" << body;

    return response.str();
  }

  friend class HttpResponseBuilder;
};

#endif
