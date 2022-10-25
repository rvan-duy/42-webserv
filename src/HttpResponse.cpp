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
 * @param index Index file to serve
 */
void HttpResponse::create_response(const HttpRequest &request, const std::string &root, const std::string &index) {
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
      std::string path = root + request.get_uri();
      logger.log("Path: " + path);

      /**************************************************/
      /* First check if the file exists                 */
      /**************************************************/

      if (!_file_exists(path)) {
        /**************************************************/
        /* If the file doesn't exist, look for an index   */
        /* file in the directory                          */
        /**************************************************/

        logger.log("File doesn't exist");
        const std::string index_path = path + index;
        logger.log("Looking for index path: " + index_path);

        /**************************************************/
        /* If the index file doesn't exist, respond with  */
        /* a 404 error                                    */
        /**************************************************/

        if (!_file_exists(index_path)) {
          logger.log("Index file doesn't exist");
          _set_response("root/404/index.html", 404, "Not Found", request.get_version());
          return;
        } else {
          logger.log("Index file exists");
          path = index_path;
        }
      }

      /**************************************************/
      /* Respond with the file                          */
      /**************************************************/

      _set_response(path, 200, "OK", request.get_version());

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
 * TODO yey yeyeye lots of comments explaining this func
 */
void HttpResponse::_set_response(const std::string &path, const int status_code, const std::string &status_message,
                                 const HttpVersion version) {
  std::ifstream file(path, std::ios::binary);
  logger.log("Requested path: " + path);

  file.seekg(0, std::ios::end);
  std::stringstream ss;
  ss << file.tellg();
  _status_code               = status_code;              // Set the status code
  _status_message            = status_message;           // Set the status message
  _version                   = version;                  // Set the version
  _headers["Content-Length"] = ss.str();                 // Set the content length header
  _headers["Content-Type"]   = _get_content_type(path);  // Set the content type header
  file.seekg(0, std::ios::beg);
  if (file.is_open()) {
    std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    _body = body;
  }
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

/*
 * Get the file type of a file
 * @param path The path to the file to check
 * @return The content type of the file
 */
std::string HttpResponse::_get_content_type(const std::string &path) const {
  /**************************************************/
  /* The map of file extensions to content types    */
  /**************************************************/

  std::map<std::string, std::string> file_types;

  file_types["html"] = "text/html";
  file_types["css"]  = "text/css";
  file_types["js"]   = "application/javascript";
  file_types["png"]  = "image/png";

  // add more file types here if needed

  /**************************************************/
  /* Get the file extension                         */
  /**************************************************/

  std::string file_extension = path.substr(path.find_last_of(".") + 1);

  /**************************************************/
  /* Check if the file extension is in the map, if  */
  /* it is, return the file type, otherwise return  */
  /* text/plain                                     */
  /**************************************************/

  if (file_types.find(file_extension) != file_types.end()) {
    return file_types[file_extension];
  }
  return "text/plain";
}
