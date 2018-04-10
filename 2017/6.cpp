#include "aoc.h" 

class Memory { 
  public:
    const int NumBanks = 16;
    bool operator== (Memory &other) { 
      return other._memory == _memory;
    }
    uint64_t GetBank(int index) { 
      return (_memory >> (index*4)) & 0xF;
    }
    void IncrementBank(int index) {
      SetBank(index, GetBank(index)+1);
    }
    void SetBank(int index, uint64_t value) { 
      assert(value <= 0xF);
      uint64_t mask = ~(((uint64_t) 0xF) << 4*index); 
      _memory = (mask & _memory)  | (value << (4*index));
    }

  private: 
    uint64_t _memory;
};

class MemoryHistory { 
  public: 
  MemoryHistory(Memory &value) : _previous(nullptr), 
    _value(value)
   {}

  inline MemoryHistory* Push(Memory &memory) { 
    return new MemoryHistory(this, memory);
  }
  
  inline Memory Last() { 
    return _value;
  }

  inline int32_t Size() { 
    auto count = 1;
    auto p = this;
    while(p->_previous) { 
      count++; 
      p = p->_previous;
    } 
    return count; 
  }

  int32_t IndexOf(Memory &value) { 
    auto index = 0;
    auto p = this;
    while(p) { 
      if(p->_value == value) {
        return index;
      }
      index++;
      p = p->_previous;
    }
    return -1;
  }

  bool HasValue(Memory &value) { 
    auto p = this;
    while(p) { 
      if(p->_value == value) {
        return true;
      }
      p = p->_previous;
    }
    return false;
  }
  
  private: 
    MemoryHistory(MemoryHistory *previous, Memory &value) : 
      _previous(previous), 
      _value(value) {}
    Memory _value; 
    MemoryHistory *_previous;
};

int main(int argc, const char ** argv) {
  auto input = fopen(argv[1], "r");
  
  /* get the initial conditions */
  Memory start;
  for(auto i = 0; i < start.NumBanks; i++) { 
    char command[3]; 
    ReadWord(command, input, '\t');
    start.SetBank(i, atoi(command));
  }

  auto history = new MemoryHistory(start);
  while(1) { 
    auto next = history->Last();
    auto maxIndex = 0;
    auto maxValue = 0; 
    for(auto i = 0; i < next.NumBanks; i++) { 
      auto curr = next.GetBank(i);
      if(curr > maxValue) { 
        maxIndex = i; 
        maxValue = curr;
      }
    }
    next.SetBank(maxIndex, 0);
    for(auto i = 0; i < maxValue; i++) { 
      next.IncrementBank((maxIndex+1+i)%next.NumBanks);
    }
    if(history->HasValue(next)) { 
      printf("--Part One--\n");
      printf("\tNumber of cycles before repeat: %d\n", history->Size());
      printf("--Part Two--\n");
      printf("\tNumber of cycles before since last occurance: %d\n", history->IndexOf(next)+1);
      break;
    } 
    history = history->Push(next);
  }
  fclose(input);
  return 0;
}