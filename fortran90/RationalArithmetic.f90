module RationalArithmetic
  type rational
     integer n, d
  end type rational

  interface operator (*)
     module procedure intRationalMultiply, &
          rationalIntMultiply, rationalRationalMultiply
  end interface operator (*)

contains

  function intRationalMultiply( i, r )
    type( rational ) intRationalMultiply
    integer, intent( in ) :: i
    type( rational ), intent( in ) :: r

    intRationalMultiply = rational( i * r%n, r%d )
  end function intRationalMultiply

  function rationalIntMultiply( r, i )
    type( rational ) rationalIntMultiply
    type( rational ), intent( in ) :: r
    integer, intent( in ) :: i

    rationalIntMultiply = rational( i * r%n, r%d )
  end function rationalIntMultiply

  function rationalRationalMultiply( r1, r2 )
    type( rational ) rationalRationalMultiply
    type( rational ), intent( in ) :: r1
    type( rational ), intent( in ) :: r2
    
    rationalRationalMultiply = rational( r1%n * r2%n, r1%d * r2%d )
  end function rationalRationalMultiply

  function rationalToReal( r )
    real rationalToReal
    type( rational ), intent( in ) :: r

    rationalToReal = real( r%n ) / real ( r%d ) 
  end function rationalToReal
      
end module RationalArithmetic
