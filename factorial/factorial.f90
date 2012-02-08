program factorial
  use mpmodulex
  implicit none

  integer i, Num
  type (mp_integer) Res
  call mpinitx(2500)
  
  Num = 50000
  Res = 1

  do i=2, Num
     Res = Res * i
  enddo

  print *, 'Calculation for ', Num, '! has been finished!'
  
end program factorial
