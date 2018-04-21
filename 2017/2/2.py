import sys
import time

import profile

f = open("./2/2.txt")
lines = f.readlines() 
f.close()

def line_checksum_part_one(line):
  nums = map(lambda s: int(s), line.split('\t'))
  return reduce(lambda p,c: max(p,c), nums, 0) - reduce(lambda p,c: min(p,c), nums, sys.maxint)

def line_checksum_part_two(line):
  nums = map(lambda s: int(s), line.split('\t'))
  nums.sort() 
  for x in xrange(len(nums)-1): 
    for y in xrange(x+1, len(nums)): 
      if nums[y] % nums[x] == 0: 
        return nums[y]/nums[x]
  return 0

print
print '-- Part One --'
start = time.clock()
part_one_checksum = reduce(lambda p,c: p+c, map(line_checksum_part_one, lines))
end = time.clock()
print 'Result ' + str(part_one_checksum) + '.'
print 'Duration (s): ' + str(end-start) + '.'
print 
print '-- Part Two --'
start = time.clock()
part_two_checksum = reduce(lambda p,c: p+c, map(line_checksum_part_two, lines))
end = time.clock()
print 'Result ' + str(part_two_checksum) + '.'
print 'Duration (s): ' + str(end-start) + '.'
print
