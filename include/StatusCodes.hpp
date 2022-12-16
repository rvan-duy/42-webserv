#pragma once

#include <string>

/* status codes for processing requests */
#define OK 200
#define BAD_REQUEST 400
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405
#define CONTENT_TOO_LARGE 413
#define I_AM_A_TEAPOT 418
#define INTERNAL_SERVER_ERROR 500

/* End of status codes */

std::string getMessageByStatusCode(int const &statusCode);