#include "aoc.h"
/* 

--- Day 4: High-Entropy Passphrases ---
A new system policy has been put in place that requires all accounts to use a passphrase instead of simply a password. 
A passphrase consists of a series of words (lowercase letters) separated by spaces.

To ensure security, a valid passphrase must contain no duplicate words.

For example:

aa bb cc dd ee is valid.
aa bb cc dd aa is not valid - the word aa appears more than once.
aa bb cc dd aaa is valid - aa and aaa count as different words.
The system's full passphrase list is available as your puzzle input. How many passphrases are valid?

*/

char Passphrase[16][16];
int ValidPassphrases = 0;
int ValidPassphrases2 = 0;

char Xor(char *string) { 
  char x = 0;
  for(auto i = 0; i < strlen(string); i++) { 
    x = x ^ string[i];
  }
  return x;
}

int main() { 
  int lines = 0;
  int words = 0;
  
  while(!feof(stdin)) { 
    auto end = ReadWord(Passphrase[words++], stdin);
    if(end != '\n' && end != EOF) {
      continue;
    }
    bool bad = false; 
    bool bad2 = false;
    auto i = 0, j = 1;
    while(i < words) { 
      while(j < words) { 
        bad |= (strncmp(Passphrase[i], Passphrase[j], 16) == 0);
        bad2 |= (Xor(Passphrase[i]) == Xor(Passphrase[j])); 
        j++;
      }
      i = i+1;
      j = i+1;
    }
    if(!bad) {
      ValidPassphrases++;
    }
    if(!bad2) { 
      ValidPassphrases2++;
    }
    words = 0;
    lines++;
  }
  printf("\n");
  printf("-- Part One --\n"); 
  printf("\tThere are %d valid passphrases.\n", ValidPassphrases); 
  printf("\n");
  printf("-- Part Two --\n"); 
  printf("\tThere are %d valid passphrases.\n", ValidPassphrases2); 
  printf("\n");

  return 0;
}