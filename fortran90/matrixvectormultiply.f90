module MatrixVectorOperations

      integer, parameter :: N = 3
      
      contains

      function matrixVectorMultiply( A, b ) result( c )
         implicit none
         
         real, dimension( :, : ), intent( in ) :: A
         real, dimension( : ), intent( in ) :: b
         real, dimension( size( b ) ) :: c

         integer N
         integer i

         N = size( b )

         c = 0.0

         do i = 1, N
            c = c + b( i ) * A( :, i )
         enddo
      end function matrixVectorMultiply

      function matrixVectorMultiplyTed( A, b ) result( c )
         implicit none
         
         real, dimension( :, : ), intent( in ) :: A
         real, dimension( : ), intent( in ) :: b
         real, dimension( size( b ) ) :: c

         integer N
         integer i, j

         N = size( b )

         c = 0.0

         do i = 1, N
            do j = 1, N
               c( i ) = c( i ) + A( i, j ) * b( j )
            enddo
         enddo
      
      end function matrixVectorMultiplyTed

end module MatrixVectorOperations

program MatrixVector
      use MatrixVectorOperations
      implicit none

      real, dimension( N, N ) :: A
      real, dimension( N ) :: b, c

      call random_number( A )
      call random_number( b )

      c = matrixVectorMultiply( A, b )

      print *, 'The matrix-vector product is ', c

      c = matrixVectorMultiplyTed( A, b )

      print *, 'with another method ', c

end program MatrixVector
