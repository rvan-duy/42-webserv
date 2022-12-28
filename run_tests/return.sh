#!/bin/bash

BASE_URL=http://localhost:8080

# Check the value of the pid variable to determine which process is running
../webserv resource/return.conf &

  # give the webserver a sec to startup
  sleep 0.1;

  # -L to follow redirects
  DIRECT=$(curl -L $BASE_URL/direction)
  REDIRECT=$(curl -L $BASE_URL/redirection)

  # compare the responses:
  if [ "$DIRECT" == "$REDIRECT" ]; then
    sleep 0.1; echo "Output matches expected output:"; echo $DIRECT; echo -e "\033[32m[SUCCESS]\033[0m"
  else
    sleep 0.1; echo "Output does not match expected output"; echo "[DIRECT]:" $DIRECT; echo "[REDIRECT]:" $REDIRECT; echo -e "\033[31m[FAILURE]\033[0m"
  fi

# kill the webserv program so it does not linger in the background
pkill webserv