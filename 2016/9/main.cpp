#include "../aoc.h"

struct CodecMultiplier {
  int64_t CurrentMultiplier() { 
    return _currentMultiplier;
  }
  
  void Add(int64_t numCharacters, int64_t multiplier) { 
    auto n = new Node { _currentStep + numCharacters, multiplier, nullptr };

    /* there is no root, so make this the root */
    if(!_head) { 
      _head = n;
      return;
    }

    /* this node will expire before the root, so insert it at the root */
    if(n->ExpirationStep < _head->ExpirationStep) { 
      n->Next = _head;
      _head = n;
      return;
    }
    
    /* otherwise, figure out where it should go */
    auto c = _head;
    while(c->Next && (n->ExpirationStep >= c->Next->ExpirationStep)) { 
      c = c->Next;  
    }
    n->Next = c->Next;
    c->Next = n;
  }

  void TakeAStep() { 
    _currentStep++;
    _currentMultiplier = 1;

    while(_head && _head->ExpirationStep < _currentStep) { 
      auto temp = _head; 
      _head = _head->Next;
      delete temp;
    }
    auto n = _head;
    while(n) { 
      _currentMultiplier *= n->Multiplier;
      n = n->Next;
    }
  }

  private: 
    struct Node { 
      int64_t ExpirationStep; 
      int64_t Multiplier;
      Node *Next;
    };  
    Node *_head;
    int64_t _currentMultiplier;
    int64_t _currentStep; 
};

int main(int argc, const char ** argv) { 
  auto f = fopen(argv[1], "r");
  if(!f) { 
    return -1;
  }

  char c = 0;
  int64_t length = 0;
  CodecMultiplier cm;

  while((c = getc(f)) != EOF) {
    cm.TakeAStep();
    if(c != '(') { 
      length += cm.CurrentMultiplier();
      continue;
    }
    int64_t numChars = 0, multiplier = 0; 
    while((c = getc(f)) != 'x') { 
      cm.TakeAStep();
      numChars = 10*numChars + (c-0x30); 
    }
    cm.TakeAStep();
    while((c = getc(f)) != ')') { 
      cm.TakeAStep();
      multiplier = 10*multiplier + (c-0x30); 
    }
    cm.TakeAStep();
    cm.Add(numChars, multiplier);
  }
  printf("The length of the decoded message is %lld.\n", length);
  return 0;
}