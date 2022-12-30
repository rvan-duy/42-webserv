#pragma once

#include <string>

enum class HTTPStatusCode {
  NOT_SET = 0,
  OK = 200,

  MULTIPLE_CHOICES = 300,
  MOVED_PERMANENTLY = 301,
  FOUND = 302,
  SEE_OTHER = 303,
  NOT_MODIFIED = 304,
  USE_PROXY = 305,
  TEMPORARY_REDIRECT = 307,
  PERMANENT_REDIRECT = 308,

  BAD_REQUEST = 400,
  FORBIDDEN = 403,
  NOT_FOUND = 404,
  METHOD_NOT_ALLOWED = 405,
  NOT_ACCEPTABLE = 406,
  CONTENT_TOO_LARGE = 413,
  I_AM_A_TEAPOT = 418,
  INTERNAL_SERVER_ERROR = 500,
};

std::string getMessageByStatusCode(HTTPStatusCode statusCode);
HTTPStatusCode intToHttpStatus(const int &statusCode);
