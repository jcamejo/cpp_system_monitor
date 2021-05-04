#include <string>

#include "format.h"

using std::string;
using std::to_string;

string Format::ElapsedTime(long seconds) {
  string time;
  string h;
  string m;
  string s;

  if (seconds == 0) {
    return "00:00:00";
  }

  long hours = seconds / 3600;
  long minutes = (seconds / 60) % 60;
  long secs = seconds % 60;

  h = to_string(hours);
  m = to_string(minutes);
  s = to_string(secs);

  h = hours < 10 ? ("0" + h) : h;
  m = minutes < 10 ? ("0" + m) : m;
  s = secs < 10 ? ("0" + s) : s;

  time = h + ":" + m + ":" + s;

  return time;
}