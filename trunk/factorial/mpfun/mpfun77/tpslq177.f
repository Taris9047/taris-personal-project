cmp+ precision level 320
cmp+ epsilon 1e-300
cmp+ mixed mode fast
cmp+ output precision 56
cmp+ scratch space 5000
      program tpslq1

c   Test program for PSLQ1 -- a one-level implementation of the standard
c   PSLQ algorithm for finding integer relations.  Fortran-90 version.

c   David H. Bailey     1999-09-15

c   These parameters are set in the parameter statement below.  Some other
c   parameters are set in other routines.
c>    dw  = Number of digits per word.  On IEEE systems, dw = 7.22472.
c           On Cray non-IEEE systems, dw = 6.62266.
c     idb = Debug level (0 - 3).
c     itr = Number of trials when KQ = 1, 2 or 3.
c     gam = Gamma, the PSLQ algorithm parameter.  Must be set to at least
c           Sqrt (4/3) = 1.154700538.
c     n   = Integer relation vector length.
c     kq  = 0 for the algebraic case [1, al, al^2, ... al^(n-1)], where
c           al = 3^(1/kr) - 2^(1/ks).  Set n = kr * ks + 1 and itr = 1.
c         = 1 for a random integer vector with a relation.
c         = 2 for a random integer vector without a relation.
c         = 3 for testing algebraic relations of a number read from a file.
c         = 4 for testing additive relations of numbers read from a file.
c         = 5 for testing multiplicative relations of numbers read from a file.
c         = 6 for custom input.
c     kr  = Degree of root of 3 when kq = 0.
c     ks  = Degree of root of 2 when kq = 0.
c     idv = Number of digits in the random vector values (except the last)
c           when kq = 1 or 2.  Normally set to ndp - 20.
c     ndp = No. digits of pecision.  Must not exceed mpipl in mpmod90.f.
c           It is recommended that this be set to at least 30 digits more
c           than the expected detection level of the relation.
c     nep = Log10 of epsilon for detections.  Normally set to 20 - ndp or so.
c           Must not exceed the accuracy of input data.
c     rb  = Max. acceptable norm of relations.  If kq = 1, set to 10.d0 * rm.
c     rm  = Maximum size of random relation coefficients when kq = 1 or 2.

      implicit none
      double precision d1, d2, drand, dw, gam, rb, rm, second, tm, tm0,
     $  tm1
      integer i, idb, idv, iq, itr, j, k, kps, kcs, kq, kr, ks, kss,
     $  n, ndp, nep
      parameter (dw = 7.22472d0, idb = 2, itr = 1, gam = 1.154700538d0,
     $  n = 37, kq = 0, kr = 6, ks = 6, idv = 100, ndp = 320, nep =
     $  -300,
     $  rb = 1.d100, rm = 1.d5)
      integer lnm(n)
      character*1 chr0(5*ndp/4+50), chr1(100)
      character*40 nam(n), namx
      double precision dr(n), r1(n)
cmp+ multip real  al, t1, t2, r, x, dmreal
      double precision  al, t1, t2, r(n), x(n)

c   This is scratch space for pslq1.  See below.

      integer is0(n)
      double precision s1(n*n+n)
cmp+ multip real  s2
      double precision  s2(2*n*n+2*n)

      write (6, 1) itr, gam, n, kq, kr, ks, idv, rb, rm, ndp, nep
1     format ('PSLQ1 Test Program'/'No. trials = ',i3,3x,
     $  'Gamma =',f12.9,
     $  3x,'n =',i4,3x,'kq =',i2/'kr =',i2,3x,'ks =',i2,3x, 'idv =',i4,
     $  3x,'rb =',1p,d12.4,3x,'rm =',d12.4/'Precision level ndp =',i6,
     $  ' digits',3x,'Epsilon level nep = ',i6)
      tm = 0.d0
      kps = 0
      kcs = 0
      if (kq .eq. 3 .or. kq .eq. 4 .or. kq .eq. 5) then
        open (11, file = 'pslq.inp')
        rewind 11
      endif

      do k = 1, itr
        write (6, 2) k
2     format (/'Start trial', i3)

        if (kq .eq. 0) then

