program Fahrenheit_Celsius

! Output a table of Fahrenheit and equivalent Celcius temperatures
! from low to high steps of step.

      real, parameter :: low = 0.0
      real, parameter :: high = 110.0
      real, parameter :: step = 10.0

      real f, c ! Fahrenheit and Celsius temperatures, respectively.
      
      f = low
      
      write( *, fmt = "(A10)", advance = "no" ) "Fahrenheit"
      write( *, fmt = "(A10)" ) "Celsius"
      do while ( f /= high )
         write( *, fmt = "(F8.3)", advance = "no" ) f
         c = 5.0 * ( f - 32 ) / 9.0
         write( *, fmt = "(F8.3)" ) c
         
         f = f + step ! Advance the Fahrenheit temperature
      end do
end program Fahrenheit_Celsius
