CMP+ PRECISION LEVEL 250000
CMP+ MIXED MODE FAST
CMP+ OUTPUT PRECISION 200000
CMP+ SCRATCH SPACE 400000
      PROGRAM FACTORIAL
CMP+ MULTIP INTEGER RES
      INTEGER*8 I, NUM
      NUM = 50000
      RES = 1+0
C
      DO I=2,NUM
         RES = RES*I
      ENDDO
C
      WRITE(*,'("Factorial of ",I10," has been finished.")') 
     +   NUM
C      WRITE(6, *) RES
      STOP
      END
