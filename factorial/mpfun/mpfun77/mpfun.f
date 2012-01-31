c*****************************************************************************

c   MPFUN: A MULTIPLE PRECISION FLOATING POINT COMPUTATION PACKAGE

c   IEEE Fortran-77 version
c   Version Date:  2001-08-02

c   Author:

c      David H. Bailey
c      NERSC, Lawrence Berkeley Lab
c      Mail Stop 50B-2239
c      Berkeley, CA 94720
c      Email: dhbailey@lbl.gov

c   Restrictions:

c   This software was written while the author was an employee of NASA.
c   This software has been approved by NASA for unrestricted distribution.
c   However, usage of this software is subject to the following:

c   1. This software is offered without warranty of any kind, either expressed
c      or implied.  The author would appreciate, however, any reports of bugs
c      or other difficulties that may be encountered.
c   2. If modifications or enhancements to this software are made to this
c      software by others, NASA Ames reserves the right to obtain this enhanced
c      software at no cost and with no restrictions on its usage.
c   3. The author and NASA Ames are to be acknowledged in any published paper
c      based on computations using this software.  Accounts of practical
c      applications or other benefits resulting from this software are of
c      particular interest.  Please send a copy of such papers to the author.

c   Description:

c   The following information is a brief description of this program.  For
c   full details and instructions for usage, see the paper "A Portable High
c   Performance Multiprecision Package", available from the author.

c   This package of Fortran subroutines performs multiprecision floating point
c   arithmetic.  If sufficient main memory is available, the maximum precision
c   level is at least 16 million digits.  The maximum dynamic range is at
c   least 10^(+-14,000,000).  It employs advanced algorithms, including an
c   FFT-based multiplication routine and some recently discovered
c   quadratically convergent algorithms for pi, exp and log.  The package also
c   features extensive debug and self-checking facilities, so that it can be
c   used as a rigorous system integrity test.  All of the routines in this
c   package have been written to facilitate vector and parallel processing.

c   For users who do not wish to manually write code that calls these routines,
c   an automatic translator program is available from the author that converts
c   ordinary Fortran-77 code into code that calls these routines.  Contact the
c   author for details.

c   This package should run correctly on any computer with a Fortran-77
c   compiler that meets certain minimal floating point accuracy standards.
c   Any system based on the IEEE floating point standard, with a 25 bit
c   mantissa in single precision and a 53 bit mantissa in double precision,
c   easily meets these requirements.  All DEC VAX systems meet these
c   requirements.  All IBM mainframes and workstations meet these requirements.
c   Cray (non-IEEE) systems meet all of these requirements with double 
c   precision disabled (i.e. by using only single precision).

c   Machine-specific tuning notes may be located by searching for the text
c   string c> in this program file.  It is highly recommended that these notes
c   be read before running this package on a specific system.  Also,
c   certain vectorizable DO loops that are often not recognized as such by
c   vectorizing compilers are prefaced with Cray cdir$ ivdep directives.  On
c   other vector systems these directives should be replaced by the
c   appropriate equivalents.

c   Instructions for compiling and testing this program are included in the
c   readme file that accompanies this file.

c*****************************************************************************

      block data

c   This initializes the parameters in MPCOM1 and the error codes in MPCOM2
c   with default values.
c>
c   On IEEE systems and most other 32 bit systems, set BBXC = 4096.D0,
c   NBTC = 24, NPRC = 32, and MCRC = 7.  On Cray (non-IEEE) systems, 
c   set BBXC = 2048.D0, NBTC = 22, NPRC = 16, and MCRC = 8.

      implicit double precision (a-h, o-z)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)

      parameter (bbxc = 4096.d0, nbtc = 24, nprc = 32, mcrc = 7,
     $  bdxc = bbxc ** 2, bx2c = bdxc ** 2, rbxc = 1.d0 / bbxc,
     $  rdxc = rbxc ** 2, rx2c = rdxc ** 2, rxxc = 16.d0 * rx2c)
      data bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr / bbxc, bdxc,
     $  bx2c, rbxc, rdxc, rx2c, rxxc, nbtc, nprc/
      data nw, idb, ldb, ier, mcr, ird, ics, ihs, ims /
     $  16, 0, 6, 0, mcrc, 1, 1, 1, 1024/
      data ker /73 * 2/
      end

      subroutine dpadd (a, na, b, nb, c, nc)

c   This adds the DPE numbers (A, NA) and (B, NB) to yield the sum (C, NC).

      implicit double precision (a-h, o-z)
      dimension pt(64)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      save pt
      data pt/ 64 * 0.d0/

      if (ier .ne. 0) then
        c = 0.d0
        nc = 0
        return
      endif

c   If this is the first call to DPADD, initialize the PT table.

      if (pt(1) .eq. 0.d0) then
        pt(1) = 0.5d0

        do 100 i = 2, 64
          pt(i) = 0.5d0 * pt(i-1)
 100    continue

      endif

c   This operation reduces to five cases.

      if (b .eq. 0.d0) then
        c = a
        nc = na
      else if (a .eq. 0.d0) then
        c = b
        nc = nb
      else if (na .eq. nb) then
        c = a + b
        nc = na
      else if (na .gt. nb) then
        k = na - nb
        nc = na
        if (k .gt. 64) then
          c = a
        else
          c = a + b * pt(k)
        endif
      else
        k = nb - na
        nc = nb
        if (k .gt. 64) then
          c = b
        else
          c = b + a * pt(k)
        endif
      endif
      if (c .eq. 0.d0) then
        nc = 0
        goto 130
      endif

c   Normalize the result to a decent range if it is not.

 110  if (abs (c) .ge. bdx) then
        c = rdx * c
        nc = nc + nbt
        goto 110
      endif

 120  if (abs (c) .lt. 1.d0) then
        c = bdx * c
        nc = nc - nbt
        goto 120
      endif

 130  return
      end

      subroutine dpdec (a, na, b, nb)

c   This converts the DPE number (A, NA) to decimal form, i.e. B * 10^NB,
c   where |B| is between 1 and 10.

      implicit double precision (a-h, o-z)
      parameter (xlt = 0.3010299956639812d0)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)

      if (a .ne. 0.d0) then
        t1 = xlt * na + log10 (abs (a))
        nb = t1
        if (t1 .lt. 0.d0) nb = nb - 1
        b = sign (10.d0 ** (t1 - nb), a)
      else
        b = 0.d0
        nb = 0
      endif

      return
      end

      subroutine dpdiv (a, na, b, nb, c, nc)

c   This divides the DPE number (A, NA) by (B, NB) to yield the quotient
c   (C, NC).

      implicit double precision (a-h, o-z)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)

      if (ier .ne. 0) then
        c = 0.d0
        nc = 0
        return
      endif
      if (b .eq. 0.d0) then
        if (ker(1) .ne. 0) then
          write (ldb, 1)
 1        format ('*** DPDIV: Divisor is zero.')
          ier = 1
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Divide A by B and subtract exponents, unless A is zero.

      if (a .eq. 0.d0) then
        c = 0.d0
        nc = 0
        goto 120
      else
        c = a / b
        nc = na - nb
      endif

c   Normalize the result to a decent range if it is not.

 100  if (abs (c) .ge. bdx) then
        c = rdx * c
        nc = nc + nbt
        goto 100
      endif

 110  if (abs (c) .lt. 1.d0) then
        c = bdx * c
        nc = nc - nbt
        goto 110
      endif

 120  return
      end

      subroutine dpmul (a, na, b, nb, c, nc)

c   This multiplies the DPE number (A, NA) by (B, NB) to yield the product
c   (C, NC).

      implicit double precision (a-h, o-z)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims

      if (ier .ne. 0) then
        c = 0.d0
        nc = 0
        return
      endif

c   Multiply A by B and add exponents, unless either is zero.

      if (a .eq. 0.d0 .or. b .eq. 0.d0) then
        c = 0.d0
        nc = 0
        goto 120
      else
        c = a * b
        nc = na + nb
      endif

c   Normalize the result to a decent range if it is not.

 100  if (abs (c) .ge. bdx) then
        c = rdx * c
        nc = nc + nbt
        goto 100
      endif

 110  if (abs (c) .lt. 1.d0) then
        c = bdx * c
        nc = nc - nbt
        goto 110
      endif

 120  return
      end

      subroutine dppwr (a, na, b, nb, c, nc)

c   This raises the DPE number (A, NA) to the (B, NB) power and places the
c   result in (C, NC).

      implicit double precision (a-h, o-z)
      parameter (cl2 = 1.4426950408889633d0)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)

      if (ier .ne. 0) then
        c = 0.d0
        nc = 0
        return
      endif
      if (a .le. 0.d0) then
        if (ker(2) .ne. 0) then
          write (ldb, 1)
 1        format ('*** DPPWR: Argument is less than or equal to zero.')
          ier = 2
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      if (b .eq. 0.d0) then
        c = 1.d0
        nc = 0
        goto 120
      endif

      if (b .eq. 1.d0 .and. nb .eq. 0) then
        c = a
        nc = na
        goto 120
      endif

c   Compute the base 2 logarithm of A and multiply by B.

      al = cl2 * log (a) + na
      call dpmul (al, 0, b, nb, t1, n1)

c   Check for possible overflow or underflow.

      if (n1 .gt. 6) then
        if (t1 .gt. 0.d0) then
          if (ker(3) .ne. 0) then
            write (ldb, 2)
 2          format ('*** DPPWR: Overflow')
            ier = 3
            if (ker(ier) .eq. 2) call mpabrt
          endif
          return
        else
          c = 0.d0
          nc = 0
          goto 120
        endif
      endif

c   Compute 2 raised to the power B * Log_2 (A).

      t1 = t1 * 2.d0 ** n1
      nc = int (t1)
      c = 2.d0 ** (t1 - nc)

c   Normalize the result to a decent range if it is not.

 100  if (abs (c) .ge. bdx) then
        c = rdx * c
        nc = nc + nbt
        goto 100
      endif

 110  if (abs (c) .lt. 1.d0) then
        c = bdx * c
        nc = nc - nbt
        goto 110
      endif

 120  return
      end

      subroutine dpsqrt (a, na, b, nb)

c   This computes the square root of the DPE number (A, NA) and places the
c   result in (B, NB).

      implicit double precision (a-h, o-z)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)

      if (ier .ne. 0) then
        b = 0.d0
        nb = 0
        return
      endif
      if (a .lt. 0.d0) then
        if (ker(4) .ne. 0) then
          write (ldb, 1)
 1        format ('*** DPSQRT: Argument is negative.')
          ier = 4
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      if (a .eq. 0.d0) then
        b = 0.d0
        nb = 0
        goto 120
      endif

c   Divide the exponent of A by two and then take the square root of A.  If
c   NA is not an even number, then we have to multiply A by 10 before taking
c   the square root.

      nb = na / 2
      if (na .eq. 2 * nb) then
        b = sqrt (a)
      else
        b = sqrt (2.d0 * a)
        if (na .lt. 0) nb = nb - 1
      endif

c   Normalize the result to a decent range if it is not.

 100  if (abs (b) .ge. bdx) then
        b = rdx * b
        nb = nb + nbt
        goto 100
      endif

 110  if (abs (b) .lt. 1.d0) then
        b = bdx * b
        nb = nb - nbt
        goto 110
      endif

 120  return
      end

      subroutine dpsub (a, na, b, nb, c, nc)

c   This subtracts the DPE number (B, NB) from (A, NA) to yield the difference
c   (C, NC).

      implicit double precision (a-h, o-z)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims

      if (ier .ne. 0) then
        c = 0.d0
        nc = 0
        return
      endif

      bb = -b
      call dpadd (a, na, bb, nb, c, nc)

      return
      end

      subroutine mpabrt
c>
c   This routine terminates execution.  Many users will want to replace the
c   default STOP with a call to a system routine that provides a traceback.
c   Examples of code that produce traceback are included here (commented out)
c   for some systems.

      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims

      write (ldb, 1) ier
 1    format ('*** MPABRT: Execution terminated, error code =',i4)

c   Use this line on Cray systems.

c      call abort

c   Use this line plus the C routine TRACBK (available from author) on
c   Silicon Graphics IRIS systems.

c      call tracbk

c   On other systems, merely terminate execution.

      stop
      end

      subroutine mpadd (a, b, c)

c   This routine adds MP numbers A and B to yield the MP sum C.  It attempts
c   to include all significance of A and B in the result, up to the maximum
c   mantissa length NW.  Debug output starts with IDB = 9.

c   Max SP space for C: NW + 4 cells.  Max DP scratch space: NW + 4 cells.

      double precision d
      parameter (ndb = 22)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom4/ d(4100)
      dimension a(nw+2), b(nw+2), c(nw+4)

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        return
      endif
      if (idb .ge. 9) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPADD I'/(6f12.0))
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 1) (b(i), i = 1, no)
      endif

      ia = sign (1., a(1))
      ib = sign (1., b(1))
      na = min (int (abs (a(1))), nw)
      nb = min (int (abs (b(1))), nw)

c   This first IF block checks for zero inputs.

      if (na .eq. 0) then

c   A is zero -- the result is B.

        c(1) = sign (nb, ib)

        do 100 i = 2, nb + 2
          c(i) = b(i)
 100    continue

        goto 420
      elseif (nb .eq. 0) then

c   B is zero -- the result is A.

        c(1) = sign (na, ia)

        do 110 i = 2, na + 2
          c(i) = a(i)
 110    continue

        goto 420
      endif
      ma = a(2)
      mb = b(2)

c   This IF block breaks the problem into different branches depending on
c   the relative sizes of the exponents of A and B.

      if (ma .eq. mb) then

c   A and B have the same exponent.

        nm = min (na, nb)
        nx = max (na, nb)
        if (ia .eq. ib) then

c   A and B have the same exponent and sign.

          d(1) = sign (nx, ia)
          d(2) = ma
          d(nx+3) = 0.d0
          d(nx+4) = 0.d0

          do 120 i = 3, nm + 2
            d(i) = dble (a(i)) + dble (b(i))
 120      continue

          if (na .gt. nb) then

c   A is longer than B -- include extra words of A in C.

            do 130 i = nm + 3, na + 2
              d(i) = a(i)
 130        continue

          elseif (nb .gt. na) then

c   B is longer than A -- include extra words of B in C.

            do 140 i = nm + 3, nb + 2
              d(i) = b(i)
 140        continue

          endif
        else

c   A and B have the same exponent but the opposite sign.  It is thus
c   necessary to scan through each vector until we find an unequal word.

          do 150 i = 3, nm + 2
            if (a(i) .ne. b(i)) goto 180
 150      continue

c   All words up to the common length are equal.

          if (na .eq. nb) then

c   The length of A is the same as B -- result is zero.

            c(1) = 0.d0
            c(2) = 0.d0
            goto 420
          elseif (na .gt. nb) then

c   A is longer -- thus trailing words of A are shifted to start of C.

            nn = na - nb
            d(1) = sign (nn, ia)
            d(2) = a(2) - nb
            d(nn+3) = 0.d0
            d(nn+4) = 0.d0

            do 160 i = 3, nn + 2
              d(i) = a(i+nb)
 160        continue

          elseif (nb .gt. na) then

c   B is longer -- thus trailing words of B are shifted to start of C.

            nn = nb - na
            d(1) = sign (nn, ib)
            d(2) = b(2) - na
            d(nn+3) = 0.d0
            d(nn+4) = 0.d0

            do 170 i = 3, nn + 2
              d(i) = b(i+na)
 170        continue

          endif
          goto 410

c   An unequal word was found.

 180      k = i - 3
          if (a(k+3) .gt. b(k+3)) then

c   A is larger -- subtract B (shifted) from A.

            d(1) = sign (nx - k, ia)
            d(2) = a(2) - k
            d(nx-k+3) = 0.d0
            d(nx-k+4) = 0.d0

            do 190 i = 3, nm - k + 2
              d(i) = dble (a(i+k)) - dble (b(i+k))
 190        continue

            do 200 i = nb - k + 3, na - k + 2
              d(i) = a(i+k)
 200        continue

            do 210 i = na - k + 3, nb - k + 2
              d(i) = - b(i+k)
 210        continue

          else

c   B is larger -- subtract A (shifted) from B.

            d(1) = sign (nx - k, ib)
            d(2) = b(2) - k
            d(nx-k+3) = 0.d0
            d(nx-k+4) = 0.d0

            do 220 i = 3, nm - k + 2
              d(i) = dble (b(i+k)) - dble (a(i+k))
 220        continue

            do 230 i = nb - k + 3, na - k + 2
              d(i) = - a(i+k)
 230        continue

            do 240 i = na - k + 3, nb - k + 2
              d(i) = b(i+k)
 240        continue

          endif
        endif
      elseif (ma .gt. mb) then

c   Exponent of A is greater.  In other words, A has a larger magnitude.

        mc = ma - mb
        la = min (mc, na)
        lb = min (mc + nb, nw + 2)
        lm = min (na, lb)
        lx = min (max (na, lb), nw)
        d(1) = sign (lx, ia)
        d(2) = a(2)
        d(lx+3) = 0.d0
        d(lx+4) = 0.d0

        do 250 i = 3, la + 2
          d(i) = a(i)
 250    continue

c   If B is shifted NW + 2 or more words to the right of A then C = A.

        if (mc .ge. nw + 2) then
          d(1) = sign (na, ia)
          d(la+3) = 0.d0
          d(la+4) = 0.d0
          goto 410
        endif
        if (mc .gt. na) then

c   There is a gap between A and the shifted B.  Fill it with zeroes.

          do 260 i = na + 3, mc + 2
            d(i) = 0.d0
 260      continue

          lm = mc
        endif
        if (ia .eq. ib) then

c   A and B have the same sign -- add common words with B shifted right.

          do 270 i = mc + 3, lm + 2
            d(i) = dble (a(i)) + dble (b(i-mc))
 270      continue

c   Include tail of A or B, whichever is longer after shift.

          if (na .gt. lb) then

            do 280 i = lm + 3, na + 2
              d(i) = a(i)
 280        continue

          else

            do 290 i = lm + 3, lb + 2
              d(i) = b(i-mc)
 290        continue

          endif
        else

c   A and B have different signs -- subtract common words with B shifted right.

          do 300 i = mc + 3, lm + 2
            d(i) = dble (a(i)) - dble (b(i-mc))
 300      continue

c   Include tail of A or B, whichever is longer after shift.

          do 310 i = lm + 3, na + 2
            d(i) = a(i)
 310      continue

          do 320 i = lm + 3, lb + 2
            d(i) = - b(i-mc)
 320      continue

        endif
      else

c   Exponent of B is greater.  In other words, B has a larger magnitude.

        mc = mb - ma
        lb = min (mc, nb)
        la = min (mc + na, nw + 2)
        lm = min (nb, la)
        lx = min (max (nb, la), nw)
        d(1) = sign (lx, ib)
        d(2) = b(2)
        d(lx+3) = 0.d0
        d(lx+4) = 0.d0

        do 330 i = 3, lb + 2
          d(i) = b(i)
 330    continue

c   If A is shifted NW + 2 or more words to the right of B then C = B.

        if (mc .ge. nw + 2) then
          d(1) = sign (nb, ib)
          d(lb+3) = 0.d0
          d(lb+4) = 0.d0
          goto 410
        endif
        if (mc .gt. nb) then

c   There is a gap between B and the shifted A.  Fill it with zeroes.

          do 340 i = nb + 3, mc + 2
            d(i) = 0.d0
 340      continue

          lm = mc
        endif
        if (ib .eq. ia) then

c   B and A have the same sign -- add common words with A shifted right.

          do 350 i = mc + 3, lm + 2
            d(i) = dble (b(i)) + dble (a(i-mc))
 350      continue

c   Include tail of B or A, whichever is longer after shift.

          do 360 i = lm + 3, nb + 2
            d(i) = b(i)
 360      continue

          do 370 i = lm + 3, la + 2
            d(i) = a(i-mc)
 370      continue

        else

c   B and A have different signs -- subtract common words with A shifted right.

          do 380 i = mc + 3, lm + 2
            d(i) = dble (b(i)) - dble (a(i-mc))
 380      continue

c   Include tail of B or A, whichever is longer after shift.

          do 390 i = lm + 3, nb + 2
            d(i) = b(i)
 390      continue

          do 400 i = lm + 3, la + 2
            d(i) = - a(i-mc)
 400      continue

        endif
      endif

c   Fix up result, since some words may be negative or exceed BDX.

 410  call mpnorm (c)

 420  if (idb .ge. 9) then
        no = min (int (abs (c(1))), ndb) + 2
        write (ldb, 2) (c(i), i = 1, no)
 2      format ('MPADD O'/(6f12.0))
      endif
      return
      end

      subroutine mpagmx (a, b)

c   This performs the arithmetic-geometric mean (AGM) iterations.  This routine
c   is called by MPLOGX.  It is not intended to be called directly by the user.

c   Max SP space for A and B: NW + 4 cells.  Max SP scratch space: 6.5*NW + 35
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)
      dimension a(nw+4), b(nw+4)

      if (ier .ne. 0) then
        a(1) = 0.
        a(2) = 0.
        b(1) = 0.
        b(2) = 0.
        return
      endif
      n4 = nw + 4
      ns = 2 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n4
      s(k0) = 0.
      s(k0+1) = 0.
      l1 = 0

 100  l1 = l1 + 1
      if (l1 .eq. 50) then
        if (ker(5) .ne. 0) then
          write (ldb, 1)
 1        format ('*** MPAGMX: Iteration limit exceeded.')
          ier = 5
          if (ker(ier) .eq. 2) call mpabrt
        endif
      endif

      s1 = s(k0+1)
      call mpadd (a, b, s(k0))
      call mpmuld (s(k0), 0.5d0, 0, s(k1))
      call mpmulx (a, b, s(k0))
      call mpsqrx (s(k0), b)
      call mpeq (s(k1), a)
      call mpsub (a, b, s(k0))

c   Check for convergence.

      if (s(k0) .ne. 0. .and. (s(k0+1) .lt. s1 .or. s(k0+1) .ge. -2))
     $  goto 100

      ics = iss
      if (idb .ge. 6) write (ldb, 2) l1, s(k0+1)
 2    format ('MPAGMX: Iter., Tol. Achieved =',i5,f8.0)
      return
      end

      subroutine mpaler

c   This outputs error messages when a single precision scratch space
c   allocation error is detected.

      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)

      if (ker(6) .ne. 0) then
        write (ldb, 1) ics - 1
 1      format ('*** MPALER: Insufficient single precision scratch ',
     $    'space.'/ 'Allocate',i10,' cells in an array in common ',
     $    'MPCOM3 of the main '/ 'program and set IMS in common ',
     $    'MPCOM1 to this size.')
        ier = 6
        if (ker(ier) .eq. 2) call mpabrt
      endif

      return
      end

      subroutine mpang (x, y, pi, a)

c   This computes the MP angle A subtended by the MP pair (X, Y) considered as
c   a point in the x-y plane.  This is more useful than an arctan or arcsin
c   routine, since it places the result correctly in the full circle, i.e.
c   -Pi < A <= Pi.  PI is the MP value of Pi computed by a previous call to
c   MPPI.  For extra high levels of precision, use MPANGX.  The last word of
c   the result is not reliable.  Debug output starts with IDB = 5.

c   Max SP space for A: NW + 4 cells.  Max SP scratch space: 15 * NW + 88
c   cells.  Max DP scratch space: NW + 7 cells.

c   The Taylor series for Sin converges much more slowly than that of Arcsin.
c   Thus this routine does not employ Taylor series, but instead computes
c   Arccos or Arcsin by solving Cos (a) = x or Sin (a) = y using one of the
c   following Newton iterations, both of which converge to a:

c           z_{k+1} = z_k - [x - Cos (z_k)] / Sin (z_k)
c           z_{k+1} = z_k + [y - Sin (z_k)] / Cos (z_k)

c   The first is selected if Abs (x) <= Abs (y); otherwise the second is used.
c   These iterations are performed with a maximum precision level NW that
c   is dynamically changed, approximately doubling with each iteration.
c   See the comment about the parameter NIT in MPDIVX.

      double precision cl2, cpi, t1, t2, t3
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0, cpi = 3.141592653589793d0,
     $  nit = 3)
      dimension a(nw+4), pi(nw+2), x(nw+2), y(nw+2)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        a(1) = 0.
        a(2) = 0.
        return
      endif
      if (idb .ge. 5) then
        call mpdeb ('MPANG I', x)
        call mpdeb ('MPANG I', y)
      endif

      ix = sign (1., x(1))
      nx = min (int (abs (x(1))), nw)
      iy = sign (1., y(1))
      ny = min (int (abs (y(1))), nw)

c   Check if both X and Y are zero.

      if (nx .eq. 0 .and. ny .eq. 0) then
        if (ker(7) .ne. 0) then
          write (ldb, 1)
 1        format ('*** MPANG: Both arguments are zero.')
          ier = 7
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Check if Pi has been precomputed.

      call mpmdc (pi, t1, n1)
      if (n1 .ne. 0 .or. abs (t1 - cpi) .gt. rx2) then
        if (ker(8) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPANG: PI must be precomputed.')
          ier = 8
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Check if one of X or Y is zero.

      if (nx .eq. 0) then
        if (iy .gt. 0) then
          call mpmuld (pi, 0.5d0, 0, a)
        else
          call mpmuld (pi, -0.5d0, 0, a)
        endif
        goto 120
      elseif (ny .eq. 0) then
        if (ix .gt. 0) then
          a(1) = 0.
          a(2) = 0.
        else
          call mpeq (pi, a)
        endif
        goto 120
      endif

      n5 = nw + 5
      ns = 5 * n5
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n5
      k2 = k1 + n5
      k3 = k2 + n5
      k4 = k3 + n5
      nws = nw
      nw = nw + 1

c   Determine the least integer MQ such that 2 ^ MQ .GE. NW.

      t1 = nws
      mq = cl2 * log (t1) + 1.d0 - rxx

c   Normalize x and y so that x^2 + y^2 = 1.

      call mpmul (x, x, s(k0))
      call mpmul (y, y, s(k1))
      call mpadd (s(k0), s(k1), s(k2))
      call mpsqrt (s(k2), s(k3))
      call mpdiv (x, s(k3), s(k1))
      call mpdiv (y, s(k3), s(k2))

c   Compute initial approximation of the angle.

      call mpmdc (s(k1), t1, n1)
      call mpmdc (s(k2), t2, n2)
      n1 = max (n1, -66)
      n2 = max (n2, -66)
      t1 = t1 * 2.d0 ** n1
      t2 = t2 * 2.d0 ** n2
      t3 = atan2 (t2, t1)
      call mpdmc (t3, 0, a)

c   The smaller of x or y will be used from now on to measure convergence.
c   This selects the Newton iteration (of the two listed above) that has the
c   largest denominator.

      if (abs (t1) .le. abs (t2)) then
        kk = 1
        call mpeq (s(k1), s(k0))
      else
        kk = 2
        call mpeq (s(k2), s(k0))
      endif

      nw = 3
      iq = 0

c   Perform the Newton-Raphson iteration described above with a dynamically
c   changing precision level NW (one greater than powers of two).

      do 110 k = 2, mq
        nw = min (2 * nw - 2, nws) + 1
 100    continue
        call mpcssn (a, pi, s(k1), s(k2))
        if (kk .eq. 1) then
          call mpsub (s(k0), s(k1), s(k3))
          call mpdiv (s(k3), s(k2), s(k4))
          call mpsub (a, s(k4), s(k1))
        else
          call mpsub (s(k0), s(k2), s(k3))
          call mpdiv (s(k3), s(k1), s(k4))
          call mpadd (a, s(k4), s(k1))
        endif
        call mpeq (s(k1), a)
        if (k .eq. mq - nit .and. iq .eq. 0) then
          iq = 1
          goto 100
        endif
 110  continue

c   Restore original precision level.

      nw = nws
      ics = iss
      call mproun (a)

 120  if (idb .ge. 5) call mpdeb ('MPANG O', a)

      return
      end

      subroutine mpangx (x, y, pi, a)

c   This computes the MP angle A subtended by the MP pair (X, Y) considered as
c   a point in the x-y plane.  This is more useful than an arctan or arcsin
c   routine, since it places the result correctly in the full circle, i.e.
c   -Pi < A <= Pi.  PI is the MP value of Pi computed by a previous call to
c   MPPI or MPPIX.  Before calling MPANGX, the array in MPCOM5 must be
c   initialized by calling MPINIX.  For modest levels of precision, use MPANG.
c   NW should be a power of two.  The last three words of the result are not
c   reliable.  Debug output starts with IDB = 6.

c   Max SP space for A: NW + 4 cells.  Max SP scratch space: 19.5 * NW + 87
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   This routine employs a complex arithmetic version of the MPLOGX alogirthm.

      double precision cpi, t1
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cpi = 3.141592653589793d0)
      dimension a(nw+4), f0(8), f1(8), f4(8), pi(nw+2), x(nw+2), y(nw+2)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        a(1) = 0.
        a(2) = 0.
        return
      endif
      if (idb .ge. 6) then
        call mpdeb ('MPANGX I', x)
        call mpdeb ('MPANGX I', y)
      endif

      ix = sign (1., x(1))
      nx = min (int (abs (x(1))), nw)
      iy = sign (1., y(1))
      ny = min (int (abs (y(1))), nw)
      ncr = 2 ** mcr

c   Check if precision level is too low to justify the advanced routine.

      if (nw .le. ncr) then
        call mpang (x, y, pi, a)
        goto 100
      endif

c   Check if both X and Y are zero.

      if (nx .eq. 0 .and. ny .eq. 0) then
        if (ker(9) .ne. 0) then
          write (ldb, 1)
 1        format ('*** MPANGX: Both arguments are zero.')
          ier = 9
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Check if Pi has been precomputed.

      call mpmdc (pi, t1, n1)
      if (n1 .ne. 0 .or. abs (t1 - cpi) .gt. rx2) then
        if (ker(10) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPANGX: PI must be precomputed.')
          ier = 10
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Check if one of X or Y is zero.

      if (nx .eq. 0) then
        if (iy .gt. 0) then
          call mpmuld (pi, 0.5d0, 0, a)
        else
          call mpmuld (pi, -0.5d0, 0, a)
        endif
        goto 100
      elseif (ny .eq. 0) then
        if (ix .gt. 0) then
          a(1) = 0.
          a(2) = 0.
        else
          call mpeq (pi, a)
        endif
        goto 100
      endif

c   Define scratch space.

      n4 = nw + 4
      n42 = 2 * n4
      ns = 4 * n42
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n42
      k2 = k1 + n42
      k3 = k2 + n42
      f0(1) = 0.
      f0(2) = 0.
      f0(3) = 0.
      f1(1) = 1.
      f1(2) = 0.
      f1(3) = 1.
      f1(4) = 0.
      f4(1) = 1.
      f4(2) = 0.
      f4(3) = 4.
      f4(4) = 0.

c   Multiply the input by a large power of two.

      call mpmdc (x, t1, n1)
      n2 = nbt * (nw / 2 + 2) - n1
      tn = n2
      call mpmuld (x, 1.d0, n2, s(k1))
      call mpmuld (y, 1.d0, n2, s(k2))
      call mpmmpc (s(k1), s(k2), n4, s(k0))

c   Perform AGM iterations.

      call mpmmpc (f1, f0, n4, s(k1))
      call mpmmpc (f4, f0, n4, s(k3))
      call mpcdvx (n4, s(k3), s(k0), s(k2))
      call mpcagx (s(k1), s(k2))

c   Compute A = Imag (Pi / (2 * Z)), where Z is the limit of the complex AGM.

      call mpmuld (s(k1), 2.d0, 0, s(k0))
      call mpmuld (s(k1+n4), 2.d0, 0, s(k0+n4))
      call mpmmpc (pi, f0, n4, s(k2))
      call mpcdvx (n4, s(k2), s(k0), s(k1))
      call mpeq (s(k1+n4), a)
      ics = iss

 100  if (idb .ge. 6) call mpdeb ('MPANGX O', a)

      return
      end

      subroutine mpcadd (l, a, b, c)

c   This computes the sum of the MPC numbers A and B and returns the MPC
c   result in C.  L is the offset between real and imaginary parts in A, B
c   and C.  L must be at least NW + 4.  Debug output starts with IDB = 9.

c   Max SP space for C: 2 * L cells.

      dimension a(2*l), b(2*l), c(2*l)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        c(l+1) = 0.
        c(l+2) = 0.
        return
      endif
      if (idb .ge. 9) write (ldb, 1)
 1    format ('MPCADD')

      if (l .lt. nw + 4) then
        if (ker(11) .ne. 0) then
          write (ldb, 2) l, nw + 4
 2        format ('*** MPCADD: Offset parameter is too small',2i8)
          ier = 11
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      l1 = l + 1
      call mpadd (a, b, c)
      call mpadd (a(l1), b(l1), c(l1))

      return
      end

      subroutine mpcagx (a, b)

c   This performs the arithmetic-geometric mean (AGM) iterations.  This routine
c   is called by MPANGX.  It is not intended to be called directly by the user.

c   Max SP space for A and B: 2*NW + 8 cells.  Max SP scratch space:11.5*NW+55
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)
      dimension a(2*nw+8), b(2*nw+8)

      if (ier .ne. 0) then
        a(1) = 0.
        a(2) = 0.
        b(1) = 0.
        b(2) = 0.
        return
      endif
      n4 = nw + 4
      ns = 4 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + 2 * n4
      s(k0) = 0.
      s(k0+1) = 0.
      l1 = 0

 100  l1 = l1 + 1
      if (l1 .eq. 50) then
        if (ker(12) .ne. 0) then
          write (ldb, 1)
 1        format ('*** MPCAGX: Iteration limit exceeded.')
          ier = 12
          if (ker(ier) .eq. 2) call mpabrt
        endif
      endif

      s1 = s(k0+1)
      call mpcadd (n4, a, b, s(k0))
      call mpmuld (s(k0), 0.5d0, 0, s(k1))
      call mpmuld (s(k0+n4), 0.5d0, 0, s(k1+n4))
      call mpcmlx (n4, a, b, s(k0))
      call mpcsqx (n4, s(k0), b)
      call mpceq (n4, s(k1), a)
      call mpsub (a, b, s(k0))

c   Check for convergence.

      if (s(k0) .ne. 0. .and. (s(k0+1) .lt. s1 .or. s(k0+1) .ge. -2))
     $  goto 100

      ics = iss
      if (idb .ge. 6) write (ldb, 2) l1, s(k0+1)
 2    format ('MPCAGX: Iter., Tol. Achieved =',i5,f8.0)
      return
      end

      subroutine mpcbrt (a, b)

c   This computes the cube root of the MP number A and returns the MP result
c   in B.  For extra high levels of precision, use MPCBRX.  Debug output
c   starts with IDB = 7.

c   Max SP space for B: NW + 4 cells.  Max SP scratch space: 3 * NW + 15
c   cells.  Max DP scratch space: NW + 5 cells.

c   This subroutine employs the following Newton-Raphson iteration, which
c   converges to A ^ (-2/3):

c          X_{k+1} = X_k + (1 - X_k^3 * A^2) * X_k / 3

c   where the muliplication () * X_k is performed with only half of the
c   normal level of precision.  These iterations are performed with a
c   maximum precision level NW that is dynamically changed, doubling with
c   each iteration.  The final iteration is performed as follows (this is
c   due to A. Karp):

c          Cbrt(A) = (A * X_n) + [A - (A * X_n)^3] * X_n / 3 (approx.)

