#ifndef __AOC__ 
#define __AOC__

//C Standard Library
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//C++ Standard Library
#include <chrono>

//SIMD Intrinsics
#include <immintrin.h>

//Platform specific directives
#define INLINE_OPT inline

#ifdef _MSC_VER 
#else 
  #define INLINE_REQ __attribute__((always_inline))
  #define MEMALIGN(width) __attribute__((aligned(width)))
#endif

#define INLINE_REQ __forceinline
#define MEMALIGN(width) __declspec(align(width))
namespace Aoc { 
  /**
   * SimdOp8 is an 8-word wide SIMD (AVX2) operand. Operators are overloaded using AVX2 simd intrinsics
   */
  struct SimdOp8 {
    static INLINE_REQ SimdOp8 Load(uint32_t *data) { 
      return SimdOp8(_mm256_load_si256((__m256i*) data));
    }  
    
    SimdOp8() : SimdOp8(0) {}
    
    SimdOp8(uint32_t val) : Data(_mm256_set1_epi32(val)) {}

    SimdOp8(const __m256i &data) : Data(data) { }

    INLINE_REQ void Store(uint32_t *memory) { 
      _mm256_store_si256((__m256i*) memory, this->Data);
    }

    INLINE_REQ SimdOp8 operator+(const SimdOp8 &other) const { 
      return { _mm256_add_epi32(this->Data, other.Data) };
    }
    INLINE_REQ SimdOp8 operator-(const SimdOp8 &other) const { 
      return { _mm256_sub_epi32(this->Data, other.Data) };
    }
    INLINE_REQ SimdOp8 operator~() const { 
      return { _mm256_xor_si256(this->Data, _mm256_set1_epi64x(0xFFFFFFFFFFFFFFFF))};
    }
    INLINE_REQ SimdOp8 operator&(const SimdOp8 &other) const { 
      return { _mm256_and_si256(this->Data, other.Data) };
    }
    INLINE_REQ SimdOp8 operator|(const SimdOp8 &other) const { 
      return { _mm256_or_si256(this->Data, other.Data) };
    }
    INLINE_REQ SimdOp8 operator^(const SimdOp8 &other) const { 
      return { _mm256_xor_si256(this->Data, other.Data) };
    }
    INLINE_REQ SimdOp8 operator<<(uint32_t n) const { 
      return { _mm256_slli_epi32(this->Data, n) };
    }
    INLINE_REQ SimdOp8 operator>>(uint32_t n) const { 
      return { _mm256_srli_epi32(this->Data, n) };
    }
    INLINE_REQ SimdOp8 operator>(const SimdOp8 &other) const { 
      return { _mm256_cmpgt_epi32(this->Data, other.Data ) };
    }
    INLINE_REQ bool operator==(uint32_t other) const { 
      if(other == 0) { 
        return _mm256_testz_si256(this->Data, this->Data);
      }
      return false;
    }
    INLINE_REQ bool operator!=(uint32_t other) const { 
      if(other == 0) { 
        return !_mm256_testz_si256(this->Data, this->Data);
      }
      return false;
    }
    INLINE_REQ SimdOp8 NotAnd(const SimdOp8 &other) const { 
      return { _mm256_andnot_si256(this->Data, other.Data) };
    }
    INLINE_REQ SimdOp8 RotateLeft(uint32_t n) const { 
      return ((*this) << n) | ((*this) >> (32-n));
    }
    private: 
      __m256i Data;
  };

  struct SimdOp16 { 
    static INLINE_REQ SimdOp16 Load(uint32_t *data) { 
      return SimdOp16(_mm512_load_si512((__m512i*) data));
    }  

    SimdOp16() : SimdOp16(0) {}
    
    SimdOp16(uint32_t val) : Data(_mm512_set1_epi32(val)) {}

    SimdOp16(const __m512i &data) : Data(data) { }

    INLINE_REQ void Store(uint32_t *memory) { 
      _mm512_store_si512((__m512i*) memory, this->Data);
    }

    INLINE_REQ SimdOp16 operator+(const SimdOp16 &other) const { 
      return { _mm512_add_epi32(this->Data, other.Data) };
    }
    INLINE_REQ SimdOp16 operator-(const SimdOp16 &other) const { 
      return { _mm512_sub_epi32(this->Data, other.Data) };
    }
    INLINE_REQ SimdOp16 operator~() const { 
      return { _mm512_xor_si512(this->Data, _mm512_set1_epi32(0xFFFFFFFF))};
    }
    INLINE_REQ SimdOp16 operator&(const SimdOp16 &other) const { 
      return { _mm512_and_si512(this->Data, other.Data) };
    }
    INLINE_REQ SimdOp16 operator|(const SimdOp16 &other) const { 
      return { _mm512_or_si512(this->Data, other.Data) };
    }
    INLINE_REQ SimdOp16 operator^(const SimdOp16 &other) const { 
      return { _mm512_xor_si512(this->Data, other.Data) };
    }
    INLINE_REQ SimdOp16 operator<<(const uint32_t n) const { 
      return { _mm512_slli_epi32(this->Data, n) };
    }
    INLINE_REQ SimdOp16 operator>>(const uint32_t n) const { 
      return { _mm512_srli_epi32(this->Data, n) };
    }
    INLINE_REQ SimdOp16 NotAnd(const SimdOp16 &other) const { 
      return { _mm512_andnot_si512(this->Data, other.Data) };
    }
    INLINE_REQ SimdOp16 RotateLeft(const uint32_t n) const { 
      return { ((*this) << n) | ((*this) >> (32-n)) };
    }
    private: 
      __m512i Data;
  };

  inline void ReadToDelimiter(FILE * stream, char * buffer, int bufferSize, char delimiter) { 
    for(auto i = 0; i < bufferSize; i++) { 
      char c = getc(stream);
      if(c == delimiter) { 
        buffer[i] = '\0';
        return;
      } else if(c == '\0') { 
        buffer[i] = '\0';
        return;
      } else { 
        buffer[i] = c;
      }
    }
    buffer[bufferSize-1] = '\0';
  } 

  inline void EatWhiteSpace(FILE * stream) { 
    while(feof(stream) == 0) { 
      char c = getc(stream);
      switch(c) { 
        case ' ': 
        case '\t':
          break; 
        default: 
          fseek(stream, -1, SEEK_CUR);
          return;
      }
    }
  }
}

#endif