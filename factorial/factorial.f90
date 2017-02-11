program factorial
  use mpfunmod
  use mpmodule
  implicit none

  integer i, Num, Prec
  type (mp_integer) Res
  parameter (Prec = 25000)
  call mpinit(Prec)
  
  Num = 5000
  Res = 1

  do i=2, Num
     Res = Res * i
  enddo

  print *, 'Calculation for ', Num, '! has been finished!'

  print *, 'The answer is...'
  call mpwrite(6, Res)
  stop
end program factorial
