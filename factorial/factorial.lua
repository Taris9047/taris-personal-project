#!/usr/bin/env lua
fact = 1
N = 50000
for i=1,N do
	fact = fact*i
end
print(fact)
print(N,"! calculation is finished!")
