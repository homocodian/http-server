#ifndef HTTP_REPONSE_BUILDER_H
#define HTTP_REPONSE_BUILDER_H

#include "http_response.h"

class HttpResponseBuilder {
private:
  HttpResponse response;
  bool keepAlive = false;
  int keepAliveTimeout = -1;

public:
  HttpResponseBuilder &setStatus(int code, const std::string &message) {
    response.statusCode = code;
    response.statusMessage = message;
    return *this;
  }

  HttpResponseBuilder &setVersion(const std::string &httpVersion) {
    response.version = httpVersion;
    return *this;
  }

  HttpResponseBuilder &addHeader(const std::string &key,
                                 const std::string &value) {
    response.headers[key] = value;
    return *this;
  }

  HttpResponseBuilder &setBody(const std::string &bodyContent) {
    response.body = bodyContent;
    response.headers["Content-Length"] = std::to_string(bodyContent.size());
    return *this;
  }

  HttpResponseBuilder &enableKeepAlive(int timeoutSeconds = -1) {
    keepAlive = true;
    keepAliveTimeout = timeoutSeconds;
    return *this;
  }

  HttpResponseBuilder &disableKeepAlive() {
    keepAlive = false;
    keepAliveTimeout = -1;
    return *this;
  }

  HttpResponse build() {
    if (keepAlive) {
      response.headers["Connection"] = "keep-alive";
      if (keepAliveTimeout > 0) {
        response.headers["Keep-Alive"] =
            "timeout=" + std::to_string(keepAliveTimeout);
      }
    } else {
      response.headers["Connection"] = "close";
    }
    return response;
  }
};

#endif