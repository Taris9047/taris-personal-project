#!/usr/bin/env python

#import math as m
#factN = m.factorial(n)

import time as t

def factorial(N):
    fac = 1
    for i in range(1, N+1):
        fac *= i
    return fac

n = 50000
start = t.time()
facN = factorial(n)
end = t.time()

print "%d! calculation is complete!"%(n)
print "Calculation finished in %.6f seconds."%(end-start)

#print "fac %d is %d"%(n, facN)
