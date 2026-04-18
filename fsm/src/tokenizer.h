#ifndef TOKENIZER_H
#define TOKENIZER_H

using namespace std;
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Tokenizer {

private:
  ifstream in_file;
  istringstream line;
  string currentToken;

  bool verbose = false;

public:
  Tokenizer(const string &filename, bool verbose) : in_file(filename) {
    if (verbose) {
      cout << "Opening file: " << filename << endl;
    }

    if (!in_file.is_open()) {
      throw runtime_error("Failed to open file: " + filename);
    }
  }

  ~Tokenizer() {
    if (in_file.is_open()) {
      if (verbose)
        cout << "Closing file" << endl;
      in_file.close();
    }
  }

  string *getToken() {
    while (true) {
      if (line >> currentToken) {
        if (verbose)
          cout << "Token: " << currentToken << endl;
        return &currentToken;
      }
      string buf;
      if (!getline(in_file, buf)) {
        if (verbose)
          cout << "EOF reached" << endl;
        return nullptr;
      }
      if (verbose)
        cout << "Reading line: " << buf << endl;
      line.clear();
      line.str(buf);
    }
  }
};

#endif
