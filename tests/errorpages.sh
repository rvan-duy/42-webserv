#!/bin/bash

BASE_URL="http://localhost:8080"
CONF="error_pages.conf"
# CONF=../../config/config0.conf

# Check the value of the pid variable to determine which process is running
../webserv resource/$CONF &

  # give the webserver a sec to startup
  sleep 0.2;

  # Curl the real 405 errorpage directly at its location
  REAL_405=$(curl $BASE_URL/error_pages/405/index.html)
  # mock 404 errorpage is set to the 405 errorpage in config
  MOCK_405=$(curl $BASE_URL/mock/not_found)

  # sleep to make sure server logger is doen printing before showing results
  sleep 0.2;

  # compare the responses:
  if [ "$REAL_405" == "$MOCK_405" && "$MOCK_405" != "" ]; then
    echo "Output matches expected output:"; echo $REAL_405; echo -e "\033[32m[SUCCESS]\033[0m"
  else
    echo "Output does not match expected output"; echo "[REAL]:" $REAL_405; echo "[MOCK]:" $MOCK_405; echo -e "\033[31m[FAILURE]\033[0m"
  fi

# kill the webserv program so it does not linger in the background
pkill webserv