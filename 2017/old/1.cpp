#include <stdio.h>
#include <stdlib.h>

const int BufferCapacity = 4*1024;
char Buffer[BufferCapacity];

int main(int argc, const char ** argv) { 
  auto file = fopen(argv[1], "r");
  if(argc < 2) { 
    fprintf(stderr, "Please provide an input file.\n");
    return -1;
  }  
  
  if(file == nullptr) { 
    fprintf(stderr, "Input file not found.\n");
    return -1;
  }

  auto bufferLength = fread(&Buffer, sizeof(char), BufferCapacity, file);
  if(!feof(file)) { 
    fclose(file);
    fprintf(stderr, "File too large for buffer...maybe dynamically grow the buffer??\n");
    return 1;
  } 

  auto straightSum = 0;
  for(auto i = 1; i < bufferLength+1; i++) { 
    if(Buffer[i%bufferLength] == Buffer[i-1]) { 
      straightSum += Buffer[i-1]-48;
    }
  }
  
  auto skippingSum = 0;
  auto halfwayAround = bufferLength / 2;
  for(auto i = 0; i < bufferLength; i++) { 
    if(Buffer[i] == Buffer[(i + halfwayAround) % bufferLength]) { 
      skippingSum += Buffer[i]-48;
    }
  }

  printf("Ladies and Gentlemen, the straight line sum is %d!\n", straightSum);
  printf("Ladies and Gentlemen, the halfway skipping sum is %d!\n", skippingSum);
  fclose(file);
  return 0; 
}