#include <assert.h>

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

enum class InstructionType { 
  Set, 
  Mul, 
  Sub,
  Jnz
};

struct Instruction { 
  InstructionType Type;
  int *RegLeft = nullptr; 
  int *RegRight = nullptr; 
  int ImmediateLeft = 0; 
  int ImmediateRight = 0;
};

/* Instruction memory and pointer */
std::vector<Instruction> InstructionMemory;  
int InstructionPointer = 0; 

/* GP Registers */
int A = 0; 
int B = 0; 
int C = 0; 
int D = 0; 
int E = 0; 
int F = 0; 
int G = 0;
int H = 0;

/* Debug Counters */
int MulCount = 0;
int JnzCount = 0; 
int SetCount = 0;
int SubCount = 0;

std::map<std::string, InstructionType> TypeMap { 
  {"jnz", InstructionType::Jnz},
  {"mul", InstructionType::Mul},
  {"set", InstructionType::Set},
  {"sub", InstructionType::Sub},
};

std::map<std::string, int*> RegMap { 
  {"a", &A},
  {"b", &B},
  {"c", &C},
  {"d", &D},
  {"e", &E},
  {"f", &F},
  {"g", &G},
  {"h", &H},
};

int main(int argc, char ** argv) { 
  /* read in the assembly program */
  std::ifstream file(argv[1], std::ifstream::in);
  while(!file.eof()) {
    Instruction i;

    std::string line;
    std::getline(file, line);

    auto typeEnd = line.find_first_of(' ');
    auto opLeftEnd = line.find_first_of(' ', typeEnd+1);
    
    auto type = line.substr(0, typeEnd);
    auto opLeft = line.substr(typeEnd+1, opLeftEnd-(typeEnd+1));
    auto opRight = line.substr(opLeftEnd+1, line.size() - (opLeftEnd+1));
  
    i.Type = TypeMap.at(type);

    try { 
      i.RegLeft = RegMap.at(opLeft); 
    } catch (std::out_of_range) { 
      i.ImmediateLeft = std::stoi(opLeft);
    }

    try {
      i.RegRight = RegMap.at(opRight);
    } catch (std::out_of_range) { 
      i.ImmediateRight = std::stoi(opRight);
    }

    InstructionMemory.push_back(i);
  }

  /* run the program */
  while(InstructionPointer < InstructionMemory.size()) {
    /* Fetch */ 
    auto i = InstructionMemory[InstructionPointer];

    /* Decode & Execute */
    switch(i.Type) { 
      case InstructionType::Jnz:
        JnzCount++;
        if(i.RegLeft && *i.RegLeft != 0 || i.ImmediateLeft != 0) { 
          InstructionPointer += i.RegRight ? *i.RegRight : i.ImmediateRight;
        } else { 
          InstructionPointer++;
        }
        break;
      case InstructionType::Mul:
        MulCount++;
        *i.RegLeft = i.RegRight ? ((*i.RegLeft) * (*i.RegRight)) : (*i.RegLeft * i.ImmediateRight);
        InstructionPointer++;
        break;
      case InstructionType::Set:
        SetCount++;
        *i.RegLeft = i.RegRight ? *i.RegRight : i.ImmediateRight;
        InstructionPointer++;
        break;
      case InstructionType::Sub:
        SubCount++;
        *i.RegLeft = i.RegRight ? (*i.RegLeft - *i.RegRight) : (*i.RegLeft - i.ImmediateRight);
        InstructionPointer++;
        break;
    }
  }

  // /* Print Debug Stats */
  printf("After Normal Execution: \n");
  printf("\tJnz's Executed: %d\n", JnzCount);
  printf("\tMul's Executed: %d\n", MulCount);
  printf("\tSet's Executed: %d\n", SetCount);
  printf("\tSub's Executed: %d\n", SubCount);
  printf("\tRegister A: %d\n", A);
  printf("\tRegister B: %d\n", B);
  printf("\tRegister C: %d\n", C);
  printf("\tRegister D: %d\n", D);
  printf("\tRegister E: %d\n", E);
  printf("\tRegister F: %d\n", F);
  printf("\tRegister G: %d\n", G);
  printf("\tRegister H: %d\n", H);
  printf("\n");

  /* With A=1,  we get into a lengthy loop that checks, very inefficiently, if B is composite and, if so, 
     increments h by 1.  Run a simplified loop below.   
   */

  auto db = 106500;
  auto dc = 123500; 
  auto h = 0;
  do { 
    auto p = 2;
    while(p < db) { 
      if(db % p == 0) {
        h++; 
        break;
      } else { 
        p++;
      }
    }
    db += 17;
  } while( db <= dc);
  printf("After \"Debug\" Execution: \n");
  printf("\tRegister H: %d\n", h);
  return 0;
}