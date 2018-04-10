#include <stdio.h>
#include <stdlib.h>
#include <vector>

std::vector<int> Jumps;

int main() { 
  while(!feof(stdin)) {
    char buffer[8];
    {
      int i = 0; 
      while(1) { 
        char c = getc(stdin);
        if(c == '\n' || c == EOF) {
          break;
        } 
        buffer[i++] = c;
      } 
      buffer[i] = '\0';
    }
    Jumps.push_back(atoi(buffer));
  }
  
  auto ip = 0;
  auto counter = 0;
  auto jp1 = Jumps;
  while(ip < jp1.size()) { 
    auto nip = ip + jp1[ip]; 
    jp1[ip] = jp1[ip] + 1; 
    ip = nip;
    counter++;
  }
  printf("\n");
  printf("-- Part One --\n");
  printf("%d steps were taken.\n", counter);
  printf("\n");


  ip = 0;
  counter = 0;
  auto jp2 = Jumps;
  while(ip < jp2.size()) { 
    auto nip = ip + jp2[ip]; 
    if(jp2[ip] >= 3) { 
      jp2[ip] = jp2[ip] - 1; 
    } else { 
      jp2[ip] = jp2[ip] + 1; 
    }
    ip = nip;
    counter++;
  }
  printf("\n");
  printf("-- Part Two --\n");
  printf("%d steps were taken.\n", counter);
  printf("\n");

  return 0;
}