c   where the multiplications A * X_n and [] * X_n are performed with only
c   half of the final level of precision.  See the comment about the parameter
c   NIT in MPDIVX.

      double precision cl2, t1, t2
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0, ndb = 22, nit = 3)
      dimension a(nw+2), b(nw+4), f(8)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 7) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPCBRT I'/(6f12.0))
      endif

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)

      if (na .eq. 0) then
        b(1) = 0.
        b(2) = 0.
        goto 120
      endif
      if (ia .lt. 0.d0) then
        if (ker(13) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPCBRT: Argument is negative.')
          ier = 13
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      n5 = nw + 5
      ns = 3 * n5
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n5
      k2 = k1 + n5
      nws = nw

c   Determine the least integer MQ such that 2 ^ MQ .GE. NW.

      t1 = nw
      mq = cl2 * log (t1) + 1.d0 - rxx

c   Compute A^2 outside of the iteration loop.

      nw = nws + 1
      call mpmul (a, a, s(k0))

c   Compute the initial approximation of A ^ (-2/3).

      call mpmdc (a, t1, n)
      n3 = - 2 * n / 3
      t2 = (t1 * 2.d0 ** (n + 3.d0 * n3 / 2.d0)) ** (-2.d0 / 3.d0)
      call mpdmc (t2, n3, b)
      f(1) = 1.
      f(2) = 0.
      f(3) = 1.
      f(4) = 0.
      nw = 3
      iq = 0

c   Perform the Newton-Raphson iteration described above with a dynamically
c   changing precision level NW (one greater than powers of two).

      do 110 k = 2, mq
        nw1 = nw
        nw = min (2 * nw - 2, nws) + 1
        nw2 = nw
 100    continue
        call mpmul (b, b, s(k1))
        call mpmul (b, s(k1), s(k2))
        call mpmul (s(k0), s(k2), s(k1))
        call mpsub (f, s(k1), s(k2))
        nw = nw1
        call mpmul (b, s(k2), s(k1))
        call mpdivd (s(k1), 3.d0, 0, s(k2))
        nw = nw2
        call mpadd (b, s(k2), s(k1))
        call mpeq (s(k1), b)
        if (k .eq. mq - nit .and. iq .eq. 0) then
          iq = 1
          goto 100
        endif
 110  continue

c   Perform last iteration using Karp's trick.

      call mpmul (a, b, s(k0))
      nw1 = nw
      nw = min (2 * nw - 2, nws) + 1
      nw2 = nw
      call mpmul (s(k0), s(k0), s(k1))
      call mpmul (s(k0), s(k1), s(k2))
      call mpsub (a, s(k2), s(k1))
      nw = nw1
      call mpmul (s(k1), b, s(k2))
      call mpdivd (s(k2), 3.d0, 0, s(k1))
      nw = nw2
      call mpadd (s(k0), s(k1), s(k2))
      call mpeq (s(k2), b)

c   Restore original precision level.

      nw = nws
      ics = iss
      call mproun (b)

 120  if (idb .ge. 7) then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 3) (b(i), i = 1, no)
 3      format ('MPCBRT O'/(6f12.0))
      endif
      return
      end

      subroutine mpcbrx (a, b)

c   This computes the cube root of the MP number A and returns the MP result
c   in B.  Before calling MPCBRX, the array in MPCOM5 must be initialized by
c   calling MPINIX.  For modest levels of precision, use MPCBRT.  NW should be
c   a power of two.  The last three words of the result are not reliable.
c   Debug output starts with IDB = 6.

c   Max SP space for B: NW + 4 cells.  Max SP scratch space: 4.5 * NW + 27
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   This routine uses basically the same Newton iteration algorithm as MPCBRT.
c   In fact, this routine calls MPCBRT to obtain an initial approximation.
c   See the comment about the parameter NIT in MPDIVX.

      double precision cl2, t1
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0, ndb = 22, nit = 1)
      dimension a(nw+2), b(nw+4), f(8)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 6) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPCBRX I'/(6f12.0))
      endif

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)
      ncr = 2 ** mcr

      if (na .eq. 0) then
        b(1) = 0.
        b(2) = 0.
        goto 120
      endif
      if (ia .lt. 0.d0) then
        if (ker(14) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPCBRX: Argument is negative.')
          ier = 14
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Check if precision level is too low to justify the advanced routine.

      if (nw .le. ncr) then
        call mpcbrt (a, b)
        goto 120
      endif
      n4 = nw + 4
      ns = 3 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4
      nws = nw

c   Determine the least integer MQ such that 2 ^ MQ .GE. NW.

      t1 = nw
      mq = cl2 * log (t1) + 1.d0 - rxx

c   Compute A^2 outside of the iteration loop.

      call mpsqx (a, s(k0))

c   Compute the initial approximation of A ^ (-2/3).

      nw = ncr
      call mpcbrt (a, s(k1))
      call mpdiv (s(k1), a, b)
      f(1) = 1.
      f(2) = 0.
      f(3) = 1.
      f(4) = 0.
      iq = 0

c   Perform the Newton-Raphson iteration described above with a dynamically
c   changing precision level NW (powers of two).

      do 110 k = mcr + 1, mq
        nw1 = nw
        nw = min (2 * nw, nws)
        nw2 = nw
 100    continue
        call mpsqx (b, s(k1))
        call mpmulx (b, s(k1), s(k2))
        call mpmulx (s(k0), s(k2), s(k1))
        call mpsub (f, s(k1), s(k2))
        nw = nw1
        call mpmulx (b, s(k2), s(k1))
        call mpdivd (s(k1), 3.d0, 0, s(k2))
        nw = nw2
        call mpadd (b, s(k2), s(k1))
        call mpeq (s(k1), b)
        if (k .eq. mq - nit .and. iq .eq. 0) then
          iq = 1
          goto 100
        endif
 110  continue

c   Perform last iteration using Karp's trick.

      call mpmulx (a, b, s(k0))
      nw1 = nw
      nw = min (2 * nw, nws)
      nw2 = nw
      call mpsqx (s(k0), s(k1))
      call mpmulx (s(k0), s(k1), s(k2))
      call mpsub (a, s(k2), s(k1))
      nw = nw1
      call mpmulx (s(k1), b, s(k2))
      call mpdivd (s(k2), 3.d0, 0, s(k1))
      nw = nw2
      call mpadd (s(k0), s(k1), s(k2))
      call mpeq (s(k2), b)
      ics = iss

 120  if (idb .ge. 6) then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 3) (b(i), i = 1, no)
 3      format ('MPCBRX O'/(6f12.0))
      endif
      return
      end

      subroutine mpcdiv (l, a, b, c)

c   This routine divides the MP complex numbers A and B to yield the MPC
c   quotient C.  L is the offset between real and imaginary parts in A, B
c   and the result C.  L must be at least NW + 4.  For extra high levels of
c   precision, use MPCDVX.  The last word is not reliable.  Debug output
c   starts with IDB = 7

c   Max SP space for C: 2 * L cells.  Max SP scratch space: 5 * NW + 20
c   cells.  Max DP scratch space: NW + 4 cells.

c   This routine employs the formula described in MPCMUL to save multiprecision
c   multiplications.

      parameter (ndb = 22)
      dimension a(2*l), b(2*l), c(2*l), f(8)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        c(l+1) = 0.
        c(l+2) = 0.
        return
      endif
      l1 = l + 1
      if (idb .ge. 7) then
        write (ldb, 1) l
 1      format ('MPCDIV I',i10)
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 2) (a(i), i = 1, no)
 2      format ('MPCDIV I'/(6f12.0))
        no = min (int (abs (a(l1))), ndb) + 2
        write (ldb, 2) (a(l+i), i = 1, no)
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 2) (b(i), i = 1, no)
        no = min (int (abs (b(l1))), ndb) + 2
        write (ldb, 2) (b(l+i), i = 1, no)
      endif

      if (l .lt. nw + 4) then
        if (ker(15) .ne. 0) then
          write (ldb, 3) l, nw + 4
 3        format ('*** MPCDIV: Offset parameter is too small',2i8)
          ier = 15
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      if (b(1) .eq. 0. .and. b(l1) .eq. 0.) then
        if (ker(16) .ne. 0) then
          write (ldb, 4)
 4        format ('*** MPCDIV: Divisor is zero.')
          ier = 16
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      n4 = nw + 4
      ns = 5 * n4
      iss = ics
      ics = ics + ns
      if (ics - 1 .gt. ims) call mpaler
      ihs = max (ics, ihs)
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4
      k3 = k2 + n4
      k4 = k3 + n4
      f(1) = 1.
      f(2) = 0.
      f(3) = 1.
      f(4) = 0.

      call mpmul (a, b, s(k0))
      call mpmul (a(l1), b(l1), s(k1))
      call mpadd (s(k0), s(k1), s(k2))
      call mpsub (s(k0), s(k1), s(k3))
      call mpadd (a, a(l1), s(k0))
      call mpsub (b, b(l1), s(k1))
      call mpmul (s(k0), s(k1), s(k4))
      call mpsub (s(k4), s(k3), s(k1))
      call mpmul (b, b, s(k0))
      call mpmul (b(l1), b(l1), s(k3))
      call mpadd (s(k0), s(k3), s(k4))
      call mpdiv (f, s(k4), s(k0))
      call mpmul (s(k2), s(k0), c)
      call mpmul (s(k1), s(k0), c(l1))
      ics = iss

      if (idb .ge. 7) then
        no = min (int (abs (c(1))), ndb) + 2
        write (ldb, 5) (c(i), i = 1, no)
 5      format ('MPCDIV O'/(6f12.0))
        no = min (int (abs (c(l1))), ndb) + 2
        write (ldb, 5) (c(l+i), i = 1, no)
      endif
      return
      end

      subroutine mpcdvx (l, a, b, c)

c   This routine divides the MP complex numbers A and B to yield the MPC
c   quotient C.  L is the offset between real and imaginary parts in A, B
c   the result C.  L must be at least NW + 4.  Before calling MPCDVX, the
c   array in MPCOM5 must be initialized by calling MPINIX.  For modest levels
c   of precision, use MPCDIV.  NW should be a power of two.  The last two
c   words are not reliable.  Debug output starts with IDB = 7

c   Max SP space for C: 2 * L cells.  Max SP scratch space: 8 * NW + 32
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   This routine employs the same scheme as MPCDIV.

      parameter (ndb = 22)
      dimension a(2*l), b(2*l), c(2*l), f(8)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        c(l+1) = 0.
        c(l+2) = 0.
        return
      endif
      l1 = l + 1
      if (idb .ge. 7) then
        write (ldb, 1) l
 1      format ('MPCDVX I',i10)
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 2) (a(i), i = 1, no)
 2      format ('MPCDVX I'/(6f12.0))
        no = min (int (abs (a(l1))), ndb) + 2
        write (ldb, 2) (a(l+i), i = 1, no)
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 2) (b(i), i = 1, no)
        no = min (int (abs (b(l1))), ndb) + 2
        write (ldb, 2) (b(l+i), i = 1, no)
      endif

      if (l .lt. nw + 4) then
        if (ker(17) .ne. 0) then
          write (ldb, 3) l, nw + 4
 3        format ('*** MPCDVX: Offset parameter is too small',2i8)
          ier = 17
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      if (b(1) .eq. 0. .and. b(l1) .eq. 0.) then
        if (ker(18) .ne. 0) then
          write (ldb, 4)
 4        format ('*** MPCDVX: Divisor is zero.')
          ier = 18
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      n4 = nw + 4
      ns = 5 * n4
      iss = ics
      ics = ics + ns
      if (ics - 1 .gt. ims) call mpaler
      ihs = max (ics, ihs)
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4
      k3 = k2 + n4
      k4 = k3 + n4
      f(1) = 1.
      f(2) = 0.
      f(3) = 1.
      f(4) = 0.

      call mpmulx (a, b, s(k0))
      call mpmulx (a(l1), b(l1), s(k1))
      call mpadd (s(k0), s(k1), s(k2))
      call mpsub (s(k0), s(k1), s(k3))
      call mpadd (a, a(l1), s(k0))
      call mpsub (b, b(l1), s(k1))
      call mpmulx (s(k0), s(k1), s(k4))
      call mpsub (s(k4), s(k3), s(k1))
      call mpsqx (b, s(k0))
      call mpsqx (b(l1), s(k3))
      call mpadd (s(k0), s(k3), s(k4))
      call mpdivx (f, s(k4), s(k0))
      call mpmul (s(k2), s(k0), c)
      call mpmul (s(k1), s(k0), c(l1))
      ics = iss

      if (idb .ge. 7) then
        no = min (int (abs (c(1))), ndb) + 2
        write (ldb, 5) (c(i), i = 1, no)
 5      format ('MPCDVX O'/(6f12.0))
        no = min (int (abs (c(l1))), ndb) + 2
        write (ldb, 5) (c(l+i), i = 1, no)
      endif
      return
      end

      subroutine mpceq (l, a, b)

c   This sets the MPC number B equal to the MPC number A.  L is the offset
c   between real and imaginary parts in A and B.  Debug output starts with
c   IDB = 10.

c   Max SP space for B: 2 * L cells.

      dimension a(2*l), b(2*l)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        b(l+1) = 0.
        b(l+2) = 0.
        return
      endif
      if (idb .ge. 10) write (ldb, 1)
 1    format ('MPCEQ')

      i1 = sign (1., a(1))
      n1 = min (int (abs (a(1))), nw, l - 2)
      i2 = sign (1., a(l+1))
      n2 = min (int (abs (a(l+1))), nw, l - 2)
      b(1) = sign (n1, i1)
      b(l+1) = sign (n2, i2)

      do 100 i = 2, n1 + 2
        b(i) = a(i)
 100  continue

      do 110 i = 2, n2 + 2
        b(l+i) = a(l+i)
 110  continue

      return
      end

      subroutine mpcfft (is, m, x, y)

c   This routine computes the 2^M -point complex-to-complex FFT of X.  See
c   article by DHB in Intl. J. of Supercomputer Applications, Spring 1988,
c   p. 82 - 87).  X and Y are double precision.  X is both the input and the
c   output array, while Y is a scratch array.  Both X and Y must be
c   dimensioned with 2 * N cells, where N = 2^M.  The data in X are assumed
c   to have real and imaginary parts separated by N cells.  A call to MPCFFT
c   with IS = 1 (or -1) indicates a call to perform a FFT with positive (or
c   negative) exponentials.  M must be at least two.  Before calling MPCRFT,
c   the array in MPCOM5 must be initialized by calling MPINIX.

c   In this application, MPCFFT is called by MPRCFT and MPCRFT, which are in
c   turn called by MPMULX.  This routine is not intended to be called directly
c   by the user.

      implicit double precision (a-h, o-z)
      dimension x(*), y(*)

      n = 2 ** m
c>
c   For Cray vector computers, it is most efficient to limit M1 to 6.  For most
c   scalar computers, it is best to limit M1 to 2.  Uncomment whichever of the
c   next two lines is appropriate.

c      m1 = min (m / 2, 6)
      m1 = min (m / 2, 2)
      m2 = m - m1
      n2 = 2 ** m1
      n1 = 2 ** m2

c   Perform one variant of the Stockham FFT.

      do 100 l = 1, m1, 2
        call mpfft1 (is, l, m, x, y)
        if (l .eq. m1) goto 120
        call mpfft1 (is, l + 1, m, y, x)
 100  continue

c   Perform a transposition of X treated as a N2 x N1 x 2 matrix.

      call mptran (n1, n2, x, y)

c   Perform second variant of the Stockham FFT from Y to X and X to Y.

      do 110 l = m1 + 1, m, 2
        call mpfft2 (is, l, m, y, x)
        if (l .eq. m) goto 160
        call mpfft2 (is, l + 1, m, x, y)
 110  continue

      goto 140

c   Perform a transposition of Y treated as a N2 x N1 x 2 matrix.

 120  call mptran (n1, n2, y, x)

c   Perform second variant of the Stockham FFT from X to Y and Y to X.

      do 130 l = m1 + 1, m, 2
        call mpfft2 (is, l, m, x, y)
        if (l .eq. m) goto 140
        call mpfft2 (is, l + 1, m, y, x)
 130  continue

      goto 160

c   Copy Y to X.

 140  do 150 i = 1, 2 * n
        x(i) = y(i)
 150  continue

 160  return
      end

      subroutine mpcmlx (l, a, b, c)

c   This routine multiplies the MP complex numbers A and B to yield the MPC
c   product C.  L is the offset between real and imaginary parts in A, B and
c   the result C.  L must be at least NW + 4.  Before calling MPCMLX, the
c   array in MPCOM5 must be initialized by calling MPINIX.  For modest levels
c   of precision, use MPCMUL.  NW should be a power of two.  The last word is
c   not reliable.  Debug output starts with IDB = 7.

c   Max SP space for C: 2 * L cells.  Max SP scratch space: 4 * NW + 16
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   This routine employs the same scheme as MPCMUL.

      parameter (ndb = 22)
      dimension a(2*l), b(2*l), c(2*l)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        c(l+1) = 0.
        c(l+2) = 0.
        return
      endif
      l1 = l + 1
      if (idb .ge. 7) then
        write (ldb, 1) l
 1      format ('MPCMLX I',i10)
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 2) (a(i), i = 1, no)
 2      format ('MPCMLX I'/(6f12.0))
        no = min (int (abs (a(l1))), ndb) + 2
        write (ldb, 2) (a(l+i), i = 1, no)
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 2) (b(i), i = 1, no)
        no = min (int (abs (b(l1))), ndb) + 2
        write (ldb, 2) (b(l+i), i = 1, no)
      endif

      if (l .lt. nw + 4) then
        if (ker(19) .ne. 0) then
          write (ldb, 3) l, nw + 4
 3        format ('*** MPCMLX: Offset parameter is too small',2i8)
          ier = 19
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      n4 = nw + 4
      ns = 4 * n4
      iss = ics
      ics = ics + ns
      if (ics - 1 .gt. ims) call mpaler
      ihs = max (ics, ihs)
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4
      k3 = k2 + n4

      call mpmulx (a, b, s(k0))
      call mpmulx (a(l1), b(l1), s(k1))
      call mpsub (s(k0), s(k1), c)
      call mpadd (s(k0), s(k1), s(k2))
      call mpadd (a, a(l1), s(k0))
      call mpadd (b, b(l1), s(k1))
      call mpmulx (s(k0), s(k1), s(k3))
      call mpsub (s(k3), s(k2), c(l1))
      ics = iss

      if (idb .ge. 7) then
        no = min (int (abs (c(1))), ndb) + 2
        write (ldb, 4) (c(i), i = 1, no)
 4      format ('MPCMLX O'/(6f12.0))
        no = min (int (abs (c(l1))), ndb) + 2
        write (ldb, 4) (c(l+i), i = 1, no)
      endif
      return
      end

      subroutine mpcmul (l, a, b, c)

c   This routine multiplies the MP complex numbers A and B to yield the MPC
c   product C.  L is the offset between real and imaginary parts in A, B and
c   the result C.  L must be at least NW + 4.  For extra high levels of
c   precision, use MPCMLX.  The last word is not reliable.  Debug output
c   starts with IDB = 7.

c   Max SP space for C: 2 * L cells.  Max SP scratch space: 4 * NW + 16
c   cells.  Max DP scratch space: NW + 4 cells.

c   This routine employs the formula

c   (a_1 + a_2 i) (b_1 + b_2 i)  =  [a_1 b_1 - a_2 b_2]  +
c                [(a_1 + a_2) (b_1 + b_2) - (a_1 b_1 + a_2 b_2)] i

c   Note that this formula can be implemented with only three multiplications
c   whereas the conventional formula requires four.

      parameter (ndb = 22)
      dimension a(2*l), b(2*l), c(2*l)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        c(l+1) = 0.
        c(l+2) = 0.
        return
      endif
      l1 = l + 1
      if (idb .ge.7) then
        write (ldb, 1) l
 1      format ('MPCMUL I',i10)
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 2) (a(i), i = 1, no)
 2      format ('MPCMUL I'/(6f12.0))
        no = min (int (abs (a(l1))), ndb) + 2
        write (ldb, 2) (a(l+i), i = 1, no)
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 2) (b(i), i = 1, no)
        no = min (int (abs (b(l1))), ndb) + 2
        write (ldb, 2) (b(l+i), i = 1, no)
      endif

      if (l .lt. nw + 4) then
        if (ker(20) .ne. 0) then
          write (ldb, 3) l, nw + 4
 3        format ('*** MPCMUL: Offset parameter is too small',2i8)
          ier = 20
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      n4 = nw + 4
      ns = 4 * n4
      iss = ics
      ics = ics + ns
      if (ics - 1 .gt. ims) call mpaler
      ihs = max (ics, ihs)
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4
      k3 = k2 + n4

      call mpmul (a, b, s(k0))
      call mpmul (a(l1), b(l1), s(k1))
      call mpsub (s(k0), s(k1), c)
      call mpadd (s(k0), s(k1), s(k2))
      call mpadd (a, a(l1), s(k0))
      call mpadd (b, b(l1), s(k1))
      call mpmul (s(k0), s(k1), s(k3))
      call mpsub (s(k3), s(k2), c(l1))
      ics = iss

      if (idb .ge. 7) then
        no = min (int (abs (c(1))), ndb) + 2
        write (ldb, 4) (c(i), i = 1, no)
 4      format ('MPCMUL O'/(6f12.0))
        no = min (int (abs (c(l1))), ndb) + 2
        write (ldb, 4) (c(l+i), i = 1, no)
      endif
      return
      end

      subroutine mpcplx (n, la, a, x1, nx, lx, x)

c   This routine finds a complex root of the N-th degree polynomial whose
c   MPC coefficients are in A by Newton-Raphson iterations, beginning
c   at the complex DPE value (X1(1), NX(1)) + i (X1(2), NX(2)), and returns
c   the MPC root in X.  The N + 1 coefficients a_0, a_1, ..., a_N are
c   assumed to start in locations A(1), A(2*LA+1), A(4*LA+1), etc.  LA is the
c   offset between the real and the imaginary parts of each input coefficient.
c   Typically LA = NW + 4.  LX, also an input parameter, is the offset between
c   the real and the imaginary parts of the result to be stored in X.  LX
c   should be at least NW + 4.  Before calling MPCPLX, the array in MPCOM5
c   be initialized by calling MPINIX.  For modest levels of precision, use
c   MPCPOL.  NW should be a power of two.  The last two words of the result
c   are not reliable.  Debug output starts with IDB = 5.

c   Max SP space for X: 2 * LX cells.  Max SP scratch space: 18 * NW + 72
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   See the note in MPPOL about repeated roots.

c   This routine employs the same scheme as MPCPOL.

      character*8 cx
      double precision t1, x1
      dimension a(2*la,n+1), nx(2), x(2*lx), x1(2)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        x(1) = 0.
        x(2) = 0.
        x(lx+1) = 0.
        x(lx+2) = 0.
      endif
      if (idb .ge. 5) then
        write (ldb, 1) n, lx
 1      format ('MPCPLX I',2i6)

        do 100 k = 0, n
          write (cx, '(I4)') k
          call mpdeb (cx, a(1,k+1))
          call mpdeb (cx, a(la+1,k+1))
 100    continue

        write (ldb, 2) x1(2), nx(2)
 2      format ('MPCPLX I',f16.12,' x 10 ^',i6,f20.12,' x 10^',i6)
      endif

c   Check if precision level is too low to justify the advanced routine.

      ncr = 2 ** mcr
      if (nw .le. ncr) then
        call mpcpol (n, la, a, x1, nx, lx, x)
        l1 = 0
        goto 150
      endif

c   Check if the polynomial is proper.

      if (a(1,1) .eq. 0. .or. a(1,n+1) .eq. 0.) then
        if (ker(21) .ne. 0) then
          write (ldb, 3)
 3        format ('*** MPCPLX: Either the first or last input ',
     $      'coefficient is zero.')
          ier = 21
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      n4 = nw + 4
      n8 = 2 * n4
      ns = 10 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n8
      k2 = k1 + n8
      k3 = k2 + n8
      k4 = k3 + n8
      nws = nw

c   Set the initial value.

      nw = ncr
      call mpcpol (n, la, a, x1, nx, n4, s(k0))
      tl = -4.
      l1 = 0
      ls = -10

c   Perform MP Newton-Raphson iterations to solve P(x) = 0.

 110  l1 = l1 + 1
      if (l1 .eq. 50) then
        if (ker(22) .ne. 0) then
          write (ldb, 4)
 4        format ('*** MPCPLX: Iteration limit exceeded.')
          ier = 22
          if (ker(ier) .eq. 2) call mpabrt
          ics = iss
          nw = nws
          return
        endif
      endif

c   Compute P(x).

      call mpmmpc (a(1,n+1), a(la+1,n+1), n4, s(k1))

      do 120 k = n - 1, 0, -1
        call mpcmlx (n4, s(k0), s(k1), s(k2))
        call mpadd (s(k2), a(1,k+1), s(k1))
        call mpadd (s(k2+n4), a(la+1,k+1), s(k1+n4))
 120  continue

c   Compute P'(x).

      t1 = n
      call mpmuld (a(1,n+1), t1, 0, s(k2))
      call mpmuld (a(la+1,n+1), t1, 0, s(k2+n4))

      do 130 k = n - 1, 1, -1
        call mpcmlx (n4, s(k0), s(k2), s(k3))
        t1 = k
        call mpmuld (a(1,k+1), t1, 0, s(k4))
        call mpmuld (a(la+1,k+1), t1, 0, s(k4+n4))
        call mpcadd (n4, s(k3), s(k4), s(k2))
 130  continue

c   Compute P(x) / P'(x) and update x.

      call mpcdvx (n4, s(k1), s(k2), s(k3))
      call mpcsub (n4, s(k0), s(k3), s(k4))

      if (idb .ge. 6) then
        write (ldb, 5) l1
 5      format ('ITERATION',i4)
        call mpdeb ('X', s(k0))
        call mpdeb (' ', s(k0+n4))
        call mpdeb ('P(X)', s(k1))
        call mpdeb (' ', s(k1+n4))
        call mpdeb ('P''(X)', s(k2))
        call mpdeb (' ', s(k2+n4))
        call mpdeb ('CORR', s(k3))
        call mpdeb (' ', s(k3+n4))
      endif
      call mpceq (n4, s(k4), s(k0))

c   If this was the second iteration at full precision, there is no need to
c   continue (the adjusted value of x is correct); otherwise repeat.

      if (l1 .eq. ls + 1) goto 140
      if (s(k3) .ne. 0. .and. s(k3+1) .gt. tl .or. s(k3+n4) .ne. 0.
     $  .and. s(k3+n4+1) .gt. tl) goto 110

c   Newton iterations have converged to current precision.  Increase precision
c   and continue.

      if (nw .eq. nws) goto 140
      nw = min (2 * nw, nws)
      if (nw .eq. nws) ls = l1
      if (nw .le. 32) then
        tl = 2 - nw
      elseif (nw .le. 256) then
        tl = 3 - nw
      else
        tl = 4 - nw
      endif
      if (idb .ge. 6) then
        write (ldb, 6) nw
 6      format (6x,'New NW =', i8)
      endif
      goto 110

 140  call mpmmpc (s(k0), s(k0+n4), lx, x)
      ics = iss

 150  if (idb .ge. 5) then
        write (ldb, 7) l1
 7      format ('Iteration count:',i5)
        call mpdeb ('MPCPLX O', x)
        call mpdeb (' ', x(lx+1))
      endif
      return
      end

      subroutine mpcpol (n, la, a, x1, nx, lx, x)

c   This routine finds a complex root of the N-th degree polynomial whose
c   MPC coefficients are in A by Newton-Raphson iterations, beginning
c   at the complex DPE value (X1(1), NX(1)) + i (X1(2), NX(2)), and returns
c   the MPC root in X.  The N + 1 coefficients a_0, a_1, ..., a_N are
c   assumed to start in locations A(1), A(2*LA+1), A(4*LA+1), etc.  LA is the
c   offset between the real and the imaginary parts of each input coefficient.
c   Typically LA = NW + 4.  LX, also an input parameter, is the offset between
c   the real and the imaginary parts of the result to be stored in X.  LX must
c   be at least NW + 4.  For extra high levels of precision, use MPCPLX.
c   Debug output starts with IDB = 5.

c   Max SP space for X: 2 * LX cells.  Max SP scratch space: 15 * NW + 75
c   cells.  Max DP scratch space: NW + 5 cells.

c   See the note about repeated roots in MPPOL.

c   This routine employs the complex form of the Newton-Raphson iteration:

c   X_{k+1} = X_k - P(X_k) / P'(X_k)

c   These iterations are performed with a maximum precision level NW that is
c   dynamically changed, approximately doubling with each iteration.

      character*8 cx
      double precision t1, x1
      dimension a(2*la,n+1), nx(2), x(2*lx), x1(2)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        x(1) = 0.
        x(2) = 0.
        x(lx+1) = 0.
        x(lx+2) = 0.
      endif
      if (idb .ge. 5) then
        write (ldb, 1) n, lx
 1      format ('MPCPOL I',2i6)

        do 100 k = 0, n
          write (cx, '(I4)') k
          call mpdeb (cx, a(1,k+1))
          call mpdeb (cx, a(la+1,k+1))
 100    continue

        write (ldb, 2) x1(1), nx(1), x1(2), nx(2)
 2      format ('MPCPOL I',f16.12,' x 10 ^',i6,f20.12,' x 10^',i6)
      endif

c  Check if the polynomial is proper.

      if (a(1,1) .eq. 0. .or. a(1,n+1) .eq. 0.) then
        if (ker(23) .ne. 0) then
          write (ldb, 3)
 3        format ('*** MPCPOL: Either the first or last input ',
     $      'coefficient is zero.')
          ier = 23
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      n5 = nw + 5
      n10 = 2 * n5
      ns = 10 * n5
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n10
      k2 = k1 + n10
      k3 = k2 + n10
      k4 = k3 + n10
      nws = nw
      nw = nw + 1

c   Set the initial value.

      call mpdmc (x1(1), nx(1), s(k0))
      call mpdmc (x1(2), nx(2), s(k0+n5))
      nw = 5
      tl = -4.
      l1 = 0
      ls = -10

c   Perform MP Newton-Raphson iterations to solve P(x) = 0.

 110  l1 = l1 + 1
      if (l1 .eq. 50) then
        if (ker(24) .ne. 0) then
          write (ldb, 4)
 4        format ('*** MPCPOL: Iteration limit exceeded.')
          ier = 24
          if (ker(ier) .eq. 2) call mpabrt
          ics = iss
          nw = nws
          return
        endif
      endif

c   Compute P(x).

      call mpmmpc (a(1,n+1), a(la+1,n+1), n5, s(k1))

      do 120 k = n - 1, 0, -1
        call mpcmul (n5, s(k0), s(k1), s(k2))
        call mpadd (s(k2), a(1,k+1), s(k1))
        call mpadd (s(k2+n5), a(la+1,k+1), s(k1+n5))
 120  continue

c   Compute P'(x).

      t1 = n
      call mpmuld (a(1,n+1), t1, 0, s(k2))
      call mpmuld (a(la+1,n+1), t1, 0, s(k2+n5))

      do 130 k = n - 1, 1, -1
        call mpcmul (n5, s(k0), s(k2), s(k3))
        t1 = k
        call mpmuld (a(1,k+1), t1, 0, s(k4))
        call mpmuld (a(la+1,k+1), t1, 0, s(k4+n5))
        call mpcadd (n5, s(k3), s(k4), s(k2))
 130  continue

c   Compute P(x) / P'(x) and update x.

      call mpcdiv (n5, s(k1), s(k2), s(k3))
      call mpcsub (n5, s(k0), s(k3), s(k4))

      if (idb .ge. 6) then
        write (ldb, 5) l1
 5      format ('Iteration',i4)
        call mpdeb ('X', s(k0))
        call mpdeb (' ', s(k0+n5))
        call mpdeb ('P(X)', s(k1))
        call mpdeb (' ', s(k1+n5))
        call mpdeb ('P''(X)', s(k2))
        call mpdeb (' ', s(k2+n5))
        call mpdeb ('CORR', s(k3))
        call mpdeb (' ', s(k3+n5))
      endif
      call mpceq (n5, s(k4), s(k0))

c   If this was the second iteration at full precision, there is no need to
c   continue (the adjusted value of x is correct); otherwise repeat.

      if (l1 .eq. ls + 1) goto 140
      if (s(k3) .ne. 0. .and. s(k3+1) .gt. tl .or. s(k3+n5) .ne. 0.
     $  .and. s(k3+n5+1) .gt. tl) goto 110

c   Newton iterations have converged to current precision.  Increase precision
c   and continue.

      if (nw .eq. nws + 1) goto 140
      nw = min (2 * nw - 2, nws) + 1
      if (nw .eq. nws + 1) ls = l1
      tl = 1 - nw
      if (idb .ge. 6) then
        write (ldb, 6) nw
 6      format (6x,'New NW =', i8)
      endif
      goto 110

 140  call mpmmpc (s(k0), s(k0+n5), lx, x)

c   Restore original precision level.

      nw = nws
      ics = iss
      call mproun (x)
      call mproun (x(lx+1))

      if (idb .ge. 5) then
        write (ldb, 7) l1
 7      format ('Iteration count:',i5)
        call mpdeb ('MPCPOL O', x)
        call mpdeb (' ', x(lx+1))
      endif
      return
      end

      subroutine mpcpr (a, b, ic)

c   This routine compares the MP numbers A and B and returns in IC the value
c   -1, 0, or 1 depending on whether A < B, A = B, or A > B.  It is faster
c   than merely subtracting A and B and looking at the sign of the result.
c   Debug output begins with IDB = 9.

      dimension a(nw+4), b(nw+4)
      parameter (ndb = 22)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        ic = 0
        return
      endif
      if (idb .ge. 9) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPCPR I'/(6f12.0))
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 1) (b(i), i = 1, no)
      endif
      ia = sign (1., a(1))
      if (a(1) .eq. 0.) ia = 0
      ib = sign (1., b(1))
      if (b(1) .eq. 0.) ib = 0

c   Compare signs.

      if (ia .ne. ib) then
        ic = sign (1, ia - ib)
        goto 110
      endif

c   The signs are the same.  Compare exponents.

      ma = a(2)
      mb = b(2)
      if (ma .ne. mb) then
        ic = ia * sign (1, ma - mb)
        goto 110
      endif

c   The signs and the exponents are the same.  Compare mantissas.

      na = min (int (abs (a(1))), nw)
      nb = min (int (abs (b(1))), nw)

      do 100 i = 3, min (na, nb) + 2
        if (a(i) .ne. b(i)) then
          ic = ia * sign (1., a(i) - b(i))
          goto 110
        endif
 100  continue

c   The mantissas are the same to the common length.  Compare lengths.

      if (na .ne. nb) then
        ic = ia * sign (1, na - nb)
        goto 110
      endif

c   The signs, exponents, mantissas and lengths are the same.  Thus A = B.

      ic = 0

 110  if (idb .ge. 9) write (ldb, 2) ic
 2    format ('MPCPR O',i4)
      return
      end

      subroutine mpcpwr (l, a, n, b)

c   This computes the N-th power of the MPC number A and returns the MPC
c   result C in B.  When N is zero, 1 is returned.  When N is negative, the
c   reciprocal of A ^ |N| is returned.  L is the offset between real and
c   imaginary parts in A and B.  L should be at least NW + 4.  For extra high
c   levels of precision, use MPCPWX.  Debug output starts with IDB = 7.

c   Max SP space for B: 2 * L cells.  Max SP scratch space: 11 * NW + 55
c   cells.  Max DP scratch space: NW + 5 cells.