c   This code generates al = 3^(1/kr) - 2^(1/ks).  al is algebraic of degree
c   kr * ks.  Set n = kr * ks + 1 to recover the polynomial satisfied by al.

          al = dpnrt (dmreal (3.d0), kr) - dpnrt (dmreal (2.d0), ks)
        elseif (kq .eq. 1) then

c   Construct a random input vector x with a relation.

          d1 = 2.d0 * rm + 1.d0
          t2 = dmreal (10.d0) ** idv
          t1 = 0.d0

          do i = 1, n - 1
            r1(i) = aint (d1 * drand ()) - rm
            x(i) = aint (t2 * dprand ())
            t1 = t1 + r1(i) * x(i)
          enddo

          r1(n) = - 1.d0
          x(n) = t1

          do i = 1, n
            write (namx, '(''ran'',i3.3)') i
            nam(i) = namx(1:6)
            lnm(i) = 6
          enddo

          write (6, 3) (r1(i), i = 1, n)
3       format ('Constructed relation:'/(6f13.0))
        elseif (kq .eq. 2) then

c  Construct a random input vector x without a relation.

          t2 = dmreal (10.d0) ** idv

          do i = 1, n
            x(i) = aint (t2 * dprand ())
            write (namx, '(''ran'',i3.3)') i
            nam(i) = namx(1:6)
            lnm(i) = 6
          enddo

        elseif (kq .eq. 3) then

c   Read an algebraic constant from a file.

          read (11, *) al
        elseif (kq .eq. 4) then

c   Read constants from a file for additive test.

          do i = 1, n
            read (11, *) al
            x(i) = al
            write (namx, '(''con'',i3.3)') i
            nam(i) = namx(1:6)
            lnm(i) = 6
          enddo
        elseif (kq .eq. 5) then

c   Read constants from a file for multiplicative test.

          do i = 1, n
            read (11, *) al
            x(i) = log (al)
            write (namx, '(''log(con'',i3.3,'')'')') i
            nam(i) = namx(1:11)
            lnm(i) = 11
          enddo
        elseif (kq .eq. 6) then

c   Produce X vector by a custom scheme.

        endif

c   If kq is 0 or 3, generate x = [1, al, al^2, ..., al^(n-1)].

        if (kq .eq. 0 .or. kq .eq. 3) then
          x(1) = 1.d0
          nam(1) = '1'
          lnm(1) = 1

          do i = 2, n
            x(i) = al * x(i-1)
            write (namx, '(''al^'',i3)') i - 1
            nam(i) = namx(1:6)
            lnm(i) = 6
          enddo
        endif

c   Perform relation search.

        tm0 = second ()
        call pslq1 (idb, gam, n, rb, x, is0, s1, s2, iq, r)
        tm1 = second ()

c   Output relation, if one was found.

        if (iq .ne. 0) then
          tm = tm + (tm1 - tm0)
          write (6, 4)
4       format ('Recovered relation: 0 =')

          do i = 1, n
            if (r(i) .ne. 0.d0) then
              call mpfform (r(i) , 24, 0, chr0, chr1)
              write (6, '(''+'',24a1,'' * '',a)') (chr1(j), j = 1, 24),
     $          nam(i)(1:lnm(i))
            endif
          enddo

c   Check if original relation was recovered.

          if (kq .ge. 1) then
            d1 = 0.d0
            d2 = 0.d0

            do i = 1, n
              d1 = d1 + abs (dble (r(i)) - r1(i))
              d2 = d2 + abs (dble (r(i)) + r1(i))
            enddo

            if (d1 .eq. 0.d0 .or. d2 .eq. 0.d0) then
              kcs = kcs + 1
              write (6, 5)
5           format ('Original relation recovered.')
            else
              kps = kps + 1
            endif
          endif
        endif
        write (6, 6) tm1 - tm0
6     format ('CPU Time =',f12.4)
      enddo

      if (itr .gt. 1) then
        write (6, 7) kps, kcs
7     format (/'No. partial successes =',i5/
     $  'No. complete successes =',i5)
        kss = kps + kcs
        if (kss .ne. 0) then
          tm = tm / kss
          write (6, 8) tm
