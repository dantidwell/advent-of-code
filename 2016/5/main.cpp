#include "aoc.h"

/**
 * MD5 Utility Functions
 */
INLINE_REQ Aoc::SimdOp8 F(const Aoc::SimdOp8 x, const Aoc::SimdOp8 y, const Aoc::SimdOp8 z) {
  return (x & y) | x.NotAnd(z);
}
INLINE_REQ Aoc::SimdOp8 G(const Aoc::SimdOp8 x, const Aoc::SimdOp8 y, const Aoc::SimdOp8 z) {
  return (x & z) | z.NotAnd(y);
}
INLINE_REQ Aoc::SimdOp8 H(const Aoc::SimdOp8 x, const Aoc::SimdOp8 y, const Aoc::SimdOp8 z) {
  return (x ^ y) ^ z;
}
INLINE_REQ Aoc::SimdOp8 I(const Aoc::SimdOp8 x, const Aoc::SimdOp8 y, const Aoc::SimdOp8 z) {
  return y ^ (x | (~z));
}
INLINE_REQ Aoc::SimdOp8 FF(const Aoc::SimdOp8 a, const Aoc::SimdOp8 b, const Aoc::SimdOp8 c, const Aoc::SimdOp8 d, uint32_t *x, uint32_t t, uint32_t s) {
  return b + (a + F(b, c, d) + Aoc::SimdOp8::Load(x) + Aoc::SimdOp8(t)).RotateLeft(s);
}
INLINE_REQ Aoc::SimdOp8 GG(const Aoc::SimdOp8 a, const Aoc::SimdOp8 b, const Aoc::SimdOp8 c, const Aoc::SimdOp8 d, uint32_t *x, uint32_t t, uint32_t s) {
  return b + (a + G(b, c, d) + Aoc::SimdOp8::Load(x) + Aoc::SimdOp8(t)).RotateLeft(s);
}
INLINE_REQ Aoc::SimdOp8 HH(const Aoc::SimdOp8 a, const Aoc::SimdOp8 b, const Aoc::SimdOp8 c, const Aoc::SimdOp8 d, uint32_t *x, uint32_t t, uint32_t s) {
  return b + (a + H(b, c, d) + Aoc::SimdOp8::Load(x) + Aoc::SimdOp8(t)).RotateLeft(s);
}
INLINE_REQ Aoc::SimdOp8 II(const Aoc::SimdOp8 a, const Aoc::SimdOp8 b, const Aoc::SimdOp8 c, const Aoc::SimdOp8 d, uint32_t *x, uint32_t t, uint32_t s) {
  return b + (a + I(b, c, d) + Aoc::SimdOp8::Load(x) + Aoc::SimdOp8(t)).RotateLeft(s);
}

/**
 * Md5EightWide - Uses AVX2 to hash 8 sequences in parallel 
 * Input is a 16x8 array of uint32_t's holding the 8 sequences to be hashed
 * Output is a length 8 uint32_t array to hold the 8 hashes  
 */
