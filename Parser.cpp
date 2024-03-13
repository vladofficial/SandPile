#include "Parser.h"
#include "sandpile.h"
#include <fstream>
#include <cstring>
#include <iostream>

bool ValidFilename(char* path) {
  std::ifstream test_file(path);
  return test_file.is_open();
}

bool IsNumber(char* num) {
  for (; *num != '\0'; ++num) {
    if (!std::isdigit(*num)) {
      return false;
    }
  }
  return true;
}

void ParseArguments(int16_t argc, char** argv, ParsedArguments* arguments) {
  for (uint8_t argument_iterator; argument_iterator < argc; ++argument_iterator) {
    if (argument_iterator == argc - 1 && argv[argument_iterator][0] == '-' && argv[argument_iterator][1] != '-') {
      std::cout << "Error: no value for argument";
    }
    if (!strcmp(argv[argument_iterator], "-i")) {
      if (ValidFilename(argv[argument_iterator + 1])) {
        arguments->input = argv[argument_iterator + 1];
      }
    }
    if (!(strncmp(argv[argument_iterator], "--input=", 8))) {
      if (ValidFilename(argv[argument_iterator] + 8)) {
        arguments->input = argv[argument_iterator] + 8;
      }
    }
    if (!strcmp(argv[argument_iterator], "-o")) {
      arguments->output = argv[argument_iterator + 1];
    }
    if (!(strncmp(argv[argument_iterator], "--output=", 9))) {
      arguments->output = argv[argument_iterator] + 9;
    }
    if (!strcmp(argv[argument_iterator], "-m")) {
      if (IsNumber(argv[argument_iterator + 1])) {
        arguments->max_iter = std::atoll(argv[argument_iterator + 1]);
      }
    }
    if (!(strncmp(argv[argument_iterator], "--max_iter=", 11))) {
      if (IsNumber(argv[argument_iterator] + 11)) {
        arguments->max_iter = std::atoll(argv[argument_iterator] + 11);
      }
    }
    if (!strcmp(argv[argument_iterator], "-f")) {
      if (IsNumber(argv[argument_iterator + 1])) {
        arguments->freq = std::atoll(argv[argument_iterator + 1]);
      }
    }
    if (!(strncmp(argv[argument_iterator], "--freq", 6))) {
      if (IsNumber(argv[argument_iterator] + 6)) {
        arguments->freq = std::atoll(argv[argument_iterator] + 6);
      }
    }
  }
}

int16_t FindMaxCords(char* path) {
  std::ifstream input(path, std::ios::binary);
  int16_t x = 0;
  int16_t y = 0;
  bool gotx = 0;
  bool goty = 0;
  char got = 'a';
  int ans = 0;
  while(input.get(got)) {
    if (got == '-') {
        continue;
    }
    if (got == '\n' || got == '\r') {
      x = std::max(x, y);
      if (ans < x) {
        ans = x;
      }
      gotx = 0;
      goty = 0; 
      x = y = 0;
      if (got == '\r'){
        input.get(got);
      }
      continue;
    }
    if (got == '\t') { 
      if (!gotx) {
        gotx = true;
      } else if (gotx && !goty) {
        goty = true;
      }
      continue;
    }
    if (gotx && !goty) {
      y = y * 10 + (got - '0');
    } else if (!gotx && !goty) {
      x = x * 10 + (got - '0');
    }
  }
  return ans;
}

void ParseFile(char* path, Sandpile* pile) {
  std::ifstream input(path, std::ios::binary);
  int16_t x = 0;
  int16_t y = 0;
  uint64_t grains = 0;
  bool gotx = 0;
  bool goty = 0;
  char got = 'a';
  bool negative = false;
  while(input.get(got)) {
    if (got == '-') {
        negative = true;
        continue;
    }
    if (got == '\n' || got == '\r') {
      pile->grid[y][x] = grains;
      negative = false;
      gotx = 0;
      goty = 0; 
      x = y = grains = 0;
      if (got == '\r'){
        input.get(got);
      }
      continue;
    }
    if (got == '\t') { 
      if (!gotx) {
        gotx = true;
        if (negative) {
          x = -x;
        }
        negative = false;
        x = x + pile->size/2;
      } else if (gotx && !goty) {
        goty = true;
        if (negative) {
          y = -y;
        }
        negative = false;
        y = y + pile->size/2;
      }
      continue;
    }
    if (gotx && goty) {
        grains = grains * 10 + (got - '0');
    } else if (gotx) {
      y = y * 10 + (got - '0');
    } else {
      x = x * 10 + (got - '0');
    }
  }
}