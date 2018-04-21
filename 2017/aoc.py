import time

def run(part_one, part_two): 
  print
  print '-- Part One --'
  start = time.clock()
  part_one_result = part_one()
  end = time.clock()
  print 'Result ' + str(part_one_result) + '.'
  print 'Duration (s): ' + str(end-start) + '.'
  print 
  print '-- Part Two --'
  start = time.clock()
  part_two_result = part_two()
  end = time.clock()
  print 'Result ' + str(part_two_result) + '.'
  print 'Duration (s): ' + str(end-start) + '.'
  print