8       format ('Ave. CPU time of PS or CS runs =',f10.4)
        endif
      endif
      stop
      end

c------------------------------

c   The following code performs the one-level, standard PSLQ algorithm.
c   David H. Bailey     1999-09-15

      subroutine pslq1 (idb, gam, n, rb, x, is0, s1, s2, iq, r)

c   This routine allocates the scratch space arrays is0, s1 and s2.  Handling
c   scratch space in this manner is not really necessary in Fortran-90, but
c   this design facilitates straightforward translation to Fortran-77.

c   Arguments are as follows:
c     idb = Debug flag: 0 (none) to 4 (full).
c     gam = PSLQ gamma parameter, normally set to sqrt(4/3).
c     n   = Dimension of input vector.
c     rb  = Maximum size (Euclidean norm) of acceptable relation (type DP).
c     x   = Input real vector (type mp_real).
c     is0 = Scratch array (type integer -- see size below).
c     s1  = Scratch array (type double precision -- see size below).
c     s2  = Scratch array (type mp_real -- see size below).
c     iq  = Output flag: 0 (unsuccessful) or 1 (successful).
c     r   = Output integer coefficient vector (type mp_real).

      implicit none
      integer idb, iq, n
      double precision gam, rb
      integer is0(n)
      double precision s1(n*n+n)
cmp+ multip real  s2, x, r
      double precision  s2(2*n*n+2*n), x(n), r(n)

      call pslq1x (idb, gam, n, rb, x, is0(1), s1(1), s1(n*n+1), s2(1),
     $  s2(n*n+1), s2(2*n*n+1), s2(2*n*n+n+1), iq, r)
      return
      end

      subroutine pslq1x (idb, gam, n, rb, x, ix, dh, dx, b, h, s, y,
     $  iq, r)

c   The following parameters are set in this routine:
c     ipi = Iteration print interval when idb >= 2.
c     ipm = Iteration check interval for MP iterations.
c     itm = Maximum iteration count.  Run is aborted when this is exceeded.

      implicit none
      integer i, idb, ipi, ipm, iq, it, itm, izm, j, n, n1, n2
      parameter (ipi = 100, ipm = 500, itm = 100000)
      double precision bound, d1, d2, d3, gam, rb, rn, second, tm0,
     $  tm1, times(5)
      common /timec/ times
      integer ix(n)
      double precision dh(n,n), dx(n)
cmp+ multip real  b, h, s, r, x, y, t1, t2
      double precision  b(n,n), h(n,n), s(n), r(n), x(n), y(n), t1, t2

c   Initialize.

      if (idb .ge. 2) write (6, 1) n
1     format ('PSLQ1 integer relation detection: n =',i5)
      iq = 0
      it = 0
      rn = 0.d0

      do i = 1, 5
        times(i) = 0.d0
      enddo

      tm0 = second ()
      call initmp (idb, n, ix, dx, b, h, s, x, y, izm)
      tm1 = second ()
      times(1) = tm1 - tm0
      if (idb .ge. 2) write (6, 2)
2     format ('MP initialization complete.')
      if (izm .ne. 0) goto 110

c   Perform MP iterations.

100   continue
      it = it + 1
      if (idb .eq. 3 .or. idb .ge. 2 .and. mod (it, ipi) .eq. 0) write
     $  (6, 3) it
3     format ('Iteration',i7)
      tm0 = second ()
      call itermp (idb, gam, it, n, ix, dx, b, h, y, izm)
      tm1 = second ()
      times(2) = times(2) + (tm1 - tm0)

      if (mod (it, ipm) .eq. 0) then

c   Find min and max absolute value of y vector.

        t1 = 1.d300
        t2 = 0.d0

        do i = 1, n
          t1 = min (t1, abs (y(i)))
          t2 = max (t2, abs (y(i)))
        enddo

        if (idb .ge. 2) then
          call decmd (t1, d1, n1)
          call decmd (t2, d2, n2)
          write (6, 4) it, d1, n1, d2, n2
4       format ('Iteration',i7,3x,'Min, max of y =',f11.6,'D',i5,
     $      f11.6,'D',i5)
        endif

