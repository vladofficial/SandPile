#include "sandpile.h"
#include <cinttypes>
#include <fstream>
#include <iostream> 

Node::Node(int64_t num, int16_t x_arg, int16_t y_arg) {
    value = num;
    x = x_arg;
    y = y_arg;
    next_node = nullptr;
}

IterationQueue::IterationQueue() {
  front = rear = nullptr;
}

void IterationQueue::push(int64_t num, int16_t x, int16_t y) {
  Node* newnode = new Node(num, x, y);
  if (rear == nullptr && front == nullptr) {
    rear = front = newnode;
    return;
  }
  front->next_node = newnode;
  front = newnode;
}

void IterationQueue::pop() {
  if (front == nullptr) {
      return;
  }
  Node* temp = rear;
  rear = rear->next_node;
  if (rear == nullptr)
    front = nullptr;
  delete (temp);
}

Sandpile::Sandpile(int16_t given_size) {
  grid = new int64_t*[given_size];
  for (int i = 0 ; i < given_size; ++i) {
    grid[i] = new int64_t[given_size];
    for (int j = 0; j < given_size; ++j) {
      grid[i][j] = 0;
    }
  }
  size = given_size;
  iteration_queue = IterationQueue();
}

void ExpandGrid(Sandpile* pile) {
  int64_t** new_grid = new int64_t*[pile->size * 2];
  for (int i = 0 ; i < pile->size * 2; ++i) {
    new_grid[i] = new int64_t[pile->size * 2];
    for (int j = 0; j < pile->size * 2; ++j) {
      new_grid[i][j] = 0;
    }
  }
  for (int i = 0; i < pile->size; ++i) {
    for (int j = 0; j < pile->size; ++j) {
      new_grid[i + (pile->size / 2)][j + (pile->size / 2)] = pile->grid[i][j];
    }
  }
  for (int i = 0; i < pile->size; ++i) {
    delete[]pile->grid[i];
  }
  delete[] pile->grid;
  pile->size *= 2;
  pile->grid = new_grid;
}

void DoIteration(Sandpile* pile) { 
  bool is_stable = true;
  for (int i = 0; i < pile->size; ++i) {
    for (int j = 0; j < pile->size; ++j) {
      if (pile->grid[i][j] >= 4) {
        pile->iteration_queue.push(pile->grid[i][j], j, i);
        is_stable = false;
      }
    }
  }
  if(is_stable) {
    pile->is_stable = true;
    return;
  }
  int16_t x;
  int16_t y;
  int64_t grains;
  int16_t expand_num = 0;
  while(pile->iteration_queue.rear!= nullptr) {
    x = pile->iteration_queue.rear->x;
    y = pile->iteration_queue.rear->y;
    grains = pile->iteration_queue.rear->value;
    pile->iteration_queue.pop();
    if (x + 2 >= pile->size || y + 2 >= pile->size || x - 2 <= 0 || y - 2 <= 0) {
      ExpandGrid(pile);
      expand_num += pile->size / 4;
    }
    if (grains >= 4) { 
      pile->grid[y + expand_num][x + expand_num] -= 4;
      ++pile->grid[y + 1 + expand_num][x + expand_num];
      ++pile->grid[y + expand_num][x + 1 + expand_num];
      ++pile->grid[y - 1 + expand_num][x + expand_num];
      ++pile->grid[y + expand_num][x - 1 + expand_num];
    }
  }
  --pile->iterations_left;
}