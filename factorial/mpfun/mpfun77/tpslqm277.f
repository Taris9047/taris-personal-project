cmp+ precision level 1040
cmp+ epsilon 1e-1010
cmp+ mixed mode fast
cmp+ output precision 56
cmp+ scratch space 5000
      program tpslqm2

c   Test program for PSLQM2 -- a two-level implementation of the multi-pair
c   PSLQ algorithm for finding integer relations.  Fortran-90 OMP version.

c   David H. Bailey     1999-09-16

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
c     nsq = Size of duplicate table in iterdp and itermp.  Typically nsq = 8.
c     rb  = Max. acceptable norm of relations.  If kq = 1, set to 10.d0 * rm.
c     rm  = Maximum size of random relation coefficients when kq = 1 or 2.

      implicit none
      double precision d1, d2, drand, dw, gam, rb, rm, second, tm, tm0,
     $  tm1
      integer i, idb, idv, iq, itr, j, k, kps, kcs, kq, kr, ks, kss,
     $  n, ndp, nep, nsq
      parameter (dw = 7.22472d0, idb = 2, itr = 1, gam = 1.154700538d0,
     $  n = 73, nsq = 8, kq = 0, kr = 8, ks = 9, idv = 100,
     $  ndp = 1040, nep = -1010, rb = 1.d100, rm = 1.d5)
      integer lnm(n)
      character*1 chr0(5*ndp/4+50), chr1(100)
      character*40 nam(n), namx
      double precision dr(n), r1(n)
cmp+ multip real  al, t1, t2, r, x, dmreal
      double precision  al, t1, t2, r(n), x(n)

c   This is scratch space for pslqm2.  See below.

      integer is0(3*n)
      double precision s1(7*n*n+nsq*n+3*n)
cmp+ multip real  s2
      double precision  s2(3*n*n+nsq*n+2*n)

      write (6, 1) itr, gam, n, nsq, kq, kr, ks, idv, rb, rm, ndp, nep
