#ifndef __AOC_H__
#define __AOC_H__

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AssertNotImplemented(msg) assert(false && msg);

inline char ReadWord(char * line, FILE *file) { 
  auto i = 0;
  while(1) {
    line[i] = getc(file);
    if(line[i] == '\n' || line[i] == ' ' || feof(file)) {
      auto c = line[i];
      line[i] = '\0';
      return c;
    }
    i++;
  }
}

inline char ReadWord(char * word, FILE *file, char delimiter) { 
  auto i = 0;
  while(1) {
    word[i] = getc(file);
    if(word[i] == delimiter || feof(file)) {
      auto c = word[i];
      word[i] = '\0';
      return c;
    }
    i++;
  }
}

/**
 * String
 */
class String {
  public:
    String() : String("", 0) {}

    String() : 

    String(const char *chars) : String(chars, strlen(chars)) {}

    String(const char *chars, int32_t length) : _hash(0), _length(length) { 
      if(length > DefaultInitialCapacity) { 
        AssertNotImplemented("Dynamic resize.");
      }
      memcpy(_chars, chars, _length);
      memset(_chars+_length, '\0', 1);

      /* pre-compute the hash */
      for(auto i = 0; i < _length; i++) { 
        _hash = (_hash*101) + _chars[i];
      }
    }
    
    const char * CString() const { 
      return _chars;
    }

    int32_t Length() const { 
      return _length;
    }

    int32_t HashCode() const {
      return _hash;
    }

    bool operator==(const String &other) const { 
      if(this->_length != other._length) { 
        return false;
      }
      for(auto i = 0; i < _length; i++) { 
        if(other._chars[i] != this->_chars[i]) { 
          return false;
        }
      }
      return true;
    }
    
    char operator[](int index) const { 
      return _chars[index];
    }

    char& operator[](int index) {
      return _chars[index];
    }
  private:
    static const int32_t DefaultInitialCapacity = 32;
    
    int32_t _length;
    uint64_t _hash;
    
    char _chars[DefaultInitialCapacity+1]; 
};

/**
 * List
 */
template<typename T> 
class List { 
  public:
    List() : _capacity(DefaultInitialCapacity), _count(0) {}
    void Add(T &item) { 
      if(_count == _capacity) { 
        AssertNotImplemented("Dynamic resize.");
      }
      _list[_count] = item;
      _count++;
    }
    int32_t Count() {
      return _count;
    }
  private: 
    static const int32_t DefaultInitialCapacity = 32;

    int32_t _capacity;
    int32_t _count;
    T _list[DefaultInitialCapacity];
};

/**
 * Map<T>
 * 
 * A basic map with string keys and generic values. 
 * 
 * This implementation uses separate chaining to handle collisions. 
 * Each bucket contains a linked list of key value pairs that has to the same value.
 * 
 * It might be interesting to whip up an alternative implementation using a different
 * collision resolution algorithm, e.g. linear probing, quadratic probing, etc...
 */

template<typename T>
struct KeyValuePair { 
  const String Key;
  const T Value; 
  KeyValuePair *Next;
};

/*  TODO: Implement an iterator-like class to enable use of range-based for's */
// class MapIterator<T> { 
//   public: 
//     bool operator!=(const MapIterator<T> &other) { 

//     }
//     T& operator*() { 

//     }
//     MapIterator<T>& operator++() { 

//     }
// };

template<typename T>
class Map { 
  public: 
    void Add(const String &key, const T &value) { 
      auto bucket = key.HashCode() % _capacity;
      auto kv = _buckets[bucket];
      if(!kv) { 
        _buckets[bucket] = new KeyValuePair<T> { 
          key, 
          value, 
          nullptr
        };
      } else { 
        /* Walk the chain and attempt to find the key/value pair in question */
        while(kv->Next) { 
          kv = kv->Next;
        }
        kv->Next = new KeyValuePair<T> { 
          key, 
          value, 
          nullptr
        };
      }
      _count++;
      return;
    }

    bool TryGet(const String &key, T& value) const { 
      auto bucket = key.HashCode() % _capacity;
      /* Walk the chain and attempt to find the key/value pair in question */
      auto kv = _buckets[bucket];
      while(kv) { 
        if(kv->Key == key) {
          value = kv->Value;
          return true;
        }
        kv = kv->Next;
      }
      return false;
    }

    int Count() const { 
      return _count;
    }

    /**
     * TODO: Implement C++-style iterator begin and end
     * so that Map<T> works with range based for loops
     */
    // MapIterator<T> begin() { 

    // }

    // MapIterator<T> end() { 

    // }

  private: 
    static const int32_t DefaultInitialCapacity = 32;
      
    int32_t _capacity = DefaultInitialCapacity;
    int32_t _count = 0;

    KeyValuePair<T>* _buckets[DefaultInitialCapacity] = { 0 };
};

// File I/O Stuff

inline String ReadLine(FILE *f) { 
  char line[255];
  int length = 0;

  while((line[length] = getc(f)) != '\n' && !feof(f)) { length++; }
 
  return String(line, length);
}

#endif

