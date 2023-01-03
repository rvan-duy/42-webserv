#!/bin/bash

BASE_URL="http://localhost:8080"
CONF="bigfile.conf"
FILE_PATH=resource/large.txt

# Check the value of the pid variable to determine which process is running
../webserv resource/$CONF &

  # give the webserver a sec to startup
  sleep 0.2;

  # -L to follow redirects
  BGFILE=$(curl -X POST --data-binary "@$FILE_PATH" $BASE_URL/cgi/upload.py)

  # sleep to make sure server logger is doen printing before showing results
  sleep 0.1;

# kill the webserv program so it does not linger in the background
pkill webserv


# curl -X POST --data-binary "Hello world\!" http://localhost:8090/cgi/upload.py 