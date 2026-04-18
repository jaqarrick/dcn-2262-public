#!/bin/bash
if ! command -v clang-format &>/dev/null; then
  echo "Error: clang-format not installed"
  echo -e "You can install with\nsudo apt install clang-format\n"
  exit 1
fi

echo "Formatting .cpp and .h files in src directory"
clang-format -i ./src/*.cpp
clang-format -i ./src/*.h
echo "Formatting complete"
