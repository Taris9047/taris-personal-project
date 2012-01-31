cmp+ precision level 1080
cmp+ epsilon 1e-1060
cmp+ mixed mode fast
cmp+ output precision 56
cmp+ scratch space 5000
      program tpslq2

c   Test program for PSLQ2 -- a two-level implementation of the standard
c   PSLQ algorithm for finding integer relations.  Fortran-90 version.

c   David H. Bailey     1999-09-15

c   These parameters are set in the parameter statement below.  Some other
c   parameters are set in other routines.
c>    dw  = Number of digits per word.  On IEEE systems, dw = 7.22472.
c           On Cray non-IEEE systems, dw = 6.62266.
c     idb = Debug level (0 - 4).
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
c           It is recommended that this be set to at least 50 digits more
c           than the expected detection level of the relation.
c     nep = Log10 of epsilon for detections.  Normally set to 30 - ndp or so.
c           Must not exceed the accuracy of input data.
c     rb  = Max. acceptable norm of relations.  If kq = 1, set to 10.d0 * rm.
c     rm  = Maximum size of random relation coefficients when kq = 1 or 2.

      implicit none
      double precision d1, d2, drand, dw, gam, rb, rm, second, tm, tm0,
     $  tm1
      integer i, idb, idv, iq, itr, j, k, kps, kcs, kq, kr, ks, kss,
     $  n, ndp, nep
      parameter (dw = 7.22472d0, idb = 2, itr = 1, gam = 1.154700538d0,
     $  n = 73, kq = 0, kr = 8, ks = 9, idv = 100, ndp = 1080, nep =
     $  -1060,
     $  rb = 1.d100, rm = 1.d5)
      integer lnm(n)
      character*1 chr0(5*ndp/4+50), chr1(100)
      character*40 nam(n), namx
      double precision dr(n), r1(n)
cmp+ multip real  al, t1, t2, r, x
      double precision  al, t1, t2, r(n), x(n), dmreal

c   This is scratch space for pslq2.  See below.

      integer is0(n)
      double precision s1(6*n*n+3*n)
cmp+ multip real  s2
      double precision  s2(2*n*n+2*n)

      write (6, 1) itr, gam, n, kq, kr, ks, idv, rb, rm, ndp, nep
