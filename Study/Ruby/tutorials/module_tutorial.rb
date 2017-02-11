# Module Basics
module FighterValues
  BAMBOO_HEAD = { 'life' => 120, 'hit' => 9 }
  DEATH = { 'life' => 90, 'hit' => 13 }
  KOALA = { 'life' => 100, 'hit' => 10 }
  CHUCK_NORRIS = { 'life' => 60000, 'hit' => 99999999}
  
  def chuck_fact
    puts "Chuck Norris' tears can cure cancer."
    puts "Too bad he never cries."
  end
end

module ConstantValues
  DEATH = -5
  EaSY_HANDICAP = 10
  MEDIUM_HANDICAP = 25
  HARD_HANDICAP = 50
end

puts FighterValues::DEATH
puts ConstantValues::DEATH

# Module inclusion
module Movement
  def run
    puts "I'm running!"
  end
  
  def walk
    puts "I'm walking a bit briskly!"
  end
  
  def crawl
    puts "I'm so slowwwww!"
  end
end

class Man
  include Movement
  
  def jump
    puts "I'm bipedal and I can jump!"
  end
end

class Sloth
  include Movement
  
  def flop
    puts "It's all a lie... all I can do is flop around."
  end
end

mister_man = Man.new
mister_man.run

mister_slothy = Sloth.new
mister_slothy.flop