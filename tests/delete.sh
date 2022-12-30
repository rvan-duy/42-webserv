#!/bin/bash

BASE_URL=http://localhost:8080
CONF="delete.conf"

# Check the value of the pid variable to determine which process is running
../webserv resource/$CONF &

  # give the webserver a sec to startup
  sleep 0.2;

  # -X DELETE to send a delete request
  DELETE=$(curl -X DELETE $BASE_URL/cgi/test.py)


  # sleep to make sure server logger is doen printing before showing results
  sleep 0.1;

  echo "DELETE response:"
  echo $DELETE

# kill the webserv program so it does not linger in the background
pkill webserv