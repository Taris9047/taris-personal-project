doubleMe x = x + x

doubleUS x y = doubleMe x + doubleMe y

doubleSmallNumber x = ( if x > 100 then x else x*2 ) + 1

length' xs = sum [1 | _ <- xs]

removeNonUppercase :: [Char] -> [Char]
removeNonUppercase st = [ c | c <-st, c `elem` ['A'..'Z']]

addThree :: Int -> Int -> Int -> Int
addThree x y z = x+y+z

factorial :: Int -> Int
factorial n = product [1..n]

factorial' :: (Integral a) => a -> a
factorial' 0 = 1
factorial' n = n * factorial' (n - 1)

lucky :: (Integral a) => a -> String
lucky 7 = "Lucky 7!!"
lucky x = "Out of luck!!"