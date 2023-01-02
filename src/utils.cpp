#include <StatusCodes.hpp>
#include <regex>

#include "Logger.hpp"

std::string getMessageByStatusCode(HTTPStatusCode statusCode) {
  switch (statusCode) {
    case HTTPStatusCode::OK:
      return "OK";
    case HTTPStatusCode::BAD_REQUEST:
      return "Bad request";
    case HTTPStatusCode::FORBIDDEN:
      return "Forbidden";
    case HTTPStatusCode::NOT_FOUND:
      return "Not found";
    case HTTPStatusCode::METHOD_NOT_ALLOWED:
      return "Method not allowed";
    case HTTPStatusCode::NOT_ACCEPTABLE:
      return "Not acceptable";
    case HTTPStatusCode::CONTENT_TOO_LARGE:
      return "Content too large";
    case HTTPStatusCode::I_AM_A_TEAPOT:
      return "I am a teapot";
    case HTTPStatusCode::INTERNAL_SERVER_ERROR:
      return "Internal server error";
    default:
      return "Bad request";
  }
}

// lol
HTTPStatusCode intToHttpStatus(const int &statusCode) {
  switch (statusCode) {
    case 200:
      return HTTPStatusCode::OK;
    case 300:
      return HTTPStatusCode::MULTIPLE_CHOICES;
    case 301:
      return HTTPStatusCode::MOVED_PERMANENTLY;
    case 302:
      return HTTPStatusCode::FOUND;
    case 303:
      return HTTPStatusCode::SEE_OTHER;
    case 304:
      return HTTPStatusCode::NOT_MODIFIED;
    case 305:
      return HTTPStatusCode::USE_PROXY;
    case 307:
      return HTTPStatusCode::TEMPORARY_REDIRECT;
    case 308:
      return HTTPStatusCode::PERMANENT_REDIRECT;
    case 400:
      return HTTPStatusCode::BAD_REQUEST;
    case 403:
      return HTTPStatusCode::FORBIDDEN;
    case 404:
      return HTTPStatusCode::NOT_FOUND;
    case 405:
      return HTTPStatusCode::METHOD_NOT_ALLOWED;
    case 406:
      return HTTPStatusCode::NOT_ACCEPTABLE;
    case 413:
      return HTTPStatusCode::CONTENT_TOO_LARGE;
    case 418:
      return HTTPStatusCode::I_AM_A_TEAPOT;
    case 500:
      return HTTPStatusCode::INTERNAL_SERVER_ERROR;
    default:
      return HTTPStatusCode::INTERNAL_SERVER_ERROR;
  }
}

std::vector<std::string> splitHeader(const std::string &headerString,
                                     const std::string &delimiter) {
  std::vector<std::string> headerLines;
  std::string currentLine;
  size_t lastLocation = 0;  // last location of delimiter
  size_t nextLocation = 0;  // next location of delimiter

  do {
    nextLocation = headerString.find(delimiter, nextLocation);
    if (nextLocation == std::string::npos) {
      break;
    }
    currentLine =
        headerString.substr(lastLocation, nextLocation - lastLocation);
    headerLines.push_back(currentLine);
    nextLocation += delimiter.length();
    lastLocation = nextLocation;
  } while (true);
  return headerLines;
}

/**
 * @brief Decodes a chunked HTTP message body by concatenating the chunk data
 * into a single string.
 *
 * This function searches for the chunk size indicator and chunk data within the
 * message body, converts the chunk size indicator from a hexadecimal string to
 * a long integer, and appends the chunk data to a new string. The function
 * repeats this process until the entire message body has been processed. If the
 * message body is not properly formatted or if the chunk size indicator is not
 * a valid hexadecimal string, the function returns an empty string.
 *
 * @param[in] body The chunked HTTP message body to be decoded.
 * @return The concatenated chunk data, or an empty string if an error occurred.
 */
std::string unChunk(std::string body) {
  Logger &logger = Logger::getInstance();
  std::string newBody = "";
  while (body.size()) {
    size_t sub = body.find("\r\n");
    if (sub == std::string::npos) {
      logger.error("[CHUNK]: No \\r\\n pair found to indicate chunkSize end");
      return "";
    }
    std::string sizeStr = body.substr(sub);

    char *check = NULL;
    long chunkSize = strtoul(sizeStr.c_str(), &check, 16);
    if (check != NULL) {
      logger.error("[CHUNK]: chunkSize is not hexadecimal");
      return "";
    }
    newBody += body.substr(sub + 2, chunkSize);
    body = body.substr(sub + chunkSize + 2);
  }
  return newBody;
}
