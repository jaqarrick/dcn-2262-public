#!/bin/bash
HOST="127.0.0.1"
PORT="9090"
curl -X GET "http://$HOST:$PORT/fibonacci?number=10"
