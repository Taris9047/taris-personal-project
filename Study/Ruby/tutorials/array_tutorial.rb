# Array Initialization
puts 'Array Initialization methods'
its_so_empty = []
oh_so_empty = Array.new
hello = ['ni hao', 'bonjour', 'hi', 'howdy']
random_types = [13, 'napkin', (1136+1).to_s]
puts its_so_empty
puts oh_so_empty
puts hello
puts random_types

# String -> Array wrapper
puts 'String -> Array wrapper'
my_haiku = %w(my dog digs it here\n)
my_haiku2 = %w(he is nice to me & cats\n)
my_haiku3 = %W(but he ate #{(2*3/6)} once)
my_haiku4 = %w(but he ate #{(2*3/6)} once)
puts my_haiku
puts my_haiku2
puts my_haiku3
puts my_haiku4


# Range -> Array conversion
puts 'Range -> Array conversion'
my_range = 1..10
my_dazzling_array = my_range.to_a
puts my_range
puts my_dazzling_array

# Manipulating Array
puts 'Array Manipulation'
my_dazzling_array[10] = 11
puts my_dazzling_array
my_dazzling_array[11] = 12
puts my_dazzling_array
my_dazzling_array.push(15, 16)
puts my_dazzling_array
my_dazzling_array.insert(-1, 17)
puts my_dazzling_array
my_dazzling_array << 14
puts my_dazzling_array
my_dazzling_array << 20 << 22 << 24
puts my_dazzling_array

# Array Access
puts 'Accessing Array'
puts my_dazzling_array[0]
index = 2
puts my_dazzling_array[index]
puts my_dazzling_array[0..2]
puts my_dazzling_array.at(0)
puts my_dazzling_array.fetch(999, "Not found!!")
puts my_dazzling_array.values_at(0,1,2)

# Array Deletion
puts 'Deleting Array Elements'
puts my_dazzling_array.pop
puts my_dazzling_array.shift
puts my_dazzling_array.delete_at(1)
puts my_dazzling_array.delete(4)
puts my_dazzling_array
puts my_dazzling_array.delete(1337) {"Wrong!"}

