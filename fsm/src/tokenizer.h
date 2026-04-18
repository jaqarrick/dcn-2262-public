#ifndef TOKENIZER_H
#define TOKENIZER_H

using namespace std;
#include <iostream>
#include <sstream>
#include <string>

class Tokenizer {

private:
  istream& in;
  istringstream line;
  string currentToken;
  bool verbose;

public:
  Tokenizer(bool verbose, istream& in = cin) : in(in), verbose(verbose) {
    if (verbose)
      cout << "Reading from stdin" << endl;
  }

  string *getToken() {
    while (true) {
      if (line >> currentToken) {
        if (verbose)
          cout << "Token: " << currentToken << endl;
        return &currentToken;
      }
      string buf;
      if (!getline(in, buf)) {
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
