#include "aoc.h"
const int NumDances = 1000000000;
const int NumPrograms = 16;

int NumCommands = 0;
char Commands[1024*1024][16];

void Spin(char * programs, char * command) { 
  int s = atoi(command);
  if(s <=0 || s >= NumPrograms) { 
    return;
  }
  char temp[NumPrograms];
  memcpy(temp, programs+(NumPrograms-s), s);
  memmove(programs+s, programs, NumPrograms-s);
  memcpy(programs, temp, s);
}

void Exchange(char * programs, char * command) {
  char a[3] = {'\0', '\0', '\0'};
  char b[3] = {'\0', '\0', '\0'};

  const auto delimA = ([command]() {
    for(auto i = 0; i < strlen(command); i++) { 
      if(command[i] == '/') { 
        return i;
      }
    } 
    return -1;
  })();
  const auto delimB = strlen(command);

  memcpy(a, command, delimA);
  memcpy(b, command+delimA+1, delimB - (delimA+1));

  auto iA = atoi(a);
  auto iB = atoi(b);

  auto temp = programs[iA]; 
  programs[iA] = programs[iB]; 
  programs[iB] = temp;
}

void Partner(char * programs, char * command) {
  char a = command[0];
  char b = command[2];

  auto find = [programs](char c) { 
    for(auto i = 0; i < NumPrograms; i++) { 
      if(programs[i] == c) { 
        return i;
      }
    }
    return -1;
  };
  auto aIdx = find(a);
  auto bIdx = find(b);

  auto temp = programs[aIdx]; 
  *(programs+aIdx) = programs[bIdx]; 
  *(programs+bIdx) = temp;
}

int Dance(char * programs, int limit) { 
  auto count = 0;
  while(count < limit) { 
    auto command = 0; 
    while(command < NumCommands) { 
      switch(Commands[command][0]) { 
        case 's':
          Spin(programs, Commands[command]+1);
          break; 
        case 'x': 
          Exchange(programs, Commands[command]+1);
          break; 
        case 'p':
          Partner(programs, Commands[command]+1);
          break; 
      }
      command++;
    }
    count++;
    if(strcmp(programs, "abcdefghijklmnop") == 0) { 
      break;
    }
  }
  return count;
}

int main(int argc, char ** argv) { 
  auto file = fopen(argv[1], "r");
  while(!feof(file)) { 
    ReadWord(Commands[NumCommands], file, ',');
    NumCommands++;
  }
  fclose(file);

  char prog1[17] = "abcdefghijklmnop";
  auto per1 = Dance(prog1, 1); 
  
  char prog2[17] = "abcdefghijklmnop";
  auto per2 = Dance(prog2, 1000000000); 
  
  char prog3[17] = "abcdefghijklmnop";
  auto per3 = Dance(prog3, 1000000000 % per2); 

  printf("--Part One--\n");
  printf("\tOrdering after 1 dance: %s\n", prog1);
  printf("--Part Two--\n");
  printf("\tOrdering after %d dance(s): %s\n", NumDances, prog3);
  return 0;
}