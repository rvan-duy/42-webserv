#!/usr/bin/env bash

curl --request POST \
     -H "Transfer-Encoding: chunked" \
     --http1.1 \
     --data-binary @- \
     http://localhost:8080/api/upload <<EOF
{
  "filename": "file.txt",
  "data": "aGVsbG8gd29ybGQhCg=="
}
EOF