#include "sandpile.h"
#include "Parser.h"
#include "bmp.h"
#include <cinttypes>
#include <string.h>
#include <fstream> 
#include <iostream>

Colour::Colour(uint8_t r_argument, uint8_t g_argument, uint8_t b_argument) {
  r = r_argument;
  g = g_argument;
  b = b_argument;
}

Colour kWhite(255, 255, 255);
Colour kBlack(0, 0, 0);
Colour kGreen(0, 128, 0);
Colour kYellow(255, 255, 0);
Colour kPurple(139, 0, 255);

void FindHeightAndWidth(Sandpile* pile, int* ans) {
  uint16_t max_x = 0;
  uint16_t max_y = 0;
  uint16_t min_x = UINT16_MAX;
  uint16_t min_y = UINT16_MAX;
  for (uint16_t i = 0; i < pile->size; ++i) {
    for (uint16_t j = 0; j < pile->size; ++j) {
        if (pile->grid[i][j] != 0)
        {
            max_x = std::max(max_x, j);
            max_y = std::max(max_y, i);
            min_x = std::min(min_x, j);
            min_y = std::min(min_y, i);
        }
    }
  }
  ans[0] = max_x;
  ans[1] = max_y;
  ans[2] = min_x;
  ans[3] = min_y;
}

const uint32_t kBitMapHeaderSize = 14;
const uint32_t kBitsPerPixel = 4;
const uint32_t kInfoHeaderSize = 40;
const uint32_t kNumOfColors = 5;

void DrawIteration(Sandpile* pile, char* path, ParsedArguments* arguments) {
  int* ans = new int[4];
  FindHeightAndWidth(pile, ans);
  const uint16_t kColourPalleteSize = kNumOfColors * 4;
  const uint16_t kWidth = ans[0] - ans[2];
  const uint16_t kHeight = ans[1] - ans[3];
  const uint16_t kRowSize = ((kBitsPerPixel * kWidth + 31) / 32) * 4;
  const uint64_t kPixelArraySize = kRowSize * kHeight;
  const uint16_t padding = (8 - (kWidth % 8)) % 8;
  const uint64_t kFileSize = kBitMapHeaderSize + kInfoHeaderSize + kColourPalleteSize + (kWidth + padding) * kHeight / 2;
  uint8_t bit_map_header[kBitMapHeaderSize] = {};
  bit_map_header[0] = 'B';
  bit_map_header[1] = 'M';
  bit_map_header[2] = (unsigned char)kFileSize;
  bit_map_header[3] = kFileSize >> 8;
  bit_map_header[4] = kFileSize >> 16;
  bit_map_header[5] = kFileSize >> 24;
  bit_map_header[6] = 0;
  bit_map_header[7] = 0;
  bit_map_header[8] = 0;
  bit_map_header[9] = 0;
  bit_map_header[10] = kBitMapHeaderSize + kInfoHeaderSize + kColourPalleteSize;
  bit_map_header[11] = 0;
  bit_map_header[12] = 0;
  bit_map_header[13] = 0;
  uint8_t info_header[kInfoHeaderSize] = {};
  info_header[0] = kInfoHeaderSize;
  info_header[1] = 0;
  info_header[2] = 0;
  info_header[3] = 0;
  info_header[4] = kWidth;
  info_header[5] = kWidth >> 8;
  info_header[6] = kWidth >> 16;
  info_header[7] = kWidth >> 24;
  info_header[8] = kHeight;
  info_header[9] = kHeight >> 8;
  info_header[10] = kHeight >> 16;
  info_header[11] = kHeight >> 24;
  info_header[12] = 1;
  info_header[13] = 0;
  info_header[14] = kBitsPerPixel;
  info_header[15] = 0;
  info_header[32] = kNumOfColors;
  info_header[33] = 0;
  info_header[34] = 0;
  info_header[35] = 0;
  uint8_t colour_pallete[kColourPalleteSize] = {};
  
  colour_pallete[0] = kWhite.b;
  colour_pallete[1] = kWhite.g;
  colour_pallete[2] = kWhite.r;
  colour_pallete[3] = 0;
  colour_pallete[4] = kGreen.b;
  colour_pallete[5] = kGreen.g;
  colour_pallete[6] = kGreen.r;
  colour_pallete[7] = 0;
  colour_pallete[8] = kYellow.b;
  colour_pallete[9] = kYellow.g;
  colour_pallete[10] = kYellow.r;
  colour_pallete[11] = 0;
  colour_pallete[12] = kPurple.b;
  colour_pallete[13] = kPurple.g;
  colour_pallete[14] = kPurple.r;
  colour_pallete[15] = 0;
  colour_pallete[16] = kBlack.b;
  colour_pallete[17] = kBlack.g;
  colour_pallete[18] = kBlack.r;
  colour_pallete[19] = 0;
  char new_path[300] = {};
  char buffer[100] = {};
  int temp = arguments->max_iter - pile->iterations_left;
  int iter = 0;
  while (temp != 0) {
    buffer[iter++] = '0' + temp % 10;
    temp /= 10;
  }
  strcat(new_path, path);
  strcat(new_path, "\\iteration");
  for(int i = 0; i < iter / 2; ++i) {
    std::swap(buffer[i], buffer[iter - i - 1]);
  }
  strcat(new_path, buffer);
  strcat(new_path, ".bmp");
  std::ofstream output(new_path, std::ios::binary);
  output.write(reinterpret_cast<char*>(bit_map_header), kBitMapHeaderSize);
  output.write(reinterpret_cast<char*>(info_header), kInfoHeaderSize);
  output.write(reinterpret_cast<char*>(colour_pallete), kColourPalleteSize);
  int64_t color, left_color, right_color;
  for (int64_t y = ans[1]; y >= ans[3]; --y) {
    for (int64_t x = ans[2]; x < ans[0] + padding; x += 2) {
      if (x > ans[0]) {
        left_color = 0;
        right_color = 0;
      } else if (x == ans[0]) {
        left_color = pile->grid[y][x];
        right_color = 0;
      } else {
        left_color = pile->grid[y][x];
        right_color = pile->grid[y][x + 1];
      }
      if (left_color > 4) {
        left_color = 4;
      }
      if (right_color > 4) {
        right_color = 4;
      }
      color = (left_color << 4) + right_color;
      output.put(color);
      
    }
  }
  delete[] ans;
  output.close();
}
