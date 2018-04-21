#include <stdio.h>
#include <stdlib.h>

#include <algorithm>

struct Line { 
  int Length;
  int Nums[128];
};

int main(int argc, char ** argv) { 
  int checksum = 0;

  auto file = fopen(argv[1], "r");
  
  const auto InitMax = 0;
  const auto InitMin = 10000;
  auto min = InitMin; 
  auto max = InitMax;
  
  Line spreadsheet[128]; 
  int numLines = 0;

  char num[5]; /* inputs are at most 4 digits */
  int numIdx = 0;
  int colIdx = 0;
  while(!feof(file)) { 
    auto c = getc(file);
    switch(c) { 
      case EOF:
      case '\n': 
        num[numIdx] = '\0';
        spreadsheet[numLines].Nums[colIdx] = atoi(num);
        spreadsheet[numLines].Length = colIdx + 1;
        colIdx = 0;
        numIdx = 0;
        numLines++;
        break;
      case ' ':
      case '\t': 
        num[numIdx] = '\0';
        spreadsheet[numLines].Nums[colIdx] = atoi(num);
        colIdx++;
        numIdx = 0;
        break;
      default: 
        num[numIdx++] = c;
    }
  }
  fclose(file);

  auto minMaxChecksum = 0;
  auto divisibleChecksum = 0;
  for(auto j = 0; j < numLines; j++) { 
    auto min = 10000; 
    auto max = 0;
    bool divisibleFound = false;
    for(auto i = 0; i < spreadsheet[j].Length; i++) { 
      auto a = spreadsheet[j].Nums[i];
      min = std::min(a, min);
      max = std::max(a, max);

      if(divisibleFound) { 
        continue; 
      }

      for(auto k = 0; k < spreadsheet[j].Length; k++) { 
        if(k == i) { 
          continue;
        }
        auto b = spreadsheet[j].Nums[k];
        if(a > b && a%b==0) { 
          divisibleChecksum += a/b;
          divisibleFound = true;
          break;
        } else if(a < b && b%a==0) { 
          divisibleChecksum += b/a;
          divisibleFound = true; 
          break;
        }
      }
    }
    minMaxChecksum += (max-min);
  }

  printf("The min-max checksum is %d!\n", minMaxChecksum);
  printf("The divisibility checksum is %d!\n", divisibleChecksum);
  return 0;
}