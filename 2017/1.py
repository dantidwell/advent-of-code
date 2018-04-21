import aoc

f = open("1.txt")
digits = map(lambda c: int(c), f.readline())
f.close()

def part_one(): 
  sum = 0
  i_1 = digits[len(digits)-1] 
  for d in digits:
    i = d
    if i_1 == i: 
      sum = sum + i_1
    i_1 = i
  return sum

def part_two():
  sum = 0 
  l = len(digits)
  for i in xrange(l):
    d = digits[i]
    d_2 = digits[(i + l/2) % l]
    if d == d_2: 
      sum = sum+d
  return sum

aoc.run(part_one, part_two)