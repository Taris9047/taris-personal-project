#!/usr/bin/env python

#import math as m
#factN = m.factorial(n)

def factorial(N):
    fac = 1
    for i in range(1, N+1):
        fac *= i
    return fac
 
n = 50000
facN = factorial(n)

print "%d! calculation is complete!"%(n)
#print "fac %d is %d"%(n, facN)
