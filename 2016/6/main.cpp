#include "../aoc.h"

const int32_t NumColumns = 8;
const int32_t NumLetters = 26;
const int32_t AsciiLowercaseOffset = 97;
char Line[NumColumns+1] = { 0 };
int32_t ColumnLetterFrequencies[NumColumns][NumLetters] = { 0 };

char FindMostFrequentLetterInColumn(int32_t column) { 
  char cMax = 'a';
  int32_t fMax = 0;
  for(auto i = 0; i < NumLetters; i++) { 
    if(ColumnLetterFrequencies[column][i] > fMax) { 
      fMax = ColumnLetterFrequencies[column][i]; 
      cMax = i+AsciiLowercaseOffset;
    }
  }
  return cMax;
}

char FindLeastFrequentLetterInColumn(int32_t column) { 
  char cMax = 'a';
  int32_t fMax = 0x7FFFFFFF;
  for(auto i = 0; i < NumLetters; i++) { 
    if(ColumnLetterFrequencies[column][i] < fMax) { 
      fMax = ColumnLetterFrequencies[column][i]; 
      cMax = i+AsciiLowercaseOffset;
    }
  }
  return cMax;
}

int32_t main(int32_t argc, const char ** argv) { 
  auto f = fopen(argv[1], "r"); 
  if(!f) {
    return -1;
  }
  while(feof(f) == 0) { 
    Aoc::ReadToDelimiter(f, Line, NumColumns+1, '\n');
    for(auto i = 0; i < NumColumns; i++) { 
      ColumnLetterFrequencies[i][Line[i] - AsciiLowercaseOffset]++;
    }
  }
  char DecodedMessageOne[NumColumns+1] = { 0 };
  char DecodedMessageTwo[NumColumns+1] = { 0 };
  for(auto i = 0; i < NumColumns; i++) { 
    DecodedMessageOne[i] = FindMostFrequentLetterInColumn(i);
    DecodedMessageTwo[i] = FindLeastFrequentLetterInColumn(i);
  }
  printf("The decoded message in Part One is %s\n", DecodedMessageOne);
  printf("The decoded message in Part Two is %s\n", DecodedMessageTwo);
  fclose(f);
  return 0;
}