c   Compute norm bound.

        do j = 1, n - 1
          do i = 1, n
            dh(i,j) = h(i,j)
          enddo
        enddo

        d3 = bound (n, dh)
        rn = max (rn, d3)
        if (idb .ge. 2) then
          write (6, 5) it, d3, rn
5       format ('Iteration',i7,3x,'Norm bound =',1p,d15.6,4x,
     $  'Max. bound =',
     $      1p,d15.6)
        endif
        if (it .gt. itm) then
          if (idb .ge. 1) write (6, 6) itm
6       format ('Iteration limit exceeded',i7)
          goto 120
        endif
        if (rn .gt. rb) then
          if (idb .ge. 1) write (6, 7) rb
7       format ('Norm bound limit exceeded.',1p,d15.6)
          goto 120
        endif
      endif

      if (izm .eq. 0) then
        goto 100
      elseif (izm .eq. 1) then
        goto 110
      elseif (izm .eq. 2) then
        goto 120
      endif

c   A relation has been detected.  Output the final norm bound and other info.

110   continue

      if (idb .ge. 1) then
        t1 = 1.d300
        t2 = 0.d0

        do i = 1, n
          t1 = min (t1, abs (y(i)))
          t2 = max (t2, abs (y(i)))
        enddo

        call decmd (t1, d1, n1)
        call decmd (t2, d2, n2)

        do j = 1, n - 1
          do i = 1, n
            dh(i,j) = h(i,j)
          enddo
        enddo

        d3 = bound (n, dh)
        rn = max (rn, d3)
        write (6, 8) it, d1, n1, d2, n2, rn
8     format ('Relation detected.',4x,'No. iterations =',i7/
     $    'Min, max of y =',0p,f11.6,'D',i5,f11.6,'D',i5/
     $  'Max. bound =',1p,d15.6)
      endif
      d3 = 1.d300

c   Select the relation with smallest norm.

      do j = 1, n
        if (abs (y(j)) .le. dpeps) then
          d1 = 0.d0

          do i = 1, n
            d1 = d1 + dble (b(i,j)) ** 2
          enddo

          d1 = sqrt (d1)
          if (idb .ge. 1) then
            call decmd (y(j), d2, n2)
            write (6, 9) j, d1, d2, n2
9         format ('Index of relation =',i4,3x,'Norm =',1p,d15.6,3x,
     $        'Residual =',0p,f11.6,'D',i5)
          endif
          if (d1 .lt. d3) then
            d3 = d1

            do i = 1, n
              r(i) = b(i,j)
            enddo
          endif
        endif
      enddo

      if (d3 .le. rb) then
        iq = 1
      else
        if (idb .ge. 2) write (6, 10)
10    format ('Relation is too large.')
      endif

120   continue
      if (idb .ge. 2) write (6, 11) times
11    format ('CPU times:'/(5f12.2))

      return
      end

c------------------------------

c   First-level subroutines.

      subroutine initmp (idb, n, ix, dx, b, h, s, x, y, izm)

c   This initializes MP arrays at the beginning.

      implicit none
      integer i, idb, izm, j, k, n, n1
      double precision d1
      integer ix(n)
      double precision dx(n)
cmp+ multip real  b, h, s, x, y, t1
      double precision  b(n,n), h(n,n), s(n), x(n), y(n), t1

      if (idb .ge. 3) then
        write (6, 1)
1     format ('initmp: Input x vector:')
        call matoutmd (1, n, ix, dx, x)
      endif
      izm = 0

c   Set b to the identity matrix.

      do j = 1, n
        do i = 1, n
          b(i,j) = 0.d0
        enddo

        b(j,j) = 1.d0
      enddo

      t1 = 0.d0

c   Compute the x vector, the square root of the partial sum of squares of x,
c   and the y vector, which is the normalized x vector.

      do i = n, 1, -1
        t1 = t1 + x(i) ** 2
        s(i) = sqrt (t1)
      enddo

      t1 = 1.d0 / s(1)

      do i = 1, n
        y(i) = t1 * x(i)
        s(i) = t1 * s(i)
      enddo

