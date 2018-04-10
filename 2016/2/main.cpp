#include "aoc.h"

struct VecPart1 { 
  int x = 0; 
  int y = 0;

  VecPart1 operator+(const VecPart1 &other) { 
    return { 
      abs(x+other.x) > 1 ? x : x + other.x, 
      abs(y+other.y) > 1 ? y : y + other.y
    };
  }

  int Floor() { 
    return Floors[1-y][x+1];
  }

  private: 
    static constexpr int Floors[5][5] = {
      {1, 2, 3}, 
      {4, 5, 6}, 
      {7, 8, 9} 
    };
};

struct VecPart2 { 
  int x = -2; 
  int y = 0;

  VecPart2 operator+(const VecPart2 &other) { 
    if(abs(x+other.x) + abs(y+other.y) > 2) { 
      return *this;
    }
    return { x+other.x, y+other.y };
  }

  int Floor() { 
    return Floors[2 - y][x + 2]; 
  }

  private: 
    static constexpr int Floors[5][5] = {
      {0,   0,   1,   0,   0}, 
      {0,   2,   3,   4,   0}, 
      {5,   6,   7,   8,   9}, 
      {0, 0xA, 0xB, 0xC,   0},
      {0,   0, 0xD,   0,   0} 
    };
};

int main(int argc, char** argv) { 
  auto f = fopen(argv[1], "r"); 
  if(!f) { 
    return 1;
  }

  int c1 = 0, c2 = 0;
  VecPart1 p1;
  VecPart2 p2;
  while(feof(f) == 0) { 
    switch(getc(f)) { 
      case 'U':
        p1 = p1 + VecPart1{0, 1};
        p2 = p2 + VecPart2{0, 1};
        break;
      case 'D':
        p1 = p1 + VecPart1{0,-1};
        p2 = p2 + VecPart2{0,-1};
        break;
      case 'L':
        p1 = p1 + VecPart1{-1,0};
        p2 = p2 + VecPart2{-1,0};
        break;
      case 'R':
        p1 = p1 + VecPart1{1,0};
        p2 = p2 + VecPart2{1,0};
        break;
      case '\n':
        c1 = c1 * 10 + p1.Floor();
        c2 = c2 * 16 + p2.Floor();
        break;
      case '\r':
        /* skip carriage returns in win32 */
        break;
    }
  }
  c1 = c1 * 10 + p1.Floor();
  c2 = c2 * 16 + p2.Floor();
  printf("The secret code in part 1 is %d\n", c1);
  printf("The secret code in part 2 is %x\n", c2);
  return 0;
}