1     format ('PSLQ2 Test Program'/'No. trials = ',i3,3x,
     $  'Gamma =',f12.9,
     $  3x,'n =',i4,3x,'kq =',i2/'kr =',i2,3x,'ks =',i2,3x,'idv =',i4,
     $  3x,'rb =',1p,d12.4,3x,'rm =',d12.4/'Precision level ndp =',
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

c   Construct a random input vector x without a relation.

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
        call pslq2 (idb, gam, n, rb, x, is0, s1, s2, iq, r)
        tm1 = second ()

c   Output relation, if one was found.

        if (iq .ne. 0) then
          tm = tm + (tm1 - tm0)
          write (6, 4)
4       format ('Recovered relation: 0 =')

          do i = 1, n
            if (r(i) .ne. 0.d0) then
              call mpfform (r(i) , 48, 0, chr0, chr1)
              write (6, '(''+'',48a1,'' * '',a)') (chr1(j), j = 1, 48),
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

c   The following code performs the two-level, standard PSLQ algorithm.
c   David H. Bailey     1999-09-15

      subroutine pslq2 (idb, gam, n, rb, x, is0, s1, s2, iq, r)

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
      double precision s1(6*n*n+3*n)
cmp+ multip real  s2, x, r
      double precision  s2(2*n*n+2*n), x(n), r(n)

      call pslq2x (idb, gam, n, rb, x, is0(1), s1(1), s1(n*n+1),
     $  s1(2*n*n+1),
     $  s1(3*n*n+1), s1(4*n*n+1), s1(5*n*n+1), s1(6*n*n+1),
     $  s1(6*n*n+n+1),
     $  s1(6*n*n+2*n+1), s2(1), s2(n*n+1), s2(2*n*n+1), s2(2*n*n+n+1),
     $  iq, r)
      return
      end

      subroutine pslq2x (idb, gam, n, rb, x, ix, da, db, dh, dsa, dsb,
     $  dsh,
     $  dx, dy, dsy, b, h, t, y, iq, r)

c   The following parameters are set in this routine:
c     ipi = Iteration print interval when idb >= 2.
c     ipm = Iteration check interval for MP iterations; also save interval.
c     itm = Maximum iteration count.  Run is aborted when this is exceeded.

      implicit none
      integer i, idb, ipi, ipm, iq, it, itm, its, izd, izm, j, n, n1, n2
      parameter (ipi = 250, ipm = 10, itm = 1000000)
      double precision bound, d1, d2, d3, gam, rb, rn, second, tm0,
     $  tm1, times(11)
      common /timec/ times
      integer ix(n)
      double precision da(n,n), db(n,n), dh(n,n), dsa(n,n), dsb(n,n),
     $  dsh(n,n), dx(n), dy(n), dsy(n)
cmp+ multip real  b, h, t, r, x, y, t1, t2
      double precision  b(n,n), h(n,n), t(n), r(n), x(n), y(n), t1, t2

c   Initialize.

      if (idb .ge. 2) write (6, 1) n
1     format ('PSLQ2 integer relation detection: n =',i5)
      iq = 0
      it = 0
      rn = 0.d0

      do i = 1, 11
        times(i) = 0.d0
      enddo

      tm0 = second ()
      call initmp (idb, n, ix, dx, b, h, t, x, y, izm)
      tm1 = second ()
      times(1) = tm1 - tm0
      if (idb .ge. 2) write (6, 2)
2     format ('MP initialization complete.')
      if (izm .ne. 0) goto 140

100   continue

c   Initialize DP arrays from MP arrays.

      tm0 = second ()
      call initdp (idb, it, n, da, db, dh, dy, h, y, izd)
      call savedp (n, da, db, dh, dy, dsa, dsb, dsh, dsy)
      tm1 = second ()
      times(2) = times(2) + (tm1 - tm0)
      its = it

      if (izd .eq. 0) then

c   Perform an LQ decomposition on dh, prior to DP iterations.

        call lqdp (n, n - 1, dh)

c   Perform DP iterations.

110   continue
        it = it + 1
        if (idb .ge. 3 .or. idb .ge. 2 .and. mod (it, ipi) .eq. 0)
     $  write (6, 3) it
3     format ('Iteration',i7)
        tm0 = second ()
        call iterdp (idb, gam, it, n, ix, da, db, dh, dy, izd)
        tm1 = second ()
        times(3) = times(3) + (tm1 - tm0)

        if (izd .eq. 0) then
          if (mod (it - its, ipm) .eq. 0) then
            call savedp (n, da, db, dh, dy, dsa, dsb, dsh, dsy)
            its = it
          endif
          goto 110
        else

c   Update MP arrays from the DP arrays.

          if (izd .eq. 2) then

c   DP iteration was aborted -- revert to previous data.

            it = its
            call savedp (n, dsa, dsb, dsh, dsy, da, db, dh, dy)
          endif
          tm0 = second ()
          call updtmp (idb, it, n, ix, dx, da, db, b, h, t, y, izm)
          tm1 = second ()
          times(4) = times(4) + (tm1 - tm0)

c   Compute norm bound.

          do j = 1, n - 1
            do i = 1, n
              dh(i,j) = h(i,j)
            enddo
          enddo

          call lqdp (n, n - 1, dh)
          d3 = bound (n, dh)
          rn = max (rn, d3)
          if (idb .ge. 2) then
            write (6, 4) it, d3, rn
4         format ('Iteration',i7,3x,'Norm bound =',1p,d15.6,4x,
     $  'Max. bound =',
     $        1p,d15.6)
          endif
          if (it .gt. itm) then
            if (idb .ge. 1) write (6, 5) itm
5         format ('Iteration limit exceeded',i7)
            goto 150
          endif
          if (rn .gt. rb) then
            if (idb .ge. 1) write (6, 6) rb
6         format ('Norm bound limit exceeded.',1p,d15.6)
            goto 150
          endif
          if (izm .eq. 0) then
            if (izd .eq. 2) then
              goto 120
            else
              goto 100
            endif
          elseif (izm .eq. 1) then
            goto 140
          elseif (izm .eq. 2) then
            goto 150
          endif
        endif
      endif

120   continue

c   Perform an LQ decomposition on h, prior to MP iterations.

      call lqmp (n, n - 1, h)

c   Perform MP iterations.

      if (idb .ge. 2 .and. it .eq. its) write (6, 7) it
7     format ('Iteration',i7,3x,'Start MP iterations')

130   continue
      it = it + 1
      if (idb .eq. 3 .or. idb .ge. 2 .and. mod (it, ipi) .eq. 0) write
     $  (6, 3) it
      tm0 = second ()
      call itermp (idb, gam, it, n, ix, dx, b, h, y, izm)
      tm1 = second ()
      times(5) = times(5) + (tm1 - tm0)

      if (izm .eq. 0) then
        if (mod (it - its, ipm) .eq. 0) then

c   Check to see if DP iterations can be resumed.

          call check (idb, it, n, h, y, izd)
          if (izd .eq. 0) then
            if (idb .ge. 2) write (6, 8) it
8         format ('Iteration',i7,3x,'Return to DP iterations')
            goto 100
          endif
        endif
        goto 130
      elseif (izm .eq. 1) then
        goto 140
      elseif (izm .eq. 2) then
        goto 150
      endif

c   A relation has been detected.  Output the final norm bound and other info.

140   continue

      tm0 = second ()
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

        call lqdp (n, n - 1, dh)
        d3 = bound (n, dh)
        rn = max (rn, d3)
        write (6, 9) it, d1, n1, d2, n2, rn
9     format ('Relation detected.',4x,'No. iterations =',i7/
     $  'Min, max of y =',0p,f11.6,'D',i5,f11.6,'D',i5/
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
            write (6, 10) j, d1, d2, n2
10       format ('Index of relation =',i4,3x,'Norm =',1p,d15.6,3x,
     $      'Residual =',0p,f11.6,'D',i5)
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
        if (idb .ge. 2) write (6, 11)
11    format ('Relation is too large.')
      endif

150   continue
      if (idb .ge. 2) write (6, 12) times
12    format ('CPU times:'/(5f12.2))

      return
      end

c------------------------------

c   First-level subroutines.

      subroutine check (idb, it, n, h, y, izd)

c   This checks h and y to see if DP iterations can be resumed.

      implicit none
      integer i, idb, it, izd, n, n1, nws
      double precision d1, deps
      parameter (deps = 1d-10)
cmp+ multip real  h, y, t1, t2, t3, t4
      double precision  h(n,n), y(n), t1, t2, t3, t4
      integer mpnwi

      nws = mpnwi ()
      call mpsetp ('nw',  10)
      izd = 0
      t1 = 1.d300
      t2 = 0.d0

c   Find the min and max absolute value in the y vector.

      do i = 1, n
        t1 = min (t1, abs (y(i)))
        t2 = max (t2, abs (y(i)))
      enddo

      t3 = t1 / t2
      t4 = deps
      if (t3 .lt. t4) then
        if (idb .ge. 2) then
          call decmd (t3, d1, n1)
          write (6, 1) it, d1, n1
1       format ('Iteration',i7,3x,
     $  'check: Small min/max ratio in y =',f11.6,
     $    'D',i5)
        endif
        izd = 1
      call mpsetp ('nw',  nws)
        return
      endif

      call mpsetp ('nw',  nws)
      return
      end

      subroutine initdp (idb, it, n, da, db, dh, dy, h, y, izd)

c   This re-initializes the DP arrays from the MP arrays.

      implicit none
      integer i, idb, it, izd, j, n, n1, nws
      double precision da(n,n), db(n,n), dh(n,n), dy(n), d1, deps
cmp+ multip real  h, y, t1, t2, t3, t4
      double precision  h(n,n), y(n), t1, t2, t3, t4
      parameter (deps = 1d-10)
      integer mpnwi

      nws = mpnwi ()
      call mpsetp ('nw',  10)
      izd = 0
      t1 = 1.d300
      t2 = 0.d0

c   Find the min and max absolute value in the y vector.

      do i = 1, n
        t1 = min (t1, abs (y(i)))
        t2 = max (t2, abs (y(i)))
      enddo

c   If the dynamic range of the y vector is too great, set the izd flag and
c   abort this initialization.

      t3 = t1 / t2
      t4 = deps
      if (t3 .lt. t4) then
        if (idb .ge. 3) then
          call decmd (t3, d1, n1)
          write (6, 1) it, d1, n1
1       format ('Iteration',i7,3x,
     $  'initdp: Small min/max ratio in y =',f11.6,
     $    'D',i5)
        endif
        izd = 1
      call mpsetp ('nw',  nws)
        return
      endif

c   Set dy to be the scaled y vector.

      t1 = 1.d0 / t2

      do i = 1, n
        dy(i) = t1 * y(i)
      enddo

c   Find the maximum absolute value of the h matrix diagonals.

      t2 = 0.d0

      do j = 1, n - 1
        t2 = max (t2, abs (h(j,j)))
      enddo

c   Set dh to be the scaled h matrix.

      t1 = 1.d0 / t2

      do j = 1, n - 1
        do i = 1, n
          dh(i,j) = t1 * h(i,j)
        enddo
      enddo

c   Set da and db to the identity.

      do j = 1, n
        do i = 1, n
          da(i,j) = 0.d0
          db(i,j) = 0.d0
        enddo

        da(j,j) = 1.d0
        db(j,j) = 1.d0
      enddo

      if (idb .ge. 4) then
        write (6, 2)
2     format ('initdp: Scaled dy vector:')
        call matoutdp (1, n, dy)
        write (6, 3)
3     format ('initdp: Scaled dh matrix:')
        call matoutdp (n, n - 1, dh)
      endif

      call mpsetp ('nw',  nws)
      return
      end

      subroutine initmp (idb, n, ix, dx, b, h, s, x, y, izm)

c   This initializes MP arrays at the beginning.

      implicit none
      integer i, idb, izm, j, k, n, n1
      double precision d1
      integer ix(n)
      double precision dx(n)
cmp+ multip real  b, h, s, x, y, t1
      double precision  b(n,n), h(n,n), s(n), x(n), y(n), t1

      if (idb .ge. 4) then
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

      if (idb .ge. 4) then
        write (6, 3)
3     format ('initmp: Initial y vector:')
        call matoutmd (1, n, ix, dx, y)
        write (6, 4)
4     format ('initmp: Initial h matrix:')
        call matoutmd (n, n - 1, ix, dx, h)
      endif

      return
      end

      subroutine iterdp (idb, gam, it, n, ix, da, db, dh, dy, izd)

c   This performs one iteration of the PSLQ algorithm using DP arithmetic.

      implicit none
      integer i, idb, im, im1, it, izd, j, j1, k, n, n1
      double precision deps, gam, t1, t2, t3, t4, tmx1, tmx2, second,
     $  tm0, tm1,
     $  tm2, tm3, times(11)
      common /timec/ times
      integer ix(n)
      double precision da(n,n), db(n,n), dh(n,n), dy(n)
c>
c   On IEEE systems, use the first line. On Cray non-IEEE systems, use
c   the second.

      parameter (tmx1 = 1.d13, tmx2 = 2.d0**52, deps = 1.d-14)
c parameter (tmx1 = 2.d11, tmx2 = 2.d0**47, deps = 2.d-13)

      izd = 0
      tm0 = second ()
      t1 = 0.d0

c   Find  im = i  such that  gam^i * |dh(i,i)|  is maximized.

      do i = 1, n - 1
        t2 = gam ** i * abs (dh(i,i))
        if (t2 .gt. t1) then
          im = i
          t1 = t2
        endif
      enddo

      im1 = im + 1

c Exchange the im and im+1 entries of dy, rows of da and dh, and columns of db.

      t1 = dy(im)
      dy(im) = dy(im1)
      dy(im1) = t1

      do i = 1, n
        t1 = da(im,i)
        da(im,i) = da(im1,i)
        da(im1,i) = t1
        t1 = db(i,im)
        db(i,im) = db(i,im1)
        db(i,im1) = t1
      enddo

      do i = 1, n - 1
        t1 = dh(im,i)
        dh(im,i) = dh(im1,i)
        dh(im1,i) = t1
      enddo

      tm1 = second ()

c  Update dh with permutation produced above.

      if (im .le. n - 2) then
        t1 = dh(im,im)
        t2 = dh(im,im1)
        t3 = sqrt (t1 ** 2 + t2 ** 2)
        t1 = t1 / t3
        t2 = t2 / t3

        do i = im, n
          t3 = dh(i,im)
          t4 = dh(i,im1)
          dh(i,im) = t1 * t3 + t2 * t4
          dh(i,im1) = - t2 * t3 + t1 * t4
        enddo
      endif

      tm2 = second ()
      t2 = 0.d0

c  Perform reduction on dh, updating dy, da, db and dh.

      do i = im1, n
        j1 = min (i - 1, im1)

        do j = j1, 1, -1
          t1 = anint (dh(i,j) / dh(j,j))
          if (t1 .ne. 0.d0) then
            dy(j) = dy(j) + t1 * dy(i)

            do k = 1, n
              da(i,k) = da(i,k) - t1 * da(j,k)
              db(k,j) = db(k,j) + t1 * db(k,i)
              t2 = max (t2, abs (da(i,k)))
              t2 = max (t2, abs (db(k,j)))
            enddo

            do k = 1, j
              dh(i,k) = dh(i,k) - t1 * dh(j,k)
            enddo
          endif
        enddo
      enddo

      tm3 = second ()

c   Find the min absolute value entry of dy.

      t1 = 1d300

      do i = 1, n
        t1 = min (t1, abs (dy(i)))
      enddo

      if (t1 .le. deps) then
        if (idb .ge. 3) write (6, 1) it, t1
1     format ('Iteration',i7,3x,'iterdp: Small value in dy =',1pd15.6)
        izd = 1
      endif

      if (t2 .gt. tmx1 .and. t2 .le. tmx2) then
        if (idb .ge. 3) write (6, 2) it, t2
2     format ('Iteration',i7,3x,
     $  'iterdp: Large value in da or db =',1pd15.6)
        izd = 1
      elseif (t2 .gt. tmx2) then
        if (idb .ge. 2) write (6, 3) it, t2
3     format ('Iteration',i7,3x,
     $  'iterdp: Very large value in da or db =',1pd15.6)
        izd = 2
        return
      endif

      if (idb .ge. 4) then
        write (6, 5)
5     format ('iterdp: Updated dy:')
        call matoutdp (1, n, dy)
        write (6, 6)
6     format ('iterdp: Updated da matrix:')
        call matoutdp (n, n, da)
        write (6, 7)
7     format ('iterdp: Updated db matrix:')
        call matoutdp (n, n, db)
        write (6, 8)
8     format ('iterdp: Updated dh matrix:')
        call matoutdp (n, n - 1, dh)
      endif

      times(6) = times(6) + (tm1 - tm0)
      times(7) = times(7) + (tm2 - tm1)
      times(8) = times(8) + (tm3 - tm2)

      return
      end

      subroutine itermp (idb, gam, it, n, ix, dx, b, h, y, izm)

c   This performs one iteration of the PSLQ algorithm using MP arithmetic.

      implicit none
      integer i, idb, im, im1, it, izm, j, j1, k, n, n1, ntl
      parameter (ntl = 72)
      double precision d1, d2, gam, second, tm0, tm1, tm2, tm3,
     $  times(11)
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

      times(9) = times(9) + (tm1 - tm0)
      times(10) = times(10) + (tm2 - tm1)
      times(11) = times(11) + (tm3 - tm2)

      return
      end

      subroutine lqdp (n, m, dh)

c   This performs an LQ decomposition on the DP matrix dh.  It is a simplified
c   and transposed adaptation of the subroutine dqrdc from Linpack.

      implicit none
      integer i, j, jp, l, lup, m, ml, n
      double precision dh(n,m), nrmxl, one, t, zero

      zero = 0.d0
      one = 1.d0
      lup = min (m,n)

c   Perform the householder reduction of dh.

      do l = 1, lup
        if (l .eq. m) go to 280

c   Compute the householder transformation for column l.

        ml = m - l
        t = zero

        do i = 0, ml
          t = t + dh(l,l+i) ** 2
        enddo

        nrmxl = sqrt (t)
        if (nrmxl .eq. zero) go to 270
        if (dh(l,l) .ne. zero) nrmxl = sign (nrmxl, dh(l,l))
        t = one / nrmxl

        do i = 0, ml
          dh(l,l+i) = t * dh(l,l+i)
        enddo

        dh(l,l) = one + dh(l,l)

c   Apply the transformation to the remaining columns, updating the norms.

        do j = l + 1, n
          t = zero

          do i = 0, ml
            t = t + dh(l,l+i) * dh(j,l+i)
          enddo

          t = - t / dh(l,l)

          do i = 0, ml
            dh(j,l+i) = dh(j,l+i) + t * dh(l,l+i)
          enddo
        enddo

c   Save the transformation.

        dh(l,l) = - nrmxl
270   continue
280   continue
      enddo

c   Zero dh above the diagonal.

      do j = 1, m
        do i = 1, j - 1
          dh(i,j) = 0.d0
        enddo
      enddo

      return
      end

      subroutine lqmp (n, m, h)

c   This performs an LQ decomposition on the MP matrix h.  It is a simplified
c   and transposed adaptation of the subroutine dqrdc from Linpack.

      implicit none
      integer i, j, jp, l, lup, m, ml, n
cmp+ multip real  h, nrmxl, one, t, zero
      double precision  h(n,m), nrmxl, one, t, zero

      zero = 0.d0
      one = 1.d0
      lup = min (m,n)

c   Perform the householder reduction of h.

      do l = 1, lup
        if (l .eq. m) go to 280

c   Compute the householder transformation for column l.

        ml = m - l
        t = zero

        do i = 0, ml
          t = t + h(l,l+i) ** 2
        enddo

        nrmxl = sqrt (t)
        if (nrmxl .eq. zero) go to 270
        if (h(l,l) .ne. zero) nrmxl = sign (nrmxl, h(l,l))
        t = one / nrmxl

        do i = 0, ml
          h(l,l+i) = t * h(l,l+i)
        enddo

        h(l,l) = one + h(l,l)

c   Apply the transformation to the remaining columns, updating the norms.

        do j = l + 1, n
          t = zero

          do i = 0, ml
            t = t + h(l,l+i) * h(j,l+i)
          enddo

          t = - t / h(l,l)

          do i = 0, ml
            h(j,l+i) = h(j,l+i) + t * h(l,l+i)
          enddo
        enddo

c   Save the transformation.

        h(l,l) = - nrmxl
270   continue
280   continue
      enddo

c   Zero h above the diagonal.

      do j = 1, m
        do i = 1, j - 1
          h(i,j) = 0.d0
        enddo
      enddo

      return
      end

      subroutine savedp (n, da, db, dh, dy, dsa, dsb, dsh, dsy)

c   This saves the arrays dy, da, db, dh in case dp iterations must be aborted.
c   A call to the same routine, with (da,db,dh,dy) and (dsa,dsb,dsh,dsy)
c   exchanged, serves to restore these arrays.

      implicit none
      integer i, j, n
      double precision da(n,n), db(n,n), dh(n,n), dy(n), dsa(n,n),
     $  dsb(n,n),
     $  dsh(n,n), dsy(n)

      do i = 1, n
        dsy(i) = dy(i)
      enddo

      do j = 1, n
        do i = 1, n
          dsa(i,j) = da(i,j)
          dsb(i,j) = db(i,j)
        enddo
      enddo

      do j = 1, n - 1
        do i = 1, n
          dsh(i,j) = dh(i,j)
        enddo
      enddo

      return
      end

      subroutine updtmp (idb, it, n, ix, dx, da, db, b, h, t, y, izm)

c   This updates the MP arrays from the DP arrays.

      implicit none
      integer i, idb, it, izm, j, n, n1, n2, ntl
      parameter (ntl = 72)
      integer ix(n)
      double precision da(n,n), db(n,n), dx(n), d1, d2
cmp+ multip real  b, h, t, y, t1, t2, teps
      double precision  b(n,n), h(n,n), t(n), y(n), t1, t2, teps

      if (idb .ge. 3) write (6, 1) it
1     format ('Iteration',i7,3x,'updtmp: MP update from DP arrays.')
      teps = 2.d0 ** ntl * dpeps
      izm = 0
      t1 = 1.d300
      t2 = 0.d0

c   Update y with db.

      call mxmdm2 (1, n, y, db, t)

      do i = 1, n
        t1 = min (t1, abs (y(i)))
        t2 = max (t2, abs (y(i)))
      enddo

      if (idb .ge. 2) then
        call decmd (t1, d1, n1)
        call decmd (t2, d2, n2)
        write (6, 2) it, d1, n1, d2, n2
2      format ('Iteration',i7,3x,'updtmp: Min, max of y =',f11.6,'D',i5,
     $     f11.6,'D',i5)
      endif
      if (t1 .le. teps) then
        if (idb .ge. 2) then
          call decmd (t1, d1, n1)
          write (6, 3) it, d1, n1
3       format ('Iteration',i7,3x,'updtmp: Small value in y =',f11.6,
     $  'D',i5)
        endif
        if (t1 .le. dpeps) then
          izm = 1
        else
          if (idb .ge. 1) write (6, 4) it
4       format ('Iteration',i7,3x,'updtmp: Precision exhausted.')
          izm = 2
        endif
      endif

c   Update b with db.

      call mxmdm2 (n, n, b, db, t)

c   Update h with da.

      call mxmdm1 (n, n - 1, da, h, t)

      if (idb .ge. 4) then
        write (6, 5)
5     format ('updtmp: Updated y:')
        call matoutmd (1, n, ix, dx, y)
        write (6, 6)
6     format ('updtmp: Updated b matrix:')
        call matoutmd (n, n, ix, dx, b)
        write (6, 7)
7     format ('updtmp: Updated h matrix:')
        call matoutmd (n, n - 1, ix, dx, h)
      endif

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

      implicit none
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

      subroutine matoutdp (n1, n2, a)

c   This outputs the DP matrix a.

      implicit none
      integer i, j, n1, n2
      double precision a(n1,n2)

      do i = 1, n1
        write (6, 1) i
1     format ('Row',i3)
        write (6, 2) (a(i,j), j = 1, n2)
2     format (1p5d15.5)
      enddo

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

      subroutine mxmdm1 (n1, n2, a, b, c)

c  This multiplies the DP square matrix a by the MP matrix b, and the result
c  is placed in b.  c is a MP scratch array with n1 cells.  n1, n2 are
c  the matrix dimensions as indicated below.

      implicit none
      integer i, j, mw4, n1, n2
      double precision a(n1,n1)
cmp+ multip real  b, c, t1, t2
      double precision  b(n1,n2), c(n1), t1, t2

      mw4 = mpnwp + 4

      do j = 1, n2
        do i = 1, n1
          call mpdotd (n1, mw4, b(1,j) , n1, a(i,1), t1 )
          c(i) = t1
        enddo

        do i = 1, n1
          b(i,j) = c(i)
        enddo
      enddo

      return
      end

      subroutine mxmdm2 (n1, n2, a, b, c)

c  This multiplies the MP matrix a by the DP square matrix b, and the result
c  is placed in a.  c is a MP scratch array with n2 cells.  n1, n2 are
c  the matrix dimensions as indicated below.

      implicit none
      integer i, j, mw4, n1, n2
      double precision b(n2,n2)
cmp+ multip real  a, c, t1, t2
      double precision  a(n1,n2), c(n2), t1, t2

      mw4 = mpnwp + 4

      do i = 1, n1
        do j = 1, n2
          call mpdotd (n2, n1*mw4, a(i,1) , 1, b(1,j), t1 )
          c(j) = t1
        enddo

        do j = 1, n2
          a(i,j) = c(j)
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