c   Compute the initial h matrix.

      do j = 1, n - 1
        do i = 1, j - 1
          h(i,j) = 0.d0
        enddo

        h(j,j) = s(j+1) / s(j)
        t1 = y(j) / (s(j) * s(j+1))

        do i = j + 1, n
          h(i,j) = - y(i) * t1
        enddo
      enddo

c  Perform reduction on h, updating y, b and h.

      do i = 2, n
        do j = i - 1, 1, -1
          t1 = anint (h(i,j) / h(j,j))
          if (t1 .ne. 0.d0) then
            y(j) = y(j) + t1 * y(i)

            do k = i, n
              b(k,j) = b(k,j) + t1 * b(k,i)
            enddo

            do k = 1, j
              h(i,k) = h(i,k) - t1 * h(j,k)
            enddo
          endif
        enddo
      enddo

      t1 = 1.d300

      do i = 1, n
        t1 = min (t1, abs (y(i)))
      enddo

      if (t1 .le. dpeps) then
        if (idb .ge. 2) then
          call decmd (t1, d1, n1)
          write (6, 2) 0, d1, n1
2       format ('Iteration',i8,3x,'initmp: Small value in y =',f11.6,
     $  'D',i5)
        endif
        izm = 1
      endif

      if (idb .ge. 3) then
        write (6, 3)
3     format ('initmp: Initial y vector:')
        call matoutmd (1, n, ix, dx, y)
        write (6, 4)
4     format ('initmp: Initial h matrix:')
        call matoutmd (n, n - 1, ix, dx, h)
      endif

      return
      end

      subroutine itermp (idb, gam, it, n, ix, dx, b, h, y, izm)

c   This performs one iteration of the PSLQ algorithm using MP arithmetic.

      implicit none
      integer i, idb, im, im1, it, izm, j, j1, k, n, n1, ntl
      parameter (ntl = 72)
      double precision d1, d2, gam, second, tm0, tm1, tm2, tm3, times(5)
      common /timec/ times
      integer ix(n)
      double precision dx(n)
cmp+ multip real  b, h, y
      double precision  b(n,n), h(n,n), y(n)
cmp+ multip real  t1, t2, t3, t4, teps
      double precision  t1, t2, t3, t4, teps

      teps = 2.d0 ** ntl * dpeps
      izm = 0
      tm0 = second ()
      t1 = 0.d0

c   Find  im = i  such that  gam^i * |h(i,i)|  is maximized.

      do i = 1, n - 1
        t2 = gam ** i * abs (h(i,i))
        if (t2 .gt. t1) then
          im = i
          t1 = t2
        endif
      enddo

      im1 = im + 1

c Exchange the im and im+1 entries of y, rows of h, and columns of b.

      t1 = y(im)
      y(im) = y(im1)
      y(im1) = t1

      do i = 1, n
        t1 = b(i,im)
        b(i,im) = b(i,im1)
        b(i,im1) = t1
      enddo

      do i = 1, n - 1
        t1 = h(im,i)
        h(im,i) = h(im1,i)
        h(im1,i) = t1
      enddo

      tm1 = second ()

c  Update h with permutation produced above.

      if (im .le. n - 2) then
        t1 = h(im,im)
        t2 = h(im,im1)
        t3 = sqrt (t1 ** 2 + t2 ** 2)
        t1 = t1 / t3
        t2 = t2 / t3

        do i = im, n
          t3 = h(i,im)
          t4 = h(i,im1)
          h(i,im) = t1 * t3 + t2 * t4
          h(i,im1) = - t2 * t3 + t1 * t4
        enddo
      endif

      tm2 = second ()

      t2 = 0.d0

c  Perform reduction on h, updating y, a, b and h.

      do i = im1, n
        j1 = min (i - 1, im1)

        do j = j1, 1, -1
          t1 = anint (h(i,j) / h(j,j))
          if (t1 .ne. 0.d0) then
            y(j) = y(j) + t1 * y(i)

            do k = 1, n
              b(k,j) = b(k,j) + t1 * b(k,i)
            enddo

            do k = 1, j
              h(i,k) = h(i,k) - t1 * h(j,k)
            enddo
          endif
        enddo
      enddo

      tm3 = second ()

