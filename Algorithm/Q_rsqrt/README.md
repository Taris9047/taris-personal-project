### Result

With Compile flag `-g3 -ggdb -W -Wall`
```
Result of rsqrt 100.00 is: 1.000000e-01
Running 5000000 repeatation.
Time taken: 0.073434 seconds...

Result of Q_rsqrt 100.00 is: 9.984488e-02, Precision: 99.844879 % 
Running 5000000 repeatation.
Time taken: 0.022390 seconds...

Q_rsqrt is 69.510033 % faster!!??

Result of Q_rsqrt2 100.00 is: 9.999964e-02, Precision: 99.999634 % 
Running 5000000 repeatation.
Time taken: 0.054636 seconds...

Q_rsqrt2 is 25.598497 % faster!!??

```

With Compie flag `-O3 -march=native -fomit-frame-pointer -pipe -W -Wall`
```
Result of rsqrt 100.00 is: 1.000000e-01
Running 5000000 repeatation.
Time taken: 0.002397 seconds...

Result of Q_rsqrt 100.00 is: 9.984488e-02, Precision: 99.844879 % 
Running 5000000 repeatation.
Time taken: 0.005826 seconds...

Q_rsqrt is -143.053818 % faster!!??

Result of Q_rsqrt2 100.00 is: 9.999964e-02, Precision: 99.999634 % 
Running 5000000 repeatation.
Time taken: 0.006251 seconds...

Q_rsqrt2 is -160.784317 % faster!!??


```

### Comment
O boy... old codes are just old codes... After optimization, cmath is obviously faster!! Technology evolves!!

