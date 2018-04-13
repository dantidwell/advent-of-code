#include "../aoc.h"
#include "md5.cpp"

struct IndexState { 
  uint32_t Index = 200;
  uint32_t IndexDigits = 3;
  uint32_t IndexStartWord = 2;
  uint32_t IndexStartByte = 2; 
};

/**
 * IncrementEightWide increments the decimal index in place and ensures all 
 * padding needed by the md5 algorithm is in place
 */
MEMALIGN(64) Aoc::SimdOp8 Increment[4];
MEMALIGN(64) Aoc::SimdOp8 CarryLimit[4];
MEMALIGN(64) Aoc::SimdOp8 CarryAddMask[4];
MEMALIGN(64) Aoc::SimdOp8 CarryCmpMask[4];
MEMALIGN(64) Aoc::SimdOp8 CarrySubMask[4];

static void InitIncrement() { 
  for(auto i = 0; i < 4; i++) { 
    Increment[i] = 0x00000008 << (8*i); 
  }
  for(auto i = 0; i < 4; i++) { 
    CarryLimit[i] = 0x00000039 << (8*i); 
  }
  for(auto i = 0; i < 4; i++) { 
    CarryAddMask[i] = 0x00000001 << (8*i); 
  }
  for(auto i = 0; i < 4; i++) { 
    CarryCmpMask[i] = 0x000000FF << (8*i);
  }
  for(auto i = 0; i < 4; i++) { 
    CarrySubMask[i] = 0x0000000A << (8*i);
  }
}

static void IncrementEightWide(uint32_t Input[][8], IndexState &s) { 
  s.Index += 8;
  auto w = s.IndexStartWord;
  auto b = s.IndexStartByte;

  /* increment the ones place and test for carry */
  auto lanes = Aoc::SimdOp8::Load(Input[w]) + Increment[b]; 
  auto carry = (lanes & CarryCmpMask[b]) > CarryLimit[b];
  lanes = lanes - (carry & CarrySubMask[b]); //subtract 10 from places than needed to be carried
  lanes.Store(Input[w]);
  w = b == 0 ? w-1 : w;
  b = b == 0 ? 3 : b-1;
  while (w >= 2) { 
    /* distribute the carry down the rest of this word */  
    lanes = Aoc::SimdOp8::Load(Input[w]);
    for(int32_t i = b; i >= 0; i--) {
      lanes = lanes + (carry & CarryAddMask[i]); //carry the one to the next decimal place 
      carry = (lanes & CarryCmpMask[i]) > CarryLimit[i]; //determine which overflowed and need to be carried
      /* no carry needed down the rest of this word, so store and bail */
      if(carry ==  0) { 
        lanes.Store(Input[w]);
        return;
      }
      lanes = lanes - (carry & CarrySubMask[i]); //subtract 10 from places than needed to be carried
    }
    lanes.Store(Input[w--]);
    /* we carried off then end of a word, so store this word and load the next word */
    b = 3;
  }

  // CASE 3: We've completely overflowed and need to add another digit
  // while also updating the md5 padding to reflect this
  s.IndexDigits++;
  s.IndexStartByte = (s.IndexStartByte + 1) % 4;
  s.IndexStartWord = 2 + ((s.IndexDigits-1) / 4);  

  /* the index will always lead with a one... */
  Aoc::SimdOp8(0x30303031).Store(Input[2]);
  
  /* add the trailing 0x80 paddng needed by md5 */
  switch(s.IndexStartByte) { 
    case 0: 
      Aoc::SimdOp8(0x00008030).Store(Input[s.IndexStartWord]);
      for(auto i = 0; i < 8; i++) { 
        Input[s.IndexStartWord][i] += (i * 0x00000001);
      }
      break;
    case 1: 
      Aoc::SimdOp8(0x00803030).Store(Input[s.IndexStartWord]);
      for(auto i = 0; i < 8; i++) { 
        Input[s.IndexStartWord][i] += (i * 0x00000100);
      }
      break;
    case 2: 
      Aoc::SimdOp8(0x80303030).Store(Input[s.IndexStartWord]);
      for(auto i = 0; i < 8; i++) { 
        Input[s.IndexStartWord][i] += (i * 0x00010000);
      }
      break;
    case 3:
      Aoc::SimdOp8(0x00000080).Store(Input[s.IndexStartWord+1]);
      for(auto i = 0; i < 8; i++) { 
        Input[s.IndexStartWord][i] += (i * 0x01000000);
      }
      break;
  }

  /* store length of the string for the md5 algorithm */
  Aoc::SimdOp8 length((s.IndexDigits + 8) * 8);
  length.Store(Input[14]);
}

void Init(const char * seed, uint32_t  Input[][8]) {  
  memset(Input, 0, 16*8*sizeof(uint32_t));

  for(auto k = 0; k < strlen(seed) / 4; k++) {
    for (auto j = 0; j < 8; j++) {
      Input[k][j] = *(((uint32_t*)seed) + k);
    }
  }
  for (auto j = 0; j < 8; j++) {
    Input[2][j] = (0x303032 + (j<<16)) | 0x80000000;
  }
  for (auto j = 0; j < 8; j++) {
    Input[14][j] = 0x58;
  }
}

int main() {  
  MEMALIGN(64) uint32_t Input[16][8];
  MEMALIGN(64) uint32_t Hashes[8] = { 0 };
  uint8_t Chars[16] = { '0','1','2','3','4','5','6','7','8','9','a', 'b', 'c', 'd', 'e', 'f'};
  
  InitIncrement();

  /* Part One */
  uint8_t passwordOne[9] = { 0 };
  uint32_t charsFoundOne =  0;
  IndexState indexOne = { 0 };

  printf("Executing Part 1...\n");
  
  auto startOne = std::chrono::high_resolution_clock::now();
  Init("wtnhxymk", Input);
  while(charsFoundOne < 8) {
    Md5EightWide(Input, Hashes);
    for (auto i = 0; i < 8; i++) { 
      if(!(Hashes[i] & 0x00F0FFFF)) { 
        passwordOne[charsFoundOne++] = Chars[(Hashes[i] & 0x000F0000) >> 16]; 
      }
    }    
    IncrementEightWide(Input, indexOne);
  }
  auto endOne = std::chrono::high_resolution_clock::now();
  printf("Found password after %d iterations.  The password is %s.\n", indexOne.Index, passwordOne);
  printf("Duration: %ld (ms)\n", (endOne-startOne).count() / 1000000);

  /* Part Two */
  uint8_t passwordTwo[9] = { 0 };
  uint32_t charsFoundTwo =  0;
  IndexState indexTwo = { 0 };

  printf("\nExecuting Part 2...\n");

  auto startTwo = std::chrono::high_resolution_clock::now();
  Init("wtnhxymk", Input);
  while(charsFoundTwo < 8) {
    Md5EightWide(Input, Hashes);
    for (auto i = 0; i < 8; i++) { 
      if(!(Hashes[i] & 0x00F0FFFF)) {
        auto idx = (Hashes[i] & 0x000F0000) >> 16;
        auto ch = (Hashes[i] & 0xF0000000) >> 28;
        if(idx < 8 && passwordTwo[idx] == 0) { 
          passwordTwo[idx] = Chars[ch];
          charsFoundTwo++;
        }
      }
    }    
    IncrementEightWide(Input, indexTwo);
  }
  auto endTwo = std::chrono::high_resolution_clock::now();
  printf("Found password after %d iterations.  The password is %s.\n", indexTwo.Index, passwordTwo);
  printf("Duration: %ld (ms)\n", (endTwo-startTwo).count() / 1000000);
  
  return 0;
}