c   This routine employs the binary method for exponentiation.

      double precision cl2, t1
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0, ndb = 22)
      dimension a(2*l), b(2*l), f1(8), f2(8)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        b(l+1) = 0.
        b(l+2) = 0.
        return
      endif
      l1 = l + 1
      if (idb .ge. 7) then
        write (ldb, 1) l, n
 1      format ('MPCPWR I',2i10)
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 2) (a(i), i = 1, no)
 2      format ('MPCPWR I'/(6f12.0))
        no = min (int (abs (a(l1))), ndb) + 2
        write (ldb, 2) (a(l+i), i = 1, no)
      endif

      na1 = min (int (abs (a(1))), nw)
      na2 = min (int (abs (a(l1))), nw)
      if (na1 .eq. 0 .and. na2 .eq. 0) then
        if (n .ge. 0) then
          b(1) = 0.
          b(2) = 0.
          b(l1) = 0.
          b(l1+1) = 0.
          goto 120
        else
          if (ker(25) .ne. 0) then
            write (ldb, 3)
 3          format ('*** MPCPWR: Argument is zero and N is negative or',
     $        ' zero.')
            ier = 25
            if (ker(ier) .eq. 2) call mpabrt
          endif
          return
        endif
      endif

      n5 = nw + 5
      ns = 6 * n5
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + 2 * n5
      k2 = k1 + 2 * n5
      nws = nw
      nw = nw + 1
      nn = abs (n)
      f1(1) = 1.
      f1(2) = 0.
      f1(3) = 1.
      f1(4) = 0.
      f2(1) = 0.
      f2(2) = 0.
      call mpmmpc (a, a(l1), n5, s(k0))
      if (nn .eq. 0) then
        call mpmmpc (f1, f2, l, b)
        nw = nws
        ics = iss
        goto 120
      elseif (nn .eq. 1) then
        call mpceq (n5, s(k0), s(k2))
        goto 110
      elseif (nn .eq. 2) then
        call mpcmul (n5, s(k0), s(k0), s(k2))
        goto 110
      endif

c   Determine the least integer MN such that 2 ^ MN .GT. NN.

      t1 = nn
      mn = cl2 * log (t1) + 1.d0 + rxx
      call mpmmpc (f1, f2, n5, s(k2))
      kn = nn

c   Compute B ^ N using the binary rule for exponentiation.

      do 100 j = 1, mn
        kk = kn / 2
        if (kn .ne. 2 * kk) then
          call mpcmul (n5, s(k2), s(k0), s(k1))
          call mpceq (n5, s(k1), s(k2))
        endif
        kn = kk
        if (j .lt. mn) then
          call mpcmul (n5, s(k0), s(k0), s(k1))
          call mpceq (n5, s(k1), s(k0))
        endif
 100  continue

c   Compute reciprocal if N is negative.

 110  if (n .lt. 0) then
        call mpmmpc (f1, f2, n5, s(k1))
        call mpcdiv (n5, s(k1), s(k2), s(k0))
        call mpceq (n5, s(k0), s(k2))
      endif
      call mpmmpc (s(k2), s(n5+k2), l, b)

c   Restore original precision level.

      nw = nws
      ics = iss
      call mproun (b)
      call mproun (b(l1))

 120  if (idb .ge. 7) then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 4) (b(i), i = 1, no)
 4      format ('MPCPWR O'/(6f12.0))
        no = min (int (abs (b(l1))), ndb) + 2
        write (ldb, 4) (b(l+i), i = 1, no)
      endif
      return
      end

      subroutine mpcpwx (l, a, n, b)

c   This computes the N-th power of the MPC number A and returns the MPC
c   result C in B.  When N is zero, 1 is returned.  When N is negative, the
c   reciprocal of A ^ |N| is returned.  L is the offset between real and
c   imaginary parts in A and B.  L should be at least NW + 4.  Before calling
c   MPCPWX, the array in MPCOM5 must be initialized by calling MPINIX.  For
c   modest levels of precision, use MPCPWR.  NW should be a power of two.
c   The last two words of the result are not reliable.  Debug output starts
c   with IDB = 6.

c   Max SP space for B: 2 * L cells.  Max SP scratch space: 14 * NW + 56
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   This routine employs the binary method for exponentiation.

      double precision cl2, t1
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0, ndb = 22)
      dimension a(2*l), b(2*l), f1(8), f2(8)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        b(l+1) = 0.
        b(l+2) = 0.
        return
      endif
      l1 = l + 1
      if (idb .ge. 6) then
        write (ldb, 1) l, n
 1      format ('MPCPWX I',2i10)
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 2) (a(i), i = 1, no)
 2      format ('MPCPWX I'/(6f12.0))
        no = min (int (abs (a(l1))), ndb) + 2
        write (ldb, 2) (a(l+i), i = 1, no)
      endif

      na1 = min (int (abs (a(1))), nw)
      na2 = min (int (abs (a(l1))), nw)
      ncr = 2 ** mcr

c   Check if precision level of A is too low to justify advanced routine.

      if (na1 .le. ncr .and. na2 .le. ncr) then
        call mpcpwr (l, a, n, b)
        goto 120
      endif
      if (na1 .eq. 0 .and. na2 .eq. 0) then
        if (n .ge. 0) then
          b(1) = 0.
          b(2) = 0.
          b(l1) = 0.
          b(l1+1) = 0.
          goto 120
        else
          if (ker(26) .ne. 0) then
            write (ldb, 3)
 3          format ('*** MPCPWX: Argument is zero and N is negative or',
     $        ' zero.')
            ier = 26
            if (ker(ier) .eq. 2) call mpabrt
          endif
          return
        endif
      endif

      n4 = nw + 4
      ns = 6 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + 2 * n4
      k2 = k1 + 2 * n4
      nn = abs (n)
      f1(1) = 1.
      f1(2) = 0.
      f1(3) = 1.
      f1(4) = 0.
      f2(1) = 0.
      f2(2) = 0.
      call mpmmpc (a, a(l1), n4, s(k0))
      if (nn .eq. 0) then
        call mpmmpc (f1, f2, l, b)
        ics = iss
        goto 120
      elseif (nn .eq. 1) then
        call mpceq (n4, s(k0), s(k2))
        goto 110
      elseif (nn .eq. 2) then
        call mpcmlx (n4, s(k0), s(k0), s(k2))
        goto 110
      endif

c   Determine the least integer MN such that 2 ^ MN .GT. NN.

      t1 = nn
      mn = cl2 * log (t1) + 1.d0 + rxx
      call mpmmpc (f1, f2, n4, s(k2))
      kn = nn

c   Compute B ^ N using the binary rule for exponentiation.

      do 100 j = 1, mn
        kk = kn / 2
        if (kn .ne. 2 * kk) then
          call mpcmlx (n4, s(k2), s(k0), s(k1))
          call mpceq (n4, s(k1), s(k2))
        endif
        kn = kk
        if (j .lt. mn) then
          call mpcmlx (n4, s(k0), s(k0), s(k1))
          call mpceq (n4, s(k1), s(k0))
        endif
 100  continue

c   Compute reciprocal if N is negative.

 110  if (n .lt. 0) then
        call mpmmpc (f1, f2, n4, s(k1))
        call mpcdvx (n4, s(k1), s(k2), s(k0))
        call mpceq (n4, s(k0), s(k2))
      endif
      call mpmmpc (s(k2), s(n4+k2), l, b)
      ics = iss

 120  if (idb .ge. 6) then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 4) (b(i), i = 1, no)
 4      format ('MPCPWX O'/(6f12.0))
        no = min (int (abs (b(l1))), ndb) + 2
        write (ldb, 4) (b(l+i), i = 1, no)
      endif
      return
      end

      subroutine mpcrft (is, m, x, y)

c   This performs an N-point complex-to-real FFT, where N = 2^M.  X and Y
c   are double precision arrays.  X is both the input and the output data
c   array, and Y is a scratch array.  N/2 + 1 complex pairs are input, with
c   real and imaginary parts separated by N/2 + 1 locations, and N real
c   values are output .  A call to MPCRFT with IS = 1 (or -1) indicates a call
c   to perform a complex-to-real FFT with positive (or negative) exponentials.
c   M must be at least three.  The arrays X and Y must be dimensioned with
c   N + 2 cells.  Before calling MPCRFT, the U array in MPCOM5 must be
c   initialized by calling MPINIX.

c   In this application, MPCRFT is called by MPMULX.  This routine is not
c   intended to be called directly by the user.

      implicit double precision (a-h, o-z)
      dimension x(*), y(*)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom5/ u(4100)

c   Set initial parameters.

      k = u(1)
      mx = mod (k, 64)
      nu = k / 64
      n = 2 ** m
      n2 = n / 2
      n21 = n2 + 1
      n4 = n / 4
      ku = n / 2
      kn = ku + nu

c   Check if input parameters are invalid.

      if ((is .ne. 1 .and. is .ne. -1) .or. m .lt. 3 .or. m .gt. mx)
     $  then
        if (ker(27) .ne. 0) then
          write (ldb, 1)  is, m, mx
 1        format ('*** MPCRFT: Either U has not been initialized'/
     $      'or else one of the input parameters is invalid', 3i5)
          ier = 27
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Construct the input to MPCFFT.

      y(1) = 0.5d0 * (x(1) + x(n21))
      y(n2+1) = 0.5d0 * (x(1) - x(n21))
      y(n4+1) = x(n4+1)
      y(n4+n2+1) = -is * x(n4+n2+2)

cdir$ ivdep
      do 100 k = 2, n4
        x11 = x(k)
        x12 = x(k+n21)
        x21 = x(n2+2-k)
        x22 = x(n+3-k)
        a1 = x11 + x21
        a2 = x11 - x21
        b1 = x12 + x22
        b2 = x12 - x22
        u1 = u(k+ku)
        u2 = is * u(k+kn)
        t1 = u1 * b1 + u2 * a2
        t2 = u1 * a2 - u2 * b1
        y(k) = 0.5d0 * (a1 - t1)
        y(k+n2) = 0.5d0 * (b2 + t2)
        y(n2+2-k) = 0.5d0 * (a1 + t1)
        y(n+2-k) = 0.5d0 * (-b2 + t2)
 100  continue

c   Perform a normal N/2-point FFT on Y.

      call mpcfft (is, m - 1, y, x)

c   Copy Y to X such that Y(k) = X(2k-1) + i X(2k).

cdir$ ivdep
      do 110 k = 1, n2
        x(2*k-1) = y(k)
        x(2*k) = y(k+n2)
 110  continue

      return
      end

      subroutine mpcshx (a, pi, al2, x, y)

c   This computes the hyperbolic cosine and sine of the MP number A and
c   returns the two MP results in X and Y, respectively.  PI is the MP value
c   of Pi computed by a previous call to MPPI or MPPIX.  AL2 is the MP value
c   of Log (10) computed by a previous call to MPLOG or MPLOGX.  Before
c   calling MPCSHX, the array in MPCOM5 must be initialized by calling MPINIX.
c   For modest levels of precision, use MPCSSH.  NW should be a power of two.
c   The last four words of the result are not reliable.  Debug output starts
c   with IDB = 5.

c   Max SP space for X and Y: NW + 4 cells.  Max SP scratch space:16.5*NW + 75
c   cells.  Max DP scratch space: 12 * NX + 6 cells.

      dimension a(nw+2), f(8), al2(nw+2), pi(nw+2), x(nw+4), y(nw+4)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        x(1) = 0.
        x(2) = 0.
        y(1) = 0.
        y(2) = 0.
        return
      endif
      if (idb .ge. 5) call mpdeb ('MPCSHX I', a)

      n4 = nw + 4
      ns = 3 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4
      f(1) = 1.
      f(2) = 0.
      f(3) = 1.
      f(4) = 0.

      call mpexpx (a, pi, al2, s(k0))
      call mpdivx (f, s(k0), s(k1))
      call mpadd (s(k0), s(k1), s(k2))
      call mpmuld (s(k2), 0.5d0, 0, x)
      call mpsub (s(k0), s(k1), s(k2))
      call mpmuld (s(k2), 0.5d0, 0, y)
      ics = iss

      if (idb .ge. 5) then
        call mpdeb ('MPCSHX O', x)
        call mpdeb ('MPCSHX O', y)
      endif
      return
      end

      subroutine mpcsqr (l, a, b)

c   This routine computes the complex square root of the MPC number C.  L is
c   the offset between real and imaginary parts in A and B.  L must be at
c   least NW + 4.  For extra high levels of precision, use MPCSQX.  The last
c   word is not reliable.  Debug output starts with IDB = 6.

c   Max SP space for B: 2 * L cells.  Max SP scratch space: 6 * NW + 27
c   cells.  Max DP scratch space: NW + 5 cells.

c   This routine uses the following formula, where A1 and A2 are the real and
c   imaginary parts of A, and where R = Sqrt [A1 ^ 2 + A2 ^2]:

c      B = Sqrt [(R + A1) / 2] + I Sqrt [(R - A1) / 2]

c   If the imaginary part of A is < 0, then the imaginary part of B is also
c   set to be < 0.

      parameter (ndb = 22)
      dimension a(2*l), b(2*l)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        b(l+1) = 0.
        b(l+2) = 0.
        return
      endif
      l1 = l + 1
      if (idb .ge. 6) then
        write (ldb, 1) l
 1      format ('MPCSQR I',i10)
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 2) (a(i), i = 1, no)
 2      format ('MPCSQR I'/(6f12.0))
        no = min (int (abs (a(l1))), ndb) + 2
        write (ldb, 2) (a(l+i), i = 1, no)
      endif

      if (a(1) .eq. 0. .and. a(l+1) .eq. 0.) then
        b(1) = 0.
        b(2) = 0.
        b(l+1) = 0.
        b(l+2) = 0.
        goto 100
      endif

      n4 = nw + 4
      ns = 3 * n4
      iss = ics
      ics = ics + ns
      if (ics - 1 .gt. ims) call mpaler
      ihs = max (ics, ihs)
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4

      call mpmul (a, a, s(k0))
      call mpmul (a(l1), a(l1), s(k1))
      call mpadd (s(k0), s(k1), s(k2))
      call mpsqrt (s(k2), s(k0))
      call mpeq (a, s(k1))
      s(k1) = abs (s(k1))
      call mpadd (s(k0), s(k1), s(k2))
      call mpmuld (s(k2), 0.5d0, 0, s(k1))
      call mpsqrt (s(k1), s(k0))
      call mpmuld (s(k0), 2.d0, 0, s(k1))
      if (a(1) .ge. 0.) then
        call mpeq (s(k0), b)
        call mpdiv (a(l1), s(k1), b(l1))
      else
        call mpdiv (a(l1), s(k1), b)
        b(1) = abs (b(1))
        call mpeq (s(k0), b(l1))
        b(l1) = sign (b(l1), a(l1))
      endif
      ics = iss

 100  if (idb .ge. 6) then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 3) (b(i), i = 1, no)
 3      format ('MPCSQR O'/(6f12.0))
        no = min (int (abs (b(l1))), ndb) + 2
        write (ldb, 3) (b(l+i), i = 1, no)
      endif
      return
      end

      subroutine mpcsqx (l, a, b)

c   This routine computes the complex square root of the MPC number C.  L is
c   the offset between real and imaginary parts in A and B.  L must be at
c   least NW + 4.  For modest levels of precision, use MPCSQR.  The last two
c   words are not reliable.  Debug output starts with IDB = 5.

c   Max SP space for B: 2 * L cells.  Max SP scratch space: 7.5 * NW + 39
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   This routine uses the same algorithm as MPCSQR.

      parameter (ndb = 22)
      dimension a(2*l), b(2*l)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        b(l+1) = 0.
        b(l+2) = 0.
        return
      endif
      l1 = l + 1
      if (idb .ge. 5) then
        write (ldb, 1) l
 1      format ('MPCSQX I',i10)
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 2) (a(i), i = 1, no)
 2      format ('MPCSQX I'/(6f12.0))
        no = min (int (abs (a(l1))), ndb) + 2
        write (ldb, 2) (a(l+i), i = 1, no)
      endif

      if (a(1) .eq. 0. .and. a(l+1) .eq. 0.) then
        b(1) = 0.
        b(2) = 0.
        b(l+1) = 0.
        b(l+2) = 0.
        goto 100
      endif

      n4 = nw + 4
      ns = 3 * n4
      iss = ics
      ics = ics + ns
      if (ics - 1 .gt. ims) call mpaler
      ihs = max (ics, ihs)
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4

      call mpsqx (a, s(k0))
      call mpsqx (a(l1), s(k1))
      call mpadd (s(k0), s(k1), s(k2))
      call mpsqrx (s(k2), s(k0))
      call mpeq (a, s(k1))
      s(k1) = abs (s(k1))
      call mpadd (s(k0), s(k1), s(k2))
      call mpmuld (s(k2), 0.5d0, 0, s(k1))
      call mpsqrx (s(k1), s(k0))
      call mpmuld (s(k0), 2.d0, 0, s(k1))
      if (a(1) .ge. 0.) then
        call mpeq (s(k0), b)
        call mpdivx (a(l1), s(k1), b(l1))
      else
        call mpdivx (a(l1), s(k1), b)
        b(1) = abs (b(1))
        call mpeq (s(k0), b(l1))
        b(l1) = sign (b(l1), a(l1))
      endif
      ics = iss

 100  if (idb .ge. 5) then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 3) (b(i), i = 1, no)
 3      format ('MPCSQX O'/(6f12.0))
        no = min (int (abs (b(l1))), ndb) + 2
        write (ldb, 3) (b(l+i), i = 1, no)
      endif
      return
      end

      subroutine mpcssh (a, al2, x, y)

c   This computes the hyperbolic cosine and sine of the MP number A and
c   returns the two MP results in X and Y, respectively.  AL2 is the MP value
c   of Log (10) computed by a previous call to MPLOG.  For extra high levels of
c   precision, use MPCSHX.  The last word of the result is not reliable.
c   Debug output starts with IDB = 5.

c   Max SP space for X and Y: NW + 4 cells.  Max SP scratch space: 9 * NW + 50
c   cells.  Max DP scratch space: NW + 6 cells.

      dimension a(nw+2), f(8), al2(nw+2), x(nw+4), y(nw+4)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        x(1) = 0.
        x(2) = 0.
        y(1) = 0.
        y(2) = 0.
        return
      endif
      if (idb .ge. 5) call mpdeb ('MPCSSH I', a)

      n5 = nw + 5
      ns = 4 * n5
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n5
      k2 = k1 + n5
      k3 = k2 + n5
      nws = nw
      nw = nw + 1
      f(1) = 1.
      f(2) = 0.
      f(3) = 1.
      f(4) = 0.

      call mpexp (a, al2, s(k0))
      call mpdiv (f, s(k0), s(k1))
      call mpadd (s(k0), s(k1), s(k2))
      call mpmuld (s(k2), 0.5d0, 0, s(k3))
      call mpeq (s(k3), x)
      call mpsub (s(k0), s(k1), s(k2))
      call mpmuld (s(k2), 0.5d0, 0, s(k3))
      call mpeq (s(k3), y)

c   Restore original precision level.

      nw = nws
      ics = iss
      call mproun (x)
      call mproun (y)

      if (idb .ge. 5) then
        call mpdeb ('MPCSSH O', x)
        call mpdeb ('MPCSSH O', y)
      endif
      return
      end

      subroutine mpcssn (a, pi, x, y)

c   This computes the cosine and sine of the MP number A and returns the two MP
c   results in X and Y, respectively.  PI is the MP value of Pi computed by a
c   previous call to MPPI.  For extra high levels of precision, use MPCSSX.
c   The last word of the result is not reliable.  Debug output starts with
c   IDB = 6.

c   Max SP space for X and Y: NW + 4 cells.  Max SP scratch space: 10 * NW + 53
c   cells.  Max DP scratch space: NW + 6 cells.

c   This routine uses the conventional Taylor's series for Sin (s):

c   Sin (s) =  s - s^3 / 3! + s^5 / 5! - s^7 / 7! ...

c   where s = t - a * pi / 2 - b * pi / 16 and the integers a and b are chosen
c   to minimize the absolute value of s.  We can then compute

c   Sin (t) = Sin (s + a * pi / 2 + b * pi / 16)
c   Cos (t) = Cos (s + a * pi / 2 + b * pi / 16)

c   by applying elementary trig identities for sums.  The sine and cosine of
c   b * pi / 16 are of the form 1/2 * Sqrt {2 +- Sqrt [2 +- Sqrt(2)]}.
c   Reducing t in this manner insures that -Pi / 32 < s <= Pi / 32, which
c   accelerates convergence in the above series.

      double precision cpi, t1, t2
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cpi = 3.141592653589793d0)
      dimension a(nw+2), f(8), pi(nw+2), x(nw+4), y(nw+4)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        x(1) = 0.
        x(2) = 0.
        y(1) = 0.
        y(2) = 0.
        return
      endif
      if (idb .ge. 6) call mpdeb ('MPCSSN I', a)

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)
      if (na .eq. 0) then
        x(1) = 1.
        x(2) = 0.
        x(3) = 1.
        y(1) = 0.
        y(2) = 0.
        l1 = 0
        goto 120
      endif

c   Check if Pi has been precomputed.

      call mpmdc (pi, t1, n1)
      if (n1 .ne. 0 .or. abs (t1 - cpi) .gt. rx2) then
        if (ker(28) .ne. 0) then
          write (ldb, 1)
 1        format ('*** MPCSSN: PI must be precomputed.')
          ier = 28
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      n5 = nw + 5
      ns = 7 * n5
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n5
      k2 = k1 + n5
      k3 = k2 + n5
      k4 = k3 + n5
      k5 = k4 + n5
      k6 = k5 + n5
      nws = nw
      nw = nw + 1
      f(1) = 1.
      f(2) = 0.
      f(3) = 1.
      f(4) = 0.

c   Reduce to between - Pi and Pi.

      call mpmuld (pi, 2.d0, 0, s(k0))
      call mpdiv (a, s(k0), s(k1))
      call mpnint (s(k1), s(k2))
      call mpsub (s(k1), s(k2), s(k3))

c   Determine nearest multiple of Pi / 2, and within a quadrant, the nearest
c   multiple of Pi / 16.  Through most of the rest of this subroutine, KA and
c   KB are the integers a and b of the algorithm above.

      call mpmdc (s(k3), t1, n1)
      if (n1 .ge. - nbt) then
        t1 = t1 * 2.d0 ** n1
        t2 = 4.d0 * t1
        ka = nint (t2)
        kb = nint (8.d0 * (t2 - ka))
      else
        ka = 0
        kb = 0
      endif
      t1 = (8 * ka + kb) / 32.d0
      call mpdmc (t1, 0, s(k1))
      call mpsub (s(k3), s(k1), s(k2))
      call mpmul (s(k0), s(k2), s(k1))

c   Compute cosine and sine of the reduced argument s using Taylor's series.

      if (s(k1) .eq. 0.) then
        s(k0) = 0.
        s(k0+1) = 0.
        l1 = 0
        goto 110
      endif
      call mpeq (s(k1), s(k0))
      call mpmul (s(k0), s(k0), s(k2))
      l1 = 0

 100  l1 = l1 + 1
      if (l1 .eq. 10000) then
        if (ker(29) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPCSSN: Iteration limit exceeded.')
          ier = 29
          if (ker(ier) .eq. 2) call mpabrt
          ics = iss
          nw = nws
          return
        endif
      endif

      t2 = - (2.d0 * l1) * (2.d0 * l1 + 1.d0)
      call mpmul (s(k2), s(k1), s(k3))
      call mpdivd (s(k3), t2, 0, s(k1))
      call mpadd (s(k1), s(k0), s(k3))
      call mpeq (s(k3), s(k0))

c   Check for convergence of the series.

      if (s(k1) .ne. 0. .and. s(k1+1) .ge. s(k0+1) - nw) goto 100

c   Compute Cos (s) = Sqrt [1 - Sin^2 (s)].

 110  call mpeq (s(k0), s(k1))
      call mpmul (s(k0), s(k0), s(k2))
      call mpsub (f, s(k2), s(k3))
      call mpsqrt (s(k3), s(k0))

c   Compute cosine and sine of b * Pi / 16.

      kc = abs (kb)
      f(3) = 2.
      if (kc .eq. 0) then
        s(k2) = 1.
        s(k2+1) = 0.
        s(k2+2) = 1.
        s(k3) = 0.
        s(k3+1) = 0.
      else
        if (kc .eq. 1) then
          call mpsqrt (f, s(k4))
          call mpadd (f, s(k4), s(k5))
          call mpsqrt (s(k5), s(k4))
        elseif (kc .eq. 2) then
          call mpsqrt (f, s(k4))
        elseif (kc .eq. 3) then
          call mpsqrt (f, s(k4))
          call mpsub (f, s(k4), s(k5))
          call mpsqrt (s(k5), s(k4))
        elseif (kc .eq. 4) then
          s(k4) = 0.
          s(k4+1) = 0.
        endif
        call mpadd (f, s(k4), s(k5))
        call mpsqrt (s(k5), s(k3))
        call mpmuld (s(k3), 0.5d0, 0, s(k2))
        call mpsub (f, s(k4), s(k5))
        call mpsqrt (s(k5), s(k4))
        call mpmuld (s(k4), 0.5d0, 0, s(k3))
      endif
      if (kb .lt. 0) s(k3) = - s(k3)

c   Apply the trigonometric summation identities to compute cosine and sine of
c   s + b * Pi / 16.

      call mpmul (s(k0), s(k2), s(k4))
      call mpmul (s(k1), s(k3), s(k5))
      call mpsub (s(k4), s(k5), s(k6))
      call mpmul (s(k1), s(k2), s(k4))
      call mpmul (s(k0), s(k3), s(k5))
      call mpadd (s(k4), s(k5), s(k1))
      call mpeq (s(k6), s(k0))

c   This code in effect applies the trigonometric summation identities for
c   (s + b * Pi / 16) + a * Pi / 2.

      if (ka .eq. 0) then
        call mpeq (s(k0), x)
        call mpeq (s(k1), y)
      elseif (ka .eq. 1) then
        call mpeq (s(k1), x)
        x(1) = - x(1)
        call mpeq (s(k0), y)
      elseif (ka .eq. -1) then
        call mpeq (s(k1), x)
        call mpeq (s(k0), y)
        y(1) = - y(1)
      elseif (ka .eq. 2 .or. ka .eq. -2) then
        call mpeq (s(k0), x)
        x(1) = - x(1)
        call mpeq (s(k1), y)
        y(1) = - y(1)
      endif

c   Restore original precision level.

      nw = nws
      ics = iss
      call mproun (x)
      call mproun (y)

 120  if (idb .ge. 6) then
        write (ldb, 3) l1
 3      format ('Iteration count:',i5)
        call mpdeb ('MPCSSN O', x)
        call mpdeb ('MPCSSN O', y)
      endif
      return
      end

      subroutine mpcssx (a, pi, x, y)

c   This computes the cosine and sine of the MP number A and returns the two MP
c   results in X and Y, respectively.  PI is the MP value of Pi computed by a
c   previous call to MPPI or MPPIX.  Before calling MPCSSX, the array in
c   MPCOM5 must be initialized by calling MPINIX.  For modest levels of
c   precision, use MPCSSN.  NW should be a power of two.  The last four words
c   of the result are not reliable.  Debug output starts with IDB = 5.

c   Max SP space for X and Y: NW + 4 cells.  Max SP scratch space:27.5*NW+119
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   This routine employs a complex arithmetic version of the scheme used in
c   MPEXPX.

      double precision cl2, cpi, t1, t2
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0, cpi = 3.141592653589793d0,
     $  nit = 1)
      dimension a(nw+2), f1(8), pi(nw+2), x(nw+4), y(nw+4)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        x(1) = 0.
        x(2) = 0.
        y(1) = 0.
        y(2) = 0.
        return
      endif
      if (idb .ge. 5) call mpdeb ('MPCSSX I', a)

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)
      ncr = 2 ** mcr

c   Check if precision level is too low to justify advanced routine.

      if (nw .le. ncr) then
        call mpcssn (a, pi, x, y)
        l1 = 0
        goto 120
      endif

c   Check if input is zero.

      if (na .eq. 0) then
        x(1) = 1.
        x(2) = 0.
        x(3) = 1.
        y(1) = 0.
        y(2) = 0.
        l1 = 0
        goto 120
      endif

c   Check if Pi has been precomputed.

      call mpmdc (pi, t1, n1)
      if (n1 .ne. 0 .or. abs (t1 - cpi) .gt. rx2) then
        if (ker(30) .ne. 0) then
          write (ldb, 1)
 1        format ('*** MPCSSX: PI must be precomputed.')
          ier = 30
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      n4 = nw + 4
      n42 = 2 * n4
      ns = 4 * n42
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n42
      k2 = k1 + n42
      k3 = k2 + n42
      f1(1) = 1.
      f1(2) = 0.
      f1(3) = 1.
      f1(4) = 0.
      nws = nw

c   Reduce argument to between - Pi and Pi.

      call mpmuld (pi, 2.d0, 0, s(k0))
      call mpdivx (a, s(k0), s(k1))
      call mpnint (s(k1), s(k2))
      call mpmul (s(k2), s(k0), s(k1))
      call mpsub (a, s(k1), s(k0))

c   Determine the least integer MQ such that 2 ^ MQ .GE. NW.

      t2 = nws
      mq = cl2 * log (t2) + 1.d0 - rxx
      call mpeq (f1, s(k2))

c   Compute initial approximation to [Cos (A), Sin (A)].

      nw = ncr
      call mpcssn (s(k0), pi, s(k3), s(k3+n4))
      iq = 0

c   Perform the Newton-Raphson iteration with a dynamically changing precision
c   level NW.

      do 110 k = mcr + 1, mq
        nw = min (2 * nw, nws)
 100    continue
        call mpangx (s(k3), s(k3+n4), pi, s(k1))
        call mpsub (s(k0), s(k1), s(k2+n4))
        call mpcmlx (n4, s(k3), s(k2), s(k1))
        call mpceq (n4, s(k1), s(k3))
        if (k .eq. mq - nit .and. iq .eq. 0) then
          iq = 1
          goto 100
        endif
 110  continue

c   The final (cos, sin) result must be normalized to have magnitude 1.

      call mpsqx (s(k3), s(k0))
      call mpsqx (s(k3+n4), s(k0+n4))
      call mpadd (s(k0), s(k0+n4), s(k1))
      call mpsqrx (s(k1), s(k2))
      call mpdivx (s(k3), s(k2), s(k0))
      call mpdivx (s(k3+n4), s(k2), s(k0+n4))
      call mpmpcm (n4, s(k0), x, y)
      ics = iss

 120  if (idb .ge. 5) then
        call mpdeb ('MPCSSX O', x)
        call mpdeb ('MPCSSX O', y)
      endif
      return
      end

      subroutine mpcsub (l, a, b, c)

c   This subracts the MPC numbers A and B and returns the MPC difference in
c   C.  L is the offset between real and imaginary parts in A, B and C.  L
c   must be at least NW + 4.  Debug output starts with IDB = 9.

c   Max SP space for C: 2 * L cells.

      dimension a(2*l), b(2*l), c(2*l)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        c(l+1) = 0.
        c(l+2) = 0.
        return
      endif
      if (idb .ge. 9) write (ldb, 1)
 1    format ('MPCSUB')

      l1 = l + 1
      call mpsub (a, b, c)
      call mpsub (a(l1), b(l1), c(l1))

      return
      end

      subroutine mpdeb (cs, a)

c   This outputs the character string CS, the exponent of the MP number A, and
c   the first 50 digits of A, all on one line.  CS must either be a literal
c   string not exceeding 12 characters in length or a variable of type
c   CHARACTER*n, where n does not exceed 12.

      character*(*) cs
      character*1 b(160)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      dimension a(nw+2)

      if (ier .ne. 0) return
      ids = idb
      idb = 0
      nws = nw
      nw = min (nw, 10)
      call mpoutc (a, b, n)
      n = min (n, 70)
      write (ldb, 1) cs, ' ', (b(k), k = 1, 4), (b(k), k = 9, n)
 1    format (a12,67a1:/(79a1))
      idb = ids
      nw = nws
      return
      end

      subroutine mpdiv (a, b, c)

c   This divides the MP number A by the MP number B to yield the MP quotient C.
c   For extra high levels of precision, use MPDIVX.  Debug output starts with
c   IDB = 8.

c   Max SP space for C: NW + 4 cells.  Max DP scratch space: NW + 4 cells.

      double precision d, rb, ss, t0, t1, t2
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (ndb = 22)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom4/ d(4100)
      dimension a(nw+2), b(nw+2), c(nw+4)

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        return
      endif
      if (idb .ge. 8) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPDIV I'/(6f12.0))
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 1) (b(i), i = 1, no)
      endif

      ia = sign (1., a(1))
      ib = sign (1., b(1))
      na = min (int (abs (a(1))), nw)
      nb = min (int (abs (b(1))), nw)

c   Check if dividend is zero.

      if (na .eq. 0) then
        c(1) = 0.
        c(2) = 0.
        goto 210
      endif
      if (nb .eq. 1 .and. b(3) .eq. 1.) then

c   Divisor is 1 or -1 -- result is A or -A.

        c(1) = sign (na, ia * ib)
        c(2) = a(2) - b(2)

        do 100 i = 3, na + 2
          c(i) = a(i)
 100    continue

        goto 210
      endif

c   Check if divisor is zero.

      if (nb .eq. 0) then
        if (ker(31) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPDIV: Divisor is zero.')
          ier = 31
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Initialize trial divisor and trial dividend.

      t0 = bdx * b(3)
      if (nb .ge. 2) t0 = t0 + b(4)
      if (nb .ge. 3) t0 = t0 + rdx * b(5)
      if (nb .ge. 4) t0 = t0 + rx2 * b(6)
      rb = 1.d0 / t0
      md = min (na + nb, nw)
      d(1) = 0.d0

      do 110 i = 2, na + 1
        d(i) = a(i+1)
 110  continue

      do 120 i = na + 2, md + 4
        d(i) = 0.d0
 120  continue

c   Perform ordinary long division algorithm.  First compute only the first
c   NA words of the quotient.

      do 150 j = 2, na + 1
        t1 = bx2 * d(j-1) + bdx * d(j) + d(j+1) + rdx * d(j+2)
        t0 = int (rb * t1)
        j3 = j - 3
        i2 = min (nb, nw + 2 - j3) + 2
        ij = i2 + j3

        do 130 i = 3, i2
          i3 = i + j3
          d(i3) = d(i3) - t0 * b(i)
 130    continue

c   Release carries periodically to avoid overflowing the exact integer
c   capacity of double precision floating point words in D.

        if (mod (j - 1, npr) .eq. 0) then
cdir$ ivdep
          do 140 i = j + 1, ij
            t1 = d(i)
            t2 = int (rdx * t1)
            d(i) = t1 - bdx * t2
            d(i-1) = d(i-1) + t2
 140      continue

        endif
        d(j) = d(j) + bdx * d(j-1)
        d(j-1) = t0
 150  continue

c   Compute additional words of the quotient, as long as the remainder
c   is nonzero.

      do 180 j = na + 2, nw + 3
        t1 = bx2 * d(j-1) + bdx * d(j) + d(j+1)
        if (j .le. nw + 2) t1 = t1 + rdx * d(j+2)
        t0 = int (rb * t1)
        j3 = j - 3
        i2 = min (nb, nw + 2 - j3) + 2
        ij = i2 + j3
        ss = 0.d0

        do 160 i = 3, i2
          i3 = i + j3
          d(i3) = d(i3) - t0 * b(i)
          ss = ss + abs (d(i3))
 160    continue

        if (mod (j - 1, npr) .eq. 0) then
cdir$ ivdep
          do 170 i = j + 1, ij
            t1 = d(i)
            t2 = int (rdx * t1)
            d(i) = t1 - bdx * t2
            d(i-1) = d(i-1) + t2
 170      continue

        endif
        d(j) = d(j) + bdx * d(j-1)
        d(j-1) = t0
        if (ss .eq. 0.d0) goto 190
        if (ij .le. nw + 1) d(ij+3) = 0.d0
 180  continue

c   Set sign and exponent, and fix up result.

      j = nw + 3

 190  d(j) = 0.d0
      if (d(1) .eq. 0.d0) then
        is = 1
      else
        is = 2
      endif
      nc = min (j - 1, nw)
      d(nc+3) = 0.d0
      d(nc+4) = 0.d0

      do 200 i = j + 1, 3, -1
        d(i) = d(i-is)
 200  continue

      d(1) = sign (nc, ia * ib)
      d(2) = a(2) - b(2) + is - 2
      call mpnorm (c)

 210  if (idb .ge. 8) then
        no = min (int (abs (c(1))), ndb) + 2
        write (ldb, 3) (c(i), i = 1, no)
 3      format ('MPDIV O'/(6f12.0))
      endif
      return
      end

      subroutine mpdivd (a, b, n, c)

c   This routine divides the MP number A by the DPE number (B, N) to yield
c   the MP quotient C.   Debug output starts with IDB = 9.

c   Max SP space for C: NW + 4 cells.  Max DP space: NW + 4 cells.

      double precision b, bb, br, d, dd, t1
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (ndb = 22)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)
      common /mpcom4/ d(4100)
      dimension a(nw+2), c(nw+4), f(8)

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        return
      endif
      if (idb .ge. 9) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPDIVD I'/(6f12.0))
        write (ldb, 2) b, n
 2      format ('MPDIVD I',1pd25.15,i10)
      endif

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)
      ib = sign (1.d0, b)

