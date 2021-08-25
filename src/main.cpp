#include "linux_parser_class.h"
#include "ncurses_display.h"
#include "os_parser.h"
#include "system.h"

int main() {
#ifdef __linux__
  LinuxParser parser;
#else
  throw "OS not supported";
#endif
  System system; // system(parser)
  IOsParser *parserptr = nullptr;
  parserptr = &parser;
  NCursesDisplay::Display(system, system.Processes().size());
}