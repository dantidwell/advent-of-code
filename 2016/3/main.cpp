#include "aoc.h"

int main(int argc, char **argv) { 
  auto f = fopen(argv[1], "r"); 
  if(!f) { 
    return -1;
  }

  int trisPart1 = 0;
  int trisPart2 = 0;

  while(feof(f) == 0) { 
    //read in a 3x3 portion of the data
    char n[16] = {0};
    int sides[3][3];
    for(auto y = 0; y < 3; y++) { 
      for(auto x = 0; x < 3; x++) { 
        Aoc::EatWhiteSpace(f);
        Aoc::ReadToDelimiter(f, n, 16, x==2 ? '\n' : ' '); 
        sides[y][x] = atoi(n);
      }
    }

    //tris by row
    for(auto y = 0; y < 3; y++) { 
      if(
        sides[y][0]+sides[y][1] > sides[y][2]
        && sides[y][0]+sides[y][2] > sides[y][1]
        && sides[y][1]+sides[y][2] > sides[y][0]
      ) { 
        trisPart1++;
      }
    }

    //tris by column
    for(auto x = 0; x < 3; x++) { 
      if(
        sides[0][x]+sides[1][x] > sides[2][x]
        && sides[0][x]+sides[2][x] > sides[1][x]
        && sides[1][x]+sides[2][x] > sides[0][x]
      ) { 
        trisPart2++;
      }
    }
  }
  printf("There are %d legit triangles in Part 1.\n", trisPart1);
  printf("There are %d legit triangles in Part 2.\n", trisPart2);
  return 0;
}