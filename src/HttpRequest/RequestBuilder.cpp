#include "HttpRequest.hpp"
#include "Server.hpp"

HttpRequest::HttpMethod HttpRequest::_parseMethod(const std::string &method) {
  Logger &logger = Logger::getInstance();
  std::map<std::string, HttpRequest::HttpMethod> method_map;

  method_map["GET"]    = GET;
  method_map["POST"]   = POST;
  method_map["DELETE"] = DELETE;

  try {
    return method_map.at(method);
  } catch (std::exception& e) {
    _status_code = 400;
    logger.error("bad request: method unsupported ( " + method + " )");
    return HttpRequest::NONE;
  }
}

//  MEMBERS
HttpRequest *Server::createRequest(std::string& msg)
{
    switch (_parseMethod(extractArgument(msg, 1))) 
    {
        case GET:
            return new GetRequest(target);
        case POST:
            return new PostRequest(target);
        case DELETE:
            return new DeleteRequest(target);
        default:
        {
            logger.error("bad request: method unsupported ( " "+ method +" " )");
            return new HttpRequest(target);
        }
    }
    return NULL;
}

void  Server::buildRequest() {
  
}
