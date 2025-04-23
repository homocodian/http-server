#ifndef HTTP_REQUEST_PARSER_H

#define HTTP_REQUEST_PARSER_H

#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>

namespace HttpRequestParser {

class HttpRequest {
public:
  std::string method;
  std::string path;
  std::string version;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
};

std::ostream &operator<<(std::ostream &os,
                         const HttpRequestParser::HttpRequest &req);

std::unique_ptr<HttpRequest> parse(const std::string &rawRequest);

std::string trim(const std::string &str);
}; // namespace HttpRequestParser

#endif
