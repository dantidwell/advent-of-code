#include "../aoc.h"

struct Bot { 
  uint8_t LowBot; 
  uint8_t HighBot;
};
struct OutputPort { 
  uint8_t Value;
};
Bot Bots[255];
OutputPort Outputs[255];

struct TinyString { 
  constexpr TinyString(const char * string, int32_t length) :
    Length(length),
    Mask(TinyString::CreateMask(length)), 
    Value(TinyString::CreateValue(string,length))
  {}
  bool IsStartOf(const char * other) const { 
    return *(reinterpret_cast<const uint64_t*>(other)) & Mask == Value;
  }
  const int32_t Length = 0;
  const uint64_t Value = 0;
  const uint64_t Mask = 0;
  private:
    static constexpr uint64_t CreateValue(const char * string, int32_t length) { 
      uint64_t val = 0;
      for(auto i = 0; i < length; i++) { 
        val |= (static_cast<uint64_t>(string[i]) << 8*i);
      }
      return val;
    }
    static constexpr uint64_t CreateMask(int32_t length) { return (1ull << 8*length)-1; }
};

static constexpr TinyString TsBot = TinyString("bot", 3);
static constexpr TinyString TsHigh = TinyString("high", 3);
static constexpr TinyString TsLow = TinyString("low", 3);
static constexpr TinyString TsOutput = TinyString("output", 6);
static constexpr TinyString TsValue = TinyString("value", 5);

static void ReadBotLine(const char * line, int32_t lineLength) { 
  line += (TsBot.Length+1);
  auto srcBot = 0;
}

static void ReadValueLine(const char * line, int32_t lineLength) { 
  line += (TsValue.Length+1);
  auto val = 0; 
  auto destBot = 0; 
}

int32_t main(int32_t argc, const char ** argv) {
  const int32_t LineLength = 128;
  char Line[LineLength];

  /* Get all the rules */
  auto f = fopen(argv[1], "r");
  while(feof(f) == 0) { 
    Aoc::ReadToDelimiter(f, Line, LineLength, '\n');
    if(TsBot.IsStartOf(Line)) { 
      ReadBotLine(Line, LineLength);
    } else if(TsValue.IsStartOf(Line)) { 
      ReadValueLine(Line, LineLength);
    }
  }
  fclose(f);

  /* Follow the rules */
   

  return 0;
}