c   Check if dividend is zero.

      if (na .eq. 0) then
        c(1) = 0.
        c(2) = 0.
        goto 150
      endif

c   Check if divisor is zero.

      if (b .eq. 0.d0) then
        if (ker(32) .ne. 0) then
          write (ldb, 3)
 3        format ('*** MPDIVD: Divisor is zero.')
          ier = 32
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif
      n1 = n / nbt
      n2 = n - nbt * n1
      bb = abs (b) * 2.d0 ** n2

c   Reduce BB to within 1 and BDX.

      if (bb .ge. bdx) then

        do 100 k = 1, 100
          bb = rdx * bb
          if (bb .lt. bdx) then
            n1 = n1 + k
            goto 120
          endif
 100    continue

      elseif (bb .lt. 1.d0) then

        do 110 k = 1, 100
          bb = bdx * bb
          if (bb .ge. 1.d0) then
            n1 = n1 - k
            goto 120
          endif
 110    continue

      endif

c   If B cannot be represented exactly in a single mantissa word, use MPDIV.

 120  if (bb .ne. aint (bb)) then
        bb = sign (bb, b)
        call mpdmc (bb, n1 * nbt, f)
        call mpdiv (a, f, c)
        goto 150
      endif

      br = 1.d0 / bb
      dd = a(3)

c   Perform short division (not vectorizable at present).  Continue as long as
c   the remainder remains nonzero.

      do 130 j = 2, nw + 3
        t1 = int (br * dd)
        d(j+1) = t1
        dd = bdx * (dd - t1 * bb)
        if (j .le. na) then
          dd = dd + a(j+2)
        else
          if (dd .eq. 0.d0) goto 140
        endif
 130  continue

c   Set sign and exponent of result.

      j = nw + 3

 140  nc = min (j - 1, nw)
      d(1) = sign (nc, ia * ib)
      d(2) = a(2) - n1
      if (j .le. nw + 2) d(j+2) = 0.d0
      if (j .le. nw + 1) d(j+3) = 0.d0
      call mpnorm (c)

 150  if (idb .ge. 9) then
        no = min (int (abs (c(1))), ndb) + 2
        write (ldb, 4) (c(i), i = 1, no)
 4      format ('MPDIVD O'/(6f12.0))
      endif
      return
      end

      subroutine mpdivx (a, b, c)

c   This divides the MP number A by the MP number B and returns the MP result
c   in C.  Before calling MPDIVX, the array in MPCOM5 must be initialized by
c   calling MPINIX.  For modest levels of precision, use MPDIV.  NW should be
c   a power of two.  The last two words of the result are not reliable.  Debug
c   output starts with IDB = 7.

c   Max SP space for C: NW + 4 cells.  Max SP scratch space: 3 * NW + 12
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   This subroutine employs the following Newton-Raphson iteration, which
c   converges to 1 / B:

c          X_{k+1} = X_k + (1 - X_k * B) * X_k

c   where the muliplication () * X_k is performed with only half of the
c   normal level of precision.  These iterations are performed with a
c   maximum precision level NW that is dynamically changed, doubling with
c   each iteration.  The final iteration is performed as follows (this is
c   due to A. Karp):

c          A / B = (A * X_n) + [A - (A * X_n) * B] * X_n  (approx.)

c   where the multiplications A * X_n and [] * X_n are performed with only
c   half of the final level of precision.

c   One difficulty with this procedure is that errors often accumulate in the
c   trailing mantissa words.  This error can be controlled by repeating one of
c   the iterations.  The iteration that is repeated is controlled by setting
c   the parameter NIT below:  If NIT = 0, the last iteration is repeated (this
c   is most effective but most expensive).  If NIT = 1, then the next-to-last
c   iteration is repeated, etc.  An extra word of precision cannot be used in
c   this routine (since MPMULX prefers powers of two), so NIT = 0 or 1 is best
c   unless the user needs maximum speed.

      double precision cl2, t1
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0, ndb = 22, nit = 1)
      dimension a(nw+2), b(nw+2), c(nw+4), f(8)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        return
      endif
      if (idb .ge. 7) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPDIVX I'/(6f12.0))
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 1) (b(i), i = 1, no)
      endif

      ia = sign (1., a(1))
      ib = sign (1., b(1))
      na = min (int (abs (a(1))), nw)
      nb = min (int (abs (b(1))), nw)
      ncr = 2 ** mcr

c   Check if dividend is zero.

      if (na .eq. 0) then
        c(1) = 0.
        c(2) = 0.
        goto 120
      endif

c   Check if divisor is zero.

      if (nb .eq. 0)  then
        if (ker(33) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPDIVX: Divisor is zero.')
          ier = 33
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Check if precision level of divisor is too low to justify the advanced
c   routine.

      if (nb .le. ncr) then
        call mpdiv (a, b, c)
        goto 120
      endif
      n4 = nw + 4
      ns = 3 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4
      nws = nw

c   Determine the least integer MQ such that 2 ^ MQ .GE. NW.

      t1 = nw
      mq = cl2 * log (t1) + 1.d0 - rxx

c   Compute the initial approximation of 1 / B to a precision of NCR words.

      nw = ncr
      f(1) = 1.
      f(2) = 0.
      f(3) = 1.
      f(4) = 0.
      call mpdiv (f, b, c)
      iq = 0

c   Perform the Newton-Raphson iterations described above.

      do 110 k = mcr + 1, mq - 1
        nw1 = nw
        nw = min (2 * nw, nws)
        nw2 = nw
 100    continue
        call mpmulx (b, c, s(k0))
        call mpsub (f, s(k0), s(k1))
        nw = nw1
        call mpmulx (c, s(k1), s(k0))
        nw = nw2
        call mpadd (c, s(k0), s(k1))
        call mpeq (s(k1), c)
        if (k .eq. mq - nit .and. iq .eq. 0) then
          iq = 1
          goto 100
        endif
 110  continue

c   Perform last iteration using Karp's trick.

      call mpmulx (a, c, s(k0))
      nw1 = nw
      nw = min (2 * nw, nws)
      nw2 = nw
      call mpmulx (s(k0), b, s(k1))
      call mpsub (a, s(k1), s(k2))
      nw = nw1
      call mpmulx (s(k2), c, s(k1))
      nw = nw2
      call mpadd (s(k0), s(k1), c)
      ics = iss

 120  if (idb .ge. 7) then
        no = min (int (abs (c(1))), ndb) + 2
        write (ldb, 3) (c(i), i = 1, no)
 3      format ('MPDIVX O'/(6f12.0))
      endif
      return
      end

      subroutine mpdmc (a, n, b)

c   This routine converts the DPE number (A, N) to MP form in B.  All bits of
c   A are recovered in B.  However, note for example that if A = 0.1D0 and N
c   is 0, then B will NOT be the multiprecision equivalent of 1/10.  Debug
c   output starts with IDB = 9.

c   Max SP space for B:  8 cells.

      double precision a, aa
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (ndb = 22)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      dimension b(nw+4)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 9) write (ldb, 1) a, n
 1    format ('MPDMC I',1pd25.15,i10)

c   Check for zero.

      if (a .eq. 0.d0) then
        b(1) = 0.
        b(2) = 0.
        goto 150
      endif
      n1 = n / nbt
      n2 = n - nbt * n1
      aa = abs (a) * 2.d0 ** n2

c   Reduce AA to within 1 and BDX.

      if (aa .ge. bdx) then

        do 100 k = 1, 100
          aa = rdx * aa
          if (aa .lt. bdx) then
            n1 = n1 + k
            goto 120
          endif
 100    continue

      elseif (aa .lt. 1.d0) then

        do 110 k = 1, 100
          aa = bdx * aa
          if (aa .ge. 1.d0) then
            n1 = n1 - k
            goto 120
          endif
 110    continue

      endif

c   Store successive sections of AA into B.

 120  b(2) = n1
      b(3) = aint (aa)
      aa = bdx * (aa - b(3))
      b(4) = aint (aa)
      aa = bdx * (aa - b(4))
      b(5) = aint (aa)
      aa = bdx * (aa - b(5))
      b(6) = aint (aa)
      b(7) = 0.
      b(8) = 0.

      do 130 i = 6, 3, -1
        if (b(i) .ne. 0.) goto 140
 130  continue

 140  aa = i - 2
      b(1) = sign (aa, a)

 150  if (idb .ge. 9) then
        no = abs (b(1)) + 2.
        write (ldb, 2) (b(i), i = 1, no)
 2      format ('MPDMC O'/(6f12.0))
      endif
      return
      end

      subroutine mpeq (a, b)

c   This routine sets the MP number B equal to the MP number A.  Debug output
c   starts with IDB = 10.

c   Max SP space for B: NW + 3 cells.

c   The fact that only NW + 3 cells, and not NW + 4 cells, are copied is
c   important in some routines that increase the precision level by one.

      parameter (ndb = 22)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      dimension a(nw+3), b(nw+3)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 10) write (ldb, 1)
 1    format ('MPEQ')

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)
      if (na .eq. 0)  then
        b(1) = 0.
        b(2) = 0.
        goto 110
      endif
      b(1) = sign (na, ia)

      do 100 i = 2, na + 3
        b(i) = a(i)
 100  continue

 110  return
      end

      subroutine mpexp (a, al2, b)

c   This computes the exponential function of the MP number A and returns the
c   MP result in B.  AL2 is the MP value of Log(2) produced by a prior call
c   to MPLOG.  For extra high levels of precision, use MPEXPX.  The last
c   word of the result is not reliable.  Debug output starts with IDB = 7.

c   Max SP space for B: NW + 4 cells.  Max SP scratch space: 5 * NW + 25
c   cells.  Max DP scratch space: NW + 5 cells.

c   This routine uses a modification of the Taylor's series for Exp (t):

c   Exp (t) =  (1 + r + r^2 / 2! + r^3 / 3! + r^4 / 4! ...) ^ q * 2 ^ n

c   where q = 256, r = t' / q, t' = t - n Log(2) and where n is chosen so
c   that -0.5 Log(2) < t' <= 0.5 Log(2).  Reducing t mod Log(2) and
c   dividing by 256 insures that -0.001 < r <= 0.001, which accelerates
c   convergence in the above series.

      double precision alt, t1, t2
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (alt = 0.693147180559945309d0, nq = 8)
      dimension a(nw+2), b(nw+4), al2(nw+2), f(8)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 7) call mpdeb ('MPEXP I', a)

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)
      call mpmdc (a, t1, n1)
      t1 = t1 * 2.d0 ** n1

c   Unless the argument is near Log (2), Log(2) must be precomputed.  This
c   exception is necessary because MPLOG calls MPEXP to initialize Log (2).

      if (abs (t1 - alt) .gt. rdx) then
        call mpmdc (al2, t2, n2)
        if (n2 .ne. - nbt .or. abs (t2 * 0.5d0 ** nbt - alt) .gt. rx2)
     $    then
          if (ker(34) .ne. 0) then
            write (ldb, 1)
 1          format ('*** MPEXP: LOG (2) must be precomputed.')
            ier = 34
            if (ker(ier) .eq. 2) call mpabrt
          endif
          return
        endif
      endif

c   Check for overflows and underflows.

      if (abs (t1) .gt. 33271064.d0) then
        if (t1 .gt. 0.d0) then
          if (ker(35) .ne. 0) then
            write (ldb, 2) t1
 2          format ('*** MPEXP: Argument is too large',1p,d25.16)
            ier = 35
            if (ker(ier) .eq. 2) call mpabrt
          endif
          return
        else
          b(1) = 0.
          b(2) = 0.
          l1 = 0
          goto 130
        endif
      endif

      n5 = nw + 5
      ns = 4 * n5
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n5
      k2 = k1 + n5
      k3 = k2 + n5
      nws = nw
      nw = nw + 1
      f(1) = 1.
      f(2) = 0.
      f(3) = 1.
      f(4) = 0.

c   Compute the reduced argument A' = A - Log(2) * Nint [A / Log(2)].  Save
c   NZ = Nint [A / Log(2)] for correcting the exponent of the final result.

      if (abs (t1 - alt) .gt. rdx) then
        call mpdiv (a, al2, s(k0))
        call mpnint (s(k0), s(k1))
        call mpmdc (s(k1), t1, n1)
        nz = t1 * 2.d0 ** n1 + sign (rxx, t1)
        call mpmul (al2, s(k1), s(k2))
        call mpsub (a, s(k2), s(k0))
      else
        call mpeq (a, s(k0))
        nz = 0
      endif
      tl = s(k0+1) - nw

c   Check if the reduced argument is zero.

      if (s(k0) .eq. 0.d0) then
        s(k0) = 1.
        s(k0+1) = 0.
        s(k0+2) = 1.
        l1 = 0
        goto 120
      endif

c   Divide the reduced argument by 2 ^ NQ.

      call mpdivd (s(k0), 1.d0, nq, s(k1))

c   Compute Exp using the usual Taylor series.

      call mpeq (f, s(k2))
      call mpeq (f, s(k3))
      l1 = 0

 100  l1 = l1 + 1
      if (l1 .eq. 10000) then
        if (ker(36) .ne. 0) then
          write (ldb, 3)
 3        format ('*** MPEXP: Iteration limit exceeded.')
          ier = 36
          if (ker(ier) .eq. 2) call mpabrt
          nw = nws
          ics = iss
          return
        endif
      endif

      t2 = l1
      call mpmul (s(k2), s(k1), s(k0))
      call mpdivd (s(k0), t2, 0, s(k2))
      call mpadd (s(k3), s(k2), s(k0))
      call mpeq (s(k0), s(k3))

c   Check for convergence of the series.

      if (s(k2) .ne. 0. .and. s(k2+1) .ge. tl) goto 100

c   Raise to the (2 ^ NQ)-th power.

      do 110 i = 1, nq
        call mpmul (s(k0), s(k0), s(k1))
        call mpeq (s(k1), s(k0))
 110  continue

c  Multiply by 2 ^ NZ.

 120  call mpmuld (s(k0), 1.d0, nz, s(k1))
      call mpeq (s(k1), b)

c   Restore original precision level.

      nw = nws
      ics = iss
      call mproun (b)

 130  if (idb .ge. 7) then
        write (ldb, 4) l1
 4      format ('Iteration count:',i5)
        call mpdeb ('MPEXP O', b)
      endif
      return
      end

      subroutine mpexpx (a, pi, al2, b)

c   This computes the exponential function of the MP number A and returns the
c   MP result in B.  PI is the MP value of Pi produced by a prior call to MPPI
c   or MPPIX.  AL2 is the MP value of Log(2) produced by a prior call to
c   MPLOG  or MPLOGX.  Before calling MPEXPX, the array in MPCOM5 must be
c   initialized by calling MPINIX.  NW should be a power of two.  For modest
c   levels of precision, use MPEXP.  The last four words of the result are
c   not reliable.  Debug output starts with IDB = 5.

c   Max SP space for B: NW + 4 cells.  Max SP scratch space: 13.5 * NW + 63
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   This routine uses the Newton iteration

c     b_{k+1} = b_k [a + 1 - log b_k]

c   with a dynamically changing level of precision.  Logs are performed using
c   MPLOGX.  See the comment about the parameter NIT in MPDIVX.

      double precision alt, cl2, cpi, t1, t2
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (alt = 0.693147180559945309d0,
     $  cl2 = 1.4426950408889633d0, cpi = 3.141592653589793238d0,
     $  nit = 1)
      dimension a(nw+2), al2(nw+2), b(nw+4), f1(8), pi(nw+2)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 5) call mpdeb ('MPEXPX I', a)

      ncr = 2 ** mcr
      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)
      call mpmdc (a, t1, n1)
      t1 = t1 * 2.d0 ** n1

c   Check if precision level is too low to justify the advanced routine.

      if (nw .le. ncr) then
        call mpexp (a, al2, b)
        goto 120
      endif

c   Check if Log(2) has been precomputed.

      call mpmdc (al2, t2, n2)
      if (n2 .ne. - nbt .or. abs (t2 * 0.5d0 ** nbt - alt) .gt. rx2)
     $  then
        if (ker(37) .ne. 0) then
          write (ldb, 1)
 1        format ('*** MPEXPX: LOG (2) must be precomputed.')
          ier = 37
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c     Check if Pi has been precomputed.

      call mpmdc (pi, t2, n2)
      if (n2 .ne. 0 .or. abs (t2 - cpi) .gt. rx2) then
        if (ker(38) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPEXPX: PI must be precomputed.')
          ier = 38
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Check for overflows and underflows.

      if (abs (t1) .gt. 33271064.d0) then
        if (t1 .gt. 0.d0) then
          if (ker(39) .ne. 0) then
            write (ldb, 3) t1
 3          format ('*** MPEXPX: Argument is too large',1p,d25.16)
            ier = 39
            if (ker(ier) .eq. 2) call mpabrt
          endif
          return
        else
          b(1) = 0.
          b(2) = 0.
          goto 120
        endif
      endif

      n4 = nw + 4
      ns = 3 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4
      nws = nw
      f1(1) = 1.
      f1(2) = 0.
      f1(3) = 1.
      f1(4) = 0.

c   Determine the least integer MQ such that 2 ^ MQ .GE. NW.

      t2 = nws
      mq = cl2 * log (t2) + 1.d0 - rxx
      call mpadd (a, f1, s(k0))

c   Compute initial approximation to Exp (A).

      nw = ncr
      call mpexp (a, al2, b)
      iq = 0

c   Perform the Newton-Raphson iteration described above with a dynamically
c   changing precision level NW.

      do 110 k = mcr + 1, mq
        nw = min (2 * nw, nws)
 100    continue
        call mplogx (b, pi, al2, s(k1))
        call mpsub (s(k0), s(k1), s(k2))
        call mpmulx (b, s(k2), s(k1))
        call mpeq (s(k1), b)
        if (k .eq. mq - nit .and. iq .eq. 0) then
          iq = 1
          goto 100
        endif
 110  continue

      ics = iss

 120  if (idb .ge. 6) call mpdeb ('MPEXPX O', b)
      return
      end

      subroutine mpfft1 (is, l, m, x, y)

c   Performs the L-th iteration of the first variant of the Stockham FFT.
c   This routine is called by MPCFFT.  It is not intended to be called directly
c   by the user.

      implicit double precision (a-h, o-z)
      dimension x(*), y(*)
      common /mpcom5/ u(4100)

c   Set initial parameters.

      n = 2 ** m
      k = u(1)
      nu = k / 64
      n1 = n / 2
      lk = 2 ** (l - 1)
      li = 2 ** (m - l)
      lj = 2 * li
      ku = li + 1
      kn = ku + nu

      do 100 k = 0, lk - 1
        i11 = k * lj + 1
        i12 = i11 + li
        i21 = k * li + 1
        i22 = i21 + n1

cdir$ ivdep
        do 100 i = 0, li - 1
          u1 = u(ku+i)
          u2 = is * u(kn+i)
          x11 = x(i11+i)
          x12 = x(i11+i+n)
          x21 = x(i12+i)
          x22 = x(i12+i+n)
          t1 = x11 - x21
          t2 = x12 - x22
          y(i21+i) = x11 + x21
          y(i21+i+n) = x12 + x22
          y(i22+i) = u1 * t1 - u2 * t2
          y(i22+i+n) = u1 * t2 + u2 * t1
 100  continue

      return
      end

      subroutine mpfft2 (is, l, m, x, y)

c   Performs the L-th iteration of the second variant of the Stockham FFT.
c   This routine is called by MPCFFT.  It is not intended to be called directly
c   by the user.

      implicit double precision (a-h, o-z)
      dimension x(*), y(*)
      common /mpcom5/ u(4100)

c   Set initial parameters.

      n = 2 ** m
      k = u(1)
      nu = k / 64
      n1 = n / 2
      lk = 2 ** (l - 1)
      li = 2 ** (m - l)
      lj = 2 * lk
      ku = li + 1

      do 100 i = 0, li - 1
        i11 = i * lk + 1
        i12 = i11 + n1
        i21 = i * lj + 1
        i22 = i21 + lk
        u1 = u(ku+i)
        u2 = is * u(ku+i+nu)

cdir$ ivdep
        do 100 k = 0, lk - 1
          x11 = x(i11+k)
          x12 = x(i11+k+n)
          x21 = x(i12+k)
          x22 = x(i12+k+n)
          t1 = x11 - x21
          t2 = x12 - x22
          y(i21+k) = x11 + x21
          y(i21+k+n) = x12 + x22
          y(i22+k) = u1 * t1 - u2 * t2
          y(i22+k+n) = u1 * t2 + u2 * t1
 100  continue

      return
      end

      subroutine mpinfr (a, b, c)

c   Sets B to the integer part of the MP number A and sets C equal to the
c   fractional part of A.  Note that if A = -3.3, then B = -3 and C = -0.3.
c   Debug output starts with IDB = 9.

c   Max SP space for B and C: NW + 4 cells.

      parameter (ndb = 22)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      dimension a(nw+2), b(nw+2), c(nw+2)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        c(1) = 0.
        c(2) = 0.
        return
      endif
      if (idb .ge. 9) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPINFR I'/(6f12.0))
      endif

c   Check if  A  is zero.

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)
      ma = a(2)
      if (na .eq. 0)  then
        b(1) = 0.
        b(2) = 0.
        c(1) = 0.
        c(2) = 0.
        goto 120
      endif

      if (ma .ge. nw - 1) then
        if (ker(40) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPINFR: Argument is too large.')
          ier = 40
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Place integer part in  B.

      nb = min (max (ma + 1, 0), na)
      if (nb .eq. 0) then
        b(1) = 0.
        b(2) = 0.
      else
        b(1) = sign (nb, ia)
        b(2) = ma
        b(nb+3) = 0.
        b(nb+4) = 0.

        do 100 i = 3, nb + 2
          b(i) = a(i)
 100    continue

      endif

c   Place fractional part in C.

      nc = na - nb
      if (nc .le. 0) then
        c(1) = 0.
        c(2) = 0.
      else
        c(1) = sign (nc, ia)
        c(2) = ma - nb
        c(nc+3) = 0.
        c(nc+4) = 0.

        do 110 i = 3, nc + 2
          c(i) = a(i+nb)
 110    continue

      endif

c   Fix up results.  B may have trailing zeros and C may have leading zeros.

      call mproun (b)
      call mproun (c)

 120  if (idb .ge. 9)  then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 3) (b(i), i = 1, no)
 3      format ('MPINFR O'/(6f12.0))
        no = min (int (abs (c(1))), ndb) + 2
        write (ldb, 3) (c(i), i = 1, no)
      endif
      return
      end

      subroutine mpinix (m)

c   This initializes the double precision array U in common MPCOM5 with roots
c   of unity required by the FFT routines, which are called by MPMULX.  Before
c   calling any of the advanced MP routines (i.e. those whose names end in X),
c   this routine must be called with M set to MX, where MX is defined as the
c   integer such that 2 ^ MX = NX, and where NX is the largest precision level
c   NW that will be used in the subsequent application.  Before calling MPINIX,
c   the user must allocate at least 2^(M + 3) double precision cells in common
c   MPCOM5, which must be placed in the user's main program.  Also, at least
c   12 * NW + 6 double precision cells must be allocated in common MPCOM4.
c   Only one call to MPINIX is required, no matter how many advanced routines
c   are called.  It is not necessary for the user to call MPINIX, to allocate
c   space in MPCOM5 or to allocate more than NW + 6 cells in MPCOM4 if the
c   advanced routines are not called.

      double precision pi, t1, t2, u
      parameter (pi = 3.141592653589793238d0)
      common /mpcom5/ u(4100)

c   Initialize the U array with sines and cosines in a manner that permits
c   stride one access at each FFT iteration.

      mm = m + 2
      n = 2 ** mm
      nu = n
      u(1) = 64 * n + mm
      ku = 2
      kn = ku + nu
      ln = 1

      do 110 j = 1, mm
        t1 = pi / ln

cdir$ ivdep
        do 100 i = 0, ln - 1
          t2 = i * t1
          u(i+ku) = cos (t2)
          u(i+kn) = sin (t2)
 100    continue

        ku = ku + ln
        kn = ku + nu
        ln = 2 * ln
 110  continue

      return
      end

      subroutine mpinp (iu, a, cs)

c   This routine reads the MP number A from logical unit IU.  CS is a scratch
c   array of type CHARACTER*1.  CS must be dimensioned at least 7.225*NW + 100.
c   The digits of A may span more than one line.  A comma at the end of the
c   last line denotes the end of the MP number.  The input lines may not
c   exceed 120 characters in length.  Embedded blanks are allowed anywhere.
c   However, if the input number contains more than 80 embedded blanks, then
c   the dimension of CS must be increased by a corresponding amount.  The
c   exponent is optional in the input number, but if present it must appear
c   first.  Two examples:

c   1073741824.,
c   10 ^  -4 x  3.14159 26535 89793 23846 26433 83279
c     50288 41971 69399 37510,

c   Max SP space for A: NW + 4 cells.  Max SP scratch space: 5 * NW + 27 cells.

      character*120 lin
      character*1 cs(*)
      dimension a(nw+2)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        a(1) = 0.
        a(2) = 0.
        return
      endif
      l = 0
      nd = 7.225d0 * nw + 100.d0

 100  read (iu, '(A)', end = 150) lin

      do 110 i = 120, 1, -1
        if (lin(i:i) .ne. ' ') goto 120
 110  continue

      goto 100

 120  k = i
      if (l .gt. nd) goto 140

      do 130 i = 1, k
        l = l + 1
        if (l .gt. nd) goto 140
        cs(l)= lin(i:i)
 130  continue

 140  if (lin(k:k) .ne. ',') goto 100
      l = l - 1

      call mpinpc (cs, l, a)
      goto 160

 150  if (ker(72) .ne. 0) then
        write (ldb, 1)
 1      format ('*** MPINP: End-of-file encountered.')
        ier = 72
        if (ker(ier) .eq. 2) call mpabrt
      endif

 160  return
      end

      subroutine mpinpc (a, n, b)

c   Converts the CHARACTER*1 array A of length N into the MP number B.  The
c   string A must be in the format '10^s a x tb.c' where a, b and c are digit
c   strings; s and t are '-', '+' or blank; x is either 'x' or '*'.  Blanks may
c   be embedded anywhere.  The digit string a is limited to nine digits and
c   80 total characters, including blanks.  The exponent portion (i.e. the
c   portion up to and including x) and the period may optionally be omitted.
c   Debug output starts with IDB = 7.

c   Max SP space for B: NW + 4 cells.  Max SP scratch space: 5 * NW + 27 cells.

c   The following example shows how this routine may be used to input a MP
c   number:

c      CHARACTER*1 CX(800)
c      READ (1, '(80A1)') (CX(I), I = 1, ND)
c      CALL MPINPC (CX, ND, B)

      double precision bi
      character*1 a, ai
      character*10 dig
      character*80 ca
      parameter (ndb = 22, dig = '0123456789')
      dimension a(n), b(nw+4), f(8)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 7) then
        no = min (n, int (7.225 * ndb) + 20)
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPINPC I'/(78a1))
      endif

      n5 = nw + 5
      ns = 3 * n5
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n5
      k2 = k1 + n5
      nws = nw
      nw = nw + 1
      i1 = 1
      nn = 0

c   Find the carat, period, plus or minus sign, whichever comes first.

      do 100 i = 1, n
        ai = a(i)
        if (ai .eq. '^') goto 110
        if (ai .eq. '.' .or. ai .eq. '+' .or. ai .eq. '-') goto 160
 100  continue

      goto 160

c   Make sure number preceding the carat is 10.

 110  i2 = i - 1
      if (i2 .gt. 80) goto 210
      ca = ' '

      do 120 i = 1, i2
        ai = a(i)
        if (ai .eq. ' ') then
          goto 120
        elseif (index (dig, ai) .eq. 0) then
          goto 210
        endif
        ca(i:i) = ai
 120  continue

      read (ca, '(BN,I80)') nn
      if (nn .ne. 10) goto 210
      i1 = i2 + 2

c   Find the x or *.

      do 130 i = i1, n
        ai = a(i)
        if (ai .eq. 'x' .or. ai .eq. '*') goto 140
 130  continue

      goto 210

c   Convert the exponent.

 140  i2 = i - 1
      l1 = i2 - i1 + 1
      if (l1 .gt. 80) goto 210
      ca = ' '
      id = 0
      is = 1

      do 150 i = 1, l1
        ai = a(i+i1-1)
        if (ai .eq. ' ' .or. ai .eq. '+') then
          goto 150
        elseif (ai .eq. '-' .and. id .eq. 0) then
          id = 1
          is = -1
          ca(i:i) = ' '
        else
          if (index (dig, ai) .eq. 0) goto 210
          id = 1
          ca(i:i) = ai
        endif
 150  continue

      read (ca, '(BN,I80)') nn
      nn = is * nn
      i1 = i2 + 2

c   Find the next nonblank character.

 160  do 170 i = i1, n
        if (a(i) .ne. ' ') goto 180
 170  continue

      goto 210

c   Check if the nonblank character is a plus or minus sign.

 180  i1 = i
      if (a(i1) .eq. '+') then
        i1 = i1 + 1
        is = 1
      elseif (a(i1) .eq. '-') then
        i1 = i1 + 1
        is = -1
      else
        is = 1
      endif
      nb = 0
      ib = 0
      id = 0
      ip = 0
      s(k2) = 0.
      s(k2+1) = 0.
      f(1) = 1.
      f(2) = 0.
      it = 0

 190  ip = 0
      ca(1:6) = '000000'

c   Scan for digits, looking for the period also.  On the first pass we just
c   count, so that on the second pass it will come out right.

      do 200 i = i1, n
        ai = a(i)
        if (ai .eq. ' ') then
        elseif (ai .eq. '.') then
          if (ip .ne. 0) goto 210
          ip = id
        elseif (index (dig, ai) .eq. 0) then
          goto 210
        else
          ib = ib + 1
          id = id + 1
          ca(ib:ib) = ai
        endif
        if (ib .eq. 6 .or. i .eq. n .and. ib .ne. 0) then
          if (it .ne. 0) then
            nb = nb + 1
            read (ca(1:6), '(F6.0)') bi
            call mpmuld (s(k2), 1.d6, 0, s(k0))
            if (bi .ne. 0) then
              f(1) = 1.
              f(3) = bi
            else
              f(1) = 0.
            endif
            call mpadd (s(k0), f, s(k2))
            ca(1:6) = '000000'
          endif
          if (i .ne. n) ib = 0
        endif
 200  continue

      if (it .eq. 0) then
        ib = 6 - ib
        if (ib .eq. 6) ib = 0
        it = 1
        goto 190
      endif
      if (is .eq. -1) s(k2) = - s(k2)
      if (ip .eq. 0) ip = id
      nn = nn + ip - id
      f(1) = 1.
      f(3) = 10.
      call mpnpwr (f, nn, s(k0))
      call mpmul (s(k2), s(k0), s(k1))
      call mpeq (s(k1), b)
      nw = nws
      call mproun (b)
      ics = iss

      if (idb .ge. 7) then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 2) (b(i), i = 1, no)
 2      format ('MPINPC O'/(6f12.0))
      endif
      goto 220

 210  if (ker(41) .ne. 0) then
        write (ldb, 3)
 3      format ('*** MPINPC: Syntax error in literal string.')
        ier = 41
        if (ker(ier) .eq. 2) call mpabrt
        nw = nws
        ics = iss
      endif

 220  return
      end

      subroutine mpinqp (ia, ib)

c   This routine returns the value of the parameter whose name is IA in common
c   MPCOM1.  By using this routine instead of merely including the MPCOM1
c   block in the code, a user may eliminate the possibility of confusion with
c   a variable name in his or her program.  IA is of type CHARACTER and IB
c   is the value.

      character*(*) ia
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims

      if (ia .eq. 'NW' .or. ia .eq. 'nw') then
        ib = nw
      elseif (ia .eq. 'IDB' .or. ia .eq. 'idb') then
        ib = idb
      elseif (ia .eq. 'LDB' .or. ia .eq. 'ldb') then
        ib = ldb
      elseif (ia .eq. 'IER' .or. ia .eq. 'ier') then
        ib = ier
      elseif (ia .eq. 'MCR' .or. ia .eq. 'mcr') then
        ib = mcr
      elseif (ia .eq. 'IRD' .or. ia .eq. 'ird') then
        ib = ird
      elseif (ia .eq. 'ICS' .or. ia .eq. 'ics') then
        ib = ics
      elseif (ia .eq. 'IHS' .or. ia .eq. 'ihs') then
        ib = ihs
      elseif (ia .eq. 'IMS' .or. ia .eq. 'ims') then
        ib = ims
      else
        ib = 0
      endif

      return
      end

      subroutine mpinrl (n, lx, x, mn, mt, lr, r, iq)

c   This routine searches for integer relations among the entries of the
c   N-long MP vector X.  An integer relation is an n-long vector r such that
c   r_1 x_1 + r_2 x_2 + ... + r_n x_n = 0.  The entries of x are assumed to
c   start at X(1), X(LX+1), X(2*LX+1), etc.  MN is the Log_10 of the maximum
c   Euclidean norm of an acceptable relation.  IQ is set to 1 if the routine
c   succeeds in recovering a relation that (1) produces zero to within the
c   relative tolerance 10^MT and (2) has Euclidean norm less than 10^MN.  If
c   no relation is found that meets these standards, IQ is set to 0.  When a
c   valid relation vector is recovered, it is placed in R, beginning at R(1),
c   R(LR+1), R(2*LR+1), etc., where LR, like LX, is an input parameter.  LR
c   should be at least MN/6 + 3.  For extra-high levels of precision, call
c   MPINRX.  Debug output starts with IDB = 4.  When IDB = 5, norm bounds are
c   output within which no relation can exist.

c   Max SP space for R: LR * N cells.  Max SP scratch space:
c   (4 * N^2 + 5 * N + 12) * (NW + 4) cells.  Max DP scratch space: NW + 4
c   cells.

c   A typical application of this routine is to determine if a given computed
c   real number r is the root of any algebraic equation of degree n - 1 with
c   integer coefficients.  One merely sets x_k = r^(k-1) for k = 1 to n and
c   calls MPINRL.  If an integer relation is found, this relation is the vector
c   of coefficients of a polynomial satisfied by r.  If MPINRL outputs a norm
c   bound of B, then r is not the root of any polynomial of degree n or less
c   with integer coefficients, where the Euclidean norm of the vector of
c   coefficients is less than B.

c   It sometimes happens that the "precision exhausted" message is output
c   before finding a relation that is known to exist.  If this happens,
c   increase NW, the working precision level, as well as scratch space
c   allocations if necessary, and try again.  Typically MT is set to roughly
c   10 - 6 * NX, where NX is the precision level used to compute X.  Repeating
c   a run with somewhat higher precision is highly recommended to certify that
c   bounds results are valid.

