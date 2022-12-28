#!/bin/bash

BASE_URL=http://localhost:8080

# Check the value of the pid variable to determine which process is running
../webserv resource/return.conf &

  sleep 0.1;

  DIRECT=$(curl -L $BASE_URL/direction)
  # -L to follow redirects
  REDIRECT=$(curl -L $BASE_URL/redirection)
  if [ "$DIRECT" == "$REDIRECT" ]; then
    sleep 1; echo "Output matches expected output:"; echo $DIRECT
  else
    sleep 1; echo "Output does not match expected output"
  fi

# kill the webserv program so it does not linger in the background
pkill webserv