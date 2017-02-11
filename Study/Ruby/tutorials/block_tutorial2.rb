# Yield
def yieldme
  print "1. Enter method. "
  yield
  print "3. Exit method. "
end
yieldme { print "2. Enter block. " }
puts

# Yield with iteration
def myeach(myarray)
  iter = 0
  while (iter < myarray.length)
    yield(myarray[iter])
    iter += 1
  end
end
testarray = [1,2,3,4,5]
myeach(testarray) {|item| print "#{item}:"}
puts

# Yield trick, usina it like Proc
def ampersand(&block)
  block.call
  yield
end
ampersand { print "I'm getting' called! "}
puts