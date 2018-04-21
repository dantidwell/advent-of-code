#include <iostream>
#include <vector>

int main(int argc, const char ** argv) { 
  long genAStart = 883; 
  long genBStart = 879;
  
  long mod = 0x7fffffff; 
  long mask = 0x0000FFFF;
  long genAFactor = 16807; 
  long genBFactor = 48271; 
  long genACurrent = genAStart; 
  long genBCurrent = genBStart;

  long score = 0;

  for(auto i = 0; i < 40000000; i++) { 
    genACurrent = (genACurrent * genAFactor) % mod;
    genBCurrent = (genBCurrent * genBFactor) % mod;

    if(((genACurrent & mask) ^ (genBCurrent & mask)) == 0) { 
      score++;
    }
  }

  std::cout << "-- Part One --\n";
  std::cout << "\tThe final score is " << score << ".\n\n";

  const int PartTwoLimit = 5000000;
  int partTwoScore = 0;

  genACurrent = genAStart; 
  std::vector<long> genAValues; 
  while(genAValues.size() < PartTwoLimit) { 
    genACurrent = (genACurrent * genAFactor) % mod;
    if(genACurrent % 4 == 0) { 
      genAValues.push_back(genACurrent); 
    }
  }
  
  genBCurrent = genBStart;
  std::vector<long> genBValues;
  while(genBValues.size() < PartTwoLimit) { 
    genBCurrent = (genBCurrent * genBFactor) % mod;
    if(genBCurrent % 8 == 0) { 
      genBValues.push_back(genBCurrent); 
    }
  }

  for(auto i = 0; i < PartTwoLimit; i++) { 
    if(((genAValues[i] & mask) ^ (genBValues[i] & mask)) == 0) { 
      partTwoScore++;
    }
  }

  std::cout << "-- Part Two --\n";
  std::cout << "\tThe final score is " << partTwoScore << ".\n\n";
  return 0;
}