#include "../aoc.h"

const int32_t LineLength = 1024; 
char Line[LineLength+1];

int32_t main(int32_t argc, const char ** argv) { 
  auto f = fopen(argv[1], "r"); 
  if(!f) {
    return -1;
  }
  auto numTlsIps = 0; 
  while(feof(f) == 0) { 
    auto length = Aoc::ReadToDelimiter(f, Line, LineLength+1, '\n');
    if(length < 4) { 
      continue;
    }
    bool inHypernet = false;
    bool hasABBA = false; 
    bool hasHypernetABBA = false;
    for(auto i = 0; i < length-3; i++) { 
      if(Line[i] == '[') { 
        inHypernet = true;
        continue;
      } else if(Line[i] == ']') { 
        inHypernet = false;
        continue;
      }
      auto isAABA = (Line[i] != Line[i+1]) && (Line[i] == Line[i+3]) && (Line[i+1] == Line[i+2]);
      hasABBA |= isAABA; 
      hasHypernetABBA |= isAABA && inHypernet;
    }
   numTlsIps += (hasABBA && !hasHypernetABBA) ? 1 : 0; 
  }
  printf("There are %d IPs that support TLS.\n", numTlsIps);
  return 0;
}