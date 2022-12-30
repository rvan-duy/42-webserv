#!/bin/bash

BASE_URL="http://localhost:8080"
CONF="return.conf"

# Check the value of the pid variable to determine which process is running
../webserv resource/$CONF &

  # give the webserver a sec to startup
  sleep 0.2;

  # -L to follow redirects
  AUTO=$(curl -L $BASE_URL/cgi)

  # sleep to make sure server logger is doen printing before showing results
  sleep 0.2;

  # compare the responses:
  if [ "$DIRECT" == "$REDIRECT" ]; then
    echo "Output matches expected output:"; echo $DIRECT; echo -e "\033[32m[SUCCESS]\033[0m"
  else
    echo "Output does not match expected output"; echo "[DIRECT]:" $DIRECT; echo "[REDIRECT]:" $REDIRECT; echo -e "\033[31m[FAILURE]\033[0m"
  fi

# kill the webserv program so it does not linger in the background
pkill webserv