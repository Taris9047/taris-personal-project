program MatrixVector
      implicit none
      integer, parameter :: N = 3

      real, dimension( N, N ) :: A
      real, dimension( N ) :: b, c

      ! Fill A and b with random entries
      call random_number( A )
      call random_number( b )

      ! Compute the matrix-vector product A*b
      c = matrixVectorMultiply( A, b )

      print *, 'The matrix-vector product is ', c

      contains

      function matrixVectorMultiply( A, b ) result( c )
         implicit none
         
         ! Assume the shape of A and b.
         real, dimension( :, : ), intent( in ) :: A
         real, dimension( : ), intent( in ) :: b
         real, dimension( size ( b ) ) :: c

         integer N
         integer i

         N = size ( b )
         
         c = 0.0

         do i = 1, N
            ! As seen here, : enables selecting rows without loops.
            c = c + b( i ) * A( :, i )
         enddo
      end function matrixVectorMultiply

end program MatrixVector
