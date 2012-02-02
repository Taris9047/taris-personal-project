cmp+ precision level 1000
cmp+ mixed mode fast
cmp+ output precision 1000
cmp+ scratch space 5000
      program factorial
cmp+ multip integer res
      integer*8 i, Num
      Num = 50000
      res = 1
c
      do i=2,Num
         res = res*i
      enddo
c
      write(*,'("Factorial of ",I10," has been finished.")') 
     +   Num
      write(6, *) res
      stop
      end
