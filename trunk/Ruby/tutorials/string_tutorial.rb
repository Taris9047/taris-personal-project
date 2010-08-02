puts 'Hello, World!'
puts 'What\'s up?'
puts 'A\tTab.'
puts "A \tTab."

puts "Inches/yard: #{12*3}"
puts "#{"Tora! "*3}"
puts %q{Hoagies & grinders!}
puts %Q;#{"Navy beans! "*3};

my_string = <<MY_STRING
This is a simple string that is
pre-formatted, which means that the
way it is formatted here including
tabs and newlines will be duplicated 
when I print it out.
MY_STRING
puts my_string