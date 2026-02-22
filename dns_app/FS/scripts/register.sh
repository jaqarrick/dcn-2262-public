#!/bin/bash
HOST="127.0.0.1"
PORT="9090"
curl -X PUT "http://$HOST:$PORT/register" \
  -H "Content-Type: application/json" \
  -d '{"hostname":"fibonacci.com","ip":"127.0.0.1","as_ip":"127.0.0.1","as_port":"53533"}'