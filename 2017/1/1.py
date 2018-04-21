import time

f = open("./1/1.txt")
digits = map(lambda c: int(c), f.readline())
f.close()

def part_one(digits): 
  sum = 0
  i_1 = digits[len(digits)-1] 
  for d in digits:
    i = d
    if i_1 == i: 
      sum = sum + i_1
    i_1 = i
  return sum

def part_two(digits):
  sum = 0 
  l = len(digits)
  for i in xrange(l):
    d = digits[i]
    d_2 = digits[(i + l/2) % l]
    if d == d_2: 
      sum = sum+d
  return sum

print
print '-- Part One --'
start = time.clock()
part_one_sum = part_one(digits)
end = time.clock()
print 'Result: ' + str(part_one_sum) + '.'
print 'Duration (s): ' + str(end-start) + '.'
print 
print '-- Part Two --'
start = time.clock()
part_two_sum = part_two(digits)
end = time.clock()
print 'Result: ' + str(part_two_sum) + '.'
print 'Duration (s): ' + str(end-start) + '.'
print