c   This routine allocates the scratch space array S for arrays.  Otherwise the
c   indexing in MPINRQ is too complicated.

      character*8 cx
      parameter (ib = 6)
      dimension r(lr,n), x(lx,n)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        iq = 0
        return
      endif
      if (idb .ge. 5) then
        write (ldb, 1) n, lx, mn, lr
 1      format ('MPINRL I',4i6)

        do 100 k = 1, n
          write (cx, '(I4)') k
          call mpdeb (cx, x(1,k))
 100    continue

      endif

c   Check if enough space is allowed for R.

      if (lr .lt. mn / ib + 3) then
        if (ker(42) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPINRL: Argument LR must be larger to match MN.')
          ier = 42
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      n4 = nw + 4
      ns = (4 * n ** 2 + 5 * n + 7) * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      kbn = n * (n + 1)
      kbs = n + 1 + kbn
      kc = n * (n + 1) + kbs
      ku = n * n + kc
      call mpinrq (n, lx, x, mn, mt, lr, r, iq, s(k0), s(kbn*n4+k0),
     $  s(kbs*n4+k0), s(kc*n4+k0), s(ku*n4+k0))
      ics = iss

      if (idb .ge. 5) then
        write (ldb, 3) iq
 3      format ('MPINRL O',i2)
        if (iq .eq. 1) then

          do 110 k = 1, n
            write (cx, '(I4)') k
            call mpdeb (cx, r(1,k))
 110      continue

        endif
      endif
      return
      end

      subroutine mpinrq (n, lx, x, mn, mt, lr, r, iq, b, bn, bs, c, u)

c   This routine implements the "Small Integer Relation Algorithm" described
c   in Hastad, Just, Lagarias, and Schnorr, "Polynomial Time Algorithms for
c   Finding Integer Relations Among Real Numbers", to appear in SIAM J. on
c   Computing.  This routine is called by MPINRL.  It is not intended to be
c   called directly by the user.

c   IMX = Number of iterations after which run is terminated.
c   ITP = Print interval.  Also the interval at which norm bounds are computed.
c   LB  = Reduction in log_10 (BN(N)) from previous iteration.  Used to detect
c         that a tentative relation has been found.

      double precision ab, bnn, bns, bnz, bx, by, t1, t2, t3, t4, tb
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (imx = 10000, itp = 10, itz = 100, lb = 20)
      dimension b(nw+4,n,0:n), bn(nw+4,0:n), bs(nw+4,n,0:n),
     $  c(nw+4,n,n), r(lr,n), u(nw+4,0:n,0:n), x(lx,n)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

c   Step 1: Initialization.

      n4 = nw + 4
      ns = 5 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4
      k3 = k2 + n4
      k4 = k3 + n4
      nws = nw
      tl = 2 - nw
      bns = 0.d0
      bnz = 0.d0
      ibs = 0
      ibz = 0
      ii = 0
      iq = 0

      do 100 i = 1, n
        call mpeq (x(1,i), b(1,i,0))
 100  continue

      do 120 j = 1, n

        do 110 i = 1, n
          b(1,i,j) = 0.
          b(2,i,j) = 0.
          c(1,i,j) = 0.
          c(2,i,j) = 0.
 110    continue

        b(1,j,j) = 1.
        b(2,j,j) = 0.
        b(3,j,j) = 1.
        c(1,j,j) = 1.
        c(2,j,j) = 0.
        c(3,j,j) = 1.
 120  continue

      do 180 i = 0, n

        do 130 k = 1, n
          call mpeq (b(1,k,i), bs(1,k,i))
 130    continue

        do 160 j = 0, i - 1
          s(k0) = 0.
          s(k0+1) = 0.

          do 140 k = 1, n
            call mpmul (b(1,k,i), bs(1,k,j), s(k1))
            call mpadd (s(k0), s(k1), s(k2))
            call mpeq (s(k2), s(k0))
 140      continue

          if (bn(1,j) .eq. 0. .or. bn(2,j) .lt. tl) then
            u(1,i,j) = 0.
            u(2,i,j) = 0.
          else
            call mpdiv (s(k0), bn(1,j), u(1,i,j))
          endif
          u(1,j,i) = 0.
          u(2,j,i) = 0.

          do 150 k = 1, n
            call mpmul (u(1,i,j), bs(1,k,j), s(k0))
            call mpsub (bs(1,k,i), s(k0), s(k1))
            call mpeq (s(k1), bs(1,k,i))
 150      continue
 160    continue

        s(k0) = 0.
        s(k0+1) = 0.

        do 170 k = 1, n
          call mpmul (bs(1,k,i), bs(1,k,i), s(k1))
          call mpadd (s(k0), s(k1), s(k2))
          call mpeq (s(k2), s(k0))
 170    continue

        call mpeq (s(k0), bn(1,i))
        u(1,i,i) = 1.
        u(2,i,i) = 0.
        u(3,i,i) = 1.
 180  continue

c   Step 2: Termination test.

 190  ii = ii + 1
      if (ier .ne. 0) return
      if (ii .gt. imx) then
        if (ker(43) .ne. 0) then
          write (ldb, 1) ii
 1        format ('*** MPINRQ: Iteration limit exceeded',i6)
          ier = 43
          if (ker(ier) .eq. 2) call mpabrt
          ics = iss
          return
        endif
      endif
      bx = 0.d0
      by = 0.d0
      ix = -10000
      iy = -10000

      do 200 i = 1, n - 1
        call mpmdc (bn(1,i), ab, ib)
        if ((ab .gt. bx .and. ib .eq. ix) .or. (ab .ne. 0.d0
     $    .and. ib .gt. ix)) then
          bx = ab
          ix = ib
        endif
        call dpmul (2.d0 ** i, 0, ab, ib, t1, n1)
        if ((t1 .gt. by .and. n1 .eq. iy) .or. (t1 .ne. 0.d0
     $    .and. n1 .gt. iy)) then
          by = t1
          iy = n1
          i1 = i
        endif
 200  continue

      call dpsqrt (bx, ix, t1, n1)
      call dpdiv (1.d0, 0, t1, n1, t2, n2)
      call dpdec (t2, n2, tb, nb)
      call mpmdc (bn(1,n), t2, n2)
      call dpdec (t2, n2, bnn, ibn)
      if ((idb .ge. 5 .and. mod (ii, itp) .eq. 0) .or. idb .ge. 6) then
        write (ldb, 2) ii, tb, nb, bnn, ibn
 2      format ('Iteration', i6/ 'Norm bound =', f10.6, ' x 10^', i6,
     $    4x, 'BN(N) =', f10.6, ' x 10^', i6)
        if (idb .ge. 6) then
          write (ldb, 3)
 3        format ('BSTAR square norms:')
          call mpmout (1, n, bn(1,1))
        endif
        if (idb .ge. 7) then
          write (ldb, 4)
 4        format ('B Matrix')
          call mpmout (n, n + 1, b)
          write (ldb, 5)
 5        format ('U Matrix')
          call mpmout (n + 1, n + 1, u)
        endif
      endif
      if (nb .gt. mn) goto 280

c   Test if current BN(N) is 10^LB times the previous BN(N).

      if (bnn .ne. 0.d0 .and. ibn .gt. ibs + lb) then
        if (idb .ge. 5) write (ldb, 6) ii, bnn, ibn
 6      format (/'Tentative relation, iteration', i6, 4x, 'BN(N) =',
     $    f10.6, ' x 10^', i6)

c   Compute residual and norm of tentative relation.

        do 220 k = n, 1, -1
          t2 = 0.d0
          n2 = 0
          s(k0) = 0.
          s(k0+1) = 0.

          do 210 j = 1, n
            nw = lr - 2
            call mpeq (c(1,j,k), r(1,j))
            nw = nws
            call mpmdc (r(1,j), t1, n1)
            call dpmul (t1, n1, t1, n1, t3, n3)
            call dpadd (t2, n2, t3, n3, t4, n4)
            t2 = t4
            n2 = n4
            call mpmul (r(1,j), x(1,j), s(k1))
            call mpadd (s(k0), s(k1), s(k2))
            call mpeq (s(k2), s(k0))
 210      continue

c   If the residual is zero or within tolerance 10^MT of zero, it is a real
c   relation.  Otherwise it was a false alarm.

          call mpmdc (s(k0), t3, n3)
          call dpdec (t3, n3, t1, n1)
          if (t1 .eq. 0.d0 .or. n1 .le. mt) then
            if (idb .ge. 4) then
              call dpsqrt (t2, n2, t3, n3)
              call dpdec (t3, n3, t1, n1)
              call mpmdc (s(k0), t4, n4)
              call dpdec (t4, n4, t2, n2)
              write (ldb, 7) k, t1, n1, t2, n2
 7            format ('Relation in column',i4,3x,'Norm =',f10.6,
     $          ' x 10^',i6/'Residual =',f10.6,' x 10^',i6)
            endif
            iq = 1
            goto 280
          endif
 220    continue

      endif

c   Test if BN(N) is the same as ITZ iterations ago.

      if (mod (ii, itz) .eq. 0) then
        if (bnn .eq. bnz .and. ibn .eq. ibz) then
          if (ker(44) .ne. 0) then
            write (ldb, 8) int (log10 (bdx) * (nw + 3))
 8          format ('*** MPINRQ: Numeric overflow has occurred.  Call ',
     $        'MPINRL with at least',i8/'digits precision.')
            ier = 44
            if (ker(ier) .eq. 2) call mpabrt
          endif
          ics = iss
          return
        endif
        bnz = bnn
        ibz = ibn
      endif
      bns = bnn
      ibs = ibn

c   Step 3: Update B and C for transformation and then exchange B and C.

      i2 = i1 + 1

c   Check if U(i2,i1) can be converted exactly to an integer.  The error
c   number and message are the same as the previous one.

      if (abs (u(2,i2,i1)) .ge. nw - 1) then
        if (ker(45) .ne. 0) then
          write (ldb, 8) int (log10 (bdx) * abs (u(2,i2,i1)))
          ier = 45
          if (ker(ier) .eq. 2) call mpabrt
        endif
        ics = iss
        return
      endif
      call mpnint (u(1,i2,i1), s(k0))

      do 230 k = 1, n
        call mpmul (s(k0), b(1,k,i1), s(k1))
        call mpsub (b(1,k,i2), s(k1), s(k2))
        call mpeq (s(k2), b(1,k,i2))
        call mpmul (s(k0), c(1,k,i2), s(k1))
        call mpadd (c(1,k,i1), s(k1), s(k2))
        call mpeq (s(k2), c(1,k,i1))
 230  continue

      do 240 k = 1, n
        call mpeq (b(1,k,i1), s(k1))
        call mpeq (b(1,k,i2), b(1,k,i1))
        call mpeq (s(k1), b(1,k,i2))
        call mpeq (c(1,k,i1), s(k1))
        call mpeq (c(1,k,i2), c(1,k,i1))
        call mpeq (s(k1), c(1,k,i2))
 240  continue

c   Update U for transformation.

      do 250 j = 0, i1
        call mpmul (s(k0), u(1,i1,j), s(k1))
        call mpsub (u(1,i2,j), s(k1), s(k2))
        call mpeq (s(k2), u(1,i2,j))
 250  continue

c   Update BN and U for exchange.

      call mpeq (u(1,i2,i1), s(k0))
      call mpmul (s(k0), s(k0), s(k1))
      call mpmul (s(k1), bn(1,i1), s(k2))
      call mpadd (bn(1,i2), s(k2), s(k1))
      if (s(k1) .ne. 0. .and. s(k1+1) .gt. tl) then
        call mpdiv (bn(1,i1), s(k1), s(k2))
        call mpmul (bn(1,i2), s(k2), s(k3))
        call mpeq (s(k3), bn(1,i2))
        call mpmul (s(k0), s(k2), s(k3))
        call mpeq (s(k3), u(1,i2,i1))
      else
        call mpeq (bn(1,i1), bn(1,i2))
        u(1,i2,i1) = 0.
        u(2,i2,i1) = 0.
      endif
      call mpeq (s(k1), bn(1,i1))

      do 260 j = 1, i1 - 1
        call mpeq (u(1,i1,j), s(k1))
        call mpeq (u(1,i2,j), u(1,i1,j))
        call mpeq (s(k1), u(1,i2,j))
 260  continue

      s(k1) = 1.
      s(k1+1) = 0.
      s(k1+2) = 1.

      do 270 j = i1 + 2, n
        call mpmul (u(1,j,i1), u(1,i2,i1), s(k2))
        call mpmul (s(k0), u(1,i2,i1), s(k3))
        call mpsub (s(k1), s(k3), s(k4))
        call mpmul (u(1,j,i2), s(k4), s(k3))
        call mpadd (s(k2), s(k3), s(k4))
        call mpmul (s(k0), u(1,j,i2), s(k2))
        call mpsub (u(1,j,i1), s(k2), u(1,j,i2))
        call mpeq (s(k4), u(1,j,i1))
 270  continue

      goto 190

 280  if (idb .ge. 4) write (ldb, 9) ii, tb, nb
 9    format ('No. iterations =',i6/'Max. bound =',1pd15.6,
     $  ' x 10^',i5)
      ics = iss
      return
      end

      subroutine mpinrx (n, lx, x, mn, mt, lr, r, iq)

c   This routine searches for integer relations among the entries of the
c   N-long MP vector X.  An integer relation is an n-long vector r such that
c   r_1 x_1 + r_2 x_2 + ... + r_n x_n = 0.  The entries of x are assumed to
c   start at X(1), X(LX+1), X(2*LX+1), etc.  MN is the Log_10 of the maximum
c   Euclidean norm of an acceptable relation.  IQ is set to 1 if the routine
c   succeeds in recovering a relation that (1) produces zero to within the
c   relative tolerance 10^MT and (2) has Euclidean norm less than 10^MN.  If
c   no relation is found that meets these standards, IQ is set to 0.  When a
c   valid relation vector is recovered, it is placed in R, beginning at R(1),
c   R(LR+1), R(2*LR+1), etc., where LR, like LX, is an input parameter.  LR
c   should be at least MN/6 + 3.  Before calling MPINRX, the array in MPCOM5
c   must be initialized by calling MPINIX.  For modest levels of precision,
c   call MPINRL.  Debug output starts with IDB = 4.  When IDB = 5, norm bounds
c   are output within which no relation can exist.

c   Max SP space for R: LR * N cells.  Max SP scratch space:
c   (4 * N^2 + 5 * N + 15) * (NW + 4) cells.  Max DP scratch space: 12 * NW + 6
c   cells.

c   See the comments in MPINRL about applying this routine.

c   This allocates the scratch space array S for arrays.  Otherwise the
c   indexing in MPINRZ is too complicated.

      character*8 cx
      parameter (ib = 6)
      dimension r(lr,n), x(lx,n)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        iq = 0
        return
      endif
      if (idb .ge. 5) then
        write (ldb, 1) n, lx, mn, lr
 1      format ('MPINRX I',4i6)

        do 100 k = 1, n
          write (cx, '(I4)') k
          call mpdeb (cx, x(1,k))
 100    continue

      endif

c   Check if enough space is allowed for R.

      if (lr .le. mn / ib) then
        if (ker(46) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPINRX: Argument LR must be larger to match MN.')
          ier = 46
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Check if the precision level is too low to justify the advanced routine.

      ncr = 2 ** mcr
      if (nw .le. ncr) then
        call mpinrl (n, lx, x, mn, mt, lr, r, iq)
        goto 110
      endif

c   Compute pointers for arrays.

      n4 = nw + 4
      ns = (4 * n ** 2 + 5 * n + 7) * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      kbn = n * (n + 1)
      kbs = n + 1 + kbn
      kc = n * (n + 1) + kbs
      ku = n * n + kc
      call mpinrz (n, lx, x, mn, mt, lr, r, iq, s(k0), s(kbn*n4+k0),
     $  s(kbs*n4+k0), s(kc*n4+k0), s(ku*n4+k0))
      ics = iss

 110  if (idb .ge. 5) then
        write (ldb, 3) iq
 3      format ('MPINRX O',i2)
        if (iq .eq. 1) then

          do 120 k = 1, n
            write (cx, '(I4)') k
            call mpdeb (cx, r(1,k))
 120      continue

        endif
      endif
      return
      end

      subroutine mpinrz (n, lx, x, mn, mt, lr, r, iq, b, bn, bs, c, u)

c   This is the extra-high precision version of MPINRQ.  See the comments
c   there for details.

      double precision ab, bnn, bns, bnz, bx, by, t1, t2, t3, t4, tb
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (imx = 10000, itp = 25, itz = 250, lb = 20)
      dimension b(nw+4,n,0:n), bn(nw+4,0:n), bs(nw+4,n,0:n),
     $  c(nw+4,n,n), r(lr,n), u(nw+4,0:n,0:n), x(lx,n)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

c   Step 1: Initialization.

      n4 = nw + 4
      ns = 5 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4
      k3 = k2 + n4
      k4 = k3 + n4
      nws = nw
      if (nw .le. 32) then
        tl = 2 - nw
      elseif (nw .le. 256) then
        tl = 3 - nw
      else
        tl = 4 - nw
      endif
      bns = 0.d0
      bnz = 0.d0
      ibs = 0
      ibz = 0
      ii = 0
      iq = 0

      do 100 i = 1, n
        call mpeq (x(1,i), b(1,i,0))
 100  continue

      do 120 j = 1, n

        do 110 i = 1, n
          b(1,i,j) = 0.
          b(2,i,j) = 0.
          c(1,i,j) = 0.
          c(2,i,j) = 0.
 110    continue

        b(1,j,j) = 1.
        b(2,j,j) = 0.
        b(3,j,j) = 1.
        c(1,j,j) = 1.
        c(2,j,j) = 0.
        c(3,j,j) = 1.
 120  continue

      do 180 i = 0, n

        do 130 k = 1, n
          call mpeq (b(1,k,i), bs(1,k,i))
 130    continue

        do 160 j = 0, i - 1
          s(k0) = 0.
          s(k0+1) = 0.

          do 140 k = 1, n
            call mpmulx (b(1,k,i), bs(1,k,j), s(k1))
            call mpadd (s(k0), s(k1), s(k2))
            call mpeq (s(k2), s(k0))
 140      continue

          if (bn(1,j) .eq. 0. .or. bn(2,j) .lt. tl) then
            u(1,i,j) = 0.
            u(2,i,j) = 0.
          else
            call mpdivx (s(k0), bn(1,j), u(1,i,j))
          endif
          u(1,j,i) = 0.
          u(2,j,i) = 0.

          do 150 k = 1, n
            call mpmulx (u(1,i,j), bs(1,k,j), s(k0))
            call mpsub (bs(1,k,i), s(k0), s(k1))
            call mpeq (s(k1), bs(1,k,i))
 150      continue
 160    continue

        s(k0) = 0.
        s(k0+1) = 0.

        do 170 k = 1, n
          call mpsqx (bs(1,k,i), s(k1))
          call mpadd (s(k0), s(k1), s(k2))
          call mpeq (s(k2), s(k0))
 170    continue

        call mpeq (s(k0), bn(1,i))
        u(1,i,i) = 1.
        u(2,i,i) = 0.
        u(3,i,i) = 1.
 180  continue

c   Step 2: Termination test.

 190  ii = ii + 1
      if (ier .ne. 0) return
      if (ii .gt. imx) then
        if (ker(47) .ne. 0) then
          write (ldb, 1) ii
 1        format ('*** MPINRZ: Iteration limit exceeded',i6)
          ier = 47
          if (ker(ier) .eq. 2) call mpabrt
          ics = iss
          return
        endif
      endif
      bx = 0.d0
      by = 0.d0
      ix = -10000
      iy = -10000

      do 200 i = 1, n - 1
        call mpmdc (bn(1,i), ab, ib)
        if ((ab .gt. bx .and. ib .eq. ix) .or. (ab .ne. 0.d0
     $    .and. ib .gt. ix)) then
          bx = ab
          ix = ib
        endif
        call dpmul (2.d0 ** i, 0, ab, ib, t1, n1)
        if ((t1 .gt. by .and. n1 .eq. iy) .or. (t1 .ne. 0.d0
     $    .and. n1 .gt. iy)) then
          by = t1
          iy = n1
          i1 = i
        endif
 200  continue

      call dpsqrt (bx, ix, t1, n1)
      call dpdiv (1.d0, 0, t1, n1, t2, n2)
      call dpdec (t2, n2, tb, nb)
      call mpmdc (bn(1,n), t2, n2)
      call dpdec (t2, n2, bnn, ibn)
      if ((idb .ge. 5 .and. mod (ii, itp) .eq. 0) .or. idb .ge. 6) then
        write (ldb, 2) ii, tb, nb, bnn, ibn
 2      format ('Iteration', i6/ 'Norm bound =', f10.6, ' x 10^', i6,
     $    4x, 'BN(N) =', f10.6, ' x 10^', i6)
        if (idb .ge. 6) then
          write (ldb, 3)
 3        format ('BSTAR square norms:')
          call mpmout (1, n, bn(1,1))
        endif
        if (idb .ge. 7) then
          write (ldb, 4)
 4        format ('B Matrix')
          call mpmout (n, n + 1, b)
          write (ldb, 5)
 5        format ('U Matrix')
          call mpmout (n + 1, n + 1, u)
        endif
      endif
      if (nb .gt. mn) goto 280

c   Test if current BN(N) is 10^LB times the previous BN(N).

      if (bnn .ne. 0.d0 .and. ibn .gt. ibs + lb) then
        if (idb .ge. 5) write (ldb, 6) ii, bnn, ibn
 6      format (/'Tentative relation, iteration', i6, 4x, 'BN(N) =',
     $    f10.6, ' x 10^', i6)

c   Compute residual and norm of tentative relation.

        do 220 k = n, 1, -1
          t2 = 0.d0
          n2 = 0
          s(k0) = 0.
          s(k0+1) = 0.

          do 210 j = 1, n
            nw = lr - 2
            call mpeq (c(1,j,k), r(1,j))
            nw = nws
            call mpmdc (r(1,j), t1, n1)
            call dpmul (t1, n1, t1, n1, t3, n3)
            call dpadd (t2, n2, t3, n3, t4, n4)
            t2 = t4
            n2 = n4
            call mpmulx (r(1,j), x(1,j), s(k1))
            call mpadd (s(k0), s(k1), s(k2))
            call mpeq (s(k2), s(k0))
 210      continue

c   If the residual is zero or within tolerance 10^MT of zero, it is a real
c   relation.  Otherwise it was a false alarm.

          call mpmdc (s(k0), t3, n3)
          call dpdec (t3, n3, t1, n1)
          if (t1 .eq. 0.d0 .or. n1 .le. mt) then
            if (idb .ge. 4) then
              call dpsqrt (t2, n2, t3, n3)
              call dpdec (t3, n3, t1, n1)
              call mpmdc (s(k0), t4, n4)
              call dpdec (t4, n4, t2, n2)
              write (ldb, 7) k, t1, n1, t2, n2
 7            format ('Relation in column',i4,3x,'Norm =',f10.6,
     $          ' x 10^',i6/'Residual =',f10.6,' x 10^',i6)
            endif
            iq = 1
            goto 280
          endif
 220    continue

      endif

c   Test if BN(N) is the same as ITZ iterations ago.

      if (mod (ii, itz) .eq. 0) then
        if (bnn .eq. bnz .and. ibn .eq. ibz) then
          if (ker(48) .ne. 0) then
            write (ldb, 8) int (log10 (bdx) * (nw + 3))
 8          format ('*** MPINRZ: Numeric overflow has occurred.  Call ',
     $        'MPINRX with at least',i8/'digits precision.')
            ier = 48
            if (ker(ier) .eq. 2) call mpabrt
          endif
          ics = iss
          return
        endif
        bnz = bnn
        ibz = ibn
      endif
      bns = bnn
      ibs = ibn

c   Step 3: Update B and C for transformation and then exchange B and C.

      i2 = i1 + 1

c   Check if U(i2,i1) can be converted exactly to an integer.  The error
c   number and message are the same as the previous one.

      if (abs (u(2,i2,i1)) .ge. nw - 1) then
        if (ker(49) .ne. 0) then
          write (ldb, 8) int (log10 (bdx) * abs (u(2,i2,i1)))
          ier = 49
          if (ker(ier) .eq. 2) call mpabrt
        endif
        ics = iss
        return
      endif
      call mpnint (u(1,i2,i1), s(k0))

      do 230 k = 1, n
        call mpmulx (s(k0), b(1,k,i1), s(k1))
        call mpsub (b(1,k,i2), s(k1), s(k2))
        call mpeq (s(k2), b(1,k,i2))
        call mpmulx (s(k0), c(1,k,i2), s(k1))
        call mpadd (c(1,k,i1), s(k1), s(k2))
        call mpeq (s(k2), c(1,k,i1))
 230  continue

      do 240 k = 1, n
        call mpeq (b(1,k,i1), s(k1))
        call mpeq (b(1,k,i2), b(1,k,i1))
        call mpeq (s(k1), b(1,k,i2))
        call mpeq (c(1,k,i1), s(k1))
        call mpeq (c(1,k,i2), c(1,k,i1))
        call mpeq (s(k1), c(1,k,i2))
 240  continue

c   Update U for transformation.

      do 250 j = 0, i1
        call mpmulx (s(k0), u(1,i1,j), s(k1))
        call mpsub (u(1,i2,j), s(k1), s(k2))
        call mpeq (s(k2), u(1,i2,j))
 250  continue

c   Update BN and U for exchange.

      call mpeq (u(1,i2,i1), s(k0))
      call mpsqx (s(k0), s(k1))
      call mpmulx (s(k1), bn(1,i1), s(k2))
      call mpadd (bn(1,i2), s(k2), s(k1))
      if (s(k1) .ne. 0. .and. s(k1+1) .gt. tl) then
        call mpdivx (bn(1,i1), s(k1), s(k2))
        call mpmulx (bn(1,i2), s(k2), s(k3))
        call mpeq (s(k3), bn(1,i2))
        call mpmulx (s(k0), s(k2), s(k3))
        call mpeq (s(k3), u(1,i2,i1))
      else
        call mpeq (bn(1,i1), bn(1,i2))
        u(1,i2,i1) = 0.
        u(2,i2,i1) = 0.
      endif
      call mpeq (s(k1), bn(1,i1))

      do 260 j = 1, i1 - 1
        call mpeq (u(1,i1,j), s(k1))
        call mpeq (u(1,i2,j), u(1,i1,j))
        call mpeq (s(k1), u(1,i2,j))
 260  continue

      s(k1) = 1.
      s(k1+1) = 0.
      s(k1+2) = 1.

      do 270 j = i1 + 2, n
        call mpmulx (u(1,j,i1), u(1,i2,i1), s(k2))
        call mpmulx (s(k0), u(1,i2,i1), s(k3))
        call mpsub (s(k1), s(k3), s(k4))
        call mpmulx (u(1,j,i2), s(k4), s(k3))
        call mpadd (s(k2), s(k3), s(k4))
        call mpmulx (s(k0), u(1,j,i2), s(k2))
        call mpsub (u(1,j,i1), s(k2), u(1,j,i2))
        call mpeq (s(k4), u(1,j,i1))
 270  continue

      goto 190

 280  if (idb .ge. 4) write (ldb, 9) ii, tb, nb
 9    format ('No. iterations =',i6/'Max. bound =',1pd15.6,
     $  ' x 10^',i5)
      ics = iss
      return
      end

      subroutine mplog (a, al2, b)

c   This computes the natural logarithm of the MP number A and returns the MP
c   result in B.  AL2 is the MP value of Log(2) produced by a prior call to
c   MPLOG.  For extra high levels of precision, use MPLOGX.  The last word of
c   the result is not reliable.  Debug output starts with IDB = 6.

c   Max SP space for B: NW + 4 cells.  Max SP scratch space: 8 * NW + 45
c   cells.  Max DP scratch space: NW + 6 cells.

c   The Taylor series for Log converges much more slowly than that of Exp.
c   Thus this routine does not employ Taylor series, but instead computes
c   logarithms by solving Exp (b) = a using the following Newton iteration,
c   which converges to b:

c           x_{k+1} = x_k + [a - Exp (x_k)] / Exp (x_k)

c   These iterations are performed with a maximum precision level NW that
c   is dynamically changed, approximately doubling with each iteration.
c   See the comment about the parameter NIT in MPDIVX.

      double precision alt, cl2, t1, t2
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (alt = 0.693147180559945309d0,
     $  cl2 = 1.4426950408889633d0, nit = 3)
      dimension a(nw+2), al2(nw+2), b(nw+4)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 6) call mpdeb ('MPLOG I', a)

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)

      if (ia .lt. 0 .or. na .eq. 0) then
        if (ker(50) .ne. 0) then
          write (ldb, 1)
 1        format ('*** MPLOG: Argument is less than or equal to zero.')
          ier = 50
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c  Unless the input is close to 2, Log (2) must have been precomputed.

      call mpmdc (a, t1, n1)
      if (abs (t1 - 2.d0) .gt. 1d-3 .or. n1 .ne. 0) then
        call mpmdc (al2, t2, n2)
        if (n2 .ne. - nbt .or. abs (t2 * 0.5d0 ** nbt - alt) .gt. rx2)
     $    then
          if (ker(51) .ne. 0) then
            write (ldb, 2)
 2          format ('*** MPLOG: LOG (2) must be precomputed.')
            ier = 51
            if (ker(ier) .eq. 2) call mpabrt
          endif
          return
        endif
      endif

c   Check if input is exactly one.

      if (a(1) .eq. 1. .and. a(2) .eq. 0. .and. a(3) .eq. 1.) then
        b(1) = 0.
        b(2) = 0.
        goto 120
      endif

      n5 = nw + 5
      ns = 3 * n5
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n5
      k2 = k1 + n5
      nws = nw

c   Determine the least integer MQ such that 2 ^ MQ .GE. NW.

      t2 = nws
      mq = cl2 * log (t2) + 1.d0 - rxx

c   Compute initial approximation of Log (A).

      t1 = log (t1) + n1 * alt
      call mpdmc (t1, 0, b)
      nw = 3
      iq = 0

c   Perform the Newton-Raphson iteration described above with a dynamically
c   changing precision level NW (one greater than powers of two).

      do 110 k = 2, mq
        nw = min (2 * nw - 2, nws) + 1
 100    continue
        call mpexp (b, al2, s(k0))
        call mpsub (a, s(k0), s(k1))
        call mpdiv (s(k1), s(k0), s(k2))
        call mpadd (b, s(k2), s(k1))
        call mpeq (s(k1), b)
        if (k .eq. mq - nit .and. iq .eq. 0) then
          iq = 1
          goto 100
        endif
 110  continue

c   Restore original precision level.

      nw = nws
      ics = iss
      call mproun (b)

 120  if (idb .ge. 6) call mpdeb ('MPLOG O', b)

      return
      end

      subroutine mplogx (a, pi, al2, b)

c   This computes the natural logarithm of the MP number A and returns the MP
c   result in B.  PI is the MP value of Pi produced by a prior call to MPPI or
c   MPPIX.  AL2 is the MP value of Log(2) produced by a prior call to MPLOG
c   or MPLOGX.  Before calling MPLOGX, the array in MPCOM5 must be
c   initialized by calling MPINIX.  For modest levels of precision, use MPLOG.
c   NW should be a power of two.  The last three words of the result are not
c   reliable.  Debug output starts with IDB = 6.

c   Max SP space for B: NW + 4 cells.  Max SP scratch space: 10.5 * NW + 51
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   This uses the following algorithm, which is due to Salamin.  If a is
c   extremely close to 1, use a Taylor series.  Otherwise select n such that
c   z = x 2^n is at least 2^m, where m is the number of bits of desired
c   precision in the result.  Then

c   Log(x) = Pi / [2 AGM (1, 4/x)]

      double precision alt, cpi, st, t1, t2, tn
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (mzl = -5, alt = 0.693147180559945309d0,
     $  cpi = 3.141592653589793d0)
      dimension al2(nw+2), f1(8), f4(8), pi(nw+2), a(nw+4), b(nw+4)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 6) call mpdeb ('MPLOGX I', a)

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)
      ncr = 2 ** mcr

c   Check if precision level is too low to justify the advanced routine.

      if (nw .le. ncr) then
        call mplog (a, al2, b)
        goto 120
      endif

      if (ia .lt. 0 .or. na .eq. 0) then

