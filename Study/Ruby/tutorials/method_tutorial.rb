# Most simple method
def my_new_method(name)
  puts "hey, " + name + ", this is my new method..."
end

# Variable inside the method
def my_method(first, second)
  puts first
  third = second
  puts second
end

my_new_method('magnus')
my_method("yes", "no")

# Errors below
#puts first
#puts third

# Method input with initial values set
def new_method_2(a = "This", b = "is", c = "fun")
  puts a + ' ' + b + ' ' + c + '.'
end

new_method_2('Rails')

# Multi input variables
def print_relation(relation, *names)
  puts "My #{relation} include: #{names.join(', ')}."
end
print_relation("cousins", "Morgan", "Miles", "Lindsey")

# Parenthesis when calling methods
puts "Look ma! No parentheses!"
puts("Look ma! Parentheses")
puts
puts()

# Returning value from method
def return_me(value)
  scoped_value = value
end
def echo_me(value)
  value
end
def multi_return
  return 'more', 'than', 'one'
end

my_value = return_me('this is fun!')
puts my_value

one, two, three = multi_return
puts one + three