void Md5EightWide(uint32_t Input[][8], uint32_t *Output) {  
  const Aoc::SimdOp8 A(0x67452301);
  const Aoc::SimdOp8 B(0xefcdab89);
  const Aoc::SimdOp8 C(0x98badcfe);
  const Aoc::SimdOp8 D(0x10325476);

  Aoc::SimdOp8 a(A), b(B), c(C), d(D);

  /* MD5 Round 1 */
  a = FF(a,b,c,d,Input[0], 0xd76aa478 , 7);
  d = FF(d,a,b,c,Input[1], 0xe8c7b756 ,12);
  c = FF(c,d,a,b,Input[2], 0x242070db ,17);
  b = FF(b,c,d,a,Input[3], 0xc1bdceee ,22);
  a = FF(a,b,c,d,Input[4], 0xf57c0faf , 7);
  d = FF(d,a,b,c,Input[5], 0x4787c62a ,12);
  c = FF(c,d,a,b,Input[6], 0xa8304613 ,17);
  b = FF(b,c,d,a,Input[7], 0xfd469501 ,22);
  a = FF(a,b,c,d,Input[8], 0x698098d8 , 7);
  d = FF(d,a,b,c,Input[9], 0x8b44f7af ,12);
  c = FF(c,d,a,b,Input[10],0xffff5bb1 ,17);
  b = FF(b,c,d,a,Input[11],0x895cd7be ,22);
  a = FF(a,b,c,d,Input[12],0x6b901122 , 7);
  d = FF(d,a,b,c,Input[13],0xfd987193 ,12);
  c = FF(c,d,a,b,Input[14],0xa679438e ,17);
  b = FF(b,c,d,a,Input[15],0x49b40821 ,22);

  /* MD5 Round 2 */
  a = GG(a,b,c,d,Input[ 1], 0xf61e2562 , 5);
  d = GG(d,a,b,c,Input[ 6], 0xc040b340 , 9);
  c = GG(c,d,a,b,Input[11], 0x265e5a51 ,14);
  b = GG(b,c,d,a,Input[ 0], 0xe9b6c7aa ,20);
  a = GG(a,b,c,d,Input[ 5], 0xd62f105d , 5);
  d = GG(d,a,b,c,Input[10], 0x02441453 , 9);
  c = GG(c,d,a,b,Input[15], 0xd8a1e681 ,14);
  b = GG(b,c,d,a,Input[ 4], 0xe7d3fbc8 ,20);
  a = GG(a,b,c,d,Input[ 9], 0x21e1cde6 , 5);
  d = GG(d,a,b,c,Input[14], 0xc33707d6 , 9);
  c = GG(c,d,a,b,Input[ 3], 0xf4d50d87, 14);
  b = GG(b,c,d,a,Input[ 8], 0x455a14ed, 20);
  a = GG(a,b,c,d,Input[13], 0xa9e3e905,  5);
  d = GG(d,a,b,c,Input[ 2], 0xfcefa3f8,  9);
  c = GG(c,d,a,b,Input[ 7], 0x676f02d9, 14);
  b = GG(b,c,d,a,Input[12], 0x8d2a4c8a, 20);

  /* MD5 Round 3 */
  a = HH(a,b,c,d,Input[ 5], 0xfffa3942 , 4);
  d = HH(d,a,b,c,Input[ 8], 0x8771f681 ,11);
  c = HH(c,d,a,b,Input[11], 0x6d9d6122 ,16);
  b = HH(b,c,d,a,Input[14], 0xfde5380c ,23);
  a = HH(a,b,c,d,Input[ 1], 0xa4beea44 , 4);
  d = HH(d,a,b,c,Input[ 4], 0x4bdecfa9 ,11);
  c = HH(c,d,a,b,Input[ 7], 0xf6bb4b60 ,16);
  b = HH(b,c,d,a,Input[10], 0xbebfbc70 ,23);
  a = HH(a,b,c,d,Input[13], 0x289b7ec6 , 4);
  d = HH(d,a,b,c,Input[ 0], 0xeaa127fa ,11);
  c = HH(c,d,a,b,Input[ 3], 0xd4ef3085, 16);
  b = HH(b,c,d,a,Input[ 6], 0x04881d05, 23);
  a = HH(a,b,c,d,Input[ 9], 0xd9d4d039,  4);
  d = HH(d,a,b,c,Input[12], 0xe6db99e5, 11);
  c = HH(c,d,a,b,Input[15], 0x1fa27cf8, 16);
  b = HH(b,c,d,a,Input[ 2], 0xc4ac5665, 23);

  /* MD5 Round 4 */
  a = II(a,b,c,d,Input[ 0], 0xf4292244 , 6);
  d = II(d,a,b,c,Input[ 7], 0x432aff97 ,10);
  c = II(c,d,a,b,Input[14], 0xab9423a7 ,15);
  b = II(b,c,d,a,Input[ 5], 0xfc93a039 ,21);
  a = II(a,b,c,d,Input[12], 0x655b59c3 , 6);
  d = II(d,a,b,c,Input[ 3], 0x8f0ccc92 ,10);
  c = II(c,d,a,b,Input[10], 0xffeff47d ,15);
  b = II(b,c,d,a,Input[ 1], 0x85845dd1 ,21);
  a = II(a,b,c,d,Input[ 8], 0x6fa87e4f , 6);
  d = II(d,a,b,c,Input[15], 0xfe2ce6e0 ,10);
  c = II(c,d,a,b,Input[ 6], 0xa3014314, 15);
  b = II(b,c,d,a,Input[13], 0x4e0811a1, 21);
  a = II(a,b,c,d,Input[ 4], 0xf7537e82,  6);
  d = II(d,a,b,c,Input[11], 0xbd3af235, 10);
  c = II(c,d,a,b,Input[ 2], 0x2ad7d2bb, 15);
  b = II(b,c,d,a,Input[ 9], 0xeb86d391, 21);

  /**
   * we're only interested in the top 32-bits of the hash so don't
   * bother computing the remaining bits
   */  
  (a + A).Store(Output);
}

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
  
  /* CASE 1: No carry was needed after incrementing ones place, so store and bail */
  if(carry == 0) { 
    lanes.Store(Input[s.IndexStartWord]);
    return;
  }

  /* CASE 2: We've overflowed the ones place in one or more lanes and need to "carry the one" */
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
  printf("Duration: %lld (ms)\n", (endOne-startOne).count() / 1000000);

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
  printf("Duration: %lld (ms)\n", (endTwo-startTwo).count() / 1000000);
  
  return 0;
}