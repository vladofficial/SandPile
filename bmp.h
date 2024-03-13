#pragma <once> 
#include "sandpile.h"
#include "Parser.h"
#include <cinttypes>
#include <fstream> 
#include <iostream>

struct Colour {
  uint8_t r;
  uint8_t g;
  uint8_t b;

  Colour(uint8_t r_argument, uint8_t g_argument, uint8_t b_argument);
};

void FindHeightAndWidth(Sandpile* pile, int* ans);

void DrawIteration(Sandpile* pile, char* path, ParsedArguments* arguments);