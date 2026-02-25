#!/bin/bash
HOST="127.0.0.1"
PORT="8080"
curl -v "http://$HOST:$PORT/fibonacci?hostname=fibonacci.com&fs_port=9090&number=10&as_ip=as&as_port=53533"