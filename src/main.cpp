#include "linux_parser.h"
#include "ncurses_display.h"
#include "os_parser.h"
#include "system.h"

int main() {
#ifdef __linux__
  LinuxParser parser;
#else
  throw "OS not supported";
#endif
  OsParser *parserptr = nullptr;
  parserptr = &parser;
  System system(parserptr); // system(parser)
  NCursesDisplay::Display(system, system.Processes().size());
}