#!/bin/bash
HOST="127.0.0.1"
PORT="8080"
curl -X GET "http://$HOST:$PORT/fibonacci?hostname=fibonacci.com&fs_port=9090&number=10&as_ip=127.0.0.1&as_port=53533"
