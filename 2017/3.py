import aoc
import math

f = open('3.txt')
puzzle_input = int(f.readline())   
f.close()

def part_one():
  '''
    The ring containing the puzzle input can be found by solving the 
    following quadratic equation and rounding the positive root up to 
    the nearest integer

    (2k + 1)^2 = puzzle_input 
    4k^2 + 4k + (1 - puzzle_input) = 0

    Using the quadratic formula:
    k = 
      -4 + sqrt(4^2 - 4 * 4 * (1-puzzle_input))
       ----------------------------------------
                      2 * 4
    
    ring = ceil(k)

  '''
  ring = math.ceil((-4 + math.sqrt(16 - 16 * (1-puzzle_input))) / 8)
  steps_from_bottom_right = math.pow((2*ring+1),2) - puzzle_input

  q = math.floor(steps_from_bottom_right / ring)
  r = steps_from_bottom_right % ring
  if q % 2 == 0: 
    return ring + (ring - r) # heading toward an axis
  else:
    return ring + r # heading away from an axis

def part_two():
  rings = 5
  grid = [([0] * (2*rings-1)) for i in xrange(2*rings-1)]
  
  c = rings - 1
  grid[c][c] = 1

  for ring in xrange(1, rings):
    #Right side 
    x = c+ring
    for y in xrange(c+ring-1, c-ring-1, -1):
      grid[y][x] = grid[y+1][x] + grid[y-1][x-1] + grid[y][x-1] + grid[y+1][x-1]
      if grid[y][x] > puzzle_input: 
        return grid[y][x]
    #Top side 
    y = c-ring
    for x in xrange(c+ring-1, c-ring-1, -1):
      grid[y][x] = grid[y][x+1] + grid[y+1][x-1] + grid[y+1][x] + grid[y+1][x+1]
      if grid[y][x] > puzzle_input: 
        return grid[y][x]

    #Left side 
    x = c-ring
    for y in xrange(c-ring+1, c+ring+1, 1):
      grid[y][x] = grid[y-1][x] + grid[y-1][x+1] + grid[y][x+1] + grid[y+1][x+1]
      if grid[y][x] > puzzle_input: 
        return grid[y][x]

    #Bottom side 
    y = c+ring
    for x in xrange(c-ring+1, c+ring+1, 1):
      grid[y][x] = grid[y][x-1] + grid[y-1][x-1] + grid[y-1][x] + grid[y-1][x+1]
      if grid[y][x] > puzzle_input: 
        return grid[y][x]
  
aoc.run(part_one, part_two)

