#include "../aoc.h"

/**
 * MD5 Constants
 */
static MEMALIGN(64) uint32_t A8[8] = {
  0x67452301,0x67452301,0x67452301,0x67452301,0x67452301,0x67452301,0x67452301,0x67452301
};
static MEMALIGN(64) uint32_t B8[8] = {
  0xefcdab89,0xefcdab89,0xefcdab89,0xefcdab89,0xefcdab89,0xefcdab89,0xefcdab89,0xefcdab89  
};
static MEMALIGN(64) uint32_t C8[8] = {
  0x98badcfe,0x98badcfe,0x98badcfe,0x98badcfe,0x98badcfe,0x98badcfe,0x98badcfe,0x98badcfe
};
static MEMALIGN(64) uint32_t D8[8] = {
  0x10325476,0x10325476,0x10325476,0x10325476,0x10325476,0x10325476,0x10325476,0x10325476
};
static MEMALIGN(64) uint32_t A16[16] = {
    0x67452301,0x67452301,0x67452301,0x67452301,0x67452301,0x67452301,0x67452301,0x67452301,
    0x67452301,0x67452301,0x67452301,0x67452301,0x67452301,0x67452301,0x67452301,0x67452301
};
static MEMALIGN(64) uint32_t B16[16] = {
  0xefcdab89,0xefcdab89,0xefcdab89,0xefcdab89,0xefcdab89,0xefcdab89,0xefcdab89,0xefcdab89,
  0xefcdab89,0xefcdab89,0xefcdab89,0xefcdab89,0xefcdab89,0xefcdab89,0xefcdab89,0xefcdab89  
};
static MEMALIGN(64) uint32_t C16[16] = {
  0x98badcfe,0x98badcfe,0x98badcfe,0x98badcfe,0x98badcfe,0x98badcfe,0x98badcfe,0x98badcfe,
  0x98badcfe,0x98badcfe,0x98badcfe,0x98badcfe,0x98badcfe,0x98badcfe,0x98badcfe,0x98badcfe
};
static MEMALIGN(64) uint32_t D16[16] = {
  0x10325476,0x10325476,0x10325476,0x10325476,0x10325476,0x10325476,0x10325476,0x10325476,
  0x10325476,0x10325476,0x10325476,0x10325476,0x10325476,0x10325476,0x10325476,0x10325476    
};
static MEMALIGN(64) uint32_t Md5TArray[64] = { 
  0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 
  0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 
  0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 
  0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 
  0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 
  0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 
  0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 
  0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};
/**
 * MD5 Utility Functions
 */
template<typename T>
INLINE_REQ T F(const T x, const T y, const T z) {
  return (x & y) | x.NotAnd(z);
}
template<typename T>
INLINE_REQ T G(const T x, const T y, const T z) {
  return (x & z) | z.NotAnd(y);
}
template<typename T>
INLINE_REQ T H(const T x, const T y, const T z) {
  return (x ^ y) ^ z;
}
template<typename T>
INLINE_REQ T I(const T x, const T y, const T z) {
  return y ^ (x | (~z));
}
template<typename T>
INLINE_REQ T FF(const T a, const T b, const T c, const T d, uint32_t *x, uint32_t t, uint32_t s) {
  return b + (a + F(b, c, d) + T::Load(x) + T(t)).RotateLeft(s);
}
template<typename T>
INLINE_REQ T GG(const T a, const T b, const T c, const T d, uint32_t *x, uint32_t t, uint32_t s) {
  return b + (a + G(b, c, d) + T::Load(x) + T(t)).RotateLeft(s);
}
template<typename T>
INLINE_REQ T HH(const T a, const T b, const T c, const T d, uint32_t *x, uint32_t t, uint32_t s) {
  return b + (a + H(b, c, d) + T::Load(x) + T(t)).RotateLeft(s);
}
template<typename T>
INLINE_REQ T II(const T a, const T b, const T c, const T d, uint32_t *x, uint32_t t, uint32_t s) {
  return b + (a + I(b, c, d) + T::Load(x) + T(t)).RotateLeft(s);
}

