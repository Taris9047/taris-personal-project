good_cookies = 1...3
bad_cookies = 1..3
burnt_cookies = 1..3

# Range Comparison test
puts 'Range Comparison Test'
puts(good_cookies == bad_cookies)
puts(good_cookies.eql?(burnt_cookies))
puts(bad_cookies == burnt_cookies)

# Range inclusion test
puts 'Range Inclusion Test'
my_guess = 2
his_guess = 19
puts(good_cookies === my_guess)
puts(good_cookies.include?(my_guess))
puts(good_cookies === his_guess)

