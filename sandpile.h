#pragma once
#include <cinttypes>
#include <fstream>
#include <iostream> 

struct Node {
  int64_t value;
  int16_t x;
  int16_t y;
  Node* next_node;
  Node(int64_t num, int16_t x_arg, int16_t y_arg);
};

struct IterationQueue{
  Node* front;
  Node* rear;

  IterationQueue();

  void push(int64_t num, int16_t x, int16_t y);

  void pop();
};

struct Sandpile {
  int64_t** grid;
  int64_t iterations_left;
  IterationQueue iteration_queue;
  int16_t freq = 0;
  int16_t size;
  bool is_stable;
  Sandpile(int16_t given_size);
};

void ExpandGrid(Sandpile* pile);

void DoIteration(Sandpile* pile);

