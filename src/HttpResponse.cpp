#include "HttpResponse.hpp"

#include "Logger.hpp"

HttpResponse::HttpResponse() {}

HttpResponse::HttpResponse(const HttpResponse &obj) : HttpMessage(obj)
{
  _statusCode = obj._statusCode;
  _statusMessage = obj._statusMessage;
}

HttpResponse::HttpResponse(HttpVersion version, HTTPStatusCode statusCode, std::string statusMessage)
    : HttpMessage(version), _statusCode(statusCode), _statusMessage(statusMessage) {}

HttpResponse::~HttpResponse() {}

/*
 * Convert the response to a string // 2 keer dubbelop?
 * @return The response as a string
 */
std::string HttpResponse::toStr() const
{
  Logger &logger = Logger::getInstance();

  logger.log("< Started creating response string", VERBOSE);
  std::string response_string;

  logger.log("2. Adding headers", VERBOSE);
  {
    std::map<std::string, std::string>::const_iterator it;

    for (it = _headers.begin(); it != _headers.end(); ++it)
    {
      response_string += it->first + ": " + it->second + "\r\n";
    }

    response_string += "\r\n"; // Add an extra CRLF to separate headers from body
  }

  logger.log("3. Adding body", VERBOSE);
  if (_body.length() > 0)
  {
    response_string += _body;
  }

  /**************************************************/
  /* Return the response string                     */
  /**************************************************/

  return response_string;
}

/*
 * Get the status code
 */
HTTPStatusCode HttpResponse::getStatusCode() const
{
  return _statusCode;
}

/*
 * Get the status message
 */
std::string HttpResponse::getStatusMessage() const
{
  return _statusMessage;
}

/*
 * Sets the HttpResponse Class attributes to the given values
 * @param path Path to the file to serve, relative to the root directory
 * @param status_code Status code to respond with
 * @param status_message Status message to respond with
 * @param version HTTP version to respond with
 */
void HttpResponse::_setResponse(const std::string &path, HTTPStatusCode status_code, const std::string &status_message,
                                const HttpVersion version)
{
  Logger &logger = Logger::getInstance();

  std::ifstream file(path.c_str()); // Open the file
  logger.log("Requested path: " + path);

  file.seekg(0, std::ios::end);                     // Seek to the end of the file
  std::stringstream ss;                             // Create a stringstream to store the file contents in
  ss << file.tellg();                               // Get the file size
  _statusCode = status_code;                        // Set the status code
  _statusMessage = status_message;                  // Set the status message
  _version = version;                               // Set the version
  _headers["Content-Length"] = ss.str();            // Set the content length header
  _headers["Content-Type"] = _getContentType(path); // Set the content type header
  file.seekg(0, std::ios::beg);                     // Seek back to the beginning of the file
  if (file.is_open())
  {
    std::string body((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>()); // Read the file into a string
    file.close();
    _body = body;
  }
}

/*
 * Checks if a file exists
 * @param path The path to the file to check
 * @return True if the file exists, false otherwise
 */
bool _fileExists(const std::string &path)
{
  Logger &logger = Logger::getInstance();

  struct stat buffer; // stat struct to store the file info
  if (stat(path.c_str(), &buffer) == -1)
  {
    logger.log("_fileExists -> File doesn't exist (" + path + ")", VERBOSE);
    return false;
  }
  if (buffer.st_mode & S_IFDIR)
  {
    logger.log("_fileExists -> File is a directory (" + path + ")", VERBOSE);
    return false;
  }
  if (buffer.st_mode & S_IFREG)
  {
    logger.log("_fileExists -> File is a regular file (" + path + ")", VERBOSE);
    return true;
  }
  logger.log("_fileExists -> File is something else (" + path + ")", VERBOSE);
  logger.error("WARNING: logic probably not implemented yet");
  return false;
}

/*
 * Get the file type of a file
 * @param path The path to the file to check
 * @return The content type of the file
 */
std::string HttpResponse::_getContentType(const std::string &path) const
{
  /**************************************************/
  /* The map of file extensions to content types    */
  /**************************************************/

  std::map<std::string, std::string> file_types;

  file_types["html"] = "text/html";
  file_types["css"] = "text/css";
  file_types["js"] = "application/javascript";
  file_types["png"] = "image/png";

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

  if (file_types.find(file_extension) != file_types.end())
  {
    return file_types[file_extension];
  }
  return "text/plain";
}