/**
 * Md5EightWide - Uses AVX2 to hash 8 sequences in parallel 
 * Input is a 16x8 array of uint32_t's holding the 8 sequences to be hashed
 * Output is a length 8 uint32_t array to hold the 8 hashes  
 */
void Md5EightWide(uint32_t Input[][8], uint32_t *Output) {  
  const auto A = Aoc::SimdOp8::Load(A8);
  const auto B = Aoc::SimdOp8::Load(B8);
  const auto C = Aoc::SimdOp8::Load(C8);
  const auto D = Aoc::SimdOp8::Load(D8);
  Aoc::SimdOp8 a(A), b(B), c(C), d(D);

  /* MD5 Round 1 */
  a = FF<Aoc::SimdOp8>(a,b,c,d,Input[0],  Md5TArray[0] , 7);
  d = FF<Aoc::SimdOp8>(d,a,b,c,Input[1],  Md5TArray[1] ,12);
  c = FF<Aoc::SimdOp8>(c,d,a,b,Input[2],  Md5TArray[2] ,17);
  b = FF<Aoc::SimdOp8>(b,c,d,a,Input[3],  Md5TArray[3] ,22);
  a = FF<Aoc::SimdOp8>(a,b,c,d,Input[4],  Md5TArray[4] , 7);
  d = FF<Aoc::SimdOp8>(d,a,b,c,Input[5],  Md5TArray[5] ,12);
  c = FF<Aoc::SimdOp8>(c,d,a,b,Input[6],  Md5TArray[6] ,17);
  b = FF<Aoc::SimdOp8>(b,c,d,a,Input[7],  Md5TArray[7] ,22);
  a = FF<Aoc::SimdOp8>(a,b,c,d,Input[8],  Md5TArray[8] , 7);
  d = FF<Aoc::SimdOp8>(d,a,b,c,Input[9],  Md5TArray[9] ,12);
  c = FF<Aoc::SimdOp8>(c,d,a,b,Input[10], Md5TArray[10] ,17);
  b = FF<Aoc::SimdOp8>(b,c,d,a,Input[11], Md5TArray[11] ,22);
  a = FF<Aoc::SimdOp8>(a,b,c,d,Input[12], Md5TArray[12] , 7);
  d = FF<Aoc::SimdOp8>(d,a,b,c,Input[13], Md5TArray[13] ,12);
  c = FF<Aoc::SimdOp8>(c,d,a,b,Input[14], Md5TArray[14] ,17);
  b = FF<Aoc::SimdOp8>(b,c,d,a,Input[15], Md5TArray[15] ,22);
  a = GG<Aoc::SimdOp8>(a,b,c,d,Input[ 1], Md5TArray[16] , 5);
  d = GG<Aoc::SimdOp8>(d,a,b,c,Input[ 6], Md5TArray[17] , 9);
  c = GG<Aoc::SimdOp8>(c,d,a,b,Input[11], Md5TArray[18] ,14);
  b = GG<Aoc::SimdOp8>(b,c,d,a,Input[ 0], Md5TArray[19] ,20);
  a = GG<Aoc::SimdOp8>(a,b,c,d,Input[ 5], Md5TArray[20] , 5);
  d = GG<Aoc::SimdOp8>(d,a,b,c,Input[10], Md5TArray[21] , 9);
  c = GG<Aoc::SimdOp8>(c,d,a,b,Input[15], Md5TArray[22] ,14);
  b = GG<Aoc::SimdOp8>(b,c,d,a,Input[ 4], Md5TArray[23] ,20);
  a = GG<Aoc::SimdOp8>(a,b,c,d,Input[ 9], Md5TArray[24] , 5);
  d = GG<Aoc::SimdOp8>(d,a,b,c,Input[14], Md5TArray[25] , 9);
  c = GG<Aoc::SimdOp8>(c,d,a,b,Input[ 3], Md5TArray[26], 14);
  b = GG<Aoc::SimdOp8>(b,c,d,a,Input[ 8], Md5TArray[27], 20);
  a = GG<Aoc::SimdOp8>(a,b,c,d,Input[13], Md5TArray[28],  5);
  d = GG<Aoc::SimdOp8>(d,a,b,c,Input[ 2], Md5TArray[29],  9);
  c = GG<Aoc::SimdOp8>(c,d,a,b,Input[ 7], Md5TArray[30], 14);
  b = GG<Aoc::SimdOp8>(b,c,d,a,Input[12], Md5TArray[31], 20);
  a = HH<Aoc::SimdOp8>(a,b,c,d,Input[ 5], Md5TArray[32] , 4);
  d = HH<Aoc::SimdOp8>(d,a,b,c,Input[ 8], Md5TArray[33] ,11);
  c = HH<Aoc::SimdOp8>(c,d,a,b,Input[11], Md5TArray[34] ,16);
  b = HH<Aoc::SimdOp8>(b,c,d,a,Input[14], Md5TArray[35] ,23);
  a = HH<Aoc::SimdOp8>(a,b,c,d,Input[ 1], Md5TArray[36] , 4);
  d = HH<Aoc::SimdOp8>(d,a,b,c,Input[ 4], Md5TArray[37] ,11);
  c = HH<Aoc::SimdOp8>(c,d,a,b,Input[ 7], Md5TArray[38] ,16);
  b = HH<Aoc::SimdOp8>(b,c,d,a,Input[10], Md5TArray[39] ,23);
  a = HH<Aoc::SimdOp8>(a,b,c,d,Input[13], Md5TArray[40] , 4);
  d = HH<Aoc::SimdOp8>(d,a,b,c,Input[ 0], Md5TArray[41] ,11);
  c = HH<Aoc::SimdOp8>(c,d,a,b,Input[ 3], Md5TArray[42], 16);
  b = HH<Aoc::SimdOp8>(b,c,d,a,Input[ 6], Md5TArray[43], 23);
  a = HH<Aoc::SimdOp8>(a,b,c,d,Input[ 9], Md5TArray[44],  4);
  d = HH<Aoc::SimdOp8>(d,a,b,c,Input[12], Md5TArray[45], 11);
  c = HH<Aoc::SimdOp8>(c,d,a,b,Input[15], Md5TArray[46], 16);
  b = HH<Aoc::SimdOp8>(b,c,d,a,Input[ 2], Md5TArray[47], 23);
  a = II<Aoc::SimdOp8>(a,b,c,d,Input[ 0], Md5TArray[48] , 6);
  d = II<Aoc::SimdOp8>(d,a,b,c,Input[ 7], Md5TArray[49] ,10);
  c = II<Aoc::SimdOp8>(c,d,a,b,Input[14], Md5TArray[50] ,15);
  b = II<Aoc::SimdOp8>(b,c,d,a,Input[ 5], Md5TArray[51] ,21);
  a = II<Aoc::SimdOp8>(a,b,c,d,Input[12], Md5TArray[52] , 6);
  d = II<Aoc::SimdOp8>(d,a,b,c,Input[ 3], Md5TArray[53] ,10);
  c = II<Aoc::SimdOp8>(c,d,a,b,Input[10], Md5TArray[54] ,15);
  b = II<Aoc::SimdOp8>(b,c,d,a,Input[ 1], Md5TArray[55] ,21);
  a = II<Aoc::SimdOp8>(a,b,c,d,Input[ 8], Md5TArray[56] , 6);
  d = II<Aoc::SimdOp8>(d,a,b,c,Input[15], Md5TArray[57] ,10);
  c = II<Aoc::SimdOp8>(c,d,a,b,Input[ 6], Md5TArray[58], 15);
  b = II<Aoc::SimdOp8>(b,c,d,a,Input[13], Md5TArray[59], 21);
  a = II<Aoc::SimdOp8>(a,b,c,d,Input[ 4], Md5TArray[60],  6);
  d = II<Aoc::SimdOp8>(d,a,b,c,Input[11], Md5TArray[61], 10);
  c = II<Aoc::SimdOp8>(c,d,a,b,Input[ 2], Md5TArray[62], 15);
  b = II<Aoc::SimdOp8>(b,c,d,a,Input[ 9], Md5TArray[63], 21);

  /**
   * we're only interested in the top 32-bits of the hash so don't
   * bother computing the remaining bits
   */  
  (a + A).Store(Output);
}

