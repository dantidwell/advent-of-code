#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char ** argv) { 
  if(argc < 2) { 
    return 1;
  }
  auto input = atoi(argv[1]);
  
  auto ring = ((int) ceil(sqrt(input))) / 2;
  printf("Input in ring #%d\n", ring);
  
  /* which octant are we in? */
  auto upperBound = (int) pow(2*ring+1, 2);
  auto increment = (2*ring+1) / 2;

  if(input >= upperBound - increment) { 
    printf("%d steps down; %d steps right", ring, input - (upperBound - increment));
  } else if (input >= upperBound - 2*increment) { 
    printf("%d steps down; %d steps left", ring, (upperBound - increment) - input);
  } else if (input >= upperBound - 3*increment) { 
    printf("%d steps left; %d steps down", ring, input - (upperBound - 3*increment));
  } else if (input >= upperBound - 4*increment) { 
    printf("%d steps left; %d steps up", ring, (upperBound - 3*increment) - input);
  } else if (input >= upperBound - 5*increment) { 
    printf("%d steps up; %d steps left", ring, input - (upperBound - 5*increment));
  } else if (input >= upperBound - 6*increment) { 
    printf("%d steps up; %d steps right", ring, (upperBound - 5*increment) - input);
  } else if (input >= upperBound - 7*increment) { 
    printf("%d steps right; %d steps up", ring, input - (upperBound - 7*increment));
  } else { 
    printf("%d steps right; %d steps down", ring, (upperBound - 7*increment) - input);
  }
  printf("\n");

  auto totes = 0;
  while(totes < input) { 
    
  }

}