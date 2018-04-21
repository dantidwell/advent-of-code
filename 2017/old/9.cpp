#include "aoc.h"

int PartOneScore = 0;
int PartTwoScore = 0;

int StackPointer = 0;

bool WadingThroughGarbage = false;
bool CancelNext = false;

int main(int argc, char ** argv) { 
  auto file = fopen(argv[1], "r");
  if(!file) { 
    return -1;
  }
  char c;
  while(!feof(file)) { 
    c = getc(file);
    if(CancelNext) { 
      CancelNext = false; 
      continue;
    }

    switch(c) {
      case '{':
        if(!WadingThroughGarbage) { 
          StackPointer++;
        } else { 
          PartTwoScore++;
        }
        break;
      case '}':
        if(!WadingThroughGarbage) { 
          PartOneScore += StackPointer; 
          StackPointer--;
        } else { 
          PartTwoScore++;
        }
        break;
      case '<':
        if(WadingThroughGarbage) { 
          PartTwoScore++;
        } else { 
          WadingThroughGarbage = true; 
        }
        break;
      case '>': 
        WadingThroughGarbage = false;
        break;
      case '!':
        CancelNext = true;
        break;
      case EOF: 
        break;
      default:
        if(WadingThroughGarbage) { 
          PartTwoScore++;
        }
    }
  }

  printf("--Part One--\n"); 
  printf("\tTotal score: %d\n", PartOneScore);
  printf("--Part Two--\n"); 
  printf("\tCharacters in garbage: %d\n", PartTwoScore);
  return 0;
}