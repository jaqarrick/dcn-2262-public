#!/bin/bash
echo "TYPE=A
NAME=fibonacci.com" | nc -u -w1 127.0.0.1 53533
