program Triangle
      real a, b, c
      print *, 'Enter the lengths of the three sides of the triangle'
      read *, a, b, c
      print *, 'Triangle''s area:  ', triangleArea( a, b, c )

      contains
      
      function triangleArea( a, b, c )
         real triangleArea
         real, intent( in ) :: a, b, c
         
         real theta
         real height

         theta = acos( ( a**2 + b**2 - c**2 ) / ( 2.0 * a * b ) )
         height = a * sin( theta )

         triangleArea = 0.5 * b * height
      end function triangleArea

end program Triangle
