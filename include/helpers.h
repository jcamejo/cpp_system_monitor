#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <vector>

using std::vector;

namespace Helpers {
vector<std::string> Tokenize(std::string str, char delim);
} // namespace Helpers
#endif