# Defining class
class MyFirstClass < Object
end

# Methods and Variables in the class
class Boogeyman
  def initialize
    puts "Yes, master?"
  end
end
monster1 = Boogeyman.new

class Boogeyman
  def initialize(name, location)
    # @xxxx are instance variables (Similar to constructors?)
    @name = name
    @location = location
    puts "Yes, master?"
  end
end
monster2 = Boogeyman.new("Mister Creepy", "New York, NY")

# More methods in a class
class Boogeyman
  def change_location(newlocation)
    @location = newlocation
    puts "I moved to #{newlocation}"
    self.get_info
  end
  def change_name(newname)
    @name = newname
    puts "I shall be called #{newname} from now on!"
    self.get_info
  end
  def get_info
    puts "I am #{@name} in #{@location}"
  end
end
monster3 = Boogeyman.new("Loopy Low", "Albuquerque, NM")
monster3.change_location("Wyoming")
monster3.change_name("Beezlebub")

# Adding new methods to a existing class
class String
  def writesize
    puts self.size
  end
end
size_writer = "Tell me my size!"
size_writer.writesize

# Setting up attributes from outside
class Boogeyman
  def scare_factor
    @scare_factor
  end
  def hiding_place
    @hiding_place
  end
  def scare_factor=(factor)
    @scare_factor = factor
  end
  def hiding_place=(place)
    @hiding_place = place
  end
end

monster4 = Boogeyman.new("Crazy Cal", "Nashville, TN")
monster4.scare_factor = 6000
puts monster4.scare_factor

# Another way to define attributes
#class Boogeyman
#  attr_reader :scare_factor, :hiding_place
#  attr_writer :scare_factor, :hiding_place
#end

# Class Scoped Objects
# Class local variable
class Boogeyman
  MY_BOSS = 'Mr. Boogeyman'
end

# @@ defines local variable in a method
class Boogeyman
  def initialize(name, location)
    @name = name
    @location = location
    
    @@latest = @name
    @@location = @location
    puts "Yes, master?"
  end
end
monster5 = Boogeyman.new("Macabre mac", "Seattle, WA")
monster6 = Boogeyman.new("Gory Gary", "Reston, WV")
puts Boogeyman.latest