c   Input is less than or equal to zero.

        if (ker(52) .ne. 0) then
          write (ldb, 1)
 1        format ('*** MPLOGX: Argument is less than or equal to zero.')
          ier = 52
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Check if Pi has been precomputed.

      call mpmdc (pi, t1, n1)
      if (n1 .ne. 0 .or. abs (t1 - cpi) .gt. rx2) then
        if (ker(53) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPLOGX: PI must be precomputed.')
          ier = 53
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c  Unless the input is 2, Log (2) must have been precomputed.

      if (a(1) .ne. 1. .or. a(2) .ne. 0. .or. a(3) .ne. 2.) then
        it2 = 0
        call mpmdc (al2, t2, n2)
        if (n2 .ne. - nbt .or. abs (t2 * 0.5d0 ** nbt - alt) .gt. rx2)
     $    then
          if (ker(54) .ne. 0) then
            write (ldb, 3)
 3          format ('*** MPLOGX: Log (2) must be precomputed.')
            ier = 54
            if (ker(ier) .eq. 2) call mpabrt
          endif
          return
        endif
      else
        it2 = 1
      endif

c   Define sections of the scratch array.

      n4 = nw + 4
      ns = 4 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4
      k3 = k2 + n4
      f1(1) = 1.
      f1(2) = 0.
      f1(3) = 1.
      f1(4) = 0.
      f4(1) = 1.
      f4(2) = 0.
      f4(3) = 4.
      f4(4) = 0.

c   If argument is 1, the result is zero.  If the argument is extremely close
c   to 1.  If so, employ a Taylor's series instead.

      call mpsub (a, f1, s(k0))
      if (s(k0) .eq. 0.) then
        b(1) = 0.
        b(2) = 0.
        goto 110
      elseif (s(k0+1) .le. mzl) then
        call mpeq (s(k0), s(k1))
        call mpeq (s(k1), s(k2))
        i1 = 1
        is = 1
        tl = s(k0+1) - nw - 1

 100    i1 = i1 + 1
        is = - is
        st = is * i1
        call mpmulx (s(k1), s(k2), s(k3))
        call mpdivd (s(k3), st, 0, s(k2))
        call mpadd (s(k0), s(k2), s(k3))
        call mpeq (s(k3), s(k0))
        if (s(k2+1) .ge. tl) goto 100

        call mpeq (s(k0), b)
        goto 110
      endif

c   If input is exactly 2, set the exponent to a large value.  Otherwise
c   multiply the input by a large power of two.

      call mpmdc (a, t1, n1)
      n2 = nbt * (nw / 2 + 2) - n1
      tn = n2
      if (it2 .eq. 1) then
        call mpdmc (1.d0, n2, s(k0))
      else
        call mpmuld (a, 1.d0, n2, s(k0))
      endif

c   Perform AGM iterations.

      call mpeq (f1, s(k1))
      call mpdivx (f4, s(k0), s(k2))
      call mpagmx (s(k1), s(k2))

c   Compute B = Pi / (2 * A), where A is the limit of the AGM iterations.

      call mpmuld (s(k1), 2.d0, 0, s(k0))
      call mpdivx (pi, s(k0), s(k1))

c  If the input was exactly 2, divide by TN.  Otherwise subtract TN * Log(2).

      if (it2 .eq. 1) then
        call mpdivd (s(k1), tn, 0, s(k0))
      else
        call mpmuld (al2, tn, 0, s(k2))
        call mpsub (s(k1), s(k2), s(k0))
      endif
      call mpeq (s(k0), b)

 110  ics = iss
 120  if (idb .ge. 6) call mpdeb ('MPLOGX O', b)
      return
      end

      subroutine mpmdc (a, b, n)

c   This converts the MP number A to the DPE form (B, N), accurate to between
c   14 and 17 digits, depending on system.  B will be between 1 and BDX.
c   Debug output starts with IDB = 9.

      double precision aa, b
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (ndb = 22)
      dimension a(nw+2)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims

      if (ier .ne. 0) then
        b = 0.d0
        n = 0
        return
      endif
      if (idb .ge. 9) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPMDC I'/(6f12.0))
      endif

      if (a(1) .eq. 0.)  then
        b = 0.d0
        n = 0
        goto 100
      endif

      na = abs (a(1))
      aa = a(3)
      if (na .ge. 2) aa = aa + rdx * a(4)
      if (na .ge. 3) aa = aa + rx2 * a(5)
      if (na .ge. 4) aa = aa + rdx * rx2 * a(6)

      n = nbt * a(2)
      b = sign (aa, dble (a(1)))

 100  if (idb .ge. 9) write (ldb, 2) b, n
 2    format ('MPMDC O',f10.0,i10)
      return
      end

      subroutine mpmmpc (a, b, l, c)

c   This converts MP numbers A and B to MPC form in C, i.e. C = A + B i.
c   L (an input parameter) is the offset between real and imaginary parts in
c   C.  Debug output starts with IDB = 10.

c   Max SP space for C: 2 * L cells.

      dimension a(nw+2), b(nw+2), c(2*l)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        c(l+1) = 0.
        c(l+2) = 0.
        return
      endif
      if (idb .ge. 10) write (ldb, 1)
 1    format ('MPMMPC')

      i1 = sign (1., a(1))
      n1 = min (int (abs (a(1))), nw, l - 2)
      i2 = sign (1., b(1))
      n2 = min (int (abs (b(1))), nw, l - 2)
      c(1) = sign (n1, i1)
      c(l+1) = sign (n2, i2)

      do 100 i = 2, n1 + 2
        c(i) = a(i)
 100  continue

      do 110 i = 2, n2 + 2
        c(l+i) = b(i)
 110  continue

      return
      end

      subroutine mpmout (n1, n2, a)

c   This produces a compact printout of the N1 x N1 MP array A.  It is called
c   MPINRQ and MPINRZ.  It is not indended to be called directly by the user.

      double precision t1, t2
      dimension a(nw+4,n1,n2)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      dimension t1(100), i1(100)

      if (ier .ne. 0) return

      do 110 j = 1, n1
        write (ldb, 1) j
 1      format ('Row', i3)

        do 100 k = 1, n2
          call mpmdc (a(1,j,k), t2, m2)
          call dpdec (t2, m2, t1(k), i1(k))
 100    continue

        write (ldb, 2) (t1(k), i1(k), k = 1, n2)
 2      format (4(f10.6,i6))
 110  continue

      return
      end

      subroutine mpmpcm (l, a, b, c)

c   This converts the MPC number A to its MP real and imaginary parts, i.e.
c   B = Real (A) and C = Imag (A).  L is the offset between real and
c   imaginary parts in A.  Debug output starts with IDB = 10.

c   Max SP space for B and C: NW + 2 cells.

      dimension a(2*l), b(nw+2), c(nw+2)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        c(1) = 0.
        c(2) = 0.
        return
      endif
      if (idb .ge. 10) write (ldb, 1)
 1    format ('MPMPCM')

      i1 = sign (1., a(1))
      n1 = min (int (abs (a(1))), nw, l - 2)
      i2 = sign (1., a(l+1))
      n2 = min (int (abs (a(l+1))), nw, l - 2)
      b(1) = sign (n1, i1)
      c(1) = sign (n2, i2)

      do 100 i = 2, n1 + 2
        b(i) = a(i)
 100  continue

      do 110 i = 2, n2 + 2
        c(i) = a(l+i)
 110  continue

      return
      end

      subroutine mpmul (a, b, c)

c   This routine multiplies MP numbers A and B to yield the MP product C.
c   When one of the arguments has a much higher level of precision than the
c   other, this routine is slightly more efficient if A has the lower level of
c   precision.  For extra high levels of precision, use MPMULX.  Debug output
c   starts with IDB = 8.

c   Max SP space for C: NW + 4 cells.  Max DP scratch space: NW + 4 cells.

c   This routine returns up to NW mantissa words of the product.  If the
c   complete double-long product of A and B is desired (for example in large
c   integer applications), then NW must be at least as large as the sum of the
c   mantissa lengths of A and B.  In other words, if the precision levels of A
c   and B are both 64 words, then NW must be at least 128 words to obtain the
c   complete double-long product in C.

      double precision d, t1, t2
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (ndb = 22)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom4/ d(4100)
      dimension a(nw+2), b(nw+2), c(nw+4)

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        return
      endif
      if (idb .ge. 8) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPMUL I'/(6f12.0))
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 1) (b(i), i = 1, no)
      endif

      ia = sign (1., a(1))
      ib = sign (1., b(1))
      na = min (int (abs (a(1))), nw)
      nb = min (int (abs (b(1))), nw)
      if (na .eq. 0 .or. nb .eq. 0) then

c   One of the inputs is zero -- result is zero.

        c(1) = 0.
        c(2) = 0.
        goto 170
      endif
      if (na .eq. 1 .and. a(3) .eq. 1.) then

c   A is 1 or -1 -- result is B or -B.

        c(1) = sign (nb, ia * ib)
        c(2) = a(2) + b(2)

        do 100 i = 3, nb + 2
          c(i) = b(i)
 100    continue

        goto 170
      elseif (nb .eq. 1 .and. b(3) .eq. 1.) then

c   B is 1 or -1 -- result is A or -A.

        c(1) = sign (na, ia * ib)
        c(2) = a(2) + b(2)

        do 110 i = 3, na + 2
          c(i) = a(i)
 110    continue

        goto 170
      endif

      nc = min (na + nb, nw)
      d2 = a(2) + b(2)

      do 120 i = 1, nc + 4
        d(i) = 0.d0
 120  continue

c   Perform ordinary long multiplication algorithm.  Accumulate at most NW + 4
c   mantissa words of the product.

      do 150 j = 3, na + 2
        t1 = a(j)
        j3 = j - 3
        n2 = min (nb + 2, nw + 4 - j3)

        do 130 i = 3, n2
          d(i+j3) = d(i+j3) + t1 * b(i)
 130    continue

c   Release carries periodically to avoid overflowing the exact integer
c   capacity of double precision floating point words in D.

        if (mod (j - 2, npr) .eq. 0) then
          i1 = max (3, j - npr)
          i2 = n2 + j3

cdir$ ivdep
          do 140 i = i1, i2
            t1 = d(i)
            t2 = int (rdx * t1)
            d(i) = t1 - bdx * t2
            d(i-1) = d(i-1) + t2
 140      continue

        endif
 150  continue

c   If D(2) is nonzero, shift the result one cell right.

      if (d(2) .ne. 0.d0) then
        d2 = d2 + 1.

cdir$ ivdep
        do 160 i = nc + 4, 3, -1
          d(i) = d(i-1)
 160    continue

      endif
      d(1) = sign (nc, ia * ib)
      d(2) = d2

c   Fix up result, since some words may be negative or exceed BDX.

      call mpnorm (c)

 170  if (idb .ge. 8) then
        no = min (int (abs (c(1))), ndb) + 2
        write (ldb, 2) (c(i), i = 1, no)
 2      format ('MPMUL O'/(6f12.0))
      endif
      return
      end

      subroutine mpmuld (a, b, n, c)

c   This routine multiplies the MP number A by the DPE number (B, N) to yield
c   the MP product C.  Debug output starts with IDB = 9.

c   Max SP space for C: NW + 4 cells.  Max DP space: NW + 4 cells.

      double precision b, bb, d
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (ndb = 22)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom3/ s(4100)
      common /mpcom4/ d(4100)
      dimension a(nw+2), c(nw+4), f(8)

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        return
      endif
      if (idb .ge. 9) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPMULD I'/(6f12.0))
        write (ldb, 2) b, n
 2      format ('MPMULD I',1pd25.15,i10)
      endif

c   Check for zero inputs.

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)
      ib = sign (1.d0, b)
      if (na .eq. 0 .or. b .eq. 0.d0) then
        c(1) = 0.
        c(2) = 0.
        goto 140
      endif
      n1 = n / nbt
      n2 = n - nbt * n1
      bb = abs (b) * 2.d0 ** n2

c   Reduce BB to within 1 and BDX.

      if (bb .ge. bdx) then

        do 100 k = 1, 100
          bb = rdx * bb
          if (bb .lt. bdx) then
            n1 = n1 + k
            goto 120
          endif
 100    continue

      elseif (bb .lt. 1.d0) then

        do 110 k = 1, 100
          bb = bdx * bb
          if (bb .ge. 1.d0) then
            n1 = n1 - k
            goto 120
          endif
 110    continue

      endif

c   If B cannot be represented exactly in a single mantissa word, use MPMUL.

 120  if (bb .ne. aint (bb)) then
        bb = sign (bb, b)
        call mpdmc (bb, n1 * nbt, f)
        call mpmul (f, a, c)
        goto 140
      endif

c   Perform short multiply operation.

cdir$ ivdep
      do 130 i = 3, na + 2
        d(i) = bb * a(i)
 130  continue

c   Set the exponent and fix up the result.

      d(1) = sign (na, ia * ib)
      d(2) = a(2) + n1
      d(na+3) = 0.d0
      d(na+4) = 0.d0
      call mpnorm (c)

 140  if (idb .ge. 9) then
        no = min (int (abs (c(1))), ndb) + 2
        write (ldb, 3) (c(i), i = 1, no)
 3      format ('MPMULD O'/(6f12.0))
      endif
      return
      end

      subroutine mpmulx (a, b, c)

c   This routine multiplies MP numbers A and B to yield the MP product C.
c   Before calling MPMULX, the array in MPCOM5 must be initialized by calling
c   MPINIX.  For modest levels of precision, use MPMUL.  NW should be a power
c   of two.  Debug output starts with IDB = 8.

c   Max SP space for C: NW + 4 cells.  Max DP scratch space: 12 * NW + 6 cells.
c   The fact that all advanced routines require this amount of DP scratch
c   space derives from the requirement in this routine, which all of them call.

c   This routine returns up to NW mantissa words of the product.  If the
c   complete double-long product of A and B is desired (for example in large
c   integer applications), then NW must be at least as large as the sum of the
c   mantissa lengths of A and B.  In other words, if the precision levels of A
c   and B are both 256 words, then NW must be at least 512 words to obtain the
c   complete double-long product in C.

c   This subroutine uses an advanced technique involving the fast Fourier
c   transform (FFT).  For high precision it is significantly faster than the
c   conventional scheme used in MPMUL.
c>
c   Two machine-dependent parameters are set in this routine:

c     ERM   Maximum tolerated FFT roundoff error.  On IEEE systems ERM =
c           0.438D0.  It is not necessary to specify ERM for modest levels of
c           precision -- see comments below.
c     MBT   Number of mantissa bits in double precision data.  MBT = 53 on
c           IEEE systems, and MBT = 48 (i.e. single precision) on Cray
c           (non-IEEE) systems.
c           It is not necessary to specify MBT for modest levels of precision.

      double precision an, cl2, d, erm, t1, t2, t3, t4
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0, erm = 0.438d0, mbt = 53,
     $  ndb = 22)
      dimension a(nw+2), b(nw+2), c(nw+4)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom4/ d(4100)

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        return
      endif
      if (idb .ge. 8)  then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPMULX I'/(6f12.0))
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 1) (b(i), i = 1, no)
      endif

      ia = sign (1., a(1))
      ib = sign (1., b(1))
      na = min (int (abs (a(1))), nw)
      nb = min (int (abs (b(1))), nw)
      ncr = 2 ** mcr

      if (na .eq. 0 .or. nb .eq. 0) then

c   One of the inputs is zero -- result is zero.

        c(1) = 0.
        c(2) = 0.
        goto 190
      endif

c   Check if precision level of one of the arguments is too low to justify the
c   advanced routine.

      if (na .le. ncr .or. nb .le. ncr) then
        call mpmul (a, b, c)
        goto 190
      endif

c   Determine N1, the smallest power of two at least as large as NA and NB.

      t1 = na
      t2 = nb
      m1 = cl2 * log (t1) + 1.d0 - rxx
      m2 = cl2 * log (t2) + 1.d0 - rxx
      m1 = max (m1, m2)
      n1 = 2 ** m1
      m2 = m1 + 2
      n2 = 2 * n1
      n4 = 2 * n2
      n6 = 3 * n2
      n8 = 4 * n2
      n21 = n2 + 1
      n42 = n4 + 2
      n63 = n6 + 3
      n84 = n8 + 4

c   Place the input data in A and B into separate sections of the scratch
c   array D.  This code also splits the input data into half-sized words.

cdir$ ivdep
      do 100 i = 1, na
        t1 = a(i+2)
        t2 = int (rbx * t1)
        d(2*i-1) = t2
        d(2*i) = t1 - bbx * t2
 100  continue

      do 110 i = 2 * na + 1, n2
        d(i) = 0.d0
 110  continue

cdir$ ivdep
      do 120 i = 1, nb
        t1 = b(i+2)
        t2 = int (rbx * t1)
        d(2*i-1+n42) = t2
        d(2*i+n42) = t1 - bbx * t2
 120  continue

      do 130 i = 2 * nb + 1, n2
        d(i+n42) = 0.d0
 130  continue

c   Set the second half of each input vector in D to zero.

cdir$ ivdep
      do 140 i = n2 + 1, n4
        d(i) = 0.d0
        d(i+n42) = 0.d0
 140  continue

c   Perform forward real-to-complex FFTs on the two vectors in D.  The complex
c   results are placed in (D(I), I = 1, N4+2) and (D(I), I = N4 + 3, N8 + 4).

      call mprcft (1, m2, d, d(n84+1))
      call mprcft (1, m2, d(n42+1), d(n84+1))

c   Multiply the resulting complex vectors.

cdir$ ivdep
      do 150 i = 1, n21
        t1 = d(i)
        t2 = d(i+n21)
        t3 = d(i+n42)
        t4 = d(i+n63)
        d(i+n42) = t1 * t3 - t2 * t4
        d(i+n63) = t1 * t4 + t2 * t3
 150  continue

c   Perform an inverse complex-to-real FFT on the resulting data.

      call mpcrft (-1, m2, d(n42+1), d(n84+1))

c   Divide by N8, recombine words and release carries.

      nc = min (na + nb, nw)
      nc1 = min (nw + 1, na + nb - 1)
      d(1) = sign (nc, ia * ib)
      d(2) = a(2) + b(2) + 1
      an = 1.d0 / n8
      t1 = an * d(n42+1)
      d(3) = aint (t1 + 0.5d0)
      d(nc+3) = 0.d0
      d(nc+4) = 0.d0
      d(n42+1) = 0.d0

cdir$ ivdep
      do 160 i = 1, nc1
        t1 = an * d(n42+2*i)
        t2 = an * d(n42+2*i+1)
        t3 = aint (t1 + 0.5d0)
        t4 = aint (t2 + 0.5d0)
c        D(N42+2*I) = ABS (T3 - T1)
c        D(N42+2*I+1) = ABS (T4 - T2)
        t1 = int (rdx * t3)
        t2 = t3 - bdx * t1
        t3 = int (rdx * t4)
        t4 = t4 - bdx * t3
        d(i+3) = bbx * t2 + t4
        d(i+2) = d(i+2) + bbx * t1 + t3
 160  continue

c   Find the largest FFT roundoff error.  Roundoff error is minimal unless
c   exceedingly high precision (i.e. over one million digits) is used.  Thus
c   this test may be disabled in normal use.  To disable this test, uncomment
c   the next line of code and comment out the two lines of the previous loop
c   that begin D(N42..

c   This code can be used as a rigorous system integrity test.  First set
c   MBT according to the system being used, and then set ERM to be fairly
c   small, say 0.001 or whatever is somewhat larger than the largest FFT
c   roundoff error typically encountered for a given precision level on the
c   computer being used.  Enable this test as explained in the previous
c   paragraph.  Then if an anomalously large roundoff error is detected, a
c   hardware or compiler error has likely occurred.

      goto 180
      t1 = 0.d0

      do 170 i = 1, 2 * nc1 + 1
        if (d(n42+i) .gt. t1) then
          i1 = i
          t1 = d(n42+i)
        endif
 170  continue

c   Check if maximum roundoff error exceeds the limit ERM, which is set above.
c   Also determine the number of fractional bits and how large the error is in
c   terms of units in the last place (ulp).

      if (t1 .gt. erm)  then
        if (ker(55) .ne. 0) then
          t2 = an * d(i1)
          i2 = cl2 * log (t1) + 1.d0 + rxx
          i3 = cl2 * log (t2) + 1.d0 + rxx
          i4 = mbt + i2 - i3
          i5 = t1 * 2 ** i4 + rxx
          write (ldb, 2) i1, t1, i4, i5
 2        format ('*** MPMULX: Excessive FFT roundoff error',i10,f10.6,
     $      2i6)
          ier = 55
          if (ker(ier) .eq. 2) call mpabrt
        endif
      endif

c   Fix up the result.

 180  call mpnorm (c)

 190  if (idb .ge. 8) then
        no = min (int (abs (c(1))), ndb) + 2
        write (ldb, 3) (c(i), i = 1, no)
 3      format ('MPMULX O'/(6f12.0))
      endif
      return
      end

      subroutine mpnint (a, b)

c   This sets B equal to the integer nearest to the MP number A.  Debug output
c   starts with IDB = 8.

c   Max SP space for B: NW + 4 cells.  Max SP scratch space: NW + 4 cells.

      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (ndb = 22)
      dimension a(nw+2), b(nw+2), f(8)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 8) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPNINT I'/(6f12.0))
      endif

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)
      ma = a(2)
      if (na .eq. 0)  then

c   A is zero -- result is zero.

        b(1) = 0.
        b(2) = 0.
        goto 110
      endif
      if (ma .ge. nw) then

c   A cannot be represented exactly as an integer.

        if (ker(56) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPNINT: Argument is too large.')
          ier = 56
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      ns = nw + 4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      f(1) = 1.
      f(2) = -1.
      f(3) = 0.5d0 * bdx
      f(4) = 0.

c   Add or subtract 1/2 from the input, depending on its sign.

      if (ia .eq. 1) then
        call mpadd (a, f, s(k0))
      else
        call mpsub (a, f, s(k0))
      endif
      ic = sign (1., s(k0))
      nc = abs (s(k0))
      mc = s(k0+1)

c   Place integer part of S in B.

      nb = min (max (mc + 1, 0), nc)
      if (nb .eq. 0) then
        b(1) = 0.
        b(2) = 0.
      else
        b(1) = sign (nb, ic)
        b(2) = mc
        b(nb+3) = 0.
        b(nb+4) = 0.

        do 100 i = 3, nb + 2
          b(i) = s(i+k0-1)
 100    continue

      endif
      ics = iss

 110  if (idb .ge. 8) then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 3) (b(i), i = 1, no)
 3      format ('MPNINT O'/(6f12.0))
      endif
      return
      end

      subroutine mpnorm (a)

c   This converts the MP number in array D of MPCOM4 to the standard
c   normalized form in A.  The MP routines often leave negative numbers or
c   values exceeding the radix MPBDX in result arrays, and this fixes them.
c   MPNORM assumes that two extra mantissa words are input at the end of D.
c   This reduces precision loss when it is necessary to shift the result to
c   the left.  This routine is not intended to be called directly by the user.
c   Debug output starts with MPIDB = 10.

c   Max SP space for A: MPNW + 4 cells.

      double precision d, s1, t1, t2, t3
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (ndb = 22)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom4/ d(4100)
      dimension a(nw+4)

      if (ier .ne. 0) then
        if (ier .eq. 99) call mpabrt
        a(1) = 0.
        a(2) = 0.
        return
      endif
      if (idb .ge. 9) then
        no = min (int (abs (d(1))), ndb) + 4
        write (ldb, 1) (d(i), i = 1, no)
1       format ('MPNORM I'/(4f18.0))
      endif

      ia = sign (1.d0, d(1))
      na = min (int (abs (d(1))), nw)
      if (na .eq. 0)  then
        a(1) = 0.
        a(2) = 0.
        goto 170
      endif
      n4 = na + 4
      a2 = d(2)
      d(2) = 0.d0

110   continue
c>
c   Try a vectorized fixup loop three times, unless A is very short.  This
c   should handle 99% of the inputs.  On RISC computers, it is more
c   efficient to completely bypass this loop, by uncommenting the next line.

      goto 120
      if (na .le. 8) goto 120

      do k = 1, 3
        s1 = 0.d0

cdir$ ivdep
        do i = 3, n4
          t2 = rdx * d(i)
          t1 = int (t2)
          if (t2 .lt. 0.d0 .and. t1 .ne. t2) t1 = t1 - 1.d0
          d(i) = d(i) - t1 * bdx
          d(i-1) = d(i-1) + t1
          s1 = s1 + abs (t1)
        enddo

        if (s1 .eq. 0.d0) goto 140
      enddo

c   Still not fixed - use recursive loop.  This loop is not vectorizable,
c   but it is guaranteed to complete the job in one pass.

120   t1 = 0.d0

      do i = n4, 3, -1
        t3 = t1 + d(i)
        t2 = rdx * (t3)
        t1 = int (t2)
        if (t2 .lt. 0.d0 .and. t1 .ne. t2) t1 = t1 - 1.d0
        d(i) = t3 - t1 * bdx
      enddo

      d(2) = d(2) + t1

140   continue

      if (d(2) .lt. 0.d0) then

c   D(2) is negative -- negate all words and re-normalize.

        ia = - ia
        d(3) = d(3) + bdx * d(2)
        d(2) = 0.d0

        do i = 2, n4
          d(i) = - d(i)
        enddo

        goto 110
      elseif (d(2) .gt. 0.d0) then

c   The fixup loops above "spilled" a nonzero number into D(2).  Shift the
c   entire number right one cell.  The exponent and length of the result
c   are increased by one.

        do i = n4, 3, -1
          a(i) = d(i-1)
        enddo

        na = min (na + 1, nw)
        a2 = a2 + 1.
      else
        do i = 3, n4
          a(i) = d(i)
        enddo
      endif

c   Perform rounding and truncation.

      a(1) = sign (na, ia)
      a(2) = a2

      call mproun (a)

170   if (idb .ge. 9) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 2) (a(i), i = 1, no)
2       format ('MPNORM O'/(6f12.0))
      endif
      return
      end

      subroutine mpnpwr (a, n, b)

c   This computes the N-th power of the MP number A and returns the MP result
c   in B.  When N is zero, 1 is returned.  When N is negative, the reciprocal
c   of A ^ |N| is returned.  For extra high levels of precision, use MPNPWX.
c   Debug output starts with IDB = 7.

c   Max SP space for B: NW + 4 cells.  Max SP scratch space: 2 * NW + 10
c   cells.  Max DP scratch space: NW + 5 cells.

c   This routine employs the binary method for exponentiation.

      double precision cl2, t1
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0, ndb = 22)
      dimension a(nw+2), b(nw+4), f1(8)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 7) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) n, (a(i), i = 1, no)
 1      format ('MPNPWR I',i5/(6f12.0))
      endif

      na = min (int (abs (a(1))), nw)
      if (na .eq. 0) then
        if (n .ge. 0) then
          b(1) = 0.
          b(2) = 0.
          goto 120
        else
          if (ker(57) .ne. 0) then
            write (ldb, 2)
 2          format ('*** MPNPWR: Argument is zero and N is negative or',
     $        ' zero.')
            ier = 57
            if (ker(ier) .eq. 2) call mpabrt
          endif
          return
        endif
      endif

      n5 = nw + 5
      ns = 2 * n5
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n5
      nws = nw
      nw = nw + 1
      nn = abs (n)
      f1(1) = 1.
      f1(2) = 0.
      f1(3) = 1.
      f1(4) = 0.
      if (nn .eq. 0) then
        call mpeq (f1, b)
        nw = nws
        ics = iss
        goto 120
      elseif (nn .eq. 1) then
        call mpeq (a, b)
        goto 110
      elseif (nn .eq. 2) then
        call mpmul (a, a, s(k0))
        call mpeq (s(k0), b)
        goto 110
      endif

c   Determine the least integer MN such that 2 ^ MN .GT. NN.

      t1 = nn
      mn = cl2 * log (t1) + 1.d0 + rxx
      call mpeq (f1, b)
      call mpeq (a, s(k0))
      kn = nn

c   Compute B ^ N using the binary rule for exponentiation.

      do 100 j = 1, mn
        kk = kn / 2
        if (kn .ne. 2 * kk) then
          call mpmul (b, s(k0), s(k1))
          call mpeq (s(k1), b)
        endif
        kn = kk
        if (j .lt. mn) then
          call mpmul (s(k0), s(k0), s(k1))
          call mpeq (s(k1), s(k0))
        endif
 100  continue

c   Compute reciprocal if N is negative.

 110  if (n .lt. 0) then
        call mpdiv (f1, b, s(k0))
        call mpeq (s(k0), b)
      endif

c   Restore original precision level.

      nw = nws
      ics = iss
      call mproun (b)

 120  if (idb .ge. 7) then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 3) (b(i), i = 1, no)
 3      format ('MPNPWR O'/(6f12.0))
      endif
      return
      end

      subroutine mpnpwx (a, n, b)

c   This computes the N-th power of the MP number A and returns the MP result
c   in B.  When N is zero, 1 is returned.  When N is negative, the reciprocal
c   of A ^ |N| is returned.  Before calling MPNPWX, the array in MPCOM5 must
c   be initialized by calling MPINIX.  For modest levels of precision, use
c   MPNPWR.  NW should be a power of two.  The last two words of the result
c   are not reliable.  Debug output starts with IDB = 6.

c   Max SP space for B: NW + 4 cells.  Max SP scratch space: 5 * NW + 20
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   This routine employs the binary method for exponentiation.

      double precision cl2, t1
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0, ndb = 22)
      dimension a(nw+2), b(nw+4), f1(8)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 6) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) n, (a(i), i = 1, no)
 1      format ('MPNPWX I',i5/(6f12.0))
      endif

      ncr = 2 ** mcr
      na = min (int (abs (a(1))), nw)

c   Check if precision level of A is too low to justify the advanced routine.

      if (na .le. ncr .and. n .ge. 0 .and. n .le. 4) then
        call mpnpwr (a, n, b)
        goto 120
      endif
      if (na .eq. 0) then
        if (n .ge. 0) then
          b(1) = 0.
          b(2) = 0.
          goto 120
        else
          if (ker(58) .ne. 0) then
            write (ldb, 2)
 2          format ('*** MPNPWX: argument is zero and N is negative or',
     $        ' zero.')
            ier = 58
            if (ker(ier) .eq. 2) call mpabrt
          endif
          return
        endif
      endif

      n4 = nw + 4
      ns = 2 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n4
      nn = abs (n)
      f1(1) = 1.
      f1(2) = 0.
      f1(3) = 1.
      f1(4) = 0.
      if (nn .eq. 0) then
        call mpeq (f1, b)
        ics = iss
        goto 120
      elseif (nn .eq. 1) then
        call mpeq (a, b)
        goto 110
      elseif (nn .eq. 2) then
        call mpsqx (a, b)
        goto 110
      endif

c   Determine the least integer MN such that 2 ^ MN .GT. NN.

      t1 = nn
      mn = cl2 * log (t1) + 1.d0 + rxx
      call mpeq (f1, b)
      call mpeq (a, s(k0))
      kn = nn

c   Compute B ^ N using the binary rule for exponentiation.

      do 100 j = 1, mn
        kk = kn / 2
        if (kn .ne. 2 * kk) then
          call mpmulx (b, s(k0), s(k1))
          call mpeq (s(k1), b)
        endif
        kn = kk
        if (j .lt. mn) then
          call mpsqx (s(k0), s(k1))
          call mpeq (s(k1), s(k0))
        endif
 100  continue

c   Compute reciprocal if N is negative.

 110  if (n .lt. 0) then
        call mpdivx (f1, b, s(k0))
        call mpeq (s(k0), b)
      endif
      ics = iss

 120  if (idb .ge. 6) then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 3) (b(i), i = 1, no)
 3      format ('MPNPWX O'/(6f12.0))
      endif
      return
      end

      subroutine mpnrt (a, n, b)

c   This computes the N-th root of the MP number A and returns the MP result
c   in B.  N must be at least one and must not exceed 2 ^ 30.  For extra high
c   levels of precision, use MPNRTX.  Debug output starts with IDB = 7.

c   Max SP space for B: NW + 4 cells.  Max SP scratch space: 6 * NW + 32
c   cells.  Max DP scratch space: NW + 6 cells.

c   This subroutine employs the following Newton-Raphson iteration, which
c   converges to A ^ (-1/N):

c          X_{k+1} = X_k + (X_k / N) * (1 - A * X_k^N)

c   The reciprocal of the final approximation to A ^ (-1/N) is the N-th root.
c   These iterations are performed with a maximum precision level NW that
c   is dynamically changed, approximately doubling with each iteration.
c   See the comment about the parameter NIT in MPDIVX.

c   When N is large and A is very near one, the following binomial series is
c   employed instead of the Newton scheme:

c       (1 + x)^(1/N)  =  1  +  x / N  +  x^2 * (1 - N) / (2! N^2)  +  ...

      double precision alt, cl2, t1, t2, tn
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (alt = 0.693147180559945309d0,
     $  cl2 = 1.4426950408889633d0, ndb = 22, nit = 3, n30 = 2 ** 30)
      dimension a(nw+2), b(nw+4), f1(8), f2(8)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 7) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) n, (a(i), i = 1, no)
 1      format ('MPNRT I',i5/(6f12.0))
      endif

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)

      if (na .eq. 0) then
        b(1) = 0.
        b(2) = 0.
        goto 140
      endif
      if (ia .lt. 0) then
        if (ker(59) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPNRT: Argument is negative.')
          ier = 59
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif
      if (n .le. 0 .or. n .gt. n30) then
        if (ker(60) .ne. 0) then
          write (ldb, 3) n
 3        format ('*** MPNRT: Improper value of N',i10)
          ier = 60
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   If N = 1, 2 or 3, call MPEQ, MPSQRT or MPCBRT.  These are faster.

      if (n .eq. 1) then
        call mpeq (a, b)
        goto 140
      elseif (n .eq. 2) then
        call mpsqrt (a, b)
        goto 140
      elseif (n .eq. 3) then
        call mpcbrt (a, b)
        goto 140
      endif

      n5 = nw + 5
      ns = 4 * n5
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n5
      k2 = k1 + n5
      k3 = k2 + n5
      nws = nw
      f1(1) = 1.
      f1(2) = 0.
      f1(3) = 1.
      f1(4) = 0.

c   Determine the least integer MQ such that 2 ^ MQ .GE. NW.

      t1 = nw
      mq = cl2 * log (t1) + 1.d0 - rxx

c   Check how close A is to 1.

      call mpsub (a, f1, s(k0))
      if (s(k0) .eq. 0.) then
        call mpeq (f1, b)
        ics = iss
        goto 140
      endif
      call mpmdc (s(k0), t1, n1)
      n2 = cl2 * log (abs (t1))
      t1 = t1 * 0.5d0 ** n2
      n1 = n1 + n2
      if (n1 .le. -30) then
        t2 = n
        n2 = cl2 * log (t2) + 1.d0 + rxx
        n3 = - nbt * nw / n1
        if (n3 .lt. 1.25 * n2) then

c   A is so close to 1 that it is cheaper to use the binomial series.

          nw = nw + 1
          call mpdivd (s(k0), t2, 0, s(k1))
          call mpadd (f1, s(k1), s(k2))
          k = 0

 100      k = k + 1
          t1 = 1 - k * n
          t2 = (k + 1) * n
          call mpmuld (s(k1), t1, 0, s(k3))
          call mpdivd (s(k3), t2, 0, s(k1))
          call mpmul (s(k0), s(k1), s(k3))
          call mpeq (s(k3), s(k1))
          call mpadd (s(k1), s(k2), s(k3))
          call mpeq (s(k3), s(k2))
          if (s(k1) .ne. 0. .and. s(k1+1) .ge. - nw) goto 100

          call mpeq (s(k2), b)
          call mpdiv (f1, s(k2), s(k0))
          goto 130
        endif
      endif

c   Compute the initial approximation of A ^ (-1/N).

      tn = n
      call mpmdc (a, t1, n1)
      n2 = - n1 / tn
      t2 = exp (-1.d0 / tn * (log (t1) + (n1 + tn * n2) * alt))
      t2 = (t1 * 2.d0 ** (n1 + tn * n2)) ** (- 1.d0 / tn)
      call mpdmc (t2, n2, b)
      call mpdmc (tn, 0, f2)
      nw = 3
      iq = 0

c   Perform the Newton-Raphson iteration described above with a dynamically
c   changing precision level NW (one greater than powers of two).

      do 120 k = 2, mq
        nw = min (2 * nw - 2, nws) + 1
 110    continue
        call mpnpwr (b, n, s(k0))
        call mpmul (a, s(k0), s(k1))
        call mpsub (f1, s(k1), s(k0))
        call mpmul (b, s(k0), s(k1))
        call mpdivd (s(k1), tn, 0, s(k0))
        call mpadd (b, s(k0), s(k1))
        call mpeq (s(k1), b)
        if (k .eq. mq - nit .and. iq .eq. 0) then
          iq = 1
          goto 110
        endif
 120  continue

c   Take the reciprocal to give final result.

      call mpdiv (f1, b, s(k1))
      call mpeq (s(k1), b)

c   Restore original precision level.

 130  nw = nws
      ics = iss
      call mproun (b)

 140  if (idb .ge. 7) then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 4) (b(i), i = 1, no)
 4      format ('MPNRT O'/(6f12.0))
      endif
      return
      end

      subroutine mpnrtx (a, n, b)

c   This computes the N-th root of the MP number A and returns the MP result
c   in B.  N must be at least one and must not exceed 2 ^ 30.  Before calling
c   MPNRTX, the array in MPCOM5 must be initialized by calling MPINIX.  For
c   modest levels of precision, use MPNRT.  NW should be a power of two.  The
c   last three words of the result are not reliable.  Debug output starts with
c   IDB = 6.

