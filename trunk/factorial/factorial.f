      program FACTORIAL

!  definition of variables
      integer i, res, number
      parameter( number=50000 )

      res = 1
      do i=2,number   ! this loop calculates the factorial
         res = res*i
      enddo

      write(*,'("Factorial of ",I3," is ",I5)') 
     +     number, res

      end