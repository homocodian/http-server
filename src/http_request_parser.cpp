#include "http_request_parser.h"
#include <memory>
#include <sstream>
#include <string>

std::string HttpRequestParser::trim(const std::string &str) {
  const auto start = str.find_first_not_of(" \t\r\n");
  const auto end = str.find_last_not_of(" \t\r\n");
  return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

std::unique_ptr<HttpRequestParser::HttpRequest>
HttpRequestParser::parse(const std::string &rawRequest) {
  std::istringstream stream(rawRequest);
  auto httpRequest = std::make_unique<HttpRequest>();
  std::string line;

  std::getline(stream, line);
  if (!line.empty() && line.back() == '\r')
    line.pop_back();
  std::istringstream requestLine(line);
  requestLine >> httpRequest->method >> httpRequest->path >>
      httpRequest->version;

  while (std::getline(stream, line) && line != "\r") {
    size_t colon = line.find(":");

    if (colon != std::string::npos) {
      std::string key = line.substr(0, colon);
      std::string value = line.substr(colon + 1);

      httpRequest->headers[trim(key)] = trim(value);
    }
  }

  std::string body;

  while (std::getline(stream, line)) {
    body += line + "\n";
  }

  httpRequest->body = body;

  return httpRequest;
}

std::ostream &
HttpRequestParser::operator<<(std::ostream &os,
                              const HttpRequestParser::HttpRequest &req) {
  os << req.method << " " << req.path << " " << req.version << "\n";

  for (const auto &[key, value] : req.headers) {
    os << key << ": " << value << "\n";
  }

  os << "\n" << req.body;
  return os;
}
