#include <regex>
#include <StatusCodes.hpp>
#include "Logger.hpp"

bool isValidIpAdress(std::string const rawLine)
{
    const std::regex ipAdressRegex("^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$");

    if (rawLine.length() > 15 || !std::regex_match(rawLine, ipAdressRegex))
        return false;
    return true;
}

std::string getMessageByStatusCode(HTTPStatusCode statusCode)
{
    switch (statusCode)
    {
    case HTTPStatusCode::OK:
        return "OK";
    case HTTPStatusCode::INTERNAL_SERVER_ERROR:
        return "Internal server error";
    case HTTPStatusCode::METHOD_NOT_ALLOWED:
        return "Method not allowed";
    case HTTPStatusCode::CONTENT_TOO_LARGE:
        return "Content too lage";
    case HTTPStatusCode::I_AM_A_TEAPOT:
        return "I am a teapot";
    default:
        return "Bad request";
    }
}

/**
 * @brief Decodes a chunked HTTP message body by concatenating the chunk data into a single string.
 *
 * This function searches for the chunk size indicator and chunk data within the message body,
 * converts the chunk size indicator from a hexadecimal string to a long integer, and appends the
 * chunk data to a new string. The function repeats this process until the entire message body has
 * been processed. If the message body is not properly formatted or if the chunk size indicator is
 * not a valid hexadecimal string, the function returns an empty string.
 *
 * @param[in] body The chunked HTTP message body to be decoded.
 * @return The concatenated chunk data, or an empty string if an error occurred.
 */
std::string unChunk(std::string body)
{
  Logger &logger = Logger::getInstance();
  std::string newBody = "";
  while (body.size())
  {
    size_t sub = body.find("\r\n");
    if (sub == std::string::npos)
    {
      logger.error("[CHUNK]: No \\r\\n pair found to indicate chunkSize end");
      return "";
    }
    std::string sizeStr = body.substr(sub);

    char  *check = NULL;
    long  chunkSize = strtoul(sizeStr.c_str(), &check, 16);
    if (check != NULL) {
      logger.error("[CHUNK]: chunkSize is not hexadecimal");
      return "";
    }
    newBody += body.substr(sub + 2, chunkSize);
    body = body.substr(sub + chunkSize + 2);
  }
  return newBody;
}
