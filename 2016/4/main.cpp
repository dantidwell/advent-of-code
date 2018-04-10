#include "aoc.h"

struct LetterFrequencies { 
  int operator [](char c) const  {
    return _frequencies[c - 97];
  }
  int& operator [](char c) {
    return _frequencies[c - 97];
  }

  bool IsNthMostCommon(char c, int n) { 
    int nGreaterOrEqual = 0;
    for(auto _c = 'a'; _c <= 'z'; _c++) { 
      if(
        (*this)[_c] > (*this)[c] 
        || ((*this)[_c] == (*this)[c] && _c <= c) 
      ) {
        nGreaterOrEqual++;
      }
    }
    return nGreaterOrEqual == n;
  }

  private: 
    int _frequencies[26] = { 0 };
    char _sorted[26] = { 0 };
};

int main(int argc, char **argv) { 
  auto f = fopen(argv[1], "r");
  if(!f) { 
    return 0;
  } 

  int sum = 0;
  while(feof(f) == 0) { 
    char n[256]; 
    Aoc::ReadToDelimiter(f, n, 256, '\n');

    /* get char frequencies */
    LetterFrequencies freq;
    char *c = n;
    do {
      if(*c >= 'a' && *c <= 'z') { 
        freq[*c]++;
      }
      c++;
    } while(*c == '-' || (*c >= 'a' && *c <= 'z'));

    /* get the sector id */
    int id = 0;
    do { 
      id = id * 10 + (*c-48);
      c++;
    } while(*c != '[');

    /* validate the checksum */
    c++;
    bool valid = true;
    for(auto i = 0; i < strlen(c) - 1; i++) { 
      valid &= freq.IsNthMostCommon(*(c+i), i+1);
    }
    if(valid) { 
      sum += id;

      /* decrypt the message */
      c = n;
      do {
        if(*c >= 'a' && *c <= 'z') { 
          *c = (((*c-97) + id) % 26) + 97;
        } else { 
          *c = ' ';
        }
        c++;
      } while(*c == '-' || (*c >= 'a' && *c <= 'z'));
      *c = '\0';
      printf("Decrypted room name: %s (Id: %d)\n", n, id);
    }
  }
  printf("The sum of all valid ids is %d\n", sum);
  return 0;
}