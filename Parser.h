#pragma once
#include "sandpile.h"
#include <cinttypes>
#include <fstream>
#include <iostream>

struct ParsedArguments {
  char* input;
  char* output;
  int64_t max_iter;
  int64_t freq = 0;
};

bool ValidFilename(char* path);

bool IsNumber(char* num);

void ParseArguments(int16_t argc, char** argv, ParsedArguments* arguments);

int16_t FindMaxCords(char* path);

void ParseFile(char* path, Sandpile* pile);
