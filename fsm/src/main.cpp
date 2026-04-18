#include "tcp.h"
#include "tokenizer.h"
#include <iostream>
#include <string>

using namespace std;

void print_usage(char *argv[]) {
  cout << "Usage: " << argv[0] << " [options]" << endl;
  cout << endl;
  cout << "Options:" << endl;
  cout << "\t-v\tVerbose Mode" << endl;
}

int main(int argc, char *argv[]) {
  bool verbose = false;

  for (int i = 1; i < argc; i++) {
    string arg = string(argv[i]);
    if (arg == "-v") {
      cout << "Verbose mode enabled" << endl;
      verbose = true;
    } else {
      cerr << "Invalid flag" << endl;
      print_usage(argv);
      return 1;
    }
  }

  Tokenizer t(verbose);
  TCP_FCM tcp_fsm(verbose);

  string *token;
  while ((token = t.getToken()) != nullptr) {
    tcp_fsm.doEvent(*token);
  }

  return 0;
}
