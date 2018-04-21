import sys
import aoc

f = open("2.txt")
lines = f.readlines() 
f.close()

def part_one():
  checksum = 0
  for line in lines:
    nums = map(lambda s: int(s), line.split('\t'))
    checksum = checksum + max(nums) - min(nums)
  return checksum

def part_two():
  checksum = 0
  for line in lines: 
    nums = map(lambda s: int(s), line.split('\t'))
    nums.sort() 
    for x in xrange(len(nums)-1): 
      for y in xrange(x+1, len(nums)): 
        if nums[y] % nums[x] == 0: 
          checksum = checksum + nums[y]/nums[x]
  return checksum

aoc.run(part_one, part_two)