c   This is the test program 'testran.f' for TRANSMP.

c   David H. Bailey     96-03-15

cmp+ precision level 100
cmp+ mixed mode fast
cmp+ output precision 56
cmp+ epsilon 1e-90

      program testran
cmp+ implicit multip real (a-h, o-z)
cmp+ multip integer ia, ib, ic
cmp+ multip real a, b
cmp+ multip complex c, d, e
      parameter (n = 25)
      implicit double precision (a-h, o-z)
      double precision a(n), b(n)
      double complex c, d, e, dpcmpl

c   MP parameter definitions.

      parameter (dpeps = 1d-15, dppic = 3.141592653589793d0)

      ee = exp (1.d0+0)
      write (6, *) dppic, ee
      s = 0.d0

c   Loop with subscripted MP variables.

      do 100 i = 1, n
        a(i) = 2 * i + 1
        b(i) = 2.d0 * a(i) * (a(i) + 1.d0)
        s = s + b(i) ** 2
 100  continue

      write (6, *) s

c   An expression with mixed MPI and MPR entities.

      ia = s
      ib = 262144
      s = (s + 327.25d0) * mod (ia, 4 * ib)
      write (6, *) s

c   A complex square root reference.

      e = sqrt (dpcmpl (2.d0 * s, s))
      write (6, *) e

c   External and intrinsic MP function references in expressions.

      s = dot (n, a, b)
      t = 2.d0 * sqrt (s) ** 2
      write (6, *) s, t
      s = s / 1048576.d0
      t = s + 2.d0 * log (s)
      x = 3 + nint (t) * 5
      write (6, *) s, t, x

c   Deeply nested expressions and function references.

      x = (s + (2 * (s - 5) + 3 * (t - 5))) * exp (cos (log (s)))
      write (6, *) x

c   A "special" subroutine call (computes both cos and sin of S).

      call dpcssn (s, x, y)
      t = 1.d0 - (x ** 2 + y ** 2)

c   IF-THEN-ELSE construct involving MP variables.

      if (abs (t) .lt. dpeps) then
        write (6, *) t
      else
        write (6, *) dpeps
      endif

      stop
      end

c   MP function subprogram.

cmp+ multip real a, b, dot, s
      function dot (n, a, b)
      double precision a(n), b(n), dot, s

      s = 0.d0

      do 100 i = 1, n
        s = s + a(i) * b(i)
 100  continue

      dot = s
      return
      end

c   DP equivalent of special subroutine DPCSSN.

      subroutine dpcssn (a, x, y)
      double precision a, x, y
      x = cos (a)
      y = sin (a)
      return
      end

c   DP equivalent is special function DPCMPL.

      function dpcmpl (a, b)
      double complex dpcmpl
      double precision a, b
      dpcmpl = dcmplx (a, b)
      return
      end
