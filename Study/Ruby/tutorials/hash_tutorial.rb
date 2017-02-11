# A sample hash
my_wombats = { 'Wally wombat' => 'The Jungle St.',
               'Wilma Wombat' => 'The House on the Corner',
               'Sam' => 'Notawombat Way,', 'Mump' => 13}
#puts my_wombats

# Create another simple hash
new_hash = Hash.new("Not here!")
puts new_hash['non-existent key']

# Insert a new element
my_wombats['Wombie McWombat'] = '123 That Street'
puts my_wombats['Wombie MacWombat']

# Reading hash
puts my_wombats['Wally Wombat']

# Hash test
puts my_wombats.has_key?('Wilma Wombat')
puts my_wombats.has_value?('Lamps and pandals')
puts my_wombats.empty?

# Remove an element
puts my_wombats.delete('Wilma Wombat')
my_wombats.clear
puts my_wombats.empty?