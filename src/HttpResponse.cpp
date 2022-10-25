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
 * @param root Directory to serve files from
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
      const std::string path = root + request.get_uri();
      logger.log("Path: " + path);

      /**************************************************/
      /* First check if the file exists                 */
      /**************************************************/

      if (!_file_exists(path)) {
        logger.error("File does not exist, sending back 404");
        set_to_404_page("Not found");
        return;
      }

      std::ifstream requested_path(path, std::ios::binary);
      logger.log("Requested path: " + path);

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

/*
 * Get the status code
 */
int HttpResponse::get_status_code() const {
  return _status_code;
}

/*
 * Get the status message
 */
std::string HttpResponse::get_status_message() const {
  return _status_message;
}

/*
 * Set the HttpResponse to a 404 html page
 * @param message The message to send back
 */
void HttpResponse::set_to_404_page(const std::string &message) {
  std::ifstream file("root/404/index.html", std::ios::binary);
  if (!file) {
    logger.error("404 file not found");
    throw std::runtime_error("404 file not found");
  }
  file.seekg(0, std::ios::end);
  std::stringstream ss;
  ss << file.tellg();
  _headers["Content-Length"] = ss.str();  // set the content length
  file.seekg(0, std::ios::beg);
  std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  file.close();
  _body                    = body;         // set the body
  _version                 = HTTP_1_1;     // set the version
  _status_code             = 404;          // set the status code
  _status_message          = message;      // set the status message
  _headers["Content-Type"] = "text/html";  // set the content type
}

/*
 * Checks if a file exists
 * @param path The path to the file to check
 * @return True if the file exists, false otherwise
 */
bool HttpResponse::_file_exists(const std::string &path) const {
  std::cout << "Checking if file exists: " << path << std::endl;

  struct stat buffer;                                   // stat struct to store the file info
  if (stat(path.c_str(), &buffer) == -1) return false;  // if the file doesn't exist
  if (buffer.st_mode & S_IFDIR) return false;           // if the file is a directory
  if (buffer.st_mode & S_IFREG) return true;            // if the file is a regular file
  return false;                                         // if the file is something else
}
