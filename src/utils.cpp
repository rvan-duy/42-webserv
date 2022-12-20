#include <StatusCodes.hpp>
#include <regex>

std::string getMessageByStatusCode(HTTPStatusCode statusCode) {
  switch (statusCode) {
    case HTTPStatusCode::OK:
      return "OK";
    case HTTPStatusCode::BAD_REQUEST:
      return "Bad request";
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

std::vector<std::string> splitHeader(std::string headerString,
                                     bool withNewline) {
  std::vector<std::string> headerLines;
  std::string currentLine;
  size_t lastLocation = 0;  // last location of \n\r
  size_t nextLocation = 0;  // next location of \n\r

  do {
    nextLocation = headerString.find("\r\n", nextLocation);
    if (nextLocation == std::string::npos) break;
    if (withNewline) nextLocation += 2;
    currentLine =
        headerString.substr(lastLocation, nextLocation - lastLocation);
    headerLines.push_back(currentLine);
    if (!withNewline) nextLocation += 2;
    lastLocation = nextLocation;
  } while (true);
  return headerLines;
}
