#!/bin/bash
HOST="127.0.0.1"
PORT="9090"

echo "Starting tests"

curl -X PUT "http://$HOST:$PORT/register" \
  -H "Content-Type: application/json" \
  -d '{"hostname":"fibonacci.com","ip":"fs","as_ip":"as","as_port":"53533"}'

echo "Registered FS in DNS"
sleep 1
echo ""
echo "Checking registration"
echo ""
echo "TYPE=A
NAME=fibonacci.com" | nc -u -w1 127.0.0.1 53533
echo ""
echo ""
echo "Registration queried"
echo ""
sleep 1

echo "Making user service request"

PORT="8080"
curl -X GET "http://$HOST:$PORT/fibonacci?hostname=fibonacci.com&fs_port=9090&number=10&as_ip=as&as_port=53533"