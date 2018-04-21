#include <vector>

#define NumLinks 256

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

void Round(const std::vector<uint8_t> &lengths, int roundNum) { 
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

std::vector<char> Seed;
char Hashes[128][32];
int main() {
  while(!feof(stdin)) { 
    char c = getc(stdin);
    if(c != EOF) { 
      Seed.push_back(c);
    }
  }
  return 0;
}