1     format ('PSLQM2 Test Program'/'No. trials = ',i3,3x,
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
        call pslqm2 (idb, gam, n, nsq, rb, x, is0, s1, s2, iq, r)
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

c   The following code performs the two-level, multi-pair PSLQ algorithm.
c   David H. Bailey     1999-09-16

      subroutine pslqm2 (idb, gam, n, nsq, rb, x, is0, s1, s2, iq, r)

c   This routine allocates the scratch space arrays is0, s1 and s2.  Handling
c   scratch space in this manner is not really necessary in Fortran-90, but
c   this design facilitates straightforward translation to Fortran-77.

c   Arguments are as follows:
c     idb = Debug flag: 0 (none) to 4 (full).
c     gam = PSLQ gamma parameter, normally set to sqrt(4/3).
c     n   = Dimension of input vector.
c     nsq = Size of tables used in iterdp and itermp.
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
      double precision s1(7*n*n+nsq*n+3*n)
cmp+ multip real  s2, x, r
      double precision  s2(3*n*n+nsq*n+2*n), x(n), r(n)

      call pslqm2x (idb, gam, n, nsq, rb, x, is0(1), is0(n+1),
     $  is0(2*n+1),
     $  s1(1), s1(n*n+1), s1(2*n*n+1), s1(3*n*n+1), s1(4*n*n+1),
     $  s1(5*n*n+1),
     $  s1(6*n*n+1), s1(7*n*n+1), s1(7*n*n+nsq*n+1),
     $  s1(7*n*n+nsq*n+n+1),
     $  s1(7*n*n+nsq*n+2*n+1), s2(1), s2(n*n+1), s2(2*n*n+1),
     $  s2(3*n*n+1),
     $  s2(3*n*n+nsq*n+1), s2(3*n*n+nsq*n+n+1), iq, r)
      return
      end

      subroutine pslqm2x (idb, gam, n, nsq, rb, x, ip, ir, is, da, db,
     $  dh, dsa,
     $  dsb, dsh, dt, dsyq, dq, dy, dsy, b, h, t, syq, s, y, iq, r)

c   The following parameters are set in this routine:
c     ipi = Iteration print interval when idb >= 2.
c     ipm = Iteration check interval for MP iterations; also save interval.
c     itm = Maximum iteration count.  Run is aborted when this is exceeded.

      implicit none
      integer i, idb, imq, ipi, ipm, iq, it, itm, its, izd, izm, j, n,
     $  n1, n2, nsq
      parameter (ipi = 100, ipm = 10, itm = 1000000)
      double precision bound, d1, d2, d3, gam, rb, rn, second, tm0,
     $  tm1, times(15)
      common /timec/ times
      integer ip(n), ir(n), is(n)
      double precision da(n,n), db(n,n), dh(n,n), dq(n), dsa(n,n),
     $  dsb(n,n),
     $  dsh(n,n), dsyq(n,nsq), dt(n,n), dy(n), dsy(n)
cmp+ multip real  b, h, s, syq, t, r, x, y, t1, t2
      double precision  b(n,n), h(n,n), s(n), syq(n,nsq), t(n,n), r(n),
     $  x(n),
     $  y(n), t1, t2

c   Initialize.

      if (idb .ge. 2) write (6, 1) n
1     format ('PSLQM2 integer relation detection: n =',i5)
      iq = 0
      it = 0
      imq = 0
      rn = 0.d0

      do i = 1, 15
        times(i) = 0.d0
      enddo

      tm0 = second ()
      call initmp (idb, n, nsq, ip, dt, b, h, s, syq, x, y)
      tm1 = second ()
      times(1) = tm1 - tm0
      if (idb .ge. 2) write (6, 2)
2     format ('MP initialization complete.')

100   continue

c   Initialize DP arrays from MP arrays.

      tm0 = second ()
      call initdp (idb, it, n, nsq, da, db, dh, dy, dsyq, h, y, izd)
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
        call iterdp (idb, gam, it, n, nsq, ip, ir, is, da, db, dh, dq,
     $  dsyq,
     $    dt, dy, imq, izd)
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
          call updtmp (idb, it, n, ip, dt, da, db, b, h, t, y, izm)
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
      call itermp (idb, gam, it, n, nsq, ip, ir, is, dt, b, h, s, syq,
     $  t, y,
     $  imq, izm)
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
            d1 = d1 + dble (b(j,i)) ** 2
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
              r(i) = b(j,i)
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

      subroutine initdp (idb, it, n, nsq, da, db, dh, dy, dsyq, h, y,
     $  izd)

c   This re-initializes the DP arrays from the MP arrays.

      implicit none
      integer i, idb, it, izd, j, n, n1, nsq, nws
      double precision da(n,n), db(n,n), dh(n,n), dy(n), dsyq(n,nsq),
     $  d1, deps
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

c   Zero the dsyq array.

      do j = 1, nsq
        do i = 1, n
          dsyq(i,j) = 0.d0
        enddo
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

      subroutine initmp (idb, n, nsq, ix, dx, b, h, s, syq, x, y)

c   This initializes MP arrays at the beginning.

      implicit none
      integer i, idb, j, n, nsq
      double precision d1
      integer ix(n)
      double precision dx(n)
cmp+ multip real  b, h, s, syq, x, y, t1
      double precision  b(n,n), h(n,n), s(n), syq(n,nsq), x(n), y(n), t1

      if (idb .ge. 4) then
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

      if (idb .ge. 4) then
        write (6, 2)
2     format ('initmp: Initial y vector:')
        call matoutmd (1, n, ix, dx, y)
        write (6, 3)
3     format ('initmp: Initial h matrix:')
        call matoutmd (n, n - 1, ix, dx, h)
      endif

      return
      end

      subroutine iterdp (idb, gam, it, n, nsq, ip, ir, is, da, db, dh,
     $  dq, dsyq,
     $  dt, dy, imq, izd)

c   This performs one iteration of the PSLQ algorithm using DP arithmetic.

      implicit none
      integer i, idb, ii, ij, im, im1, imq, it, izd, j, j1, j2, k, mpr,
     $  mq, n,
     $  n1, nsq
      double precision deps, gam, t1, t2, t3, t4, tmx1, tmx2, second,
     $  tm0, tm1,
     $  tm2, tm3, tm4, tm5, times(15)
      common /timec/ times
      integer ip(n), ir(n), is(n)
      double precision da(n,n), db(n,n), dh(n,n), dq(n), dsyq(n,nsq),
     $  dt(n,n), dy(n)
c>
c   On IEEE systems, use the first line. On Cray non-IEEE systems, use
c   the second.

      parameter (tmx1 = 1.d13, tmx2 = 2.d0**52, deps = 1.d-14)
c parameter (tmx1 = 2.d11, tmx2 = 2.d0**47, deps = 2.d-13)

      izd = 0
      mpr = nint (0.4d0 * n)
      tm0 = second ()

c   Compute dq vector = {gam^i * |dh(i,i)|}, then sort in ascending order.

      do i = 1, n - 1
        dq(i) = gam ** i * abs (dh(i,i))
      enddo

      call qsortdp (n - 1, dq, ip)

c   Select up to mpr disjoint pairs of indices (m,m+1), where m is an index
c   from the list of the largest dq(i).

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

c   Exchange the pairs of entries of dy, and rows of da, db and dh.

      do j = 1, mq
        im = ir(j)
        im1 = im + 1
        t1 = dy(im)
        dy(im) = dy(im1)
        dy(im1) = t1

        do i = 1, n
          t1 = da(im,i)
          da(im,i) = da(im1,i)
          da(im1,i) = t1
          t1 = db(im,i)
          db(im,i) = db(im1,i)
          db(im1,i) = t1
        enddo

        do i = 1, n - 1
          t1 = dh(im,i)
          dh(im,i) = dh(im1,i)
          dh(im1,i) = t1
        enddo
      enddo

      tm1 = second ()

c   Eliminate the "corners" produced by the above permutation in dh.

      do j = 1, mq
        im = ir(j)
        im1 = im + 1
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
      enddo

      tm2 = second ()

c   Perform reduction on dh, using the diagonal scheme.  Multipliers are
c   saved in the dt array.

      do i = 2, n
        do j = 1, n - i + 1
          ij = i + j - 1

          do k = j + 1, ij - 1
            dh(ij,j) = dh(ij,j) - dt(ij,k) * dh(k,j)
          enddo

          dt(ij,j) = anint (dh(ij,j) / dh(j,j))
          dh(ij,j) = dh(ij,j) - dt(ij,j) * dh(j,j)
        enddo
      enddo

      tm3 = second ()

c   Update dy, using the dt array.  Find min absolute value of dy.

      t1 = abs (dy(n))

      do j = 1, n - 1
        do i = j + 1, n
          dy(j) = dy(j) + dt(i,j) * dy(i)
        enddo

        t1 = min (t1, abs (dy(j)))
      enddo

c   Update da and db, using the dt array.  Find the max absolute value of
c   da and db entries as they are calculated (not merely at the end).

      t2 = 0.d0

c$omp parallel do private (i, j, k, t3)
      do k = 1, n
        t3 = 0.d0

        do j = 1, n - 1
          do i = j + 1, n
            da(i,k) = da(i,k) - dt(i,j) * da(j,k)
            db(j,k) = db(j,k) + dt(i,j) * db(i,k)
            t3 = max (t3, abs (da(i,k)))
            t3 = max (t3, abs (db(j,k)))
          enddo
        enddo

        dq(k) = t3
      enddo
c$omp end parallel do

      do k = 1, n
        t2 = max (t2, dq(k))
      enddo

      tm4 = second ()

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

c   Compare the dy vector with those of recent iterations.  If a duplicate is
c   found, then the next iteration must be performed with mq = 1.

      do j = 1, nsq
        t1 = 0.d0

        do i = 1, n
          t1 = max (t1, abs (dy(i) - dsyq(i,j)))
        enddo

        if (t1 .le. deps) then
          if (idb .ge. 2) write (6, 4) it, j
4       format ('Iteration',i7,3x,'iterdp: Duplicate found, j =',i6)
          imq = 1
          goto 120
        endif
      enddo

c   Place the vector dy in the table dsyq.

120     continue
      k = 1 + mod (it, nsq)

      do i = 1, n
        dsyq(i,k) = dy(i)
      enddo

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

      tm5 = second ()
      times(6) = times(6) + (tm1 - tm0)
      times(7) = times(7) + (tm2 - tm1)
      times(8) = times(8) + (tm3 - tm2)
      times(9) = times(9) + (tm4 - tm3)
      times(10) = times(10) + (tm5 - tm4)

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
     $  tm5, times(15)
      common /timec/ times
cmp+ multip real  t1, t2, t3, t4, teps
      double precision  t1, t2, t3, t4, teps
      integer ip(n), ir(n), is(n)
      double precision dx(n)
cmp+ multip real  b, h, q, syq, t, y
      double precision  b(n,n), h(n,n), q(n), syq(n,nsq), t(n,n), y(n)

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
 2     format ('Iteration',i7,3x,'itermp: Precision exhausted.')
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
3       format ('Iteration',i7,3x,'itermp: Duplicate found, j =',i6)
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

      if (idb .ge. 4) then
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
      times(11) = times(11) + (tm1 - tm0)
      times(12) = times(12) + (tm2 - tm1)
      times(13) = times(13) + (tm3 - tm2)
      times(14) = times(14) + (tm4 - tm3)
      times(15) = times(15) + (tm5 - tm4)

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

c$omp parallel do private (i, j, t) copyin (mpnw)
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
c$omp end parallel do

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

      call mxmdm (n, 1, db, y, t)

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

      call mxmdm (n, n, db, b, t)

c   Update h with da.  There is no need to perform a LQ decomposition on h.

      call mxmdm (n, n - 1, da, h, t)

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

      subroutine mxmdm (n1, n2, a, b, c)

c  This multiplies the DP square matrix a by the MP matrix b, and the result
c  is placed in b.  c is a MP scratch array with n1 cells.  n1, n2 are
c  the matrix dimensions as indicated below.

      implicit none
      integer i, j, mw4, n1, n2
      double precision a(n1,n1)
cmp+ multip real  b, c, t1, t2
      double precision  b(n1,n2), c(n1), t1, t2

      mw4 = mpnwp + 4

c$omp parallel do private (i, j, c, t1) copyin (mpnw)
      do j = 1, n2
        do i = 1, n1
          call mpdotd (n1, mw4, b(1,j) , n1, a(i,1), t1 )
          c(i) = t1
        enddo

        do i = 1, n1
          b(i,j) = c(i)
        enddo
      enddo
c$omp end parallel do

      return
      end

      subroutine qsortdp (n, a, ip)

c   This routine sorts the entries of the N-long DP vector A into ascending
c   order using the quicksort algorithm.  The permutation vector that would
c   sort the vector is returned in IP.

      implicit none
      integer i, iq, it, j, jq, jz, k, l, n
      integer ip(n), ik(50), jk(50)
      double precision a(n), s0, s1, s2

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

      subroutine qsortmp (n, a, ip)

c   This routine sorts the entries of the N-long MP vector A into ascending
c   order using the quicksort algorithm.  The permutation vector that would
c   sort the vector is returned in IP.

      implicit none
      integer i, iq, it, j, jq, jz, k, l, n
cmp+ multip real  a, s0, s1, s2
      double precision  a(n), s0, s1, s2
      integer ip(n), ik(50), jk(50)

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
