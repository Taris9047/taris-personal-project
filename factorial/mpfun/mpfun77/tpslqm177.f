cmp+ precision level 320
cmp+ epsilon 1e-300
cmp+ mixed mode fast
cmp+ output precision 56
cmp+ scratch space 5000
      program tpslqm1

c   Test program for PSLQM1 -- a one-level implementation of the multi-pair
c   PSLQ algorithm for finding integer relations.  Fortran-90 OMP version.

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
c     nsq = Size of duplicate table in iterdp and itermpw.
c     rb  = Max. acceptable norm of relations.  If kq = 1, set to 10.d0 * rm.
c     rm  = Maximum size of random relation coefficients when kq = 1 or 2.

      implicit none
      double precision d1, d2, drand, dw, gam, rb, rm, second, tm, tm0,
     $  tm1
      integer i, idb, idv, iq, itr, j, k, kps, kcs, kq, kr, ks, kss,
     $  n, ndp, nep, nsq
      parameter (dw = 7.22472d0, idb = 2, itr = 1, gam = 1.154700538d0,
     $  n = 37, nsq = 8, kq = 0, kr = 6, ks = 6, idv = 100,
     $  ndp = 320, nep = -300, rb = 1.d100, rm = 1.d5)
      integer lnm(n)
      character*1 chr0(5*ndp/4+50), chr1(100)
      character*40 nam(n), namx
      double precision dr(n), r1(n)
cmp+ multip real  al, t1, t2, r, x
      double precision  al, t1, t2, r(n), x(n), dmreal

c   This is scratch space for pslqm1.  See below.

      integer is0(3*n)
      double precision s1(2*n*n)
cmp+ multip real  s2
      double precision  s2(3*n*n+nsq*n+2*n)

      write (6, 1) itr, gam, n, nsq, kq, kr, ks, idv, rb, rm, ndp, nep
1     format ('PSLQM1 Test Program'/'No. trials = ',i3,3x,
     $  'Gamma =',f12.9,
     $  3x,'n =',i4,3x,'nsq =',i3,3x,'kq =',i2/'kr =',i2,3x,
     $  'ks =',i2,3x,
     $  'idv =',i4,3x,'rb =',1p,d12.4,3x,'rm =',d12.4/
     $  'Precision level ndp =',
     $  i6,' digits',3x,'Epsilon level nep = ',i6)
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
        call pslqm1 (idb, gam, n, nsq, rb, x, is0, s1, s2, iq, r)
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

c   The following code performs the one-level, multi-pair PSLQ algorithm.
c   David H. Bailey     1999-09-15

      subroutine pslqm1 (idb, gam, n, nsq, rb, x, is0, s1, s2, iq, r)

c   This routine allocates the scratch space arrays is0, s1 and s2.  Handling
c   scratch space in this manner is not really necessary in Fortran-90, but
c   this design facilitates straightforward translation to Fortran-77.

c   Arguments are as follows:
c     idb = Debug flag: 0 (none) to 4 (full).
c     gam = PSLQ gamma parameter, normally set to sqrt(4/3).
c     n   = Dimension of input vector.
c     nsq = Size of tables used in itermpw.
c     rb  = Maximum size (Euclidean norm) of acceptable relation (type DP).
c     x   = Input real vector (type mp_real).
c     is0 = Scratch array (type integer -- see size below).
c     s1  = Scratch array (type double precision -- see size below).
c     s2  = Scratch array (type mp_real -- see size below).
c     iq  = Output flag: 0 (unsuccessful) or 1 (successful).
c     r   = Output integer coefficient vector (type mp_real).

      implicit none
      integer idb, iq, n, nsq
      double precision gam, rb
      integer is0(3*n)
      double precision s1(n*n)
cmp+ multip real  s2, x, r
      double precision  s2(3*n*n+nsq*n+2*n), x(n), r(n)

      call pslqm1x (idb, gam, n, nsq, rb, x, is0(1), is0(n+1),
     $  is0(2*n+1),
     $  s1(1), s1(n*n+1), s2(1), s2(n*n+1), s2(2*n*n+1), s2(3*n*n+1),
     $  s2(3*n*n+nsq*n+1), s2(3*n*n+nsq*n+n+1), iq, r)
      return
      end

      subroutine pslqm1x (idb, gam, n, nsq, rb, x, ip, ir, is, dh, dx,
     $  b, h, t,
     $  syq, s, y, iq, r)

c   The following parameters are set in this routine:
c     ipi = Iteration print interval when idb >= 2.
c     ipm = Iteration check interval for MP iterations.
c     itm = Maximum iteration count.  Run is aborted when this is exceeded.

      implicit none
      integer i, idb, imq, ipi, ipm, iq, it, itm, izm, j, n, n1, n2, nsq
      parameter (ipi = 25, ipm = 100, itm = 100000)
      double precision bound, d1, d2, d3, gam, rb, rn, second, tm0,
     $  tm1, times(7)
      common /timec/ times
      integer ip(n), ir(n), is(n)
      double precision dh(n,n), dx(n)
