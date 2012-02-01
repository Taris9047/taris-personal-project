      program FACTORIAL

!  definition of variables
      integer*8 i, res, number
      parameter( number=50 )

      res = 1
      do i=2,number             ! this loop calculates the factorial
         res = res*i
      enddo

      write(*,'("Factorial of ",I10," is ",I30)') 
     +   number, res

      end program FACTORIAL
