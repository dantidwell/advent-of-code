#include "aoc.h"

#define NameCapacity 16
#define CalleesCapacity 16
#define ProgramsCapacity 4096

struct Program {
  String Name;
  int Weight;

  Program* Caller;
  List<Program*> Callees;
};

int main(int argc, const char ** argv) {  
  auto file = fopen(argv[1], "r");
  if(!file) { 
    return -1;
  }

  Map<Program> map; 
  while(!feof(file)) { 
    auto line = ReadLine(file);

    const char *l = line, *r = line;
    while(*r != ' ') { r++; }

    String name(l, r-l);
    [Program caller; 
    if(!map.TryGet(name, caller)) { 
      caller = Program { 
        name
      };
    }

    /* Get the weight */
    r++;
    l = r;
    while(*r != ' ' && *r != '\0') { r++; }
    // memcpy(weight, l+1, r-l-2); 
    // weight[r-l-2] = '\0';
    caller.Weight = 0;

    /* Any callee's? */
    if(*r == '\0') { 
      continue;
    }

    /* Get the callee's */
    r+=4;
    l = r;

    while(1) {
      while(*r != ',' && *r != '\0') { r++; } 
      String _name(l, r-l);
      Program callee;
      if(!TryGet(_name, callee) { 
        
      }
      callee->Caller = caller;
      caller->Callees.Add(callee);

      if(*r == '\0') {
        break;
      }
      r+=2; 
      l = r;
    }
  }

  fclose(file);
  return 0;
}

