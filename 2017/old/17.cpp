#include "aoc.h"

struct Node { 
  int Value; 
  Node *Next;
};

Node* AllocNode(int value, Node *next) { 
  auto n = (Node*) malloc(sizeof(Node));
  n->Value = value; 
  n->Next = next == nullptr ? n : next;
  return n;
}

int main(int argc, char ** argv) {
  auto steps = atoi(argv[1]);
  
  auto buffer = AllocNode(0, nullptr);
  auto zero = buffer;
  
  auto FirstStop = 2017; 
  auto SecondStop = 50000000;

  auto count = 0; 
  while(count < FirstStop) { 
    for(auto i = 0; i < steps; i++) { 
      buffer = buffer->Next;
    }
    Node *temp = buffer->Next;
    buffer->Next = AllocNode(++count, temp);
    buffer = buffer->Next;
  }
  printf("--Part One--\n");
  printf("\tValue after 2017: %d\n", buffer->Next->Value);

  while(count < SecondStop) { 
    for(auto i = 0; i < steps; i++) { 
      buffer = buffer->Next;
    }
    Node *temp = buffer->Next;
    buffer->Next = AllocNode(++count, temp);
    buffer = buffer->Next;
  }
  printf("--Part Two--\n");
  printf("\tValue after 0 in the buffer (%d iterations): %d\n", SecondStop, zero->Next->Value);   
  return 0;
}