c   Find the min of y.

      t1 = 1d300

      do i = 1, n
        t1 = min (t1, abs (y(i)))
      enddo

      if (t1 .le. teps) then
        if (idb .ge. 2) then
          call decmd (t1, d1, n1)
          write (6, 1) it, d1, n1
1       format ('Iteration',i7,3x,'itermp: Small value in y =',f11.6,
     $  'D',i5)
        endif
        if (t1 .le. dpeps) then
          izm = 1
        else
          if (idb .ge. 1) write (6, 2) it
2       format ('Iteration',i7,3x,'itermp: Precision exhausted.')
          izm = 2
        endif
      endif

      if (idb .ge. 3) then
        write (6, 4)
4     format ('itermp: Updated y:')
        call matoutmd (1, n, ix, dx, y)
        write (6, 5)
5     format ('itermp: Updated b matrix:')
        call matoutmd (n, n, ix, dx, b)
        write (6, 6)
6     format ('itermp: Updated h matrix:')
        call matoutmd (n, n - 1, ix, dx, h)
      endif

      times(3) = times(3) + (tm1 - tm0)
      times(4) = times(4) + (tm2 - tm1)
      times(5) = times(5) + (tm3 - tm2)

      return
      end

c------------------------------

c   Second- and third-level subroutines.

      function bound (n, dh)

c   This computes the norm bound using DP arithmetic.

      implicit none
      integer i, n
      double precision bound, t1, t2
      double precision dh(n,n)

      t1 = 0.d0

      do i = 1, n - 1
        t1 = max (t1, abs (dh(i,i)))
      enddo

      bound = 1.d0 / t1

      return
      end

      subroutine decmd (a, b, ib)

c   For input MP value a, this routine returns DP b and integer ib such that
c   a = b * 10^ib, with 1 <= abs (b) < 10 for nonzero a.

      implicit none
      integer ia, ib
      double precision da, b, t1, xlt
      parameter (xlt = 0.3010299956639812d0)
cmp+ multip real  a
      double precision  a

      call mpmdc (a , da, ia)
      if (da .ne. 0.d0) then
        t1 = xlt * ia + log10 (abs (da))
        ib = t1
        if (t1 .lt. 0.d0) ib = ib - 1
        b = sign (10.d0 ** (t1 - ib), da)
      else
        b = 0.d0
        ib = 0
      endif

      return
      end

      function drand ()

c   This routine returns a pseudorandom DP floating number nearly uniformly
c   distributed between 0 and 1 by means of a linear congruential scheme.
c   2^28 pseudorandom numbers with 30 bits each are returned before repeating.

      double precision drand, f7, r30, sd, t1, t2, t30
      parameter (f7 = 78125.d0, r30 = 0.5d0 ** 30, t30 = 2.d0 ** 30)
      save sd
      data sd/314159265.d0/

      t1 = f7 * sd
      t2 = aint (r30 * t1)
      sd = t1 - t30 * t2
      drand = r30 * sd

      return
      end

      subroutine matoutmd (n1, n2, ix, dx, a)

c   This outputs the MP matrix a as a DP matrix.

      implicit none
      integer i, j, n1, n2
      integer ix(n2)
      double precision dx(n2)
cmp+ multip real  a
      double precision  a(n1,n2)

      do i = 1, n1
        write (6, 1) i
1     format ('Row',i3)

        do j = 1, n2
          call decmd (a(i,j), dx(j), ix(j))
        enddo

        write (6, 2) (dx(j), ix(j), j = 1, n2)
2     format (4(f13.8,'D',i5))
      enddo

      return
      end

      subroutine matoutmp (n1, n2, a)

c   This outputs the MP matrix a.  It may be used in place of calls to matoutmd
c   in the code above if greater accuracy is desired in debug output.

      implicit none
      integer i, j, n1, n2
cmp+ multip real  a
      double precision  a(n1,n2)

      do i = 1, n1
        write (6, 1) i
1     format ('Row',i3)

        do j = 1, n2
          write (6, *) a(i,j)
        enddo
      enddo

      return
      end

cmp+ multip real dmreal
      function dmreal (a)
      double precision a
      dmreal = a
      return
      end

      function mpnwi ()
      call mpinqp ('nw', nw)
      mpnwi = nw
      return
      end
