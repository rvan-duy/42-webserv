#!/bin/bash

# Set the number of requests to send
REQUESTS=100

# Set the base URL of the web server
BASE_URL=http://localhost:8090

# Set the name of the file to compare the output with
FILE=control.txt
CONTROL_HTML=$(curl $BASE_URL/error)
CONTROL_JS=$(curl $BASE_URL/error_pages/404/script.js)
CONTROL_CSS=$(curl $BASE_URL/error_pages/404/style.css)

# Send the requests
for i in $(seq 1 $REQUESTS)
do
  # Send the request and store the output in a variable
  OUTPUT=$(curl $BASE_URL/error)

  # Compare the output with the contents of the file
  if [ "$OUTPUT" == "$CONTROL_HTML" ]; then
    echo "Output matches expected output"
  else
    echo "[HTML] Output does not match expected output"
    break
  fi

  # Send the request and store the output in a variable
  OUTPUT=$(curl $BASE_URL/error_pages/404/script.js)

  # Compare the output with the contents of the file
  if [ "$OUTPUT" == "$CONTROL_JS" ]; then
    echo "Output matches expected output"
  else
    echo "[JS] Output does not match expected output"
    break
  fi

  # Send the request and store the output in a variable
  OUTPUT=$(curl $BASE_URL/error_pages/404/style.css)

  # Compare the output with the contents of the file
  if [ "$OUTPUT" == "$CONTROL_CSS" ]; then
    echo "Output matches expected output"
  else
    echo "[CSS] Output does not match expected output"
    break
  fi
done

# Wait for all the curl processes to finish
wait