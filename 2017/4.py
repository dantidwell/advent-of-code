import aoc 

f = open('4.txt')
lines = f.readlines()
f.close()

def part_one(): 
  num_good = 0
  for line in lines: 
    words = line.rstrip('\n').split(' ')
    intern_table = { w: True for w in words }
    if len(words) == len(intern_table):
      num_good = num_good + 1
  return num_good
    
def part_two(): 
  num_good = 0
  for line in lines:
    words = line.rstrip('\n').split(' ')
    intern_table = { ''.join(sorted(w)): True for w in words }
    if len(words) == len(intern_table):
      num_good = num_good + 1
  return num_good

aoc.run(part_one, part_two)