cmp+ multip real  b, h, s, syq, t, r, x, y, t1, t2
      double precision  b(n,n), h(n,n), s(n), syq(n,nsq), t(n,n), r(n),
     $  x(n),
     $  y(n), t1, t2

c   Initialize.

      if (idb .ge. 2) write (6, 1) n
1     format ('PSLQM1 integer relation detection: n =',i5)
      iq = 0
      it = 0
      imq = 0
      rn = 0.d0

      do i = 1, 7
        times(i) = 0.d0
      enddo

      tm0 = second ()
      call initmp (idb, n, nsq, ip, dx, b, h, s, syq, x, y)
      tm1 = second ()
      times(1) = tm1 - tm0
      if (idb .ge. 2) write (6, 2)
2     format ('MP initialization complete.')

c   Perform MP iterations.

100   continue
      it = it + 1
      if (idb .eq. 3 .or. idb .ge. 2 .and. mod (it, ipi) .eq. 0) write
     $  (6, 3) it
3     format ('Iteration',i7)
      tm0 = second ()
      call itermp (idb, gam, it, n, nsq, ip, ir, is, dx, b, h, s, syq,
     $  t, y,
     $  imq, izm)
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
            d1 = d1 + dble (b(j,i)) ** 2
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
              r(i) = b(j,i)
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

      subroutine initmp (idb, n, nsq, ix, dx, b, h, s, syq, x, y)

c   This initializes MP arrays at the beginning.

      implicit none
      integer i, idb, j, n, nsq
      double precision d1
      integer ix(n)
      double precision dx(n)
cmp+ multip real  b, h, s, syq, x, y, t1
      double precision  b(n,n), h(n,n), s(n), syq(n,nsq), x(n), y(n), t1

      if (idb .ge. 3) then
        write (6, 1)
1     format ('initmp: Input x vector:')
        call matoutmd (1, n, ix, dx, x)
      endif

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

c$omp parallel do private (i, j, t1) copyin (mpnw)
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
c$omp end parallel do

c   Zero the syq array.

      do j = 1, nsq
        do i = 1, n
          syq(i,j) = 0.d0
        enddo
      enddo

      if (idb .ge. 3) then
        write (6, 2)
2     format ('initmp: Initial y vector:')
        call matoutmd (1, n, ix, dx, y)
        write (6, 3)
3     format ('initmp: Initial h matrix:')
        call matoutmd (n, n - 1, ix, dx, h)
      endif

      return
      end

      subroutine itermp (idb, gam, it, n, nsq, ip, ir, is, dx, b, h, q,
     $  syq, t,
     $  y, imq, izm)

c   This performs one iteration of the PSLQM algorithm using MP arithmetic.

      implicit none
      integer i, idb, ii, ij, im, im1, imq, it, izm, j, j1, j2, k, mpr,
     $  mq, n, n1,
     $  nsq, ntl
      parameter (ntl = 72)
      double precision d1, d2, gam, second, tm0, tm1, tm2, tm3, tm4,
     $  tm5, tm6,
     $  times(7)
      common /timec/ times
      integer ip(n), ir(n), is(n)
      double precision dx(n)
cmp+ multip real  b, h, q, syq, t, y
      double precision  b(n,n), h(n,n), q(n), syq(n,nsq), t(n,n), y(n)
cmp+ multip real  t1, t2, t3, t4, teps
      double precision  t1, t2, t3, t4, teps

      teps = 2.d0 ** ntl * dpeps
      izm = 0
      mpr = nint (0.4d0 * n)
      tm0 = second ()

c   Compute q vector = {gam^i * |h(i,i)|}, then sort in ascending order.

      do i = 1, n - 1
        q(i) = gam ** i * abs (h(i,i))
      enddo

      call qsortmp (n - 1, q, ip)

c   Select up to mpr disjoint pairs of indices (m,m+1), where m is an index
c   from the list of the largest q(i).

      do i = 1, n
        is(i) = 0
      enddo

      if (imq .eq. 0) then
        mq = mpr
      else
        mq = 1
        imq = 0
      endif
      ii = n

      do i = 1, mq
100   continue
        ii = ii - 1
        if (ii .eq. 0) then
          mq = i - 1
          goto 110
        endif
        j1 = ip(ii)
        j2 = j1 + 1
        if (is(j1) .ne. 0 .or. is(j2) .ne. 0) goto 100
        ir(i) = j1
        is(j1) = 1
        is(j2) = 1
      enddo

110   continue

c   Exchange the pairs of entries of y, and rows of b and h.

c$omp parallel do private (i, j, im, im1, t1) copyin (mpnw)
      do j = 1, mq
        im = ir(j)
        im1 = im + 1
        t1 = y(im)
        y(im) = y(im1)
        y(im1) = t1

        do i = 1, n
          t1 = b(im,i)
          b(im,i) = b(im1,i)
          b(im1,i) = t1
        enddo

        do i = 1, n - 1
          t1 = h(im,i)
          h(im,i) = h(im1,i)
          h(im1,i) = t1
        enddo
      enddo