c   Max SP space for B: NW + 4 cells.  Max SP scratch space: 9 * NW + 36
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   This routine uses basically the same Newton iteration algorithm as MPNRT.
c   In fact, this routine calls MPNRT to obtain an initial approximation.
c   See the comment about the parameter NIT in MPDIVX.

      double precision cl2, t1, t2, tn
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0, ndb = 22, nit = 1,
     $  n30 = 2 ** 30)
      dimension a(nw+2), b(nw+4), f1(8), f2(8)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 6) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) n, (a(i), i = 1, no)
 1      format ('MPNRTX I',i5/(6f12.0))
      endif

      ncr = 2 ** mcr
      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)

      if (na .eq. 0) then
        b(1) = 0.
        b(2) = 0.
        goto 140
      endif
      if (ia .lt. 0) then
        if (ker(61) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPNRTX: Argument is negative.')
          ier = 61
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif
      if (n .le. 0 .or. n .gt. n30) then
        if (ker(62) .ne. 0) then
          write (ldb, 3) n
 3        format ('*** MPNRTX: Improper value of N',i10)
          ier = 62
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Check if precision level is too low to justify the advanced routine.

      if (nw .le. ncr) then
        call mpnrt (a, n, b)
        goto 140
      endif

c   If N = 1, 2 or 3, call MPEQ, MPSQRX or MPCBRX.  These are faster.

      if (n .eq. 1) then
        call mpeq (a, b)
        goto 140
      elseif (n .eq. 2) then
        call mpsqrx (a, b)
        goto 140
      elseif (n .eq. 3) then
        call mpcbrx (a, b)
        goto 140
      endif

      n4 = nw + 4
      ns = 4 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4
      k3 = k2 + n4
      nws = nw
      f1(1) = 1.
      f1(2) = 0.
      f1(3) = 1.
      f1(4) = 0.

c   Determine the least integer MQ such that 2 ^ MQ .GE. NW.

      t1 = nw
      mq = cl2 * log (t1) + 1.d0 - rxx

c   Check how close A is to 1.

      call mpsub (a, f1, s(k0))
      if (s(k0) .eq. 0.) then
        call mpeq (f1, b)
        goto 130
      endif
      call mpmdc (s(k0), t1, n1)
      n2 = cl2 * log (abs (t1))
      t1 = t1 * 0.5d0 ** n2
      n1 = n1 + n2
      if (n1 .le. -30) then
        t2 = n
        n2 = cl2 * log (t2) + 1.d0 + rxx
        n3 = - nbt * nw / n1
        if (n3 .lt. 1.25 * n2) then

c   A is so close to 1 that it is cheaper to use the binomial series.

          call mpdivd (s(k0), t2, 0, s(k1))
          call mpadd (f1, s(k1), s(k2))
          k = 0

 100      k = k + 1
          t1 = 1 - k * n
          t2 = (k + 1) * n
          call mpmuld (s(k1), t1, 0, s(k3))
          call mpdivd (s(k3), t2, 0, s(k1))
          call mpmulx (s(k0), s(k1), s(k3))
          call mpeq (s(k3), s(k1))
          call mpadd (s(k1), s(k2), s(k3))
          call mpeq (s(k3), s(k2))
          if (s(k1) .ne. 0. .and. s(k1+1) .ge. - nw) goto 100

          call mpeq (s(k2), b)
          goto 130
        endif
      endif

c   Compute the initial approximation of A ^ (-1/N).

      nw = ncr
      call mpnrt (a, n, s(k0))
      call mpdiv (f1, s(k0), b)
      tn = n
      call mpdmc (tn, 0, f2)
      iq = 0

c   Perform the Newton-Raphson iteration described above with a dynamically
c   changing precision level NW (powers of two).

      do 120 k = mcr + 1, mq
        nw1 = nw
        nw = min (2 * nw, nws)
        nw2 = nw
 110    continue
        call mpnpwx (b, n, s(k0))
        call mpmulx (a, s(k0), s(k1))
        call mpsub (f1, s(k1), s(k0))
        nw = nw1
        call mpmulx (b, s(k0), s(k1))
        call mpdivd (s(k1), tn, 0, s(k0))
        nw = nw2
        call mpadd (b, s(k0), s(k1))
        call mpeq (s(k1), b)
        if (k .eq. mq - nit .and. iq .eq. 0) then
          iq = 1
          goto 110
        endif
 120  continue

c   Take the reciprocal to give final result.

      call mpdivx (f1, b, s(k0))
      call mpeq (s(k0), b)

 130  ics = iss

 140  if (idb .ge. 6) then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 4) (b(i), i = 1, no)
 4      format ('MPNRTX O'/(6f12.0))
      endif
      return
      end

      subroutine mpout (iu, a, la, cs)

c   This routine writes the exponent plus LA mantissa digits of the MP number
c   A to logical unit IU.  CS is a scratch array of type CHARACTER*1.  CS must
c   be dimensioned at least LA + 25.  The digits of A may span more than one
c   line.  A comma is placed at the end of the last line to denote the end of
c   the MP number.  Here is an example of the output:

c   10 ^        -4 x  3.14159265358979323846264338327950288419716939937510,

c   Max SP scratch space: 4 * NW + 22 cells.

      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      character*1 cs(la+25)
      dimension a(nw+2)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims

      if (ier .ne. 0) return

      nws = nw
      ll = la / log10 (bdx) + 2.d0
      nw = min (nw, ll)
      call mpoutc (a, cs, l)
      nw = nws
      l = min (l, la + 20) + 1
      cs(l) = ','
      write (iu, '(78A1)') (cs(i), i = 1, l)

      return
      end

      subroutine mpoutc (a, b, n)

c   Converts the MP number A into character form in the CHARACTER*1 array B.
c   N (an output parameter) is the length of the output.  In other words, B is
c   contained in B(1), ..., B(N).  The format is analogous to the Fortran
c   exponential format (E format), except that the exponent is placed first.
c   Debug output starts with IDB = 7.

c   Max CHARACTER*1 space for B: 7.225 * NW + 30 cells.  Max SP scratch space:
c   4 * NW + 22 cells.

c   This routine is called by MPOUT, but it may be directly called by the user
c   if desired for custom output.  Example:

c      CHARACTER*1 CX(800)
c      CALL MPOUTC (A, CX, ND)
c      WRITE (1, '(20A1/(72A1))') (CX(I), I = 1, ND)

      double precision aa, al2, t1
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      character*1 b
      character*16 ca
      parameter (al2 = 0.301029995663981195d0, con = 0.8304820235d0,
     $  ndb = 22)
      dimension a(nw+2), b(*), f(8)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = ' '
        n = 0
        return
      endif
      if (idb .ge. 7) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPOUTC I'/(6f12.0))
      endif

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)
      n5 = nw + 5
      ns = 2 * n5
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n5
      nws = nw
      nw = nw + 1
      f(1) = 1.
      f(2) = 0.
      f(3) = 10.
      f(4) = 0.

c   Determine exact power of ten for exponent.

      if (na .ne. 0) then
        aa = a(3)
        if (na .ge. 2) aa = aa + rdx * a(4)
        if (na .ge. 3) aa = aa + rx2 * a(5)
        if (na .ge. 4) aa = aa + rdx * rx2 * a(6)
        t1 = al2 * nbt * a(2) + log10 (aa)
        if (t1 .ge. 0.d0) then
          nx = t1
        else
          nx = t1 - 1.d0
        endif
        call mpnpwr (f, nx, s(k0))
        call mpdiv (a, s(k0), s(k1))

c   If we didn't quite get it exactly right, multiply or divide by 10 to fix.

 100    if (s(k1+1) .lt. 0.) then
          nx = nx - 1
          call mpmuld (s(k1), 10.d0, 0, s(k0))
          call mpeq (s(k0), s(k1))
          goto 100
        elseif (s(k1+2) .ge. 10.) then
          nx = nx + 1
          call mpdivd (s(k1), 10.d0, 0, s(k0))
          call mpeq (s(k0), s(k1))
          goto 100
        endif
        s(k1) = abs (s(k1))
      else
        nx = 0
      endif

c   Place exponent first instead of at the very end as in Fortran.

      b(1) = '1'
      b(2) = '0'
      b(3) = ' '
      b(4) = '^'
      write (ca, '(I10)') nx

      do 110 i = 1, 10
        b(i+4) = ca(i:i)
 110  continue

      b(15) = ' '
      b(16) = 'x'
      b(17) = ' '

c   Insert sign and first digit.

      if (ia .eq. -1) then
        b(18) = '-'
      else
        b(18) = ' '
      endif
      if (na .ne. 0) then
        nn = s(k1+2)
      else
        nn = 0
      endif
      write (ca, '(I1)') nn
      b(19) = ca(1:1)
      b(20) = '.'
      ix = 20
      if (na .eq. 0) goto 190
      f(3) = nn
      call mpsub (s(k1), f, s(k0))
      if (s(k0) .eq. 0) goto 190
      call mpmuld (s(k0), 1.d6, 0, s(k1))
      nl = max (nw * log10 (bdx) / 6.d0 - 1.d0, 1.d0)

c   Insert the digits of the remaining words.

      do 130 j = 1, nl
        if (s(k1+1) .eq. 0.) then
          nn = s(k1+2)
          f(1) = 1.
          f(3) = nn
        else
          f(1) = 0.
          nn = 0
        endif
        write (ca, '(I6.6)') nn

        do 120 i = 1, 6
          b(i+ix) = ca(i:i)
 120    continue

        ix = ix + 6
        call mpsub (s(k1), f, s(k0))
        call mpmuld (s(k0), 1.d6, 0, s(k1))
        if (s(k1) .eq. 0.) goto 140
 130  continue

c   Check if trailing zeroes should be trimmed.

      j = nl + 1

 140  l = ix
      if (b(l) .eq. '0' .or. (j .gt. nl .and. b(l-1) .eq. '0' .and.
     $  b(l-2) .eq. '0' .and. b(l-3) .eq. '0')) then
        b(l) = ' '

        do 150 i = l - 1, 21, -1
          if (b(i) .ne. '0') then
            ix = i
            goto 190
          endif
          b(i) = ' '
 150    continue

        ix = 20

c   Check if trailing nines should be rounded up.

      elseif (j .gt. nl .and. b(l-1) .eq. '9' .and. b(l-2) .eq. '9'
     $    .and. b(l-3) .eq. '9') then
        b(l) = ' '

        do 160 i = l - 1, 21, -1
          if (b(i) .ne. '9') goto 180
          b(i) = ' '
 160    continue

c   We have rounded away all digits to the right of the decimal point, and the
c   digit to the left of the digit is a 9.  Set the digit to 1 and increase
c   the exponent by one.

        ix = 20
        if (b(19) .eq. '9') then
          b(19) = '1'
          write (ca, '(I10)') nx + 1

          do 170 i = 1, 10
            b(i+4) = ca(i:i)
 170      continue

        else
          ca = b(19)
          read (ca, '(I1)') nn
          write (ca, '(I1)') nn + 1
          b(19) = ca(1:1)
        endif
        goto 190

 180    ca = b(i)
        read (ca, '(I1)') nn
        write (ca, '(I1)') nn + 1
        b(i) = ca(1:1)
        ix = i
      endif

 190  n = ix
      nw = nws
      ics = iss
      if (idb .ge. 7) then
        no = min (n, 6 * ndb + 20)
        write (ldb, 2) (b(i), i = 1, no)
 2      format ('MPOUTC O'/(78a1))
      endif
      return
      end

      subroutine mppi (pi)

c   This computes Pi to available precision (NW mantissa words).  For extra
c   high levels of precision, use MPPIX.  Debug output starts with IDB = 7.

c   Max SP space for PI: NW + 4 cells.  Max SP scratch space: 8 * NW + 43
c   cells.  Max DP scratch space: NW + 6 cells.

c   The algorithm that is used for computing Pi, which is due to Salamin
c   and Brent, is as follows:

c   Set  A_0 = 1,  B_0 = 1/Sqrt(2)  and  D_0 = Sqrt(2) - 1/2.

c   Then from k = 1 iterate the following operations:

c      A_k = 0.5 * (A_{k-1} + B_{k-1})
c      B_k = Sqrt (A_{k-1} * B_{k-1})
c      D_k = D_{k-1} - 2^k * (A_k - B_k) ^ 2

c   Then  P_k = (A_k + B_k) ^ 2 / D_k  converges quadratically to Pi.
c   In other words, each iteration approximately doubles the number of correct
c   digits, providing all iterations are done with the maximum precision.

      double precision cl2, t1
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0)
      dimension f(8), pi(nw+4)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        pi(1) = 0.
        pi(2) = 0.
        return
      endif

c   Perform calculations to one extra word accuracy.

      n5 = nw + 5
      ns = 5 * n5
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n5
      k2 = k1 + n5
      k3 = k2 + n5
      k4 = k3 + n5
      nws = nw
      nw = nw + 1

c   Determine the number of iterations required for the given precision level.
c   This formula is good only for this Pi algorithm.

      t1 = nws * log10 (bdx)
      mq = cl2 * (log (t1) - 1.d0) + 1.d0

c  Initialize as above.

      s(k0) = 1.
      s(k0+1) = 0.
      s(k0+2) = 1.
      f(1) = 1.
      f(2) = 0.
      f(3) = 2.
      f(4) = 0.
      call mpsqrt (f, s(k2))
      call mpmuld (s(k2), 0.5d0, 0, s(k1))
      f(2) = -1.
      f(3) = 0.5d0 * bdx
      call mpsub (s(k2), f, s(k4))

c   Perform iterations as described above.

      do 100 k = 1, mq
        call mpadd (s(k0), s(k1), s(k2))
        call mpmul (s(k0), s(k1), s(k3))
        call mpsqrt (s(k3), s(k1))
        call mpmuld (s(k2), 0.5d0, 0, s(k0))
        call mpsub (s(k0), s(k1), s(k2))
        call mpmul (s(k2), s(k2), s(k3))
        t1 = 2.d0 ** k
        call mpmuld (s(k3), t1, 0, s(k2))
        call mpsub (s(k4), s(k2), s(k3))
        call mpeq (s(k3), s(k4))
 100  continue

c   Complete computation.

      call mpadd (s(k0), s(k1), s(k2))
      call mpmul (s(k2), s(k2), s(k3))
      call mpdiv (s(k3), s(k4), s(k2))
      call mpeq (s(k2), pi)

c   Restore original precision level.

      nw = nws
      ics = iss
      call mproun (pi)

      if (idb .ge. 7) call mpdeb ('MPPI O', pi)
      return
      end

      subroutine mppix (pi)

c   This computes Pi to available precision (NW mantissa words).  Before
c   calling MPPIX, the array in MPCOM5 must be initialized by calling MPINIX.
c   For modest levels of precision, use MPPI.  NW should be a power of two.
c   The last three words of the result are not reliable.  Debug output starts
c   with IDB = 7.

c   Max SP space for PI: NW + 4 cells.  Max SP scratch space: 9.5 * NW + 47
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   This routine uses basically the same algorithm as MPPI.

      double precision cl2, t1
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0)
      dimension f(8), pi(nw+4)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        pi(1) = 0.
        pi(2) = 0.
        return
      endif
      ncr = 2 ** mcr

c   Check if precision level is too low to justify the advanced routine.

      if (nw .le. ncr) then
        call mppi (pi)
        goto 110
      endif
      n4 = nw + 4
      ns = 5 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4
      k3 = k2 + n4
      k4 = k3 + n4

c   Determine the number of iterations required for the given precision level.
c   This formula is good only for this Pi algorithm.

      t1 = nw * log10 (bdx)
      mq = cl2 * (log (t1) - 1.d0) + 1.d0

c  Initialize as above.

      s(k0) = 1.
      s(k0+1) = 0.
      s(k0+2) = 1.
      f(1) = 1.
      f(2) = 0.
      f(3) = 2.
      f(4) = 0.
      call mpsqrx (f, s(k2))
      call mpmuld (s(k2), 0.5d0, 0, s(k1))
      f(2) = -1.
      f(3) = 0.5d0 * bdx
      call mpsub (s(k2), f, s(k4))

c   Perform iterations as described above.

      do 100 k = 1, mq
        call mpadd (s(k0), s(k1), s(k2))
        call mpmulx (s(k0), s(k1), s(k3))
        call mpsqrx (s(k3), s(k1))
        call mpmuld (s(k2), 0.5d0, 0, s(k0))
        call mpsub (s(k0), s(k1), s(k2))
        call mpsqx (s(k2), s(k3))
        t1 = 2.d0 ** k
        call mpmuld (s(k3), t1, 0, s(k2))
        call mpsub (s(k4), s(k2), s(k3))
        call mpeq (s(k3), s(k4))
 100  continue

c   Complete computation.

      call mpadd (s(k0), s(k1), s(k2))
      call mpsqx (s(k2), s(k3))
      call mpdivx (s(k3), s(k4), s(k2))
      call mpeq (s(k2), pi)
      ics = iss

 110  if (idb .ge. 7) call mpdeb ('MPPIX O', pi)
      return
      end

      subroutine mppol (n, l, a, x1, nx, x)

c   This finds a real root of the N-th degree polynomial whose MP coefficients
c   are in A by Newton-Raphson iterations, beginning at the DPE value (X1, NX)
c   and returns the MP root in X.  The N + 1 coefficients a_0, a_1, ..., a_N
c   are assumed to start in locations A(1), A(L+1), A(2*L+1), etc.  For extra
c   high levels of precision, use MPPOLX.  Debug output starts with IDB = 6.

c   Max SP space for X: NW + 4 cells.  Max SP scratch space: 5 * NW + 25
c   cells.  Max DP scratch space: NW + 5 cells.

c   One requirement for this routine to work is that the desired root is not
c   a repeated root.  If one wishes to apply this routine to find a repeated
c   root, it is first necessary to reduce the polynomial to one that has only
c   simple roots.  This can be done by performing the Euclidean algorithm in
c   the ring of polynomials to determine the greatest common divisor Q(t) of
c   P(t) and P'(t).  Here P(t) is the polynomial a_0 + a_1 t + a_2 t^2 +
c   ... + a_n t^n, and P'(t) is the derivative of P(t).  Then R(t) = P(t)/Q(t)
c   is a polynomial that has only simple roots.

c   This routine employs the standard form of the Newton-Raphson iteration:

c   X_{k+1} = X_k - P(X_k) / P'(X_k)

c   These iterations are performed with a maximum precision level NW that is
c   dynamically changed, approximately doubling with each iteration.

      character*8 cx
      double precision t1, x1
      dimension a(l,n+1), x(nw+4)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        x(1) = 0.
        x(2) = 0.
        return
      endif
      if (idb .ge. 6) then
        write (ldb, 1) n
 1      format ('MPPOL I',i4)

        do 100 k = 0, n
          write (cx, '(I4)') k
          call mpdeb (cx, a(1,k+1))
 100    continue

        write (ldb, 2) x1, nx
 2      format ('MPPOL I',f16.12,' x 10 ^',i6)
      endif

c  Check if the polynomial is proper.

      if (a(1,1) .eq. 0. .or. a(1,n+1) .eq. 0.) then
        if (ker(63) .ne. 0) then
          write (ldb, 3)
 3        format ('*** MPPOL: Either the first or last input ',
     $      'coefficient is zero.')
          ier = 63
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      n5 = nw + 5
      ns = 5 * n5
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n5
      k2 = k1 + n5
      k3 = k2 + n5
      k4 = k3 + n5
      nws = nw
      nw = nw + 1

c   Set the initial value.

      call mpdmc (x1, nx, s(k0))
      nw = 5
      tl = -4.
      l1 = 0
      ls = -10

c   Perform MP Newton-Raphson iterations to solve P(x) = 0.

 110  l1 = l1 + 1
      if (l1 .eq. 50) then
        if (ker(64) .ne. 0) then
          write (ldb, 4)
 4        format ('*** MPPOL: Iteration limit exceeded.')
          ier = 64
          if (ker(ier) .eq. 2) call mpabrt
          nw = nws
          ics = iss
          return
        endif
      endif

c   Compute P(x).

      call mpeq (a(1,n+1), s(k1))

      do 120 k = n - 1, 0, -1
        call mpmul (s(k0), s(k1), s(k2))
        call mpadd (s(k2), a(1,k+1), s(k1))
 120  continue

c   Compute P'(x).

      t1 = n
      call mpmuld (a(1,n+1), t1, 0, s(k2))

      do 130 k = n - 1, 1, -1
        call mpmul (s(k0), s(k2), s(k3))
        t1 = k
        call mpmuld (a(1,k+1), t1, 0, s(k4))
        call mpadd (s(k3), s(k4), s(k2))
 130  continue

c   Compute P(x) / P'(x) and update x.

      call mpdiv (s(k1), s(k2), s(k3))
      call mpsub (s(k0), s(k3), s(k4))

      if (idb .ge. 7) then
        write (ldb, 5) l1
 5      format ('Iteration',i4)
        call mpdeb ('X', s(k0))
        call mpdeb ('P(X)', s(k1))
        call mpdeb ('P''(X)', s(k2))
        call mpdeb ('CORR', s(k3))
      endif
      call mpeq (s(k4), s(k0))

c   If this was the second iteration at full precision, there is no need to
c   continue (the adjusted value of x is correct); otherwise repeat.

      if (l1 .eq. ls + 1) goto 140
      if (s(k3) .ne. 0. .and. s(k3+1) .gt. tl) goto 110

c   Newton iterations have converged to current precision.  Increase precision
c   and continue.

      if (nw .eq. nws + 1) goto 140
      nw = min (2 * nw - 2, nws) + 1
      if (nw .eq. nws + 1) ls = l1
      tl = 1 - nw
      if (idb .ge. 7) then
        write (ldb, 6) nw
 6      format (6x,'New NW =', i8)
      endif
      goto 110

 140  call mpeq (s(k0), x)

c   Restore original precision level.

      nw = nws
      ics = iss
      call mproun (x)

      if (idb .ge. 6) then
        write (ldb, 7) l1
 7      format ('Iteration count:',i5)
        call mpdeb ('MPPOL O', x)
      endif
      return
      end

      subroutine mppolx (n, l, a, x1, nx, x)

c   This finds a real root of the N-th degree polynomial whose MP coefficients
c   are in A by Newton-Raphson iterations, beginning at the DP value (X1, NX)
c   and returns the MP root in X.  The N + 1 coefficients a_0, a_1, ..., a_N
c   are assumed to start in locations A(1), A(L+1), A(2*L+1), etc.  Before
c   calling MPPOLX, the array in MPCOM5 must be initialized by calling MPINIX.
c   For modest levels of precision, use MPPOL.  NW should be a power of two.
c   The last three words of the result are not reliable.  Debug output starts
c   with IDB = 5.

c   Max SP space for X: NW + 4 cells.  Max SP scratch space: 8 * NW + 32
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   For a discussion of the algorithm and usage, see MPPOL.  This routine uses
c   basically the same Newton iteration algorithm as MPPOL.  In fact, this
c   routine calls MPPOL to obtain an initial approximation.

      character*8 cx
      double precision t1, x1
      dimension a(l,n+1), x(nw+4)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        x(1) = 0.
        x(2) = 0.
        return
      endif
      if (idb .ge. 5) then
        write (ldb, 1) n
 1      format ('MPPOLX I',i4)

        do 100 k = 0, n
          write (cx, '(I4)') k
          call mpdeb (cx, a(1,k+1))
 100    continue

        write (ldb, 2) x1, nx
 2      format ('MPPOLX I',f16.12,' x 10 ^',i6)
      endif

c   Check if precision level is too low to justify the advanced routine.

      ncr = 2 ** mcr
      if (nw .le. ncr) then
        call mppol (n, l, a, x1, nx, x)
        l1 = 0
        goto 150
      endif

c  Check if the polynomial is proper.

      if (a(1,1) .eq. 0. .or. a(1,n+1) .eq. 0.) then
        if (ker(65) .ne. 0) then
          write (ldb, 3)
 3        format ('*** MPPOLX: Either the first or last input ',
     $      'coefficient is zero.')
          ier = 65
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      n4 = nw + 4
      ns = 5 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4
      k3 = k2 + n4
      k4 = k3 + n4
      nws = nw

c   Compute the initial approximation.

      nw = ncr
      call mppol (n, l, a, x1, nx, x)
      call mpeq (x, s(k0))
      tl = 2 - nw
      l1 = 0
      ls = -10

c   Perform MP Newton-Raphson iterations to solve P(x) = 0.

 110  l1 = l1 + 1
      if (l1 .eq. 50) then
        if (ker(66) .ne. 0) then
          write (ldb, 4)
 4        format ('*** MPPOLX: Iteration limit exceeded.')
          ier = 66
          if (ker(ier) .eq. 2) call mpabrt
          nw = nws
          ics = iss
          return
        endif
      endif

c   Compute P(x).

      call mpeq (a(1,n+1), s(k1))

      do 120 k = n - 1, 0, -1
        call mpmulx (s(k0), s(k1), s(k2))
        call mpadd (s(k2), a(1,k+1), s(k1))
 120  continue

c   Compute P'(x).

      t1 = n
      call mpmuld (a(1,n+1), t1, 0, s(k2))

      do 130 k = n - 1, 1, -1
        call mpmulx (s(k0), s(k2), s(k3))
        t1 = k
        call mpmuld (a(1,k+1), t1, 0, s(k4))
        call mpadd (s(k3), s(k4), s(k2))
 130  continue

c   Compute P(x) / P'(x) and update x.

      call mpdivx (s(k1), s(k2), s(k3))
      call mpsub (s(k0), s(k3), s(k4))

      if (idb .ge. 6) then
        write (ldb, 5) l1
 5      format ('Iteration',i4)
        call mpdeb ('X', s(k0))
        call mpdeb ('P(X)', s(k1))
        call mpdeb ('P''(X)', s(k2))
        call mpdeb ('CORR', s(k3))
      endif
      call mpeq (s(k4), s(k0))

c   If this was the second iteration at full precision, there is no need to
c   continue (the adjusted value of x is correct); otherwise repeat.

      if (l1 .eq. ls + 1) goto 140
      if (s(k3) .ne. 0. .and. s(k3+1) .gt. tl) goto 110

c   Newton iterations have converged to current precision.  Increase precision
c   and continue.

      if (nw .eq. nws) goto 140
      nw = min (2 * nw, nws)
      if (nw .eq. nws) ls = l1
      if (nw .le. 32) then
        tl = 2 - nw
      elseif (nw .le. 256) then
        tl = 3 - nw
      else
        tl = 4 - nw
      endif
      if (idb .ge. 6) then
        write (ldb, 6) nw
 6      format (6x,'New NW =', i8)
      endif
      goto 110

 140  call mpeq (s(k0), x)
      ics = iss

 150  if (idb .ge. 5) then
        write (ldb, 7) l1
 7      format ('Iteration count:',i5)
        call mpdeb ('MPPOLX O', x)
      endif
      return
      end

      subroutine mprand (a)
c>
c   This returns a pseudo-random MP number A between 0 and 1.  This routine
c   calls the pseudo-random number generator routine MPRANQ in the file below.
c   Better routines than MPRANQ are available for this purpose on some
c   computer systems.  If so, it is suggested that the call to MPRANQ here be
c   replaced by a call to its equivalent on the host system.  Note, however,
c   that test no. 55 of the TESTMP test suite will fail if another generator
c   is used.  Debug output starts with IDB = 9.

c   Max SP space for A: NW + 4 cells.

      double precision mpranq, sd, s0
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (ndb = 22, s0 = 314159265.d0)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      dimension a(nw+4)
      save sd
      data sd/s0/

      if (ier .ne. 0) then
        a(1) = 0.
        a(2) = 0.
        return
      endif
      a(1) = nw
      a(2) = -1.

      do 100 i = 3, nw + 4
        a(i) = aint (bdx * mpranq (sd))
 100  continue

      call mproun (a)

      if (idb .ge. 9) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPRAND O'/(6f12.0))
      endif
      return
      end

      function mpranq (sd)

c   This routine returns a pseudorandom DP floating number uniformly
c   distributed between 0 and 1, computed from the seed SD, which is updated
c   after each reference.  The initial value of SD should be an odd whole
c   number in the range (1, 2 ^ 30).  2 ^ 28 pseudorandom numbers with 30 bits
c   each are returned before repeating.  The same sequence is generated on any
c   computer system.

      double precision f7, r30, sd, t1, t2, t30, mpranq
      parameter (f7 = 78125.d0)
      save r30, t30
      data r30/0.d0/

c   If this is the first time MPRANQ has been called, compute R30 = 2^(-30)
c   and T30 = 2^30.  This must be done in a loop rather than by merely using
c   ** in order to insure the results are exact on all systems.

      if (r30 .eq. 0.d0) then
        r30 = 1.d0
        t30 = 1.d0

        do 100 i = 1, 30
          r30 = 0.5d0 * r30
          t30 = 2.d0 * t30
 100    continue

      endif

c   Generate a pseudorandom number using a linear congruential scheme.

      t1 = f7 * sd
      t2 = aint (r30 * t1)
      sd = t1 - t30 * t2
      mpranq = r30 * sd

      return
      end

      subroutine mprcft (is, m, x, y)

c   This performs an N-point real-to-complex FFT, where N = 2^M.  X and Y
c   are double precision arrays.  X is both the input and the output data
c   array, and Y is a scratch array.  N real values are input and N/2 + 1
c   complex pairs are output, with real and imaginary parts separated by
c   N/2 + 1 locations.  A call to MPRCFT with IS = 1 (or -1) indicates a call
c   to perform a complex-to-real FFT with positive (or negative) exponentials.
c   M must be at least three.  The arrays X and Y must be dimensioned with
c   N + 2 cells.  Before calling MPRCFT, the U array in MPCOM5 must be
c   initialized by calling MPINIX.

c   In this application, MPRCFT is called by MPMULX.  This routine is not
c   intended to be called directly by the user.

      implicit double precision (a-h, o-z)
      dimension x(*), y(*)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom5/ u(4100)

c   Set initial parameters.

      k = u(1)
      mx = mod (k, 64)
      nu = k / 64
      n = 2 ** m
      n2 = n / 2
      n21 = n2 + 1
      n4 = n / 4
      ku = n / 2
      kn = ku + nu

c   Check if input parameters are invalid.

      if ((is .ne. 1 .and. is .ne. -1) .or. m .lt. 3 .or. m .gt. mx)
     $  then
        if (ker(67) .ne. 0) then
          write (ldb, 1)  is, m, mx
 1        format ('*** MPRCFT: either U has not been initialized'/
     $      'or else one of the input parameters is invalid',3i5)
          ier = 67
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Copy X to Y such that Y(k) = X(2k-1) + i X(2k).

cdir$ ivdep
      do 100 k = 1, n2
        y(k) = x(2*k-1)
        y(k+n2) = x(2*k)
 100  continue

c   Perform a normal N/2-point FFT on Y.

      call mpcfft (is, m - 1, y, x)

c   Reconstruct the FFT of X.

      x(1) = 2.d0 * (y(1) + y(n21))
      x(n21+1) = 0.d0
      x(n4+1) = 2.d0 * y(n4+1)
      x(n4+1+n21) = 2.d0 * is * y(n4+n2+1)
      x(n21) = 2.d0 * (y(1) - y(n21))
      x(n+2) = 0.d0

cdir$ ivdep
      do 110 k = 2, n4
        y11 = y(k)
        y12 = y(k+n2)
        y21 = y(n2+2-k)
        y22 = y(n+2-k)
        a1 = y11 + y21
        a2 = y11 - y21
        b1 = y12 + y22
        b2 = y12 - y22
        u1 = u(k+ku)
        u2 = is * u(k+kn)
        t1 = u1 * b1 + u2 * a2
        t2 = - u1 * a2 + u2 * b1
        x(k) = a1 + t1
        x(k+n21) = b2 + t2
        x(n2+2-k) = a1 - t1
        x(n+3-k) = -b2 + t2
 110  continue

      return
      end

      subroutine mproun (a)

c   This performs rounding and truncation of the MP number A.  It is called
c   by MPNORM, and also by other subroutines when the precision level is
c   reduced by one.  It is not intended to be directly called by the user.

c   Maximum SP space for A:  NW + 4 cells.

c   The parameter AMX is the absolute value of the largest exponent word
c   allowed for MP numbers.

      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (amx = 2.e6)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      dimension a(nw+4)

      if (ier .ne. 0) then
        a(1) = 0.
        a(2) = 0.
        return
      endif

c   Check for initial zeroes.

      a2 = a(2)
      a(2) = 0.
      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)
      n4 = na + 4
      if (a(3) .eq. 0.) then

c   Find the first nonzero word and shift the entire number left.  The length
c   of the result is reduced by the length of the shift.

        do 100 i = 4, n4
          if (a(i) .ne. 0.)  goto 110
 100    continue

        a(1) = 0.
        a(2) = 0.
        goto 170

 110    k = i - 3

cdir$ ivdep
        do 120 i = 3, n4 - k
          a(i) = a(i+k)
 120    continue

        a2 = a2 - k
        na = na - max (k - 2, 0)
        if (k .eq. 2) a(na+3) = 0.
      endif

c   Perform rounding depending on IRD.

      if (na .eq. nw .and. ird .ge. 1) then
        if (ird .eq. 1 .and. a(na+3) .ge. 0.5d0 * bdx .or. ird .eq. 2
     $    .and. a(na+3) .ge. 1.) a(na+2) = a(na+2) + 1.

c   Release carries as far as necessary due to rounding.

        do 130 i = na + 2, 3, -1
          if (a(i) .lt. bdx) goto 140
          a(i) = a(i) - bdx
          a(i-1) = a(i-1) + 1.
 130    continue

c   Release of carries due to rounding continued all the way to the start --
c   i.e. number was entirely 9's.

        a(3) = a(2)
        na = 1
        a2 = a2 + 1.
      endif

 140  if (a(na+2) .eq. 0.) then

c   At least the last mantissa word is zero.  Find the last nonzero word
c   and adjust the length of the result accordingly.

        do 150 i = na + 2, 3, -1
          if (a(i) .ne. 0.) goto 160
 150    continue

        a(1) = 0.
        a(2) = 0.
        goto 170

 160    na = i - 2
      endif

