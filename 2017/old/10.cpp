
#include <stdio.h>
#include <stdint.h>
#include <vector>

#define NumLinks 256
#define NumLengths 16

#define Ascii(x) (x) >= 10 ? ((x) + 87) : ((x) + 48); 

std::vector<uint8_t> PartOneLengths = { 
  63,
  144,
  180,
  149,
  1,
  255,
  167,
  84,
  125,
  65,
  188,
  0,
  2,
  254,
  229,
  24
};

std::vector<uint8_t> PartTwoLengths;
std::vector<uint8_t> PartTwoExtraCrap = { 
  17, 31, 73, 47, 23
};

int Ring[NumLinks];
int Skip; 
int Cursor;

void Init() { 
  for(auto i = 0; i<NumLinks; i++) { 
    Ring[i] = i;
  }
  Cursor = 0; 
  Skip = 0;
}

void Round(const std::vector<uint8_t> &lengths) { 
  for(auto l:lengths) { 
    /* STEP 1: do the reversal */
    for(auto c = 0; c < l/2; c++) { 
      auto t = Ring[(c+Cursor)%NumLinks];
      Ring[(c+Cursor)%NumLinks] = Ring[(Cursor+(l-1)-c)%NumLinks];
      Ring[(Cursor+(l-1)-c)%NumLinks] = t;
    }
    /* STEP 2: increment the Cursor */
    Cursor = Cursor + l + Skip; 
    /* STEP 3: increment the Skip */
    Skip++;
  }
}

int main() {
  Init();
  Round(PartOneLengths);

  printf("\n");
  printf("--- Part One ---\n"); 
  printf("%d * %d = %d\n", Ring[0], Ring[1], Ring[0]*Ring[1]);

  while(!feof(stdin)) {
    auto c = getc(stdin);
    if(c != EOF) { 
      PartTwoLengths.push_back(c);
    }
  }
  for(auto l:PartTwoExtraCrap) {
    PartTwoLengths.push_back(l);
  }

  Init();
  for(auto i = 0; i<64; i++) { 
    Round(PartTwoLengths); 
  }

  char hash[33];
  for(auto i = 0; i < 16; i++) { 
    char x = 0;
    for(auto j = 0; j < 16; j++) {
      x ^= Ring[i*16+j];
    }
    hash[2*i] = Ascii((x & 0xF0) >> 4);
    hash[2*i+1] = Ascii(x & 0x0F);
  }
  hash[32] = '\0';

  printf("\n");
  printf("--- Part Two ---\n"); 
  printf("Hash: %s\n", hash);
  printf("\n");

  return 0;
} 