c$omp end parallel do

      tm1 = second ()

c   Eliminate the "corners" produced by the above permutation in h.

c$omp parallel do private (i, j, im, im1, t1, t2, t3, t4) copyin (mpnw)
      do j = 1, mq
        im = ir(j)
        im1 = im + 1
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
      enddo
c$omp end parallel do

      tm2 = second ()

c   Perform reduction on h, using the diagonal scheme.  Multipliers are
c   saved in the t array.

      do i = 2, n
c$omp parallel do private (ij, j, k) copyin (mpnw)
        do j = 1, n - i + 1
          ij = i + j - 1

          do k = j + 1, ij - 1
            h(ij,j) = h(ij,j) - t(ij,k) * h(k,j)
          enddo

          t(ij,j) = anint (h(ij,j) / h(j,j))
          h(ij,j) = h(ij,j) - t(ij,j) * h(j,j)
        enddo
c$omp end parallel do
      enddo

      tm3 = second ()

c   Update y, using the t array.  Find min absolute value of y.

      t1 = abs (y(n))

      do j = 1, n - 1
        do i = j + 1, n
          y(j) = y(j) + t(i,j) * y(i)
        enddo

        t1 = min (t1, abs (y(j)))
      enddo

c   Update b, using the t array.

c$omp parallel do private (i, j, k) copyin (mpnw)
      do k = 1, n
        do j = 1, n - 1
          do i = j + 1, n
            b(j,k) = b(j,k) + t(i,j) * b(i,k)
          enddo
        enddo
      enddo
c$omp end parallel do

      tm4 = second ()

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

c   Compare the y vector with those of recent iterations.  If a duplicate is
c   found, then the next iteration must be performed with mq = 1.

      do j = 1, nsq
        t1 = 0.d0

        do i = 1, n
          t1 = max (t1, abs (y(i) - syq(i,j)))
        enddo

        if (t1 .le. teps) then
          if (idb .ge. 2) write (6, 3) it, j
 3     format ('Iteration',i7,3x,'itermp: Duplicate found, j =',i6)
          imq = 1
          goto 120
        endif
      enddo

c   Place the vector y in the table syq.

120     continue
      k = 1 + mod (it, nsq)

      do i = 1, n
        syq(i,k) = y(i)
      enddo

      if (idb .ge. 3) then
        write (6, 4)
4     format ('itermp: Updated y:')
        call matoutmd (1, n, ip, dx, y)
        write (6, 5)
5     format ('itermp: Updated b matrix:')
        call matoutmd (n, n, ip, dx, b)
        write (6, 6)
6     format ('itermp: Updated h matrix:')
        call matoutmd (n, n - 1, ip, dx, h)
      endif

      tm5 = second ()
      times(3) = times(3) + (tm1 - tm0)
      times(4) = times(4) + (tm2 - tm1)
      times(5) = times(5) + (tm3 - tm2)
      times(6) = times(6) + (tm4 - tm3)
      times(7) = times(7) + (tm5 - tm4)

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

      subroutine qsortmp (n, a, ip)

c   This routine sorts the entries of the N-long MP vector A into ascending
c   order using the quicksort algorithm.  The permutation vector that would
c   sort the vector is returned in IP.

      implicit none
      integer i, iq, it, j, jq, jz, k, l, n
      integer ip(n), ik(50), jk(50)
cmp+ multip real  a, s0, s1, s2
      double precision  a(n), s0, s1, s2

      do i = 1, n
        ip(i) = i
      enddo

      if (n .eq. 1) return

      k = 1
      ik(1) = 1
      jk(1) = n

130   i = ik(k)
      j = jk(k)
      iq = i
      jq = j
      it = (i + j + 1) / 2
      l = ip(j)
      ip(j) = ip(it)
      ip(it) = l
      s0 = a(ip(j))
      j = j - 1

140   continue

      do l = i, j
        if (s0 .lt. a(ip(l))) goto 160
      enddo

      i = j
      goto 190

160   i = l

      do l = j, i, -1
        if (s0 .gt. a(ip(l))) goto 180
      enddo

      j = i
      goto 190

180   j = l
      if (i .ge. j) goto 190
      l = ip(i)
      ip(i) = ip(j)
      ip(j) = l
      goto 140

190   continue
      if (s0 .ge. a(ip(i))) goto 200
      l = ip(jq)
      ip(jq) = ip(i)
      ip(i) = l

200   k = k - 1
      jz = 0
      if (j .eq. iq) goto 210
      k = k + 1
      jk(k) = j
      jz = 1

210   i = i + 1
      if (i .eq. jq) goto 220
      k = k + 1
      ik(k) = i
      jk(k) = jq
      if (jz .eq. 0) goto 220
      if (j - iq .ge. jq - i) goto 220
      ik(k-1) = i
      jk(k-1) = jq
      ik(k) = iq
      jk(k) = j

220   if (k .gt. 0) goto 130

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
