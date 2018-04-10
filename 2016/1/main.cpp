#include "aoc.h"

const int BufferSize = 8;
char Buffer[BufferSize] = {0};

struct Vec2 { 
  int x = 0; 
  int y = 0;
  bool operator==(const Vec2 &other) { return ((this->x == other.x) && (this->y == other.y)); }
  Vec2 operator+(const Vec2 &other) { return { this->x + other.x, this->y + other.y }; }
  int AbsoluteValue() { return abs(x) + abs(y); }
};

class Vec2HashTable { 
  public: 
    inline bool TryAdd(Vec2 &v) { 
      auto x = abs(v.x) % NumBuckets, y = abs(v.y) % NumBuckets;
      //walk the linked list at the bucket location
      //if we find the vec2 return false
      //else add it and return true
      if(!_table[x][y]) { 
        _table[x][y] = new Vec2HashTableNode { v, nullptr };
        return true;
      }
      auto b = _table[x][y];
      while(1) { 
        if(b->Item == v) { 
          return false;
        }
        if(!b->Next) { 
          b->Next = new Vec2HashTableNode { v, nullptr };
          return true;
        }
        b = b->Next;
      }
    }
  private: 
    struct Vec2HashTableNode { 
      Vec2 Item; 
      Vec2HashTableNode *Next;
    };

    static const int NumBuckets = 10; 
    Vec2HashTableNode* _table[NumBuckets][NumBuckets] = { 0 };
};

int main(int argc, char ** argv) {
  Vec2 position = {0,0};
  Vec2 direction = {0,1};
  Vec2HashTable history;
  history.TryAdd(position);

  auto f = fopen(argv[1], "r"); 
  if(!f) {
    return -1;
  }

  while(feof(f) == 0) {
    Aoc::EatWhiteSpace(f);
    Aoc::ReadToDelimiter(f, Buffer, BufferSize, ',');
    if(Buffer[0] == 'L') { 
      direction = { -direction.y, direction.x }; //rotates the direction vector by 90 degrees   
    } else if(Buffer[0] == 'R') { 
      direction = { direction.y, -direction.x }; //rotates the direction vector by -90 degrees   
    }
    for(auto i = atoi(Buffer+1); i > 0; i--) { 
      position = position + direction; // add the direction vector to the position vector
      if(!history.TryAdd(position)) { 
        printf("You have revisited the location x: %d, y: %d, which is %d blocks away.\n", position.x, position.y, position.AbsoluteValue());
      }
    }
  }
  fclose(f);    
  printf("You have journeyed %d blocks total.\n", position.AbsoluteValue());
  return 0;
}