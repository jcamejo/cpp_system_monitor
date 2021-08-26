#include "helpers.h"

#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::stringstream;
using std::vector;

vector<string> Helpers::Tokenize(string str, char delim) {
  stringstream ss(str);
  string tmp;
  vector<string> tokens{};

  while (std::getline(ss, tmp, delim)) {
    tokens.push_back(tmp);
  }

  return tokens;
}

bool IsNumber(const string &str) {
  for (char const &c : str) {
    if (std::isdigit(c) == 0) {
      return false;
    }
  }

  return true;
}