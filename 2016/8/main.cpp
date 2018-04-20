#include "../aoc.h"

struct Display { 
  void Set(int32_t width, int32_t height) { 
    for(auto i = 0; i < height; i++) { 
      Rows[i] = Rows[i] | (((1ull << width)-1) << (64-(width)));
    }   
  }

  void RotateRow(int32_t row, int32_t distance) { 
    Rows[row] = ((Rows[row] >> distance) & ActiveColumnMask) 
                | ((((1ull << (ColumnOffset+distance))-1) & Rows[row]) << (NumColumns-distance));
  }

  void RotateColumn(int32_t column, int32_t distance) {
    uint64_t rows[NumRows];
    memcpy(rows, Rows, NumRows * sizeof(uint64_t));
    auto colMask = 0x8000000000000000 >> column;
    auto skip = NumRows - (distance % NumRows);
    for(auto i = 0; i < NumRows; i++) { 
      Rows[i] = (rows[i] & (~colMask)) | (rows[(i+skip)%NumRows] & colMask);
    }
  }

  int32_t PixelsActive() { 
    auto active = 0;
    for(auto j = 0; j < NumRows; j++) {
      auto mask = 0x8000000000000000;
      for(auto i = 0; i < NumColumns; i++) { 
        if(Rows[j] & (mask >> i)) { 
          active++;
        }
      }
    }
    return active;
  }

  int32_t Draw() { 
    auto active = 0;
    auto mask = 0x8000000000000000;
    for(auto j = 0; j < NumRows; j++) {
      for(auto i = 0; i < NumColumns; i++) { 
        if(Rows[j] & (mask >> i)) { 
          printf("#");
        } else { 
          printf(" ");
        }
      }
      printf("\n");
    }
    return active;
  }

  private:
    static const int32_t NumColumns = 50; 
    static const int32_t NumRows = 6;
    static const int32_t ColumnOffset = 64-NumColumns;
    static const uint64_t ActiveColumnMask = ~((1ull << ColumnOffset)-1);
    uint64_t Rows[NumRows] = { 0 };
};

#define IsRect(b) (*((uint64_t*)(b)) & 0x00000000FFFFFFFF) == 0x0000000074636572
#define IsRotate(b) (*((uint64_t*)(b)) & 0x0000FFFFFFFFFFFF) == 0x0000657461746F72
#define IsRow(b) (*((uint64_t*)(b)) & 0x0000000000FFFFFF) == 0x0000000000776F72
#define IsColumn(b) (*((uint64_t*)(b)) & 0x0000FFFFFFFFFFFF) == 0x00006E6D756C6F63
#define AsciiToDec(c) ((c)-0x30)

int32_t main(int32_t argc, const char ** argv) { 
  auto f = fopen(argv[1], "r"); 
  if(!f) { 
    return -1;
  }
  const int32_t bufferSize = 32;
  char buffer[bufferSize] = { 0 };
  Display d;
  while(feof(f) == 0) { 
    /* Read out the first word of the line */ 
    Aoc::ReadToDelimiter(f, buffer, bufferSize, ' ');
    if(IsRect(buffer)) { 
      int32_t width = 0, height = 0;
      auto numWDigits = Aoc::ReadToDelimiter(f, buffer, bufferSize, 'x');
      for(auto i = 0; i < numWDigits; i++) { width = width*10 + AsciiToDec(buffer[i]); }
      auto numHDigits = Aoc::ReadToDelimiter(f, buffer, bufferSize, '\n');
      for(auto i = 0; i < numHDigits; i++) { height = height*10 + AsciiToDec(buffer[i]); }
      d.Set(width, height);
    } else if (IsRotate(buffer)) { 
      Aoc::ReadToDelimiter(f, buffer, bufferSize, ' ');
      if(IsRow(buffer)) {
        auto row = 0;
        auto distance = 0;
        auto numDigits = Aoc::ReadToDelimiter(f, buffer, bufferSize, ' ');
        for(auto i = 2; i < numDigits; i++) { row = row*10 + AsciiToDec(buffer[i]); }
        numDigits = Aoc::ReadToDelimiter(f, buffer, bufferSize, ' ');
        numDigits = Aoc::ReadToDelimiter(f, buffer, bufferSize, '\n');
        for(auto i = 0; i < numDigits; i++) { distance = distance*10 + AsciiToDec(buffer[i]); }
        d.RotateRow(row, distance);
      } else if(IsColumn(buffer)) { 
        auto column = 0;
        auto distance = 0;
        auto numDigits = Aoc::ReadToDelimiter(f, buffer, bufferSize, ' ');
        for(auto i = 2; i < numDigits; i++) { column = column*10 + AsciiToDec(buffer[i]); }
        numDigits = Aoc::ReadToDelimiter(f, buffer, bufferSize, ' ');
        numDigits = Aoc::ReadToDelimiter(f, buffer, bufferSize, '\n');
        for(auto i = 0; i < numDigits; i++) { distance = distance*10 + AsciiToDec(buffer[i]); }
        d.RotateColumn(column, distance);
      }
    }
  } 
  printf("There are %d pixels active.\n", d.PixelsActive());
  printf("\n\n");
  d.Draw();
  return 0;
}