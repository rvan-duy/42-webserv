#!/bin/bash

BASE_URL="http://localhost:8080"
CONF="autoindex.conf"

# Check the value of the pid variable to determine which process is running
../webserv resource/$CONF &

  # give the webserver a sec to startup
  sleep 0.2;

  # -L to follow redirects
  AUTO=$(curl -L $BASE_URL/cgi)

  # sleep to make sure server logger is doen printing before showing results
  sleep 0.1;

  # compare the responses:
  echo "AUTOINDEX html:"
  echo $AUTO

# kill the webserv program so it does not linger in the background
pkill webserv