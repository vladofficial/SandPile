#include "Parser.h"
#include "sandpile.h"
#include "bmp.h"
#include <iostream>

int main(int argc, char** argv) {
  ParsedArguments arguments;
  ParseArguments(argc, argv, &arguments);
  int16_t size = FindMaxCords(arguments.input);
  Sandpile pile = Sandpile(size * 2 + 2);
  pile.is_stable = false;
  pile.freq = arguments.freq;
  ParseFile(arguments.input, &pile);
  pile.iterations_left = arguments.max_iter;
  while (pile.iterations_left != 0 && !pile.is_stable) {
    DoIteration(&pile);
    if (pile.freq != 0 && pile.iterations_left % pile.freq == 0) {
      DrawIteration(&pile, arguments.output, &arguments);
    }
  }
  if (pile.freq == 0 || pile.is_stable) {
    DrawIteration(&pile, arguments.output, &arguments);
  }
}