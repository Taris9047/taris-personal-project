# Block examples
myarray = %w{one two three four}
myarray.each {|element| print "[" + element + "]... \n"}
myarray.each do |element|
  print "[" + element + "]... \n"
end

# Proc and Blocks examples
myproc = Proc.new {|animal| puts "I love #{animal}!"}
myproc.call("Pandas")

def make_show_name(show)
  Proc.new {|host| show + " with " + host}
end
show1 = make_show_name("Practical Cannibalism")
show2 = make_show_name("Cotillions in the Amazon")
puts show1.call("H. Annabellector")
puts show2.call("Jack Hannah")
puts show1.call("Kirstie Alley")

# lambda method
myproc = lambda {|x| puts "Argument: #{x}"}
puts myproc.call("Texas forever!")

# lambda method vs. Proc.new
lproc = lambda {|a,b| puts "#{a+b} <- the sum"}
nproc = Proc.new {|a,b| puts "#{a+b} <- the sum"}
#puts lproc.call(1,2,3) # Error here!
puts nproc.call(1,2,3)

def procnew
  new_proc = Proc.new { return "I got here..."}
  # returns this and quit
  new_proc.call
  
  # does not return this
  return "... but not here."
end

def lambdaproc
  new_proc = lambda { return "You get here..." }
  new_proc.call
  
  # returns this
  return "And I got here!"
end
puts lambdaproc
puts procnew