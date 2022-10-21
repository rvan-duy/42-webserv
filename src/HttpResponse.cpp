#include "HttpResponse.hpp"

#include "Logger.hpp"

Logger &logger = Logger::getInstance();

HttpResponse::HttpResponse() {}

HttpResponse::HttpResponse(const HttpResponse &obj) : HttpMessage(obj) {
  _status_code    = obj._status_code;
  _status_message = obj._status_message;
}

HttpResponse::~HttpResponse() {}

/*
 * Create the response
 * @param request Information from the request object
 */
void HttpResponse::create_response(const HttpRequest &request, const std::string &root) {
  /**************************************************/
  /* Respond based on the request method            */
  /**************************************************/

  logger.log("Creating response");
  switch (request.get_method()) {
      /**************************************************/
      /* Respond to a GET request                       */
      /**************************************************/

    case HttpRequest::GET: {
      logger.log("Response type: GET");
      std::ifstream requested_path(root + request.get_uri(), std::ios::binary);
      logger.log("Requested path: " + root + request.get_uri());

      /**************************************************/
      /* Respond with a 404 if the file does not exist  */
      /**************************************************/

      if (!requested_path) {
        std::ifstream file("root/404/index.html", std::ios::binary);
        if (!file) {
          logger.error("404 file not found");
          throw std::runtime_error("404 file not found");
        }
        file.seekg(0, std::ios::end);
        std::stringstream ss;
        ss << file.tellg();
        _status_code               = 404;
        _status_message            = "Not Found";
        _headers["Content-Type"]   = "text/html";
        _headers["Content-Length"] = ss.str();
        file.close();
        std::ifstream file2("root/404/index.html", std::ios::binary);
        if (file2.is_open()) {
          std::string body((std::istreambuf_iterator<char>(file2)), std::istreambuf_iterator<char>());
          file2.close();
          _body = body;
          break;
        }
      } else {
        /**************************************************/
        /* Respond with the file if it exists             */
        /**************************************************/

        requested_path.seekg(0, std::ios::end);
        std::stringstream ss;
        ss << requested_path.tellg();
        _status_code               = 200;
        _status_message            = "OK";
        _headers["Content-Type"]   = "text/html";
        _headers["Content-Length"] = ss.str();
        requested_path.close();
        std::ifstream requested_path2(root + request.get_uri(), std::ios::binary);
        if (requested_path2.is_open()) {
          std::string body((std::istreambuf_iterator<char>(requested_path2)), std::istreambuf_iterator<char>());
          requested_path2.close();
          _body = body;
          break;
        }
      }

      /**************************************************/
      /* Respond with the file if it exists             */
      /**************************************************/

      _version = request.get_version();

      break;
    }

      /**************************************************/
      /* Respond to a POST request                      */
      /**************************************************/

    case HttpRequest::POST: {
      // todo
      break;
    }

      /**************************************************/
      /* Respond to a DELETE request                    */
      /**************************************************/

    case HttpRequest::DELETE: {
      // todo
      break;
    }

      /**************************************************/
      /* Respond to unsupported method                  */
      /**************************************************/
    default: {
      // todo prob throw 404?
      break;
    }
  }
}

/*
 * Convert the response to a string
 * @return The response as a string
 */
std::string HttpResponse::to_str() const {
  Logger &logger = Logger::getInstance();

  /**************************************************/
  /* Create the response string to store the        */
  /* response in                                    */
  /**************************************************/

  logger.log("< Started creating response string");
  std::string response_string;

  /**************************************************/
  /* Add the status line                            */
  /**************************************************/

  logger.log("1. Adding status line");
  response_string += get_version_to_str() + " " + std::to_string(_status_code) + " " + _status_message + "\r\n";

  /**************************************************/
  /* Add the headers                                */
  /**************************************************/

  logger.log("2. Adding headers");
  {
    std::map<std::string, std::string>::const_iterator it;

    for (it = _headers.begin(); it != _headers.end(); ++it) {
      response_string += it->first + ": " + it->second + "\r\n";
    }

    response_string += "\r\n";  // Add an extra CRLF to separate headers from body
  }

  /**************************************************/
  /* Add the body                                   */
  /**************************************************/

  logger.log("3. Adding body");
  if (_body.length() > 0) {
    response_string += _body;
  }

  /**************************************************/
  /* Return the response string                     */
  /**************************************************/

  logger.log("! Finished creating response string:\n---------------------------\n" + response_string +
             "\n---------------------------\n");
  return response_string;
}
