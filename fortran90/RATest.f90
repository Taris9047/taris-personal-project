program RATest
  use RationalArithmetic
  implicit none

  integer, parameter :: N = 100
  type( rational ), parameter :: RatNum = rational( 2, 3 )
  type( rational ) IntRatNum
  type( rational ) RatNumInt
  type( rational ) RatRat
  real RationalValue

  print *, 'Integer number is : ', N

  print *, 'The Rational Number is : ', RatNum%n, '/', RatNum%d

  IntRatNum = N * RatNum

  print *, 'Int * RationalNumber is : ', IntRatNum%n, '/', IntRatNum%d

  RatNumInt = RatNum * N

  print *, 'RationalNumber * Int is : ', RatNumInt%n, '/', RatNumInt%d

  RatRat = RatNum * RatNum

  print *, 'Rational * Rational is : ', RatRat%n, '/', RatRat%d

  RationalValue = rationalToReal( RatNum )

  print *, 'Real Number expression is : ', RationalValue

end program RATest
