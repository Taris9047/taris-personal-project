program Fahrenheit_Celsius
      real, parameter :: low = 0.0
      real, parameter :: high = 110.0
      real, parameter :: step = 10.0

      real f, c ! Fahrenheit and Celsius temperatures, respectively.
      
      f = low
      
      write( *, fmt = "(A10)", advance = "no" ) "Fahrenheit"
      write( *, fmt = "(A10)" ) "Celsius"
      do while ( f /= high )
         write( *, fmt = "(F8.3)", advance = "no" ) f
         c = ftoc ( f )
         write( *, fmt = "(F8.3)" ) c
         
         f = f + step ! Advance the Fahrenheit temperature
      end do      

      contains

      function ftoc ( f )
        real, intent( in ) :: f
        real ftoc
        
        ftoc = 5.0 * ( f - 32 ) / 9.0
      end function ftoc
      
end program Fahrenheit_Celsius
