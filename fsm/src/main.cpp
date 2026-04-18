#include "tcp.h"
#include "tokenizer.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void print_usage(char *argv[]) {
  cout << "Usage: " << argv[0] << "[options] inputfile" << endl;
  cout << endl;
  cout << "Options:" << endl;
  cout << "\t-v\tVerbose Mode" << endl;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    print_usage(argv);
    return 1;
  }

  bool verbose = false;
  char *filename = nullptr;

  for (int i = 1; i < argc; i++) {
    string arg = string(argv[i]);
    if (arg == "-v") {
      cout << "Verbose mode enabled" << endl;
      verbose = true;
    } else if (arg[0] == '-') {
      cerr << "Invalid flag" << endl;
      print_usage(argv);
      return 1;
    } else {
      filename = argv[i];
    }
  }

  if (filename == nullptr) {
    cerr << "Error: no input file specified" << endl;
    print_usage(argv);
    return 1;
  }

  Tokenizer t(filename, verbose);
  TCP_FCM tcp_fsm(verbose);

  string *token;
  while ((token = t.getToken()) != nullptr) {
    tcp_fsm.doEvent(*token);
  }

  return 0;
}