c   Check for overflow and underflow.

      if (a2 .lt. - amx) then
        if (ker(68) .ne. 0) then
          write (ldb, 1)
 1        format ('*** MPROUN: Exponent underflow.')
          ier = 68
          if (ker(ier) .eq. 2) call mpabrt
        endif
      elseif (a2 .gt. amx) then
        if (ker(69) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPROUN: Exponent overflow.')
          ier = 69
          if (ker(ier) .eq. 2) call mpabrt
        endif
      endif

c   Check for zero.

      if (a(3) .eq. 0.) then
        a(1) = 0.
        a(2) = 0.
      else
        a(1) = sign (na, ia)
        a(2) = a2
        a(na+3) = 0.
        a(na+4) = 0.
      endif

 170  return
      end

      subroutine mpsetp (ia, ib)

c   This routine sets the parameter whose name is IA in common MPCOM1 to the
c   value IB.  By using this routine instead of merely including the MPCOM1
c   block in the code, a user may eliminate the possibility of confusion with
c   a variable name in his or her program.  IA is of type CHARACTER and IB
c   is the integer value.

      character*(*) ia
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims

      if (ia .eq. 'NW' .or. ia .eq. 'nw') then
        nw = ib
      elseif (ia .eq. 'IDB' .or. ia .eq. 'idb') then
        idb = ib
      elseif (ia .eq. 'LDB' .or. ia .eq. 'ldb') then
        ldb = ib
      elseif (ia .eq. 'IER' .or. ia .eq. 'ier') then
        ier = ib
      elseif (ia .eq. 'MCR' .or. ia .eq. 'mcr') then
        mcr = ib
      elseif (ia .eq. 'IRD' .or. ia .eq. 'ird') then
        ird = ib
      elseif (ia .eq. 'ICS' .or. ia .eq. 'ics') then
        ics = ib
      elseif (ia .eq. 'IHS' .or. ia .eq. 'ihs') then
        ihs = ib
      elseif (ia .eq. 'IMS' .or. ia .eq. 'ims') then
        ims = ib
      endif

      return
      end

      subroutine mpsort (n, la, a, ip)

c   This routine sorts the entries of the N-long MP vector A into ascending
c   order using the quicksort algorithm.  The entries of A are assumed to
c   start at A(1), A(LA+1), A(2*LA+1), etc. The permutation vector that would
c   sort the vector is returned in IP.  Debug output starts with IDB = 7.

c   Max integer space for IP: N cells.  Max SP scratch space: 2 * NW + 8 cells.

      character*8 cx
      dimension a(la,n), ip(n), ik(50), jk(50)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then

        do 100 i = 1, n
          ip(i) = i
 100    continue

        return
      endif
      if (idb .ge. 7) then
        write (ldb, 1) n, la
 1      format ('MPSORT I',2i6)

        do 110 k = 1, n
          write (cx, '(I4)') k
          call mpdeb (cx, a(1,k))
 110    continue

      endif

      n4 = nw + 4
      ns = 2 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n4

      do 120 i = 1, n
        ip(i) = i
 120  continue

      k = 1
      ik(1) = 1
      jk(1) = n

 130  i = ik(k)
      j = jk(k)
      iq = i
      jq = j
      it = (i + j + 1) / 2
      l = ip(j)
      ip(j) = ip(it)
      ip(it) = l
      call mpeq (a(1,ip(j)), s(k0))
      j = j - 1

 140  do 150 l = i, j
        call mpcpr (s(k0), a(1,ip(l)), ic)
        if (ic .lt. 0) goto 160
 150  continue

      i = j
      goto 190

 160  i = l

      do 170 l = j, i, -1
        call mpcpr (s(k0), a(1,ip(l)), ic)
        if (ic .gt. 0) goto 180
 170  continue

      j = i
      goto 190

 180  j = l
      if (i .ge. j)  goto 190
      l = ip(i)
      ip(i) = ip(j)
      ip(j) = l
      goto 140

 190  call mpcpr (s(k0), a(1,ip(i)), ic)
      if (ic .ge. 0) goto 200
      l = ip(jq)
      ip(jq) = ip(i)
      ip(i) = l

 200  k = k - 1
      jz = 0
      if (j .eq. iq)  goto 210
      k = k + 1
      jk(k) = j
      jz = 1

 210  i = i + 1
      if (i .eq. jq)  goto 220
      k = k + 1
      ik(k) = i
      jk(k) = jq
      if (jz .eq. 0)  goto 220
      if (j - iq .ge. jq - i)  goto 220
      ik(k-1) = i
      jk(k-1) = jq
      ik(k) = iq
      jk(k) = j

 220  if (k .gt. 0)  goto 130

      ics = iss
      if (idb .ge. 7) write (ldb, 2) ip
 2    format ('MPSORT O'/(8i9))
      return
      end

      subroutine mpsqrt (a, b)

c   This computes the square root of the MP number A and returns the MP result
c   in B.  For extra high levels of precision, use MPSQRX.  Debug output
c   starts with IDB = 7.

c   Max SP space for B: NW + 4 cells.  Max SP scratch space: 3 * NW + 15
c   cells.  Max DP scratch space: NW + 5 cells.

c   This subroutine employs the following Newton-Raphson iteration, which
c   converges to 1 / Sqrt(A):

c          X_{k+1} = X_k + 0.5 * (1 - X_k^2 * A) * X_k

c   where the muliplication () * X_k is performed with only half of the
c   normal level of precision.  These iterations are performed with a
c   maximum precision level NW that is dynamically changed, doubling with
c   each iteration.  The final iteration is performed as follows (this is
c   due to A. Karp):

c          Sqrt(A) = (A * X_n) + 0.5 * [A - (A * X_n)^2] * X_n  (approx.)

c   where the multiplications A * X_n and [] * X_n are performed with only
c   half of the final level of precision.  See the comment about the parameter
c   NIT is MPDIVX.

      double precision cl2, t1, t2
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0, ndb = 22, nit = 3)
      dimension a(nw+2), b(nw+4), f(8)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 7) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPSQRT I'/(6f12.0))
      endif

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)

      if (na .eq. 0) then
        b(1) = 0.
        b(2) = 0.
        goto 120
      endif
      if (ia .lt. 0.d0) then
        if (ker(70) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPSQRT: Argument is negative.')
          ier = 70
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

      n5 = nw + 5
      ns = 3 * n5
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n5
      k2 = k1 + n5
      nws = nw

c   Determine the least integer MQ such that 2 ^ MQ .GE. NW.

      t1 = nw
      mq = cl2 * log (t1) + 1.d0 - rxx

c   Compute the initial approximation of 1 / Sqrt(A).

      call mpmdc (a, t1, n)
      n2 = - n / 2
      t2 = sqrt (t1 * 2.d0 ** (n + 2 * n2))
      t1 = 1.d0 / t2
      call mpdmc (t1, n2, b)
      f(1) = 1.
      f(2) = 0.
      f(3) = 1.
      f(4) = 0.
      nw = 3
      iq = 0

c   Perform the Newton-Raphson iteration described above with a dynamically
c   changing precision level NW (one greater than powers of two).

      do 110 k = 2, mq - 1
        nw1 = nw
        nw = min (2 * nw - 2, nws) + 1
        nw2 = nw
 100    continue
        call mpmul (b, b, s(k0))
        call mpmul (a, s(k0), s(k1))
        call mpsub (f, s(k1), s(k0))
        nw = nw1
        call mpmul (b, s(k0), s(k1))
        call mpmuld (s(k1), 0.5d0, 0, s(k0))
        nw = nw2
        call mpadd (b, s(k0), s(k1))
        call mpeq (s(k1), b)
        if (k .eq. mq - nit .and. iq .eq. 0) then
          iq = 1
          goto 100
        endif
 110  continue

c   Perform last iteration using Karp's trick.

      call mpmul (a, b, s(k0))
      nw1 = nw
      nw = min (2 * nw - 2, nws) + 1
      nw2 = nw
      call mpmul (s(k0), s(k0), s(k1))
      call mpsub (a, s(k1), s(k2))
      nw = nw1
      call mpmul (s(k2), b, s(k1))
      call mpmuld (s(k1), 0.5d0, 0, s(k2))
      nw = nw2
      call mpadd (s(k0), s(k2), s(k1))
      call mpeq (s(k1), b)

c   Restore original precision level.

      nw = nws
      ics = iss
      call mproun (b)

 120  if (idb .ge. 7) then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 3) (b(i), i = 1, no)
 3      format ('MPSQRT O'/(6f12.0))
      endif
      return
      end

      subroutine mpsqrx (a, b)

c   This computes the square root of the MP number A and returns the MP result
c   in B.  Before calling MPSQRX, the array in MPCOM5 must be initialized by
c   calling MPINIX.  For modest levels of precision, use MPSQRT.  NW should be
c   a power of two.  The last three words of the result are not reliable.
c   Debug output starts with IDB = 6.

c   Max SP space for B: NW + 4 cells.  Max SP scratch space: 4.5 * NW + 27
c   cells.  Max DP scratch space: 12 * NW + 6 cells.

c   This routine uses basically the same Newton iteration algorithm as MPSQRT.
c   In fact, this routine calls MPSQRT to obtain an initial approximation.
c   See the comment about the parameter NIT in MPDIVX.

      double precision cl2, t1
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0, ndb = 22, nit = 1)
      dimension a(nw+2), b(nw+4), f(8)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom3/ s(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 6) then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPSQRX I'/(6f12.0))
      endif

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)
      ncr = 2 ** mcr

      if (na .eq. 0) then
        b(1) = 0.
        b(2) = 0.
        goto 120
      endif
      if (ia .lt. 0.d0) then
        if (ker(71) .ne. 0) then
          write (ldb, 2)
 2        format ('*** MPSQRX: Argument is negative.')
          ier = 71
          if (ker(ier) .eq. 2) call mpabrt
        endif
        return
      endif

c   Check if precision level is too low to justify the advanced routine.

      if (nw .le. ncr) then
        call mpsqrt (a, b)
        goto 120
      endif
      n4 = nw + 4
      ns = 3 * n4
      iss = ics
      ics = ics + ns
      ihs = max (ics, ihs)
      if (ics - 1 .gt. ims) call mpaler
      k0 = iss
      k1 = k0 + n4
      k2 = k1 + n4
      nws = nw

c   Determine the least integer MQ such that 2 ^ MQ .GE. NW.

      t1 = nw
      mq = cl2 * log (t1) + 1.d0 - rxx

c   Compute the initial approximation of 1 / Sqrt(A).

      nw = ncr
      call mpsqrt (a, s(k0))
      call mpdiv (s(k0), a, b)
      f(1) = 1.
      f(2) = 0.
      f(3) = 1.
      f(4) = 0.
      iq = 0

c   Perform the Newton-Raphson iteration described above with a dynamically
c   changing precision level NW (powers of two).

      do 110 k = mcr + 1, mq - 1
        nw1 = nw
        nw = min (2 * nw, nws)
        nw2 = nw
 100    continue
        call mpsqx (b, s(k0))
        call mpmulx (a, s(k0), s(k1))
        call mpsub (f, s(k1), s(k0))
        nw = nw1
        call mpmulx (b, s(k0), s(k1))
        call mpmuld (s(k1), 0.5d0, 0, s(k0))
        nw = nw2
        call mpadd (b, s(k0), s(k1))
        call mpeq (s(k1), b)
        if (k .eq. mq - nit .and. iq .eq. 0) then
          iq = 1
          goto 100
        endif
 110  continue

c   Perform last iteration using Karp's trick.

      call mpmulx (a, b, s(k0))
      nw1 = nw
      nw = min (2 * nw, nws)
      nw2 = nw
      call mpsqx (s(k0), s(k1))
      call mpsub (a, s(k1), s(k2))
      nw = nw1
      call mpmulx (s(k2), b, s(k1))
      call mpmuld (s(k1), 0.5d0, 0, s(k2))
      nw = nw2
      call mpadd (s(k0), s(k2), s(k1))
      call mpeq (s(k1), b)
      ics = iss

 120  if (idb .ge. 6) then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 3) (b(i), i = 1, no)
 3      format ('MPSQRX O'/(6f12.0))
      endif
      return
      end

      subroutine mpsqx (a, b)

c   This routine squares the MP number A to yield the MP product B.
c   Before calling MPSQX, the array in MPCOM5 must be initialized by calling
c   MPINIX.  For modest levels of precision, use MPMUL.  NW should be a power
c   of two.  Debug output starts with IDB = 8.

c   Max SP space for B: NW + 4 cells.  Max DP scratch space: 8 * NW + 4 cells.

c   This subroutine uses the same FFT technique as MPMULX.  It is faster
c   because only one forward FFT has to be computed.  See the comments in
c   MPMULX about obtaining the complete double-long result.
c>
c   See comments in MPMULX about the machine-dependent parameters ERM and MBT.

      double precision an, cl2, d, erm, t1, t2, t3, t4
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      parameter (cl2 = 1.4426950408889633d0, erm = 0.438d0, mbt = 53,
     $  ndb = 22)
      dimension a(nw+2), b(nw+4)
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom4/ d(4100)

      if (ier .ne. 0) then
        b(1) = 0.
        b(2) = 0.
        return
      endif
      if (idb .ge. 8)  then
        no = min (int (abs (a(1))), ndb) + 2
        write (ldb, 1) (a(i), i = 1, no)
 1      format ('MPSQX I'/(6f12.0))
      endif

      ia = sign (1., a(1))
      na = min (int (abs (a(1))), nw)
      ncr = 2 ** mcr

      if (na .eq. 0) then
        b(1) = 0.
        b(2) = 0.
        goto 170
      endif

c   Check if precision level of the argument is too low to justify this
c   advanced routine.

      if (na .le. ncr) then
        call mpmul (a, a, b)
        goto 170
      endif

c   Determine N1, the smallest power of two at least as large as NA.

      t1 = na
      m1 = cl2 * log (t1) + 1.d0 - rxx
      n1 = 2 ** m1
      m2 = m1 + 2
      n2 = 2 * n1
      n4 = 2 * n2
      n6 = 3 * n2
      n8 = 4 * n2
      n21 = n2 + 1
      n42 = n4 + 2
      n63 = n6 + 3

c   Place the input data in A into the scratch array D.
c   This code also splits the input data into half-sized words.

cdir$ ivdep
      do 100 i = 1, na
        t1 = a(i+2)
        t2 = int (rbx * t1)
        d(2*i-1) = t2
        d(2*i) = t1 - bbx * t2
 100  continue

      do 110 i = 2 * na + 1, n2
        d(i) = 0.d0
 110  continue

c   Set the second half of the input vector in D to zero.

      do 120 i = n2 + 1, n4
        d(i) = 0.d0
 120  continue

c   Perform a forward real-to-complex FFT on the vector in D.

      call mprcft (1, m2, d, d(n42+1))

c   Square the resulting complex vector.

cdir$ ivdep
      do 130 i = 1, n21
        t1 = d(i)
        t2 = d(i+n21)
        d(i+n42) = t1 * t1 - t2 * t2
        d(i+n63) = 2.d0 * t1 * t2
 130  continue

c   Perform an inverse complex-to-real FFT on the resulting data.

      call mpcrft (-1, m2, d(n42+1), d)

c   Divide by N8, recombine words and release carries.

      nb = min (2 * na, nw)
      nb1 = min (nw + 1, 2 * na - 1)
      d(1) = abs (nb)
      d(2) = 2 * a(2) + 1
      an = 1.d0 / n8
      t1 = an * d(n42+1)
      d(3) = aint (t1 + 0.5d0)
      d(nb+3) = 0.d0
      d(nb+4) = 0.d0
      d(n42+1) = 0.d0

cdir$ ivdep
      do 140 i = 1, nb1
        t1 = an * d(n42+2*i)
        t2 = an * d(n42+2*i+1)
        t3 = aint (t1 + 0.5d0)
        t4 = aint (t2 + 0.5d0)
c        D(N42+2*I) = ABS (T3 - T1)
c        D(N42+2*I+1) = ABS (T4 - T2)
        t1 = int (rdx * t3)
        t2 = t3 - bdx * t1
        t3 = int (rdx * t4)
        t4 = t4 - bdx * t3
        d(i+3) = bbx * t2 + t4
        d(i+2) = d(i+2) + bbx * t1 + t3
 140  continue

c   Find the largest FFT roundoff error.  See comments in MPMULX about this
c   test.  To disable this test, uncomment the next line of code and comment
c   out the two lines of the previous loop that begin D(N42..

      goto 160
      t1 = 0.d0

      do 150 i = 1, 2 * nb1 + 1
        if (d(n42+i) .gt. t1) then
          i1 = i
          t1 = d(n42+i)
        endif
 150  continue

c   Check if maximum roundoff error exceeds the limit ERM, which is set above.

      if (t1 .gt. erm)  then
        if (ker(55) .ne. 0) then
          t2 = an * d(i1)
          i2 = cl2 * log (t1) + 1.d0 + rxx
          i3 = cl2 * log (t2) + 1.d0 + rxx
          i4 = mbt + i2 - i3
          i5 = t1 * 2 ** i4 + rxx
          write (ldb, 2) i1, t1, i4, i5
 2        format ('*** MPSQX: Excessive FFT roundoff error',i10,f10.6,
     $      2i6)
          ier = 55
          if (ker(ier) .eq. 2) call mpabrt
        endif
      endif

c   Fix up the result.

 160  call mpnorm (b)

 170  if (idb .ge. 8) then
        no = min (int (abs (b(1))), ndb) + 2
        write (ldb, 3) (b(i), i = 1, no)
 3      format ('MPSQX O'/(6f12.0))
      endif
      return
      end

      subroutine mpsub (a, b, c)

c   This routine subtracts MP numbers A and B to yield the MP difference C,
c   by negating B and adding.  Debug output starts with IDB = 9.

c   Max SP space for C: NW + 4 cells.

      dimension a(nw+2), b(nw+2), c(nw+2)
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        return
      endif
      if (idb .ge. 9) write (ldb, 1)
 1    format ('MPSUB')

c   Check if A = B.  This is necessary because A and B might be same array,
c   in which case negating B below won't work.

      if (a(1) .ne. b(1)) goto 110

      do 100 i = 2, int (abs (a(1))) + 2
        if (a(i) .ne. b(i)) goto 110
 100  continue

c   A = B.  Result is zero.

      c(1) = 0.
      c(2) = 0.
      if (idb .ge. 9) write (ldb, 2) (c(i), i = 1, 2)
 2    format ('MPSUB O'/2f9.0)
      goto 120

c   Save the sign of B, and then negate B.

 110  b1 = b(1)
      b(1) = - b1

c   Perform addition and restore the sign of B.

      call mpadd (a, b, c)
      b(1) = b1

 120  return
      end

      subroutine mptran (n1, n2, x, y)

c   Performs a transpose of the vector X, returning the result in Y.  X is
c   treated as a N1 x N2 complex matrix, and Y is treated as a N2 x N1 complex
c   matrix.  The complex data is assumed stored with real and imaginary parts
c   separated by N1 x N2 locations.

c   This routine is called by MPCFFT.  It is not intended to be called directly
c   by the user.

      implicit double precision (a-h, o-z)
      parameter (na = 32, nc = 32)
      dimension x(2*n1*n2), y(2*n1*n2), z(nc,2*nc)

      n = n1 * n2
c>
c   Use different techniques, depending on the system, N1 and N2.  For Cray
c   vector systems, uncomment the next line.

c      goto 100

c   This strategy is good for many scalar cache memory computers.  The
c   value of NC (i.e. the size of Z) may have to be changed depending on
c   how large the cache is.

      if (n1 .le. nc .or. n2 .le. nc) then
        if (n1 .ge. n2) then
          goto 110
        else
          goto 130
        endif
      else
        goto 150
      endif

c   This strategy is best for Cray vector systems.

 100  if (n1 .lt. na .or. n2 .lt. na) then
        if (n1 .ge. n2) then
          goto 110
        else
          goto 130
        endif
      else
        goto 220
      endif

c   Scheme 1:  Perform a simple transpose in the usual way.

 110  do 120 j = 0, n2 - 1
        j1 = j + 1
        j2 = j * n1 + 1

cdir$ ivdep
        do 120 i = 0, n1 - 1
          y(i*n2+j1) = x(i+j2)
          y(i*n2+j1+n) = x(i+j2+n)
 120  continue

      goto 260

c   Scheme 2:  Perform a simple transpose with the loops reversed.

 130  do 140 i = 0, n1 - 1
        i1 = i * n2 + 1
        i2 = i + 1

cdir$ ivdep
        do 140 j = 0, n2 - 1
          y(j+i1) = x(j*n1+i2)
          y(j+i1+n) = x(j*n1+i2+n)
 140  continue

      goto 260

c   Scheme 3:  Perform a transpose using the intermediate array Z.  This gives
c   better performance than schemes 1 and 2 on certain cache memory systems.
c   The size of the array Z (i.e. the parameter NC above) may have to be
c   adjusted for optimal performance.

 150  do 210 jj = 0, n2 - 1, nc
        do 200 ii = 0, n1 - 1, nc

          do 170 j = 1, nc
            j1 = ii + (j - 1 + jj) * n1

cdir$ ivdep
            do 160 i = 1, nc
              z(j,i) = x(i+j1)
              z(j,i+nc) = x(i+j1+n)
 160        continue

 170      continue

          do 190 i = 1, nc
            i1 = jj + (i - 1 + ii) * n2

cdir$ ivdep
            do 180 j = 1, nc
              y(j+i1) = z(j,i)
              y(j+i1+n) = z(j,i+nc)
 180        continue

 190      continue

 200    continue
 210  continue

      goto 260

c   Scheme 4:  Perform the transpose along diagonals to insure odd strides.
c   This works well on moderate vector, variable stride computers, when both
c   N1 and N2 are divisible by reasonably large powers of two (32 or larger on
c   Cray vector computers).

 220  n11 = n1 + 1
      n21 = n2 + 1
      if (n1 .ge. n2) then
        k1 = n1
        k2 = n2
        i11 = n1
        i12 = 1
        i21 = 1
        i22 = n2
      else
        k1 = n2
        k2 = n1
        i11 = 1
        i12 = n2
        i21 = n1
        i22 = 1
      endif

      do 230 j = 0, k2 - 1
        j1 = j * i11 + 1
        j2 = j * i12 + 1

cdir$ ivdep
        do 230 i = 0, k2 - 1 - j
          y(n21*i+j2) = x(n11*i+j1)
          y(n21*i+j2+n) = x(n11*i+j1+n)
 230  continue

      do 240 j = 1, k1 - k2 - 1
        j1 = j * i21 + 1
        j2 = j * i22 + 1

cdir$ ivdep
        do 240 i = 0, k2 - 1
          y(n21*i+j2) = x(n11*i+j1)
          y(n21*i+j2+n) = x(n11*i+j1+n)
 240  continue

      do 250 j = k1 - k2, k1 - 1
        j1 = j * i21 + 1
        j2 = j * i22 + 1

cdir$ ivdep
        do 250 i = 0, k1 - 1 - j
          y(n21*i+j2) = x(n11*i+j1)
          y(n21*i+j2+n) = x(n11*i+j1+n)
 250  continue

 260  return
      end

      subroutine mpdotd (n, isa, a, isb, db, c)

c   This routine computes the dot product of the MP vector A with the DP
c   vector DB, returning the MP result in C.  This routine is used in the
c   author's customized PSLQ routine, resulting in substantial speedup.
c   The length of both the A and DB vectors is N, and ISA and ISB are the
c   skip distances between successive elements of A and DB, measured in
c   single precision words, and in DP words, respectively.  ISA must be
c   at least NW + 4.  The DP values in DB must be whole numbers, so for
c   example they cannot be larger than 2^53 on IEEE systems.  Debug output
c   begins with IDB = 8.  The common blocks mpcom6 and mpcom7 have a
c   hard-wired dimension of 4100, which can be increased below.

      double precision db(isb*n), d1, d2, dmax, dt0, dt1, dt2, dt3, dt4
c>
c   Set DMAX to the largest DP whole number that can be represented exactly:
c   2.d0 ** 53 on IEEE systems, or 2.d0 ** 48 on Cray non-IEEE systems.

      parameter (dmax = 2.d0 ** 53, ndim = 4100)
      real a(isa*n), c(nw+4), s1
      parameter (ndb = 22)
      double precision bbx, bdx, bx2, rbx, rdx, rx2, rxx
      common /mpcom0/ bbx, bdx, bx2, rbx, rdx, rx2, rxx, nbt, npr
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims
      common /mpcom2/ ker(73)
      common /mpcom4/ d1(4100)
      common /mpcom6/ d2(ndim)
      common /mpcom7/ s1(ndim)

      if (ier .ne. 0) then
        c(1) = 0.
        c(2) = 0.
        return
      endif
      if (idb .ge. 8) then
        write (6, 1) n, isa, isb
1     format ('mpdotd input:',3i8)

        do k = 1, n
          ka = (k - 1) * isa
          kb = (k - 1) * isb + 1
          call mpmdc (a(1+ka), dt1, n1)
          dt1 = dt1 * 2.d0 ** n1
          write (6, '(i4,1p,2d25.16)') k, dt1, db(kb)
        enddo
      endif

      do i = 1, 4
        d1(i) = 0.d0
        d2(i) = 0.d0
      enddo

c   ND is the length of D1, IXD is the exponent (as in ordinary mpfun format).
c   In the code below ND + 1 mantissa words are maintained whenever possible.

      nd = 0
      ixd = 0
      nrel = 0

c   Loop over the n input data pairs.

      do k = 1, n
        ka = (k - 1) * isa
        kb = (k - 1) * isb + 1

        do kk = 1, nw + 4
          s1(kk) = a(ka+kk)
        enddo

        na = min (int (abs (s1(1))), nw)
        dt0 = db(kb)

        if (na .eq. 0 .or. dt0 .eq. 0.d0) goto 100

c   Check to make sure the input DP value satisfies the requirements.

        if (abs (dt0) .ge. dmax .or. mod (dt0, 1.d0) .ne. 0.d0) then
          if (ker(73) .ne. 0) then
            write (6, 2) k, dt0
2         format ('mpdotd: improper dp value:',i4,1p,d25.15)
            ier = 73
            if (ker(ier) .eq. 2) call mpabrt
          endif
          return
        endif

c   Save the two initial and the two final words of A, then zero these words.

        ia1 = s1(1)
        ia2 = s1(2)
        ia3 = s1(na+3)
        ia4 = s1(na+4)
        s1(1) = 0.
        s1(2) = 0.
        s1(na+3) = 0.
        s1(na+4) = 0.
        if (ia1 .lt. 0) dt0 = - dt0

c   Split the input DP value into high-order, mid-order and a low-order values.

        dt1 = int (rx2 * dt0)
        dt0 = dt0 - bx2 * dt1
        dt2 = int (rdx * dt0)
        dt3 = dt0 - bdx * dt2

        if (dt1 .eq. 0.d0 .and. dt2 .eq. 0.d0) then

c   Only the low-order part of the input DP value is nonzero.

          ish = ia2 - ixd
          if (nd .eq. 0) ish = 0

          if (ish .ge. 0) then

c   The product a(k) * db(k) has greater exponent than the cumulative sum.
c   Thus the cumulative sum must be shifted to the right by ish words.

            m1 = min (na, ish)
            m2 = min (na, nd + ish)
            m3 = na
            m4 = min (max (na, ish), nw + 1)
            m5 = min (max (na, nd + ish), nw + 1)
            d2(1) = 0.d0
            d2(2) = 0.d0

            do i = 1, m1
              d2(i+2) = dt3 * s1(i+2)
            enddo

            do i = m1 + 1, m2
              d2(i+2) = d1(i+2-ish) + dt3 * s1(i+2)
            enddo

            do i = m2 + 1, m3
              d2(i+2) = dt3 * s1(i+2)
            enddo

            do i = m3 + 1, m4
              d2(i+2) = 0.d0
            enddo

            do i = m4 + 1, m5
              d2(i+2) = d1(i+2-ish)
            enddo

c   Copy d2 back to d1.

            do i = 1, m5 + 2
              d1(i) = d2(i)
            enddo

            nd = m5
            ixd = ia2
            d1(nd+3) = 0.d0
            d1(nd+4) = 0.d0
          else

c   The product a(k) * db(k) has smaller exponent than the cumulative sum.
c   Thus the product must be shifted to the right by -ish words.

            nsh = - ish
            m1 = min (nd, nsh)
            m2 = min (nd, na + nsh)
            m3 = nd
            m4 = min (max (nd, nsh), nw + 1)
            m5 = min (max (nd, na + nsh), nw + 1)

            do i = m1 + 1, m2
              d1(i+2) = d1(i+2) + dt3 * s1(i+2-nsh)
            enddo

            do i = m3 + 1, m4
              d1(i+2) = 0.d0
            enddo

            do i = m4 + 1, m5
              d1(i+2) = dt3 * s1(i+2-nsh)
            enddo

            nd = m5
            d1(nd+3) = 0.d0
            d1(nd+4) = 0.d0
          endif
          nrel = nrel + 1
        elseif (dt1 .eq. 0.d0) then

c   Only the lower two parts of the input DP value are nonzero.

          ish = ia2 + 1 - ixd
          if (nd .eq. 0) ish = 0

          if (ish .ge. 0) then

c   The product a(k) * db(k) has greater exponent than the cumulative sum.
c   Thus the cumulative sum must be shifted to the right by ish words.

            m1 = min (na + 1, ish)
            m2 = min (na + 1, nd + ish)
            m3 = na + 1
            m4 = min (max (na + 1, ish), nw + 1)
            m5 = min (max (na + 1, nd + ish), nw + 1)
            d2(1) = 0.d0
            d2(2) = 0.d0

            do i = 1, m1
              d2(i+2) = dt2 * s1(i+2) + dt3 * s1(i+1)
            enddo

            do i = m1 + 1, m2
              d2(i+2) = d1(i+2-ish) + dt2 * s1(i+2) + dt3 * s1(i+1)
            enddo

            do i = m2 + 1, m3
              d2(i+2) = dt2 * s1(i+2) + dt3 * s1(i+1)
            enddo

            do i = m3 + 1, m4
              d2(i+2) = 0.d0
            enddo

            do i = m4 + 1, m5
              d2(i+2) = d1(i+2-ish)
            enddo

c   Copy d2 back to d1.

            do i = 1, m5 + 2
              d1(i) = d2(i)
            enddo

            nd = m5
            ixd = ia2 + 1
            d1(nd+3) = 0.d0
            d1(nd+4) = 0.d0
          else

c   The product a(k) * db(k) has smaller exponent than the cumulative sum.
c   Thus the product must be shifted to the right by -ish words.

            nsh = - ish
            m1 = min (nd, nsh)
            m2 = min (nd, na + 1 + nsh)
            m3 = nd
            m4 = min (max (nd, nsh), nw + 1)
            m5 = min (max (nd, na + 1 + nsh), nw + 1)

            do i = m1 + 1, m2
              d1(i+2) = d1(i+2) + dt2 * s1(i+2-nsh) + dt3 * s1(i+1-nsh)
            enddo

            do i = m3 + 1, m4
              d1(i+2) = 0.d0
            enddo

            do i = m4 + 1, m5
              d1(i+2) = dt2 * s1(i+2-nsh) + dt3 * s1(i+1-nsh)
            enddo

            nd = m5
            d1(nd+3) = 0.d0
            d1(nd+4) = 0.d0
          endif
          nrel = nrel + 2
        else

c   All three parts of the input DP value are nonzero.

          ish = ia2 + 2 - ixd
          if (nd .eq. 0) ish = 0

          if (ish .ge. 0) then

c   The product a(k) * db(k) has greater exponent than the cumulative sum.
c   Thus the cumulative sum must be shifted to the right by ish words.

            m1 = min (na + 2, ish)
            m2 = min (na + 2, nd + ish)
            m3 = na + 2
            m4 = min (max (na + 2, ish), nw + 1)
            m5 = min (max (na + 2, nd + ish), nw + 1)
            d2(1) = 0.d0
            d2(2) = 0.d0

            do i = 1, m1
              d2(i+2) = dt1 * s1(i+2) + dt2 * s1(i+1) + dt3 * s1(i)
            enddo

            do i = m1 + 1, m2
              d2(i+2) = d1(i+2-ish) + dt1 * s1(i+2) + dt2 * s1(i+1)
     $          + dt3 * s1(i)
            enddo

            do i = m2 + 1, m3
              d2(i+2) = dt1 * s1(i+2) + dt2 * s1(i+1) + dt3 * s1(i)
            enddo

            do i = m3 + 1, m4
              d2(i+2) = 0.d0
            enddo

            do i = m4 + 1, m5
              d2(i+2) = d1(i+2-ish)
            enddo

c   Copy d2 back to d1.

            do i = 1, m5 + 2
              d1(i) = d2(i)
            enddo

            nd = m5
            ixd = ia2 + 2
            d1(nd+3) = 0.d0
            d1(nd+4) = 0.d0
          else

c   The product a(k) * db(k) has smaller exponent than the cumulative sum.
c   Thus the product must be shifted to the right by -ish words.

            nsh = - ish
            m1 = min (nd, nsh)
            m2 = min (nd, na + 2 + nsh)
            m3 = nd
            m4 = min (max (nd, nsh), nw + 1)
            m5 = min (max (nd, na + 2 + nsh), nw + 1)

            do i = m1 + 1, m2
              d1(i+2) = d1(i+2) + dt1 * s1(i+2-nsh) + dt2 * s1(i+1-nsh)
     $          + dt3 * s1(i-nsh)
            enddo

            do i = m3 + 1, m4
              d1(i+2) = 0.d0
            enddo

            do i = m4 + 1, m5
              d1(i+2) = dt1 * s1(i+2-nsh) + dt2 * s1(i+1-nsh)
     $          + dt3 * s1(i-nsh)
            enddo

            nd = m5
            d1(nd+3) = 0.d0
            d1(nd+4) = 0.d0
          endif
          nrel = nrel + 3
        endif

100   continue

        if (nd .eq. 0) goto 120

        if (nrel .ge. npr - 1 .or. k .eq. n) then

c   Release carries using a vectorizable scheme.  Results may be negative, but
c   that is not a problem -- these will be fixed in the final call to mpnorm.

          nrel = 0

cdir$ ivdep
          do i = 3, nd + 2
            dt1 = d1(i)
            dt2 = int (rdx * dt1)
            d1(i) = dt1 - bdx * dt2
            d1(i-1) = d1(i-1) + dt2
          enddo

c   If d1(2) is nonzero due to carry release, shift result to right.

          if (d1(2) .ne. 0.d0) then
            ish = 1
            ixd = ixd + 1
            nd = min (nd + 1, nw + 1)
          else
            ish = 0
          endif

          if (ish .ne. 0) then
            do i = nd + 2, 3, -1
              d1(i) = d1(i-ish)
            enddo

            d1(1) = 0.d0
            d1(2) = 0.d0
          endif
          d1(nd+3) = 0.d0
          d1(nd+4) = 0.d0
        endif

c   Check to see if there are leading zeros.

        do i = 1, nd + 1
          if (d1(i+2) .ne. 0.d0) goto 110
        enddo

c   The cumulative sum is now zero.

        nd = 0
        ixd = 0
        nrel = 0
        d1(1) = 0.d0
        d1(2) = 0.d0
        goto 120

110   kz = i - 1

        if (kz .gt. 0) then

c   Leading zeroes -- shift cumulative sum to left.

          do i = 1, nd - kz + 1
            d1(i+2) = d1(i+2+kz)
          enddo

          nd = nd - kz
          ixd = ixd - kz
          d1(nd+3) = 0.d0
          d1(nd+4) = 0.d0
        endif

120   continue

      enddo

c   Call mpnorm to fix up result and store in c.

      d1(1) = nd
      d1(2) = ixd
      call mpnorm (c)

      if (idb .ge. 8) then
        call mpmdc (c, dt1, n1)
        dt1 = dt1 * 2.d0 ** n1
        write (6, 4) dt1
4       format ('mpdotd output:',1p,d25.15)
        write (6, '(6f13.0)') (c(i), i = 1, min (int (abs (c(1))) + 2,
     $  ndb))
      endif

      return
      end

      subroutine mpfform (a, n1, n2, b, c)

c   This routine converts the MP number A to F format, i.e. F N1.N2.
c   B is a scratch array of size 8*nw+50 (type CHARACTER*1), and C is the 
c   output array (type CHARACTER*1) of size N1.

      real a(nw+2)
      character*1 b(8*nw+50), c(n1)
      character*40 chr40
      common /mpcom1/ nw, idb, ldb, ier, mcr, ird, ics, ihs, ims

      call mpoutc (a, b, n)
      write (chr40, '(10a1)') (b(i), i = 5, 14)
      read (chr40, '(i10)') ix
      if (a(1) .ge. 0.) then
        ls = 0
      else
        ls = 1
      endif
      if (ix .ge. 0 .and. a(1) .ne. 0.) then
        lz = 0
      else
        lz = 1
      endif
      mx = max (ix, 0)

c   Check for overflow of field length.

      if (ls + lz + mx + n2 + 2 .gt. n1) then
        do i = 1, n1
          c(i) = '*'
        enddo

        goto 200
      endif

c   Check if a zero should be output.

      if (a(1) .eq. 0 .or. -ix .gt. n2) then
        do i = 1, n1 - n2 - 2
          c(i) = ' '
        enddo

        c(n1-n2-1) = '0'
        c(n1-n2) = '.'

        do i = 1, n2
          c(i+n1-n2) = '0'
        enddo

        goto 200
      endif

c   Process other cases.

      do i = 1, n1 - n2 - mx - 2
        c(i) = ' '
      enddo

      if (a(1) .lt. 0.) c(n1-n2-mx-2) = '-'
      if (ix .ge. 0) then
        c(n1-n2-ix-1) = b(19)
        kx = min (n - 20, ix)

        do i = 1, kx
          c(i+n1-n2-ix-1) = b(i+20)
        enddo

        do i = kx + 1, ix
          c(i+n1-n2-ix-1) = '0'
        enddo

        c(n1-n2) = '.'
        kx = max (min (n - ix - 20, n2), 0)

        do i = 1, kx
          c(i+n1-n2) = b(i+ix+20)
        enddo

        do i = kx + 1, n2
          c(i+n1-n2) = '0'
        enddo
      else
        nx = - ix
        c(n1-n2-1) = '0'
        c(n1-n2) = '.'

        do i = 1, nx - 1
          c(i+n1-n2) = '0'
        enddo

        c(n1-n2+nx) = b(19)
        kx = min (n - 20, n2 - nx)

        do i = 1, kx
          c(i+n1-n2+nx) = b(i+20)
        enddo

        do i = kx + 1, n2 - nx
          c(i+n1-n2+nx) = '0'
        enddo
      endif

200   continue

      return
      end
            