// void Md5SixteenWide(uint32_t Input[][16], uint32_t Output[16]) { 
//   const auto A = Aoc::SimdOp16::Load(A16);
//   const auto B = Aoc::SimdOp16::Load(B16);
//   const auto C = Aoc::SimdOp16::Load(C16);
//   const auto D = Aoc::SimdOp16::Load(D16);
//   Aoc::SimdOp16 a(A), b(B), c(C), d(D);

//   /* MD5 Round 1 */
//   a = FF<Aoc::SimdOp16>(a,b,c,d,Input[0],  Md5TArray16[0] , 7);
//   d = FF<Aoc::SimdOp16>(d,a,b,c,Input[1],  Md5TArray16[1] ,12);
//   c = FF<Aoc::SimdOp16>(c,d,a,b,Input[2],  Md5TArray16[2] ,17);
//   b = FF<Aoc::SimdOp16>(b,c,d,a,Input[3],  Md5TArray16[3] ,22);
//   a = FF<Aoc::SimdOp16>(a,b,c,d,Input[4],  Md5TArray16[4] , 7);
//   d = FF<Aoc::SimdOp16>(d,a,b,c,Input[5],  Md5TArray16[5] ,12);
//   c = FF<Aoc::SimdOp16>(c,d,a,b,Input[6],  Md5TArray16[6] ,17);
//   b = FF<Aoc::SimdOp16>(b,c,d,a,Input[7],  Md5TArray16[7] ,22);
//   a = FF<Aoc::SimdOp16>(a,b,c,d,Input[8],  Md5TArray16[8] , 7);
//   d = FF<Aoc::SimdOp16>(d,a,b,c,Input[9],  Md5TArray16[9] ,12);
//   c = FF<Aoc::SimdOp16>(c,d,a,b,Input[10], Md5TArray16[10] ,17);
//   b = FF<Aoc::SimdOp16>(b,c,d,a,Input[11], Md5TArray16[11] ,22);
//   a = FF<Aoc::SimdOp16>(a,b,c,d,Input[12], Md5TArray16[12] , 7);
//   d = FF<Aoc::SimdOp16>(d,a,b,c,Input[13], Md5TArray16[13] ,12);
//   c = FF<Aoc::SimdOp16>(c,d,a,b,Input[14], Md5TArray16[14] ,17);
//   b = FF<Aoc::SimdOp16>(b,c,d,a,Input[15], Md5TArray16[15] ,22);
//   a = GG<Aoc::SimdOp16>(a,b,c,d,Input[ 1], Md5TArray16[16] , 5);
//   d = GG<Aoc::SimdOp16>(d,a,b,c,Input[ 6], Md5TArray16[17] , 9);
//   c = GG<Aoc::SimdOp16>(c,d,a,b,Input[11], Md5TArray16[18] ,14);
//   b = GG<Aoc::SimdOp16>(b,c,d,a,Input[ 0], Md5TArray16[19] ,20);
//   a = GG<Aoc::SimdOp16>(a,b,c,d,Input[ 5], Md5TArray16[20] , 5);
//   d = GG<Aoc::SimdOp16>(d,a,b,c,Input[10], Md5TArray16[21] , 9);
//   c = GG<Aoc::SimdOp16>(c,d,a,b,Input[15], Md5TArray16[22] ,14);
//   b = GG<Aoc::SimdOp16>(b,c,d,a,Input[ 4], Md5TArray16[23] ,20);
//   a = GG<Aoc::SimdOp16>(a,b,c,d,Input[ 9], Md5TArray16[24] , 5);
//   d = GG<Aoc::SimdOp16>(d,a,b,c,Input[14], Md5TArray16[25] , 9);
//   c = GG<Aoc::SimdOp16>(c,d,a,b,Input[ 3], Md5TArray16[26], 14);
//   b = GG<Aoc::SimdOp16>(b,c,d,a,Input[ 8], Md5TArray16[27], 20);
//   a = GG<Aoc::SimdOp16>(a,b,c,d,Input[13], Md5TArray16[28],  5);
//   d = GG<Aoc::SimdOp16>(d,a,b,c,Input[ 2], Md5TArray16[29],  9);
//   c = GG<Aoc::SimdOp16>(c,d,a,b,Input[ 7], Md5TArray16[30], 14);
//   b = GG<Aoc::SimdOp16>(b,c,d,a,Input[12], Md5TArray16[31], 20);
//   a = HH<Aoc::SimdOp16>(a,b,c,d,Input[ 5], Md5TArray16[32] , 4);
//   d = HH<Aoc::SimdOp16>(d,a,b,c,Input[ 8], Md5TArray16[33] ,11);
//   c = HH<Aoc::SimdOp16>(c,d,a,b,Input[11], Md5TArray16[34] ,16);
//   b = HH<Aoc::SimdOp16>(b,c,d,a,Input[14], Md5TArray16[35] ,23);
//   a = HH<Aoc::SimdOp16>(a,b,c,d,Input[ 1], Md5TArray16[36] , 4);
//   d = HH<Aoc::SimdOp16>(d,a,b,c,Input[ 4], Md5TArray16[37] ,11);
//   c = HH<Aoc::SimdOp16>(c,d,a,b,Input[ 7], Md5TArray16[38] ,16);
//   b = HH<Aoc::SimdOp16>(b,c,d,a,Input[10], Md5TArray16[39] ,23);
//   a = HH<Aoc::SimdOp16>(a,b,c,d,Input[13], Md5TArray16[40] , 4);
//   d = HH<Aoc::SimdOp16>(d,a,b,c,Input[ 0], Md5TArray16[41] ,11);
//   c = HH<Aoc::SimdOp16>(c,d,a,b,Input[ 3], Md5TArray16[42], 16);
//   b = HH<Aoc::SimdOp16>(b,c,d,a,Input[ 6], Md5TArray16[43], 23);
//   a = HH<Aoc::SimdOp16>(a,b,c,d,Input[ 9], Md5TArray16[44],  4);
//   d = HH<Aoc::SimdOp16>(d,a,b,c,Input[12], Md5TArray16[45], 11);
//   c = HH<Aoc::SimdOp16>(c,d,a,b,Input[15], Md5TArray16[46], 16);
//   b = HH<Aoc::SimdOp16>(b,c,d,a,Input[ 2], Md5TArray16[47], 23);
//   a = II<Aoc::SimdOp16>(a,b,c,d,Input[ 0], Md5TArray16[48] , 6);
//   d = II<Aoc::SimdOp16>(d,a,b,c,Input[ 7], Md5TArray16[49] ,10);
//   c = II<Aoc::SimdOp16>(c,d,a,b,Input[14], Md5TArray16[50] ,15);
//   b = II<Aoc::SimdOp16>(b,c,d,a,Input[ 5], Md5TArray16[51] ,21);
//   a = II<Aoc::SimdOp16>(a,b,c,d,Input[12], Md5TArray16[52] , 6);
//   d = II<Aoc::SimdOp16>(d,a,b,c,Input[ 3], Md5TArray16[53] ,10);
//   c = II<Aoc::SimdOp16>(c,d,a,b,Input[10], Md5TArray16[54] ,15);
//   b = II<Aoc::SimdOp16>(b,c,d,a,Input[ 1], Md5TArray16[55] ,21);
//   a = II<Aoc::SimdOp16>(a,b,c,d,Input[ 8], Md5TArray16[56] , 6);
//   d = II<Aoc::SimdOp16>(d,a,b,c,Input[15], Md5TArray16[57] ,10);
//   c = II<Aoc::SimdOp16>(c,d,a,b,Input[ 6], Md5TArray16[58], 15);
//   b = II<Aoc::SimdOp16>(b,c,d,a,Input[13], Md5TArray16[59], 21);
//   a = II<Aoc::SimdOp16>(a,b,c,d,Input[ 4], Md5TArray16[60],  6);
//   d = II<Aoc::SimdOp16>(d,a,b,c,Input[11], Md5TArray16[61], 10);
//   c = II<Aoc::SimdOp16>(c,d,a,b,Input[ 2], Md5TArray16[62], 15);
//   b = II<Aoc::SimdOp16>(b,c,d,a,Input[ 9], Md5TArray16[63], 21);

//   /**
//    * we're only interested in the top 32-bits of the hash so don't
//    * bother computing the remaining bits
//    */  
//   (a + A).Store(Output);
// }