      program convip

c   This program processes the output of transmp to reduce the dimensions
c   of intermediate precision variables to mpwdsw + 4 (see below).
c   This is not a foolproof, robust tool.  It is intended only for DHB's codes.

c   David H Bailey     1999-05-28

      character*160 line
      character*80 linex
      character*26 alphal, alphau
      character*11 digits
      character*2 dim
      parameter (nnam = 1000)
      character*32 lcase, name, names(nnam)
      parameter (digits = '0123456789 ', 
     $  alphal = 'abcdefghijklmnopqrstuvwxyz',
     $  alphau = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ')
      parameter (mpwdsw = 18, idm = mpwdsw + 4)

      write (dim, '(i2)') idm
      klin = 0
      knam = 0

100   continue
      read (5, '(a)', end = 200) linex
      ln = lnblk (linex)
      klin = klin + 1
      line = linex

105   continue
      if (line(1:28) .eq. 'c+   intermediate precision') then
        write (6, '(a)') line(1:ln)
        read (5, '(a)', end = 210) linex
        klin = klin + 1
        ln = lnblk (linex)
        line = linex
        lnam = 0
        name = ' '

        do i = 18, ln
          j = index (alphal, line(i:i))
          if (line(i:i) .eq. ',') then
            knam = knam + 1
            if (knam .gt. nnam) goto 210
            names(knam) = name
            lnam = 0
            name = ' '
          elseif (j .ne. 0 .or. lnam .ne. 0) then
            lnam = lnam + 1
            if (lnam .gt. 32) stop
            name(lnam:lnam) = line(i:i)
          endif
          if (i .eq. ln .and. lnam .gt. 0) then
            knam = knam + 1
            if (knam .gt. nnam) goto 210
            names(knam) = name
            lnam = 0
            name = ' '
          endif
        enddo

        write (6, '(a)') line(1:ln)
        goto 100
      elseif (line(1:11) .eq. '      REAL ') then
        read (5, '(a)', end = 210) linex
        klin = klin + 1
        ln2 = lnblk (linex)
        if (linex(1:5) .eq. ' ' .and. linex(6:6) .ne. ' ') then
          line(ln+1:ln+ln2-6) = linex(7:ln2)
          ln = ln + ln2 - 6
          kcont = 1
        else
          kcont = 0
        endif
        lnam = 0
        name = ' '

        do i = 12, ln
          j = max (index (alphal, line(i:i)), 
     $             index (alphau, line(i:i)))
          if (line(i:i) .eq. '(') then
            name = lcase (name)

            do j = 1, knam
              if (name .eq. names(j)) goto 110
            enddo

            goto 120

110         continue
            i0 = i + 1
            j1 = index (line(i0:ln), ',')
            j2 = index (line(i0:ln), ')')
            if (j1 .eq. 0) j1 = 1000
            if (j2 .eq. 0) j2 = 1000
            i1 = i - 1 + min (j1, j2)
            ldim = i1 - i0 + 1
            if (ldim .lt. 2 .or. ldim .gt. 50) goto 210

            do j = i0, i0 + ldim - 2
              line(j:j) = '0'
            enddo

            line(i1-1:i1) = dim

120         continue
            lnam = 0
            name = ' '
          elseif(line(i:i) .eq. ')') then
            name = lcase (name)
            lnam = 0
            name = ' '
          elseif (j .ne. 0 .or. lnam .ne. 0) then
            lnam = lnam + 1
            if (lnam .gt. 32) stop
            name(lnam:lnam) = line(i:i)
          endif
        enddo

        if (ln .lt. 72) then
          write (6, '(a)') line(1:ln)
        else
          write (6, '(a)') line(1:72)
          write (6, '("     $",a)') line(73:ln)
        endif
        if (kcont .eq. 0) then
          line = linex
          ln = ln2
          goto 105
        else
          goto 100
        endif
      elseif (line(1:9) .eq. '      end' .and. ln .eq. 9) then
        write (6, '(a)') line(1:ln)
        knam = 0
        goto 100
      else
        write (6, '(a)') line(1:ln)
        goto 100
      endif

200   stop

210   write (6, 1) klin
1     format ('Syntax error at line',i6)
      stop
      end

      function lnblk (lin)
      character*(*) lin

c   This finds the index of the last non-blank character in LIN.

      ln = len (lin)

      do i = ln, 1, -1
        if (lin(i:i) .ne. ' ') goto 110
      enddo

      i = 0
 110  lnblk = i

      return
      end

      function lcase (name)

c  This returns the lowercase equivalent of name.

      character*32 name, namx, lcase
      character*26 alphal, alphau, alpha
      character*1 cj
      parameter (alphal = 'abcdefghijklmnopqrstuvwxyz',
     $           alphau = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ')

      alpha = alphal
      namx = ' '

      do j = 1, 32
        cj = name(j:j)
        i1 = index (alphau, cj)
        if (i1 .gt. 0) then
          namx(j:j) = alpha(i1:i1)
        else
          namx(j:j) = cj
        endif
      enddo

      lcase = namx
      return
      end
