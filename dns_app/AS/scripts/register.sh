#!/bin/bash
echo "TYPE=A
NAME=fibonacci.com VALUE=127.0.0.1 TTL=1000" | nc -u -w1 127.0.0.1 53533
