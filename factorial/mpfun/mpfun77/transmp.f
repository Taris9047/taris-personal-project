      program transmp

c   This translates a standard Fortran-77 code input on standard input (unit 5)
c   to a code that calls DHB's MPFUN multiprecision routines, which is output
c   on standard output (unit 6).  This output program may then be compiled and
c   linked with the MPFUN library file.   IEEE version.

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

c******************************************************************************

c   The following information is a brief description of this program.  For
c   full details and instructions for usage, see the paper "Automatic
c   Translation of Fortran to Multiprecision", available from the author.
c   This program works in conjunction with MPFUN, the author's package of
c   multiprecision functions.

c   This translation program allows one to extend the Fortran-77 language
c   with the data types MULTIP INTEGER, MULTIP REAL and MULTIP COMPLEX.
c   These data types can be used for integer, floating point or complex
c   numbers of an arbitrarily high but pre-specified level of precision.
c   Variables in the input program may be declared to have one of these
c   multiprecision types in the output program by placing directives
c   (special comments) in the input file.  In this way, the input file
c   remains an ANSI Fortran-77 compatible program and can be run at any
c   time using ordinary arithmetic on any Fortran system for comparison
c   with the multiprecision equivalent.

c   Instructions for compiling and testing this program are included in the
c   readme file that accompanies this code.

c******************************************************************************

c   This is the start of the main program of the translator.  In each
c   subprogram below, c+ delimits common data specifications.  Specifications
c   following the second c+ are for local variables.  c* comments delimit
c   debug printout code.  c> comments indicate machine-dependent code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*80 lin, arg(mar)
      character*16 linq, namq, numx, ucase
      dimension itar(mar), lar(mar)

c   Start of input program file -- initialize and read first line.

      imm = 1
      ite = 1
      mpp = 0
      mss = 0
      mxp = 0
      ieof = 0
c*

c   Uncomment one of the next two lines -- the first for normal use, the
c   second for debug.

      open (11, status = 'SCRATCH')
c      open (11, file = 'scratch')
c*
      open (12, status = 'SCRATCH')
      rewind 11
      rewind 12
      read (5, '(A)', end = 150) lin
      lc = 1
      l1 = 1
      l2 = lnblk (lin)
      ls = l2
      nvar = mint
      namq = 'TANH'
      iqtanh = itab (0, 0, namq)

c   Start processing a new subprogram.

 100  istp = 0
      nvar = mint
      iex = 0
      ifl = 0
      kdo = 0
      lsm = 0
      mpa = 0
      mpf = 0
      mpt = 0
      iend = 0

c   Initialize the implicit type table and the special constant usage table.

      do 110 i = 1, 26
        impl(i) = imps(i)
 110  continue

      do 120 i = 1, 5
        kcon(i) = 0
 120  continue

c   Start reading a new statement.

 130  ln = 0
      lct = lc
      line = ' '

 140  continue
      ll = l2 - l1 + 1
      line(ln+1:ln+ll) = lin(l1:l2)
      ln = ln + ll
      read (5, '(A)', end = 150) lin
      lc = lc + 1
      l1 = 1
      l2 = lnblk (lin)
      if (lin(1:1) .eq. 'C' .or. lin(1:1) .eq. 'c' .or. l2 .eq. 0)
     $  goto 160
      if (l2 .gt. 72) then
        write (11, 1) lc
 1      format ('CMP*'/'CMP* Characters past column 72 in line',i6,
     $    ' are ignored.'/'CMP*')
        l2 = 72
      endif
      if (lin(6:6) .ne. ' ') then
        k1 = 72 - ls
        if (k1 .ge. 1) line(ln+1:ln+k1) = ' '
        ln = ln + k1
        l1 = 7
        ls = l2
        goto 140
      endif
      ls = l2
      goto 160

c   The end of file has been encountered.

 150  ieof = 1

c   A complete multiline statement has been read.  Check if it is a comment.

 160  continue

c   Optionally print out current statement.
c*
c      write (6, *) '%'//line(1:ln)//'%'
c*
      lq = min (16, ln)
      linq = ucase (line(1:lq))
      if (ln .eq. 0 .or. linq(1:1) .eq. 'C') then

c   Check if this comment is a MP directive.

        call outlin (11, 0, ln, line)
        if (ln .ge. 4 .and. linq(1:4) .eq. 'CMP+') call mpdec (ln)
        goto 240
      endif
      k1 = nblk (7, ln, line)
      lq = min (k1 + 15, ln)
      linq = ucase (line(k1:lq))

c   Check if this is an end statement.

      if (linq(1:5) .eq. 'END  ') then

c   Parse the argument list of this subprogram, compare with the subprogram
c   table, and add it if it is not there.

        lsv = lct
        lct = lcs
        call arlist (11, lsar, sarg, nar, itar, lar, arg)
        call chkarg (11, fnam, nar, itar, lar, arg)
        lct = lsv
        iend = 1
        if (ifl .ne. 0) then
          call errmes (110, 0)
          write (6, 2)
2         format ('IF blocks not closed.')
          call abrt
        endif
        call outlin (11, 1, ln, line)
        goto 240
      endif

c   If the executable portion of this subprogram has already been encountered,
c   and if this subprogram contains no MP variables, there is no need to check
c   the line any further.

      if (iex .ne. 0 .and. mpt .eq. 0 .and. mpa .eq. 0) then
        call outlin (11, 1, ln, line)
        goto 240
      endif

c   Check if this line is a program, subroutine, function or block data
c   statement.

      if (ipfsb (k1, ln) .ne. 0) then
        lcs = lct
        goto 240
      endif

c   At the beginning of a subprogram, one of the above should have been noted.

      if (istp .eq. 0) then
        call errmes (1, 0)
        write (6, 3)
3       format ('PROGRAM, SUBROUTINE, FUNCTION, or BLOCK DATA',
     $    ' statement is missing.')
        call abrt
      endif

c   Check if it is an implicit statement.

      if (linq(1:8) .eq. 'IMPLICIT') then
        if (iex .ne. 0) goto 250
        k1 = nblk (k1 + 8, ln, line)
        call implic (k1, ln)
        call outlin (11, 1, ln, line)
        goto 240
      endif

c   Check if it is a type statement.

 170  it = ntype (k1, ln)
      if (it .ne. 0) then
        if (iex .ne. 0) goto 250
        call type (it, k1, ln)
        goto 240
      endif

c   Check if it is a parameter statement.

      if (linq(1:9) .eq. 'PARAMETER') then
        if (iex .ne. 0) goto 250
        i1 = iscan (k1 + 9, ln, line)
        if (i1 .ge. 1) then
          k1 = k1 + 9
          call outlin (11, 2, ln, line)
          call param (k1, ln)
        else
          call outlin (11, 1, ln, line)
        endif
        goto 240
      endif

c   Check if it is a dimension statement.

      if (linq(1:9) .eq. 'DIMENSION') then
        if (iex .ne. 0) goto 250
        call dimen (k1 + 9, ln)
        goto 240
      endif

c   Check if it is a common statement.

      if (linq(1:6) .eq. 'COMMON') then
        if (iex .ne. 0) goto 250
        k1 = k1 + 6
        i1 = indx (k1, ln, '/', line)
        if (i1 .ne. 0) then
          i2 = indx (i1 + 1, ln, '/', line)
          if (i2 .eq. 0) call errmes (2, 1)
          k1 = i2 + 1
        endif
        call dimen (k1, ln)
        goto 240
      endif

c   Check if it is an equivalence statement.

      if (linq(1:11) .eq. 'EQUIVALENCE') then
        if (iex .ne. 0) goto 250
        call outlin (11, 2, ln, line)

c   Append '1,' to subscripted MP variables.

        k2 = ln
        call fixsub (k1 + 11, k2, ln)
        call outlin (11, 1, ln, line)
        write (11, 4)
4       format ('CMP<')
        goto 240
      endif

c   Check if it is an external, intrinsic, save or namelist statement.

      if (linq(1:8) .eq. 'EXTERNAL' .or. linq(1:9) .eq. 'INTRINSIC')
     $  then
        if (iex .ne. 0) goto 250
        k1 = k1 + 9
        call type (0, k1, ln)
        goto 240
      endif
      if (linq(1:5) .eq. 'SAVE ') then
        if (iex .ne. 0) goto 250
        call outlin (11, 1, ln, line)
        goto 240
      endif
      if (linq(1:8) .eq. 'NAMELIST') then
        i1 = iscan (k1 + 8, ln, line)
        if (i1 .ne. 0) goto 260
        call outlin (11, 1, ln, line)
        goto 240
      endif

c   Check if this is the start of the executable part of the subprogram.

      if (iex .eq. 0) then
        iex = 1

c   Check if the subprogram name is valid and set its type.  The type of a
c   program or subroutine name is set to 0 (undefined), whereas the type of
c   a function name is set according to previous implicit or type statements.

        ix = itab (1, 0, fnam)
        if (ix .le. iqtanh) then
          call errmes (3, 0)
          write (6, 5)
5         format ('This name may not be used as a subroutine or',
     $      ' function name.')
          call abrt
        elseif (ix .gt. mint .and. istp .eq. 3 .and. ktyp(ix) .ge. 8
     $      .and. kdim(ix) .ne. -2) then
          call errmes (4, 0)
          write (6, 6)
6         format ('MP function names must be declared with an explicit',
     $      ' MP type directive'/'immediately preceding the function',
     $      ' statement.')
          call abrt
        endif
        kdec(ix) = 1
        if (istp .ne. 3) ktyp(ix) = 0

c   Parse the argument list of this subprogram, compare with the subprogram
c   table, and add it if it is not there.

        call arlist (11, lsar, sarg, nar, itar, lar, arg)
        call chkarg (11, fnam, nar, itar, lar, arg)

c   If any MP variables have been defined, or if this is the main program,
c   Insert a marker in the temporary file to mark the location of the MP
c   declarations for INIMP.

        if (mpt .ne. 0 .or. mpa .ne. 0 .or. istp .eq. 1) then
          write (11, 7)
7         format ('CMP>>>')
        endif

c   If no MP variables have been identified, there is no need to further
c   analyze the statement.

        if (mpt .eq. 0 .and. mpa .eq. 0) then
          call outlin (11, 1, ln, line)
          goto 240
        endif
      endif

c   Check if this is a data statement.

      if (linq(1:5) .eq. 'DATA ') then
        i1 = iscan (k1 + 5, ln, line)
        if (i1 .ne. 0) goto 260
        call outlin (11, 1, ln, line)
        goto 240
      endif

c   Check if this is an endif statement.  If so, previous if blocks
c   generated as translations of elseif statements must be closed.

      if (linq(1:5) .eq. 'ENDIF' .or. linq(1:6) .eq. 'END IF') then
        if (ifl .eq. 0) call errmes (108, 1)
        call outlin (11, 1, ln, line)
        if (ifbl(ifl) .gt. 0) then
          i1 = ifbl(ifl)

          do 180 i = 1, i1
            write (11, 8)
8           format (6x,'ENDIF')
 180      continue

        endif
        ifl = ifl - 1
        goto 240
      endif

c   Nothing needs to be processed for these non-executable statements.

      if (linq(1:6) .eq. 'ELSE  ' .or. linq(1:6) .eq. 'ENDDO ' .or.
     $  linq(1:7) .eq. 'END DO ' .or. linq(1:8) .eq. 'CONTINUE' .or.
     $  linq(1:6) .eq. 'FORMAT') then
        call outlin (11, 1, ln, line)
        goto 240
      endif

c   Nothing needs to be processed for these executable statements.

      if (linq(1:5) .eq. 'CLOSE' .or. linq(1:5) .eq. 'GOTO ' .or.
     $  linq(1:4) .eq. 'OPEN' .or. linq(1:5) .eq. 'STOP ' .or.
     $  linq(1:9) .eq. 'BACKSPACE' .or. linq(1:7) .eq. 'ENDFILE' .or.
     $  linq(1:7) .eq. 'INQUIRE' .or. linq(1:6) .eq. 'RETURN' .or.
     $  linq(1:6) .eq. 'REWIND') then
      call outlin (11, 1, ln, line)
      goto 240
      endif

c   Check if the statement is a DO statement.  If so, place the DO terminal
c   number in the IDON table.

      if (linq(1:3) .eq. 'DO ') then
        k1 = nblk (k1 + 3, ln, line)
        i1 = iscan (k1, ln, line)
        if (i1 .ne. 0) goto 260
        if (index (dig, line(k1:k1)) .ne. 0) then
          k2 = index (line(k1:ln), ' ')
          if (k2 .eq. 0) call errmes (5, 1)
          k2 = k1 - 1 + k2
          numx = line(k1:k2)
          read (numx, '(BN,I16)', err = 270) k
          kdo = kdo + 1
          if (kdo .gt. ndo) then
            call errmes (6, 0)
            write (6, 9)
9           format ('Too many DO statements in this subprogram.')
            call abrt
          endif
          idon(kdo) = k
        endif
        call outlin (11, 1, ln, line)
        goto 240
      endif

c   Check if this is a logical if statement.

      if (linq(1:3) .eq. 'IF(' .or. linq(1:3) .eq. 'IF ') then
        if (ucase (line(ln-3:ln)) .eq. 'THEN') ifl = ifl + 1
        if (ifl .gt. mifl) then
          call errmes (109, 0)
          write (6, 10)
10        format ('IF-THEN block is too deeply nested.')
          call abrt
        endif
        ifbl(ifl) = 0
      endif

c   Check if statement has any MP variables.  If not, there is no need for
c   any further analysis.

      i1 = iscan (k1, ln, line)
      if (i1 .eq. 0) then
        call outlin (11, 1, ln, line)
        goto 240
      endif

c   Output original statement as a comment.

      call outlin (11, 2, ln, line)

c   Check if a line number is presesnt.  If so, output as a continue statement.

      read (line(1:5), '(BN,I5)', err = 270) k
      if (k .lt. 0) goto 270
      if (k .ne. 0) then

        do 190 i = 1, kdo
          if (k .eq. idon(i)) then
            call errmes (7, 0)
            write (6, 11)
11          format ('MP variables may not appear in the terminal line',
     $        ' of a DO loop.')
            call abrt
          endif
 190    continue

        write (11, 12) k
12      format (i5,' CONTINUE')
      endif

c   Check if this is a logical if or elseif statement.

      if (linq(1:3) .eq. 'IF(' .or. linq(1:3) .eq. 'IF ') then
        k3 = nblk (k1 + 2, ln, line)
        call ifst (ifl, 1, k3, ln)
        goto 210
      elseif (linq(1:6) .eq. 'ELSEIF') then
        ifbl(ifl) = ifbl(ifl) + 1
        k3 = nblk (k1 + 6, ln, line)
        call ifst (ifl, 2, k3, ln)
        goto 210
      elseif (linq(1:7) .eq. 'ELSE IF') then
        ifbl(ifl) = ifbl(ifl) + 1
        k3 = nblk (k1 + 7, ln, line)
        call ifst (ifl, 2, k3, ln)
        goto 210
      endif

c   Fix subscripts of MP variables and change names of special constants.

 200  k2 = ln
      call fixsub (k1, k2, ln)

c   Process other kinds of MP executable statements.

      call exec (k1, ln)

c   Insert a comment to mark the end of the translation of the MP executable
c   statement.

 210  write (11, 4)

c   Check if the itmp table is properly zeroed.  If not, one of the routines
c   dealing with MP statements erred.

      do 230 j = 1, ntyp
        do 220 i = 1, 9
          if (itmp(i,j) .ne. 0) then
            call errmes (8, 0)
            write (6, 13) ctm(j), i
13          format ('Translator error: active temporary: MP',a1,i1/
     $        'Please contact the author.')
            call abrt
          endif
 220    continue
 230  continue

c   If this is an end statement, copy the scratch file to the output file,
c   inserting MP declarations at the marker.

 240  if (iend .eq. 1) then
        call copy
        if (ieof .eq. 1) stop
        goto 100
      else
        if (ieof .eq. 1) then
          if (istp .eq. 0) stop
          call errmes (9, 0)
          write (6, 14)
14        format ('The last line of the file was not an END statement.')
          call abrt
        endif
        goto 130
      endif

 250  call errmes (10, 0)
      write (6, 15)
15    format ('A declarative statement may not appear after an',
     $  ' executable statement.')
      call abrt

 260  call errmes (11, 0)
      write (6, 16)
16    format ('MP variables may not appear in this statement.')
      call abrt

 270  call errmes (12, 0)
      write (6, 17)
17    format ('Syntax error in line number.')
      call abrt

      stop
      end

      block data

c   This sets all data in common.  Here is a brief description of these
c   variables and arrays, in alphabetical order.

c   ALPL   Lower case alphabet.
c   ALPU   Upper case alphabet.
c   CTM    Table of one-character type abbreviations.
c   CTP    Table of two-character type abbreviations.
c   DEL    Standard Fortran delimiters.
c   DIG    The ten digits.
c   EPS    The mantissa and exponent of the current epsilon, in character form.
c   FNAM   The function name, in function subprograms.
c   IDON   DO line number table.
c   IEX    1 if the executable portion of the subprogram has been encountered.
c   IFBL   Number of blocks at each level of a nested if-then-else stmnt.
c   IMM    Mixed mode option (0:FAST, 1:SAFE).
c   IMPL   Implicit type definition table.
c   IMPS   Default implicit type definitions.
c   ISTP   Type of subprogram (1:PROGRAM, 2:SUBROUTINE, 3:FUNCTION, 4: BLOCK D)
c   ITE    Type error option (0:OFF, 1:ON).
c   ITMP   MP temporary usage table.
c   KCON   Special constant usage table.
c   KDEC   MP variable declaration table.
c   KDIM   Dimension and misc. information for names in VAR:
c          -3   Special constant or parameter.
c          -2   MP function name (within its defining subprogram).
c          -1   Function name.
c           0   Scalar variable.
c           1   Dimensioned variable.
c           2-10   Indicates number of dimensions (not yet implemented).
c   KDO    Number of entries in DO number table.
c   KEYW   Table of Fortran keywords.
c   KOP    Operator precedence table.
c   KSTP   Table of types of subroutine argument lists.
c   KTYP   Types of variables in VAR:
c          -1   Dependent on argument (for intrinsic function names)
c           0   Undefined
c           1   Integer
c           2   Real
c           3   Double Precision
c           4   Complex
c           5   Double Complex
c           6   Character
c           7   Logical
c           8   MP Integer
c           9   MP Real
c          10   MP Complex
c   LCT    Current line count.
c   LEP    Lengths of epsilon strings.
c   LINE   Current extended working line read from file.
c   LOP    Lengths of operators in LOPR and UOPR.
c   LOPR   Lower case operators.
c   LSAR   Length of subroutine argument list.
c   LSM    Longest numeric string in current subprogram.
c   LVAR   Lengths of the names in VAR.
c   MPA    Number of MP parameters in current subprogram.
c   MPLC   Table of implicit/explicit status of names in VAR.
c   MPP    Current output precision level.
c   MPT    Set to 1 if an MP variable has been encountered in subprogram.
c   MSS    Scratch space.
c   MXP    Maximum precision level in words.
c   NARS   Table of number of arguments in argument list array KSTP.
c   NSUB   Number of subprograms encountered.
c   NVAR   Number of variables defined in current subprogram (including
c          standard and MP intrinsic names).
c   SARG   Argument list of current subprogram.
c   SNAM   Table of subroutine names.
c   UOPR   Upper case operators.
c   VAR    Table of variable names for current subprogram.

c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      parameter (np1 = mvar - mint, npc = 5, npi = 36, npf = 6, nps = 2)
      data idon /ndo * 0/
      data impl /26 * 0/
      data imps /8 * 2, 6 * 1, 12 * 2/
      data kdec /mint * 1, np1 * 0/
      data kdim /10 * -3, 44 * -1, np1 * 0/
      data ktyp /1, 4 * 3, 1, 4 * 9, 3 * -1, 4, 4 * -1, 6, 4, 3 * -1, 3,
     $  5, -1, 5, 5, -1, 4 * 1, 6 * -1, 2, 6 * -1, 8, 10, 4 * 9, 2 * 9,
     $  np1 * 0/
      data lvar /10 * 5, 3, 4, 4, 5, 5, 4, 4, 5, 4, 5, 5, 3, 4, 4, 6, 3,
     $  5, 5, 3, 5, 5, 3, 3, 3, 5, 3, 3, 3, 4, 4, 4, 3, 4, 4, 3, 4, 5,
     $  6, 6, 6, 5, 6, 6, 6, np1 * 0/
      data kop /1, 6, 6, 8, 7, 7, 5, 5, 5, 5, 5, 5, 2, 3, 4/
      data lop /1, 1, 1, 2, 1, 1, 4, 4, 4, 4, 4, 4, 4, 5, 5/
      data lep /2 * 0/
      data mplc /npc * 0, npc * 1, npi * 1, npf * 0, nps * 1, np1 * 1/
      data alpl /'abcdefghijklmnopqrstuvwxyz'/
      data alpu /'ABCDEFGHIJKLMNOPQRSTUVWXYZ'/
      data del /' ,().=+-*/:'/
      data dig /'0123456789'/
      data ctm /'I', 'R', 'D', 'C', 'X', 'A', 'L', 'J', 'M', 'Z'/
      data ctp /'IN', 'SP', 'DP', 'CO', 'DC', 'CH', 'LO', 'MPI', 'MPR',
     $  'MPC'/
      data eps /2 * ' '/
      data keyw/
     $  'BACKSPACE', 'BLOCK', 'CALL', 'CHARACTER', 'CLOSE', 'COMMON',
     $  'COMPLEX', 'CONTINUE', 'DATA', 'DIMENSION', 'DO', 'DOUBLE',
     $  'ELSE', 'ELSEIF', 'END', 'ENDFILE', 'ENDIF', 'ENTRY',
     $  'EQUIVALENCE', 'EXTERNAL', 'FORMAT', 'FUNCTION', 'GO', 'GOTO',
     $  'IF', 'IMPLICIT', 'INQUIRE', 'INTEGER', 'INTRINSIC', 'LOGICAL',
     $  'OPEN', 'PARAMETER', 'PRECISION', 'PRINT', 'PROGRAM', 'READ',
     $  'REAL', 'RETURN', 'REWIND', 'SAVE', 'STOP', 'SUBROUTINE',
     $  'THEN', 'TO', 'WRITE'/
      data lopr /'=', '+', '-', '**', '*', '/', '.eq.', '.ne.', '.gt.',
     $  '.lt.', '.ge.', '.le.', '.or.', '.and.', '.not.'/
      data sfun/
     $  'ALOG', 'ALOG10', 'AMAX0', 'AMAX1', 'AMIN0', 'AMIN1', 'AMOD',
     $  'CABS', 'CCOS', 'CEXP', 'CLOG', 'CSIN', 'CSQRT', 'DABS',
     $  'DACOS', 'DASIN', 'DATAN', 'DATAN2', 'DCOS', 'DCOSH', 'DCOSH',
     $  'DDIM', 'DEXP', 'DIM', 'DINT', 'DLOG', 'DLOG10', 'DMAX1',
     $  'DMIN1', 'DMOD', 'DNINT', 'DPROD', 'DSIGN', 'DSIN', 'DSINH',
     $  'DSQRT', 'DTAN', 'DTANH', 'FLOAT', 'IABS', 'IDIM', 'IDINT',
     $  'IDNINT','IFIX', 'ISIGN', 'MAX0', 'MAX1', 'MIN0', 'MIN1',
     $  'SNGL'/
      data var /
     $  'MPNWP', 'DPEPS', 'DPL02', 'DPL10', 'DPPIC', 'MPNWQ', 'MPEPS',
     $  'MPL02', 'MPL10', 'MPPIC',
     $  'ABS', 'ACOS', 'AINT', 'AIMAG', 'ANINT', 'ASIN', 'ATAN',
     $  'ATAN2', 'CHAR', 'CMPLX', 'CONJG', 'COS', 'COSH', 'DBLE',
     $  'DCMPLX', 'DIM', 'DIMAG', 'DREAL', 'EXP', 'ICHAR', 'INDEX',
     $  'INT', 'LEN', 'LOG', 'LOG10', 'MAX', 'MIN', 'MOD', 'NINT',
     $  'REAL', 'SIGN', 'SIN', 'SINH', 'SQRT', 'TAN', 'TANH',
     $  'MPINT', 'DPCMPL', 'DPIMAG', 'DPREAL', 'DPNRT', 'DPRAND',
     $  'DPCSSN', 'DPCSSH', np1 * ' '/
      data uopr /'=', '+', '-', '**', '*', '/', '.EQ.', '.NE.', '.GT.',
     $  '.LT.', '.GE.', '.LE.', '.OR.', '.AND.', '.NOT.'/
      end

      subroutine abrt
c>
c   This terminates execution.  For debug purposes it may be preferable to
c   replace the standard STOP with a call to a system routine that produces
c   a traceback.

      stop
      end


      subroutine arlist (lu, la, lina, nar, itar, lar, arg)

c   This processes an argument list in LINA, which has length LA.  Any
c   expressions in any argument are first processed with subroutine EXPRES.
c   The resulting argument list (NAR elements) is placed in ARG, with types
c   in ITAR and lengths in ARG.  LU is the logical unit number of output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb
      character*80 arg(mar), argx
      dimension itar(mar), lar(mar)
c*
c      write (lu, *) 'enter arlist'
c      write (lu, *) '%'//lina(1:la)//'%'
c      call flush (lu)
c*
      k1 = 1
      nar = 0
      iend = 0
      if (la .eq. 0 .or. lina(1:la) .eq. ' ') goto 120

 100  if (k1 .gt. la) call errmes (13, 1)
      k1 = nblk (k1, la, lina)
      if (k1 .eq. 0) call errmes (14, 1)
      k2 = match (1, k1, la, lina)
      if (k2 .eq. 0) then
        k2 = la + 1
        iend = 1
      endif
      k2 = k2 - 1
      lb = k2 - k1 + 1
      linb(1:lb) = lina(k1:k2)

c   Evaluate the expression with EXPRES after setting the mixed mode option
c   to FAST, except if this is a parameter statement or an intrinsic function
c   reference.

      ims = imm
      if (lu .ne. 12 .and. imm .ne. 2) imm = 0
      call expres1 (lu, lb, linb, itpx, lx, argx)
      imm = ims

c   Add this argument to the list.

 110  nar = nar + 1
      if (nar .gt. mar) then
        call errmes (15, 0)
        write (6, 1)
 1      format ('List has too many arguments.')
        call abrt
      endif
      itar(nar) = itpx
      lar(nar) = lx
      arg(nar)(1:lx) = argx(1:lx)
      k1 = k2 + 2
      if (iend .eq. 0) goto 100

 120  continue
c*
c      write (lu, *) 'exit arlist  args:'
c      do 111 i = 1, nar
c        write (lu, *) '%'//arg(i)(1:lar(i))//'%'
c 111  continue
c      call flush (lu)
c*
      return
      end
      subroutine arlist1 (lu, la, lina, nar, itar, lar, arg)

c   This processes an argument list in LINA, which has length LA.  Any
c   expressions in any argument are first processed with subroutine EXPRES.
c   The resulting argument list (NAR elements) is placed in ARG, with types
c   in ITAR and lengths in ARG.  LU is the logical unit number of output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb
      character*80 arg(mar), argx
      dimension itar(mar), lar(mar)
c*
c      write (lu, *) 'enter arlist'
c      write (lu, *) '%'//lina(1:la)//'%'
c      call flush (lu)
c*
      k1 = 1
      nar = 0
      iend = 0
      if (la .eq. 0 .or. lina(1:la) .eq. ' ') goto 120

 100  if (k1 .gt. la) call errmes (13, 1)
      k1 = nblk (k1, la, lina)
      if (k1 .eq. 0) call errmes (14, 1)
      k2 = match (1, k1, la, lina)
      if (k2 .eq. 0) then
        k2 = la + 1
        iend = 1
      endif
      k2 = k2 - 1
      lb = k2 - k1 + 1
      linb(1:lb) = lina(k1:k2)

c   Evaluate the expression with EXPRES after setting the mixed mode option
c   to FAST, except if this is a parameter statement or an intrinsic function
c   reference.

      ims = imm
      if (lu .ne. 12 .and. imm .ne. 2) imm = 0
      call expres2 (lu, lb, linb, itpx, lx, argx)
      imm = ims

c   Add this argument to the list.

 110  nar = nar + 1
      if (nar .gt. mar) then
        call errmes (15, 0)
        write (6, 1)
 1      format ('List has too many arguments.')
        call abrt
      endif
      itar(nar) = itpx
      lar(nar) = lx
      arg(nar)(1:lx) = argx(1:lx)
      k1 = k2 + 2
      if (iend .eq. 0) goto 100

 120  continue
c*
c      write (lu, *) 'exit arlist  args:'
c      do 111 i = 1, nar
c        write (lu, *) '%'//arg(i)(1:lar(i))//'%'
c 111  continue
c      call flush (lu)
c*
      return
      end
      subroutine arlist2 (lu, la, lina, nar, itar, lar, arg)

c   This processes an argument list in LINA, which has length LA.  Any
c   expressions in any argument are first processed with subroutine EXPRES.
c   The resulting argument list (NAR elements) is placed in ARG, with types
c   in ITAR and lengths in ARG.  LU is the logical unit number of output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb
      character*80 arg(mar), argx
      dimension itar(mar), lar(mar)
c*
c      write (lu, *) 'enter arlist'
c      write (lu, *) '%'//lina(1:la)//'%'
c      call flush (lu)
c*
      k1 = 1
      nar = 0
      iend = 0
      if (la .eq. 0 .or. lina(1:la) .eq. ' ') goto 120

 100  if (k1 .gt. la) call errmes (13, 1)
      k1 = nblk (k1, la, lina)
      if (k1 .eq. 0) call errmes (14, 1)
      k2 = match (1, k1, la, lina)
      if (k2 .eq. 0) then
        k2 = la + 1
        iend = 1
      endif
      k2 = k2 - 1
      lb = k2 - k1 + 1
      linb(1:lb) = lina(k1:k2)

c   Evaluate the expression with EXPRES after setting the mixed mode option
c   to FAST, except if this is a parameter statement or an intrinsic function
c   reference.

      ims = imm
      if (lu .ne. 12 .and. imm .ne. 2) imm = 0
      call expres3 (lu, lb, linb, itpx, lx, argx)
      imm = ims

c   Add this argument to the list.

 110  nar = nar + 1
      if (nar .gt. mar) then
        call errmes (15, 0)
        write (6, 1)
 1      format ('List has too many arguments.')
        call abrt
      endif
      itar(nar) = itpx
      lar(nar) = lx
      arg(nar)(1:lx) = argx(1:lx)
      k1 = k2 + 2
      if (iend .eq. 0) goto 100

 120  continue
c*
c      write (lu, *) 'exit arlist  args:'
c      do 111 i = 1, nar
c        write (lu, *) '%'//arg(i)(1:lar(i))//'%'
c 111  continue
c      call flush (lu)
c*
      return
      end
      subroutine arlist3 (lu, la, lina, nar, itar, lar, arg)

c   This processes an argument list in LINA, which has length LA.  Any
c   expressions in any argument are first processed with subroutine EXPRES.
c   The resulting argument list (NAR elements) is placed in ARG, with types
c   in ITAR and lengths in ARG.  LU is the logical unit number of output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb
      character*80 arg(mar), argx
      dimension itar(mar), lar(mar)
c*
c      write (lu, *) 'enter arlist'
c      write (lu, *) '%'//lina(1:la)//'%'
c      call flush (lu)
c*
      k1 = 1
      nar = 0
      iend = 0
      if (la .eq. 0 .or. lina(1:la) .eq. ' ') goto 120

 100  if (k1 .gt. la) call errmes (13, 1)
      k1 = nblk (k1, la, lina)
      if (k1 .eq. 0) call errmes (14, 1)
      k2 = match (1, k1, la, lina)
      if (k2 .eq. 0) then
        k2 = la + 1
        iend = 1
      endif
      k2 = k2 - 1
      lb = k2 - k1 + 1
      linb(1:lb) = lina(k1:k2)

c   Evaluate the expression with EXPRES after setting the mixed mode option
c   to FAST, except if this is a parameter statement or an intrinsic function
c   reference.

      ims = imm
      if (lu .ne. 12 .and. imm .ne. 2) imm = 0
      call expres4 (lu, lb, linb, itpx, lx, argx)
      imm = ims

c   Add this argument to the list.

 110  nar = nar + 1
      if (nar .gt. mar) then
        call errmes (15, 0)
        write (6, 1)
 1      format ('List has too many arguments.')
        call abrt
      endif
      itar(nar) = itpx
      lar(nar) = lx
      arg(nar)(1:lx) = argx(1:lx)
      k1 = k2 + 2
      if (iend .eq. 0) goto 100

 120  continue
c*
c      write (lu, *) 'exit arlist  args:'
c      do 111 i = 1, nar
c        write (lu, *) '%'//arg(i)(1:lar(i))//'%'
c 111  continue
c      call flush (lu)
c*
      return
      end
      subroutine arlist4 (lu, la, lina, nar, itar, lar, arg)

c   This processes an argument list in LINA, which has length LA.  Any
c   expressions in any argument are first processed with subroutine EXPRES.
c   The resulting argument list (NAR elements) is placed in ARG, with types
c   in ITAR and lengths in ARG.  LU is the logical unit number of output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb
      character*80 arg(mar), argx
      dimension itar(mar), lar(mar)
c*
c      write (lu, *) 'enter arlist'
c      write (lu, *) '%'//lina(1:la)//'%'
c      call flush (lu)
c*
      k1 = 1
      nar = 0
      iend = 0
      if (la .eq. 0 .or. lina(1:la) .eq. ' ') goto 120

 100  if (k1 .gt. la) call errmes (13, 1)
      k1 = nblk (k1, la, lina)
      if (k1 .eq. 0) call errmes (14, 1)
      k2 = match (1, k1, la, lina)
      if (k2 .eq. 0) then
        k2 = la + 1
        iend = 1
      endif
      k2 = k2 - 1
      lb = k2 - k1 + 1
      linb(1:lb) = lina(k1:k2)

c   Evaluate the expression with EXPRES after setting the mixed mode option
c   to FAST, except if this is a parameter statement or an intrinsic function
c   reference.

      ims = imm
      if (lu .ne. 12 .and. imm .ne. 2) imm = 0
      call expres5 (lu, lb, linb, itpx, lx, argx)
      imm = ims

c   Add this argument to the list.

 110  nar = nar + 1
      if (nar .gt. mar) then
        call errmes (15, 0)
        write (6, 1)
 1      format ('List has too many arguments.')
        call abrt
      endif
      itar(nar) = itpx
      lar(nar) = lx
      arg(nar)(1:lx) = argx(1:lx)
      k1 = k2 + 2
      if (iend .eq. 0) goto 100

 120  continue
c*
c      write (lu, *) 'exit arlist  args:'
c      do 111 i = 1, nar
c        write (lu, *) '%'//arg(i)(1:lar(i))//'%'
c 111  continue
c      call flush (lu)
c*
      return
      end
      subroutine arlist5 (lu, la, lina, nar, itar, lar, arg)

c   This processes an argument list in LINA, which has length LA.  Any
c   expressions in any argument are first processed with subroutine EXPRES.
c   The resulting argument list (NAR elements) is placed in ARG, with types
c   in ITAR and lengths in ARG.  LU is the logical unit number of output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb
      character*80 arg(mar), argx
      dimension itar(mar), lar(mar)
c*
c      write (lu, *) 'enter arlist'
c      write (lu, *) '%'//lina(1:la)//'%'
c      call flush (lu)
c*
      k1 = 1
      nar = 0
      iend = 0
      if (la .eq. 0 .or. lina(1:la) .eq. ' ') goto 120

 100  if (k1 .gt. la) call errmes (13, 1)
      k1 = nblk (k1, la, lina)
      if (k1 .eq. 0) call errmes (14, 1)
      k2 = match (1, k1, la, lina)
      if (k2 .eq. 0) then
        k2 = la + 1
        iend = 1
      endif
      k2 = k2 - 1
      lb = k2 - k1 + 1
      linb(1:lb) = lina(k1:k2)

c   Evaluate the expression with EXPRES after setting the mixed mode option
c   to FAST, except if this is a parameter statement or an intrinsic function
c   reference.

      ims = imm
      if (lu .ne. 12 .and. imm .ne. 2) imm = 0
      call expres6 (lu, lb, linb, itpx, lx, argx)
      imm = ims

c   Add this argument to the list.

 110  nar = nar + 1
      if (nar .gt. mar) then
        call errmes (15, 0)
        write (6, 1)
 1      format ('List has too many arguments.')
        call abrt
      endif
      itar(nar) = itpx
      lar(nar) = lx
      arg(nar)(1:lx) = argx(1:lx)
      k1 = k2 + 2
      if (iend .eq. 0) goto 100

 120  continue
c*
c      write (lu, *) 'exit arlist  args:'
c      do 111 i = 1, nar
c        write (lu, *) '%'//arg(i)(1:lar(i))//'%'
c 111  continue
c      call flush (lu)
c*
      return
      end
      subroutine arlist6 (lu, la, lina, nar, itar, lar, arg)

c   This processes an argument list in LINA, which has length LA.  Any
c   expressions in any argument are first processed with subroutine EXPRES.
c   The resulting argument list (NAR elements) is placed in ARG, with types
c   in ITAR and lengths in ARG.  LU is the logical unit number of output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb
      character*80 arg(mar), argx
      dimension itar(mar), lar(mar)
c*
c      write (lu, *) 'enter arlist'
c      write (lu, *) '%'//lina(1:la)//'%'
c      call flush (lu)
c*
      k1 = 1
      nar = 0
      iend = 0
      if (la .eq. 0 .or. lina(1:la) .eq. ' ') goto 120

 100  if (k1 .gt. la) call errmes (13, 1)
      k1 = nblk (k1, la, lina)
      if (k1 .eq. 0) call errmes (14, 1)
      k2 = match (1, k1, la, lina)
      if (k2 .eq. 0) then
        k2 = la + 1
        iend = 1
      endif
      k2 = k2 - 1
      lb = k2 - k1 + 1
      linb(1:lb) = lina(k1:k2)

c   Evaluate the expression with EXPRES after setting the mixed mode option
c   to FAST, except if this is a parameter statement or an intrinsic function
c   reference.

      ims = imm
      if (lu .ne. 12 .and. imm .ne. 2) imm = 0
      call expres7 (lu, lb, linb, itpx, lx, argx)
      imm = ims

c   Add this argument to the list.

 110  nar = nar + 1
      if (nar .gt. mar) then
        call errmes (15, 0)
        write (6, 1)
 1      format ('List has too many arguments.')
        call abrt
      endif
      itar(nar) = itpx
      lar(nar) = lx
      arg(nar)(1:lx) = argx(1:lx)
      k1 = k2 + 2
      if (iend .eq. 0) goto 100

 120  continue
c*
c      write (lu, *) 'exit arlist  args:'
c      do 111 i = 1, nar
c        write (lu, *) '%'//arg(i)(1:lar(i))//'%'
c 111  continue
c      call flush (lu)
c*
      return
      end
      subroutine arlist7 (lu, la, lina, nar, itar, lar, arg)

c   This processes an argument list in LINA, which has length LA.  Any
c   expressions in any argument are first processed with subroutine EXPRES.
c   The resulting argument list (NAR elements) is placed in ARG, with types
c   in ITAR and lengths in ARG.  LU is the logical unit number of output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb
      character*80 arg(mar), argx
      dimension itar(mar), lar(mar)
c*
c      write (lu, *) 'enter arlist'
c      write (lu, *) '%'//lina(1:la)//'%'
c      call flush (lu)
c*
      k1 = 1
      nar = 0
      iend = 0
      if (la .eq. 0 .or. lina(1:la) .eq. ' ') goto 120

 100  if (k1 .gt. la) call errmes (13, 1)
      k1 = nblk (k1, la, lina)
      if (k1 .eq. 0) call errmes (14, 1)
      k2 = match (1, k1, la, lina)
      if (k2 .eq. 0) then
        k2 = la + 1
        iend = 1
      endif
      k2 = k2 - 1
      lb = k2 - k1 + 1
      linb(1:lb) = lina(k1:k2)

c   Evaluate the expression with EXPRES after setting the mixed mode option
c   to FAST, except if this is a parameter statement or an intrinsic function
c   reference.

      ims = imm
      if (lu .ne. 12 .and. imm .ne. 2) imm = 0
      call expres8 (lu, lb, linb, itpx, lx, argx)
      imm = ims

c   Add this argument to the list.

 110  nar = nar + 1
      if (nar .gt. mar) then
        call errmes (15, 0)
        write (6, 1)
 1      format ('List has too many arguments.')
        call abrt
      endif
      itar(nar) = itpx
      lar(nar) = lx
      arg(nar)(1:lx) = argx(1:lx)
      k1 = k2 + 2
      if (iend .eq. 0) goto 100

 120  continue
c*
c      write (lu, *) 'exit arlist  args:'
c      do 111 i = 1, nar
c        write (lu, *) '%'//arg(i)(1:lar(i))//'%'
c 111  continue
c      call flush (lu)
c*
      return
      end
      subroutine arlist8 (lu, la, lina, nar, itar, lar, arg)

c   This processes an argument list in LINA, which has length LA.  Any
c   expressions in any argument are first processed with subroutine EXPRES.
c   The resulting argument list (NAR elements) is placed in ARG, with types
c   in ITAR and lengths in ARG.  LU is the logical unit number of output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb
      character*80 arg(mar), argx
      dimension itar(mar), lar(mar)
c*
c      write (lu, *) 'enter arlist'
c      write (lu, *) '%'//lina(1:la)//'%'
c      call flush (lu)
c*
      k1 = 1
      nar = 0
      iend = 0
      if (la .eq. 0 .or. lina(1:la) .eq. ' ') goto 120

 100  if (k1 .gt. la) call errmes (13, 1)
      k1 = nblk (k1, la, lina)
      if (k1 .eq. 0) call errmes (14, 1)
      k2 = match (1, k1, la, lina)
      if (k2 .eq. 0) then
        k2 = la + 1
        iend = 1
      endif
      k2 = k2 - 1
      lb = k2 - k1 + 1
      linb(1:lb) = lina(k1:k2)

c   Evaluate the expression with EXPRES after setting the mixed mode option
c   to FAST, except if this is a parameter statement or an intrinsic function
c   reference.

      ims = imm
      if (lu .ne. 12 .and. imm .ne. 2) imm = 0
      call expres (lu, lb, linb, itpx, lx, argx)
      imm = ims

c   Add this argument to the list.

 110  nar = nar + 1
      if (nar .gt. mar) then
        call errmes (15, 0)
        write (6, 1)
 1      format ('List has too many arguments.')
        call abrt
      endif
      itar(nar) = itpx
      lar(nar) = lx
      arg(nar)(1:lx) = argx(1:lx)
      k1 = k2 + 2
      if (iend .eq. 0) goto 100

 120  continue
c*
c      write (lu, *) 'exit arlist  args:'
c      do 111 i = 1, nar
c        write (lu, *) '%'//arg(i)(1:lar(i))//'%'
c 111  continue
c      call flush (lu)
c*
      return
      end
      subroutine asst (k1, ln)

c   This processes MP assignment statements.  K1 and LN are the indices of the
c   first and last non-blank characters in the statement.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*80 argx

c   Evaluate the entire statement as an expression.

      la = ln - k1 + 1
      lina(1:la) = line(k1:ln)
      call expres (11, la, lina, itpx, lx, argx)

c   Check if the final result of evaluation of the expression is non-empty.
c   If so, then the last operation was not an equal operation, and thus the
c   statement is not a valid assignment statement.

      if (lx .ne. 0) then
        call errmes (16, 0)
        write (6, 1)
 1      format ('This is not a valid MP assignment statement.')
        call abrt
      endif

      return
      end

      subroutine callst (k1, ln)

c   This processes MP call statements.  K1 and LN are the indices of the
c   first (after 'call') and last non-blank characters in the statement.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb
      character*80 arg(mar)
      character*16 namq, namx
      dimension itar(mar), lar(mar)
      character*4 tmp1

c   Identify subroutine name.

      i1 = indx (k1, ln, '(', line)
      if (i1 .eq. 0) i1 = ln + 1
      l1 = min (i1 - k1, 16)
      namx = line(k1:k1+l1-1)
      ix = itab (0, 0, namx)
      if (ix .eq. 0) then
        call errmes (17, 0)
        write (6, 1) namx
 1      format ('This Fortran keyword may not CALLed: ',a)
        call abrt
      endif
      if (ix .gt. mint) ktyp(ix) = 0
      if (i1 .gt. ln) then
        nar = 0
        call chkarg (11, namx, nar, itar, lar, arg)
        call outlin (11, 1, ln, line)
        goto 130
      endif
      lina(1:11) = '      CALL '
      iss = 0

c   Determine if this is one of the special DP subroutine names.

      namq = 'DPCSSN'
      iq1 = itab (0, 0, namq)
      namq = 'DPCSSH'
      iq2 = itab (0, 0, namq)
      if (ix .eq. iq1) then
        kcon(5) = 1
        lina(12:17) = 'MPCSSN'
        la = 17
        iss = 1
      elseif (ix .eq. iq2) then
        kcon(3) = 1
        lina(12:17) = 'MPCSSH'
        la = 17
        iss = 2
      else
        lina(12:l1+11) = namx(1:l1)
        la = l1 + 11
      endif

      lina(la+1:la+1) = '('
      la = la + 1

c   Identify and process list of arguments.

      i2 = match (0, i1 + 1, ln, line)
      if (i2 .ne. ln) call errmes (18, 1)
      lb = ln - i1 - 1
      if (lb .eq. 0) then
        la = la + 2
      else
        linb(1:lb) = line(i1+1:ln-1)
      endif
      call arlist (11, lb, linb, nar, itar, lar, arg)

c   Check the argument list with the subprogram table.

      if (iss .eq. 0) call chkarg (11, namx, nar, itar, lar, arg)

c   Change the argument list if this is a special DP subroutine.

      if (iss .eq. 1) then
        if (nar .ne. 3) goto 120
        nar = 4
        itar(4) = itar(3)
        lar(4) = lar(3)
        arg(4) = arg(3)
        itar(3) = itar(2)
        lar(3) = lar(2)
        arg(3) = arg(2)
        itar(2) = 9
        lar(2) = 5
        arg(2) = 'MPPIC'
      elseif (iss .eq. 2) then
        if (nar .ne. 3) goto 120
        nar = 4
        itar(4) = itar(3)
        lar(4) = lar(3)
        arg(4) = arg(3)
        itar(3) = itar(2)
        lar(3) = lar(2)
        arg(3) = arg(2)
        itar(2) = 9
        lar(2) = 5
        arg(2) = 'MPL02'
      endif

c   Append the argument list.

      do 100 j = 1, nar
        l1 = lar(j)
        lina(la+1:la+l1) = arg(j)(1:l1)
        lina(la+l1+1:la+l1+2) = ', '
        la = la + l1 + 2
 100  continue

      lina(la-1:la-1) = ')'
      la = la - 1
      call outlin (11, 1, la, lina)

c   Release any temporaries among the arguments.

      do 110 i = 1, nar
        li = lar(i)
        if (li .eq. 4) then
          if (arg(i)(1:2) .eq. 'MP') then
            tmp1 = arg(i)(1:4)
            call rltmp (tmp1)
          endif
        endif
 110  continue

      goto 130

 120  call errmes (19, 0)
      write (6, 2) namx
 2    format ('Improper number of arguments for this special',
     $  ' subroutine: ',a)
      call abrt

 130  return
      end

      subroutine chkarg (lu, nam, nar, itar, lar, arg)

c   This routine checks to see if a subroutine or function name is in the
c   subprogram table.  If it is, the calling sequence is compared with that
c   in the table.  If not, it is added to the table.  LU is the unit number
c   for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*80 arg(mar)
      character*16 nam, namx
      dimension itar(mar), lar(mar)

      ix = itab (0, 0, nam)
      if (ix .eq. 0) call errmes (20, 1)
      namx = var(ix)
      lx = lvar(ix)

c   Check if the function name is in the subprogram table.

      do 100 i = 1, nsub
        if (snam(i) .eq. namx) goto 120
 100  continue

c   Insert this function and its calling sequence in the subprogram table.

      nsub = nsub + 1
      if (nsub .gt. msub) then
        call errmes (21, 0)
        write (6, 1)
 1      format ('Too many program units in this file.')
        call abrt
      endif
      snam(nsub) = namx
      nars(nsub) = nar
      kstp(0,nsub) = ktyp(ix)

      do 110 i = 1, nar
        kstp(i,nsub) = itar(i)
 110  continue

      goto 150

c   The function name is in the subprogram table.  Check if the types of the
c   result and the arguments are the same as in the table.

 120  ks = i
      i = -1
      if (nar .ne. nars(ks)) goto 140
      i = 0
      if (ktyp(ix) .ne. kstp(0,ks) .and. ktyp(ix) * kstp(0,ks) .ne. 0)
     $  goto 140

      do 130 i = 1, nar
        if (itar(i) .ne. kstp(i,ks) .and. itar(i) * kstp(i,ks) .ne. 0)
     $    goto 140
 130  continue

      goto 150

c   A warning message or a fatal error is generated, depending on the type
c   error flag ITE.

 140  if (ite .eq. 0) then
        write (lu, 2) nam(1:lx), i
 2      format ('CMP*'/'CMP*  The result type or argument list of this',
     $    ' function or subroutine is'/'CMP*  incompatible with a',
     $    ' previous definition or reference: ',a,i4/'CMP*')
      else
        call errmes (22, 0)
        write (6, 3) nam(1:lx), i
 3      format ('The result type or argument list of this function or',
     $    ' subroutine is'/'incompatible with a previous definition or',
     $    ' reference: ',a,i4)
        call abrt
      endif

 150  return
      end

      subroutine copy

c   This reads the generated code for one subprogram and copies it to the
c   output file, inserting MP declaration code at the marker if required.

      character*80 lin

      endfile 11
      rewind 11
      endfile 12
      rewind 12

 100  read (11, '(A)', end = 110) lin
      ln = lnblk (lin)
      if (ln .eq. 6 .and. lin(1:6) .eq. 'CMP>>>') then
        call inimp
      else
        write (6, '(A)') lin(1:ln)
      endif
      goto 100

 110  rewind 11
      rewind 12
      return
      end

      subroutine dimen (k1, ln)

c   This processes dimension and common statements by delimiting variable
c   names, inserting in table if required and correcting dimensions of MP
c   variables.  K1 and LN are the indices of the first (after 'dimension' or
c   'common') and last non-blank characters in the statement.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*16 nam
      character*1 cj
      character*8 dim1, dim2, dimy

c   Place the MP dimension into the character variable DIMX.

      write (dimy, '(I8)') mxp + 4
      i1 = nblk (1, 8, dimy)
      ld1 = 9 - i1
      dim1 = dimy(i1:8)
      write (dimy, '(I8)') 2 * mxp + 8
      i1 = nblk (1, 8, dimy)
      ld2 = 9 - i1
      dim2 = dimy(i1:8)
      j1 = k1

c   Output statement as a comment.

      call outlin (11, 2, ln, line)

c   Extract the next character from the line.

 100  if (j1 .gt. ln) goto 130
      j1 = nblk (j1, ln, line)
      cj = line(j1:j1)

c   Check if it the start of a name.

      if (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then

        do 110 j = j1, ln
          cj = line(j:j)
          if (max (index (alpl, cj), index (alpu, cj)) .ne. 0) goto 110
          if (index (dig, cj) .ne. 0 .or. cj .eq. '_') goto 110
          if (index (del, cj) .ne. 0) goto 120
          call errmes (23, 1)
 110    continue

        j = ln + 1
 120    j2 = j - 1
        nam = line(j1:j2)
        ix = itab (1, 0, nam)
        if (ix .eq. 0) then
          call errmes (24, 0)
          write (6, 1) nam
 1        format ('This Fortran keyword may not appear in a dimension',
     $      ' or common statement: '/a)
          call abrt
        endif
        ktp = ktyp(ix)
        if (ktp .lt. 8) kdec(ix) = 1
        if (j2 .ge. ln) goto 130
        k3 = nblk (j2 + 1, ln, line)
        cj = line(k3:k3)

c   Check if this variable has a dimension declaration.

        if (cj .eq. '(') then
          kdim(ix) = 1

c   If this is a MP variable, correct the dimension.

          if (ktp .ge. 8) then
            lina(1:k3) = line(1:k3)
            if (ktp .lt. 10) then
              lina(k3+1:k3+ld1) = dim1(1:ld1)
              ldx = ld1
            else
              lina(k3+1:k3+ld2) = dim2(1:ld2)
              ldx = ld2
            endif
            lina(k3+ldx+1:k3+ldx+1) = ','
            lina(k3+ldx+2:ln+ldx+1) = line(k3+1:ln)
            ln = ln + ldx + 1
            line(1:ln) = lina(1:ln)
          endif
          j2 = match (0, k3 + 1, ln, line)
          if (j2 .eq. 0) call errmes (25, 1)
          i1 = iscan (k3, j2, line)
          if (i1 .ne. 0) then
            call errmes (26, 0)
            write (6, 2) nam
 2          format ('The MP dimension on this variable is not',
     $        ' allowed: ',a)
            call abrt
          endif
        endif
        j1 = j2 + 1
        goto 100

c   The only other character that should appear here is a comma.

      elseif (cj .eq. ',') then
        j1 = j1 + 1
        goto 100
      else
        call errmes (27, 1)
      endif

 130  call outlin (11, 1, ln, line)
      write (11, 3)
 3    format ('CMP<')

      return
      end

      subroutine errmes (ia, ib)

c   This outputs a syntax error message with the line number.  If IB is
c   nonzero, ABRT is also called.  IA is the message code, which currently
c   is in the range 1 - 96.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      if (ib .eq. 0) then
        write (6, 1) lct, ia
 1      format ('*** Error in statement starting at line',i6,3x,'Code',
     $    i6)
      else
        write (6, 2) lct, ia
 2      format ('*** Syntax error in statement starting at line',i6,
     $    3x,'Code',i6)
        call abrt
      endif

      return
      end

      subroutine exec (k1, ln)

c   This handles MP executable statements.  K1 and LN are the indices of the
c   first and last non-blank characters in the statement.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*16 linq, ucase

c   Check if this is a call statement.

      j1 = k1
      lq = min (j1 + 15, ln)
      linq = ucase (line(j1:lq))
      if (linq(1:5) .eq. 'CALL ') then
        j1 = nblk (j1 + 5, ln, line)
        i1 = indx (j1, ln, '(', line)
        call callst (j1, ln)
        goto 110
      endif

c   Check if this is a read or write statement.

      irw = 0
      if (linq(1:4) .eq. 'READ') then
        irw = 1
        k3 = j1 + 4
      elseif (linq(1:5) .eq. 'WRITE') then
        irw = 2
        k3 = j1 + 5
      endif
      if (irw .ne. 0) then
        k3 = nblk (k3, ln, line)
        if (line(k3:k3) .ne. '(') goto 100
        j1 = k3
        j2 = match (0, j1 + 1, ln, line)
        if (j2 .eq. 0) call errmes (28, 1)
        call rdwr (irw, j1, j2, ln)
        goto 110
      endif

c   Check if this is an assignment statement.

 100  call asst (j1, ln)

 110  return
      end


      subroutine expres (lu, la, lina, itpx, lx, argx)

c   This processes the arithmetic and/or logical expression in LINA, of length
c   = LA.  The result, after evaluation, is placed in ARGX, with type = ITPX,
c   and length = LX.  If the last result was =, then the ARGX is set to blanks,
c   and ITPX and LX are set to zero.  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb, linc
      character*80 ar(2), arg(mar), arg1, argx, argy
      character*16 nam
      dimension itar(mar), lar(mar), ita(2), lna(2), id1(2), id2(2),
     $  iop(2)
      character*1 cj
c*
c      write (lu, *) 'enter expres'
c      call flush (lu)
c*
c   Search for an executable operation (one that is not dependent on the
c   results of operations with higher precedence) in the statement.  The two
c   arguments of the operation, their types and lengths, are identified.

      lb = la
      linb(1:lb) = lina(1:la)

 100  i1 = 1
c*
c      write (lu, *) 'expres lb =', lb
c      write (lu, *) '%'//linb(1:lb)//'%'
c      call flush (lu)
c*
 110  do 200 ii = 1, 2
        i1 = nblk (i1, lb, linb)
        if (i1 .eq. 0) call errmes (29, 1)
        cj = linb(i1:i1)
        ix = 0
c*
c        write (lu, *) 'cj =', cj
c        call flush (lu)
c*
c   Check if this is the start of a numeric constant.

        is1 = index (dig, cj)
        if ((cj .eq. '-' .or. cj .eq. '.') .and. i1 .lt. lb) then
          i2 = nblk (i1 + 1, lb, linb)
          is2 = index (dig, linb(i2:i2))
        else
          is2 = 0
        endif

        if (is1 .ne. 0 .or. is2 .ne. 0) then
          it = numcon (i1, i2, lb, linb)

c   If the mixed mode safe flag is on, then the numeric constant is always
c   of type MP.

          if (imm .ge. 1) then
            if (it .eq. 1) then
              it = 8
            elseif (it .eq. 2 .or. it .eq. 3) then
              it = 9
            endif
          endif
          if (it .le. 3) then
            id1(ii) = i1
            id2(ii) = i2
            ita(ii) = it
            lna(ii) = i2 - i1 + 1
            ar(ii) = linb(i1:i2)
          else
            id1(1) = i1
            id2(2) = i2
            itpy = it
            call gencon (lu, i1, i2, linb, itpy, ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220
          endif

c   Check if string is a variable name.

        elseif (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then

          do 120 j = i1, lb
            cj = linb(j:j)
            if (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then
              goto 120
            elseif (index (dig, cj) .ne. 0 .or. cj .eq. '_') then
              goto 120
            else
              goto 130
            endif
 120      continue

          j = lb + 1

c   Variable or function name has been identified.

 130      i2 = j - 1
          l1 = i2 - i1 + 1
          nam = linb(i1:i2)
          ix = itab (1, 0, nam)
          if (ix .eq. 0) then
            call errmes (30, 0)
            write (6, 1) nam
 1          format ('This Fortran keyword may not appear in an',
     $        ' expression: ',a)
            call abrt
          elseif (ktyp(ix) .eq. 0 .and. kdec(ix) .eq. 0) then
            call errmes (31, 0)
            write (6, 2) nam
 2          format ('This variable has not been typed: ',a)
            call abrt
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = ktyp(ix)
          lna(ii) = l1
          ar(ii)(1:l1) = linb(i1:i2)

c   Check if string is a logical constant.

        elseif (cj .eq. '.' .and. i1 .lt. lb .and.
     $      (linb(i1+1:i1+1) .eq. 'T' .or. linb(i1+1:i1+1) .eq. 't' .or.
     $      linb(i1+1:i1+1) .eq. 'F' .or. linb(i1+1:i1+1) .eq. 'f'))
     $      then
          if (linb(i1:i1+2) .eq. '.T.' .or. linb(i1:i1+2) .eq. '.t.'.or.
     $      linb(i1:i1+2) .eq. '.F.' .or. linb(i1:i1+2) .eq. '.f.')
     $      then
            i2 = i1 + 2
          elseif (linb(i1:i1+5) .eq. '.TRUE.' .or.
     $        linb(i1:i1+5) .eq. '.true.') then
            i2 = i1 + 5
          elseif (linb(i1:i1+6) .eq. '.FALSE.' .or.
     $        linb(i1:i1+6) .eq. '.false.') then
            i2 = i1 + 6
          else
            call errmes (32, 1)
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 7
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by ".

        elseif (cj .eq. '"') then
          j1 = i1
 140      i2 = indx (j1 + 1, lb, '"', linb)
          if (i2 .eq. 0) call errmes (33, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '"') then
            j1 = i2 + 1
            goto 140
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by '.

        elseif (cj .eq. '''') then
          j1 = i1
 150      i2 = indx (j1 + 1, lb, '''', linb)
          if (i2 .eq. 0) call errmes (34, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '''') then
            j1 = i2 + 1
            goto 150
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a unary minus sign (i.e. argument 1 is null).

        elseif (cj .eq. '-') then
          if (ii .eq. 2) goto 110
          id1(1) = i1
          id2(1) = i1
          ita(1) = 0
          lna(1) = 1
          ar(1) = ' '
          iop(1) = 3
          i1 = i1 + 1
          goto 200

c   Check if argument is a logical not operation (i.e. argument 1 is null).

        elseif (cj .eq. '.' .and. i1 + 4 .le. lb) then
          if (linb(i1:i1+4) .eq. '.NOT.' .or. linb(i1:i1+4) .eq.
     $      '.not.') then
            if (ii .eq. 2) goto 110
            id1(1) = i1
            id2(1) = i1
            ita(1) = 0
            lna(1) = 1
            ar(1) = ' '
            iop(1) = 15
            i1 = i1 + 5
            goto 200
          endif

c   Check if the next character is a left parenthesis.  If so, evaluate the
c   expression in parentheses.

        elseif (cj .eq. '(') then
          i2 = match (0, i1 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (35, 1)
          lc = i2 - i1 - 1
          linc(1:lc) = linb(i1+1:i2-1)
          id1(1) = i1
          id2(2) = i2
          if (lb .eq. 0) call errmes (36, 1)
          call expres1 (lu, lc, linc, itpy, ly, argy)
          goto 220
        else
          call errmes (37, 0)
          write (6, 3) cj
 3        format ('Illegal character: ',a)
          call abrt
        endif

c   Check if the end of the variable or constant is the end of the statement.

 160    continue
        if (i2 .eq. lb .or. linb(i2+1:lb) .eq. ' ') then

c   If this occurs on the first pass, we are done.

          if (ii .eq. 1) then
            itpx = ita(1)
            lx = lna(1)
            argx(1:lx) = ar(1)(1:lx)
            goto 230

c   If this occurs on the second pass, proceed to evaluate.

          else
            goto 210
          endif
        endif

c   Check if the next character after the variable name is a left parenthesis.

        i3 = nblk (i2 + 1, lb, linb)
        cj = linb(i3:i3)
        if (cj .eq. '(') then
          if (ix .eq. 0) call errmes (38, 1)
          i2 = match (0, i3 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (39, 1)
          k1 = index (linb(i2:lb), '=')
 170      continue

c   Check if the subscripted variable is really a function reference.

          if (kdim(ix) .eq. -1) then
            if (k1 .ne. 0) then
              call errmes (40, 0)
              write (6, 4) nam(1:l1)
 4            format ('A function name may not appear on the LHS of',
     $          ' an assignment statement: ',a)
              call abrt
            endif

c   Generate a function call.  If it is an intrinsic reference and the mixed
c   mode SAFE option is in effect, set IMM = 2 as a flag to inform ARLIST not
c   to revert to mixed mode FAST while evaluating the argument list.

            id1(1) = i1
            id2(2) = i2
            lc = i2 - i3 - 1
            linc(1:lc) = linb(i3+1:i2-1)
            ims = imm
            if (ix .le. mint .and. imm .ge. 1) imm = 2
            call arlist1 (lu, lc, linc, nar, itar, lar, arg)
            imm = ims
            it1 = ita(ii)
            ln1 = lna(ii)
            arg1 = ar(ii)
            call genfun (lu, it1, ln1, arg1, nar, itar, lar, arg, itpy,
     $        ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220

c   Check if the subscripted variable has a dimension.  If not, this is
c   assumed to be an external function reference.

          elseif (kdim(ix) .eq. 0) then
            if (k1 .eq. 0) then

c   If this function name was earlier listed in an external statement and was
c   not explicitly typed, it must now be implicitly typed.

              if (kdec(ix) .eq. 1 .and. mplc(ix) .eq. 1) then
                cj = nam(1:1)
                ij = max (index (alpu, cj), index (alpl, cj))
                ktyp(ix) = impl(ij)
                ita(ii) = ktyp(ix)
              endif
              kdec(ix) = 1
              kdim(ix) = -1
              kt = ktyp(ix)
              write (lu, 5) nam(1:l1), ctp(kt)
 5            format ('CMP*'/'CMP*  Undimensioned variable assumed to',
     $          ' be an external function.'/'CMP*  Name: ',a,4x,
     $          'Type: ',a/'CMP*')
              goto 170
            else
              call errmes (41, 0)
              write (6, 6)
 6            format ('MP variables may not be used in statement',
     $          ' function definitions.'/'Define an external function',
     $          ' for this purpose.')
              call abrt
            endif

c   Otherwise it must be an ordinary array with subscript.  The combination of
c   the variable name and the subscript will now be treated as a variable.
c   IX is set to 0 as a flag indicating that this has been done.

          else
            i4 = iscan (i3, i2, linb)
            if (i4 .ne. 0) then
              call errmes (42, 0)
              write (6, 7) nam
 7            format ('The MP subscript on this variable is not',
     $          ' allowed: ',a)
              call abrt
            endif
            l1 = i2 - i1 + 1
            id2(ii) = i2
            lna(ii) = l1
            ar(ii)(1:l1) = linb(i1:i2)
            ix = 0
            goto 160
          endif
        else

c   The variable does not have a subscript.  Check if it has a dimension.

          if (ix .gt. 0) then
            if (kdim(ix) .gt. 0) then
              call errmes (43, 0)
              write (6, 8) nam(1:l1)
 8            format ('This dimensioned variable is used without a',
     $          ' subscript: ',a)
              call abrt
            endif
          endif
        endif
        i1 = i3

c   Identify the operator.

        do 180 i = 1, nop
          l1 = lop(i) - 1
          if (linb(i1:i1+l1) .eq. lopr(i) .or. linb(i1:i1+l1) .eq.
     $      uopr(i)) goto 190
 180    continue

        call errmes (44, 0)
        write (6, 9) linb(i1:i1)
 9      format ('Illegal operator: ',a)
        call abrt

 190    iop(ii) = i
        i1 = i1 + lop(i)
 200  continue

c   Compare the precedence levels of the two operators.

      if (kop(iop(1)) .lt. kop(iop(2))) then
        i1 = id1(2)
        goto 110
      endif

c   An operation can be performed.

210   call gen (lu, ita, lna, ar, iop(1), itpy, ly, argy)

c   Replace the two operands and the operator with the result in LINB.

 220  if (ly .ne. 0) then
        i1 = id1(1)
        i2 = id2(2)
        l1 = i2 - i1 + 1
        ld = ly - l1
        if (i1 .gt. 0) linc(1:i1-1) = linb(1:i1-1)
        linc(i1:i1+ly-1) = argy(1:ly)
        if (lb .gt. i2) linc(i1+ly:lb+ld) = linb(i2+1:lb)
        lb = lb + ld
        linb(1:lb) = linc(1:lb)
        goto 100
      else
        itpx = itpy
        lx = ly
        argx = argy
      endif

c   Finished at last.

 230  continue
c*
c      write (lu, *) 'exit express, argx = %'//argx(1:lx)//'%'
c      call flush (lu)
c*
      return
      end
      subroutine expres1 (lu, la, lina, itpx, lx, argx)

c   This processes the arithmetic and/or logical expression in LINA, of length
c   = LA.  The result, after evaluation, is placed in ARGX, with type = ITPX,
c   and length = LX.  If the last result was =, then the ARGX is set to blanks,
c   and ITPX and LX are set to zero.  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb, linc
      character*80 ar(2), arg(mar), arg1, argx, argy
      character*16 nam
      dimension itar(mar), lar(mar), ita(2), lna(2), id1(2), id2(2),
     $  iop(2)
      character*1 cj
c*
c      write (lu, *) 'enter expres'
c      call flush (lu)
c*
c   Search for an executable operation (one that is not dependent on the
c   results of operations with higher precedence) in the statement.  The two
c   arguments of the operation, their types and lengths, are identified.

      lb = la
      linb(1:lb) = lina(1:la)

 100  i1 = 1
c*
c      write (lu, *) 'expres lb =', lb
c      write (lu, *) '%'//linb(1:lb)//'%'
c      call flush (lu)
c*
 110  do 200 ii = 1, 2
        i1 = nblk (i1, lb, linb)
        if (i1 .eq. 0) call errmes (29, 1)
        cj = linb(i1:i1)
        ix = 0
c*
c        write (lu, *) 'cj =', cj
c        call flush (lu)
c*
c   Check if this is the start of a numeric constant.

        is1 = index (dig, cj)
        if ((cj .eq. '-' .or. cj .eq. '.') .and. i1 .lt. lb) then
          i2 = nblk (i1 + 1, lb, linb)
          is2 = index (dig, linb(i2:i2))
        else
          is2 = 0
        endif

        if (is1 .ne. 0 .or. is2 .ne. 0) then
          it = numcon (i1, i2, lb, linb)

c   If the mixed mode safe flag is on, then the numeric constant is always
c   of type MP.

          if (imm .ge. 1) then
            if (it .eq. 1) then
              it = 8
            elseif (it .eq. 2 .or. it .eq. 3) then
              it = 9
            endif
          endif
          if (it .le. 3) then
            id1(ii) = i1
            id2(ii) = i2
            ita(ii) = it
            lna(ii) = i2 - i1 + 1
            ar(ii) = linb(i1:i2)
          else
            id1(1) = i1
            id2(2) = i2
            itpy = it
            call gencon (lu, i1, i2, linb, itpy, ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220
          endif

c   Check if string is a variable name.

        elseif (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then

          do 120 j = i1, lb
            cj = linb(j:j)
            if (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then
              goto 120
            elseif (index (dig, cj) .ne. 0 .or. cj .eq. '_') then
              goto 120
            else
              goto 130
            endif
 120      continue

          j = lb + 1

c   Variable or function name has been identified.

 130      i2 = j - 1
          l1 = i2 - i1 + 1
          nam = linb(i1:i2)
          ix = itab (1, 0, nam)
          if (ix .eq. 0) then
            call errmes (30, 0)
            write (6, 1) nam
 1          format ('This Fortran keyword may not appear in an',
     $        ' expression: ',a)
            call abrt
          elseif (ktyp(ix) .eq. 0 .and. kdec(ix) .eq. 0) then
            call errmes (31, 0)
            write (6, 2) nam
 2          format ('This variable has not been typed: ',a)
            call abrt
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = ktyp(ix)
          lna(ii) = l1
          ar(ii)(1:l1) = linb(i1:i2)

c   Check if string is a logical constant.

        elseif (cj .eq. '.' .and. i1 .lt. lb .and.
     $      (linb(i1+1:i1+1) .eq. 'T' .or. linb(i1+1:i1+1) .eq. 't' .or.
     $      linb(i1+1:i1+1) .eq. 'F' .or. linb(i1+1:i1+1) .eq. 'f'))
     $      then
          if (linb(i1:i1+2) .eq. '.T.' .or. linb(i1:i1+2) .eq. '.t.'.or.
     $      linb(i1:i1+2) .eq. '.F.' .or. linb(i1:i1+2) .eq. '.f.')
     $      then
            i2 = i1 + 2
          elseif (linb(i1:i1+5) .eq. '.TRUE.' .or.
     $        linb(i1:i1+5) .eq. '.true.') then
            i2 = i1 + 5
          elseif (linb(i1:i1+6) .eq. '.FALSE.' .or.
     $        linb(i1:i1+6) .eq. '.false.') then
            i2 = i1 + 6
          else
            call errmes (32, 1)
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 7
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by ".

        elseif (cj .eq. '"') then
          j1 = i1
 140      i2 = indx (j1 + 1, lb, '"', linb)
          if (i2 .eq. 0) call errmes (33, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '"') then
            j1 = i2 + 1
            goto 140
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by '.

        elseif (cj .eq. '''') then
          j1 = i1
 150      i2 = indx (j1 + 1, lb, '''', linb)
          if (i2 .eq. 0) call errmes (34, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '''') then
            j1 = i2 + 1
            goto 150
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a unary minus sign (i.e. argument 1 is null).

        elseif (cj .eq. '-') then
          if (ii .eq. 2) goto 110
          id1(1) = i1
          id2(1) = i1
          ita(1) = 0
          lna(1) = 1
          ar(1) = ' '
          iop(1) = 3
          i1 = i1 + 1
          goto 200

c   Check if argument is a logical not operation (i.e. argument 1 is null).

        elseif (cj .eq. '.' .and. i1 + 4 .le. lb) then
          if (linb(i1:i1+4) .eq. '.NOT.' .or. linb(i1:i1+4) .eq.
     $      '.not.') then
            if (ii .eq. 2) goto 110
            id1(1) = i1
            id2(1) = i1
            ita(1) = 0
            lna(1) = 1
            ar(1) = ' '
            iop(1) = 15
            i1 = i1 + 5
            goto 200
          endif

c   Check if the next character is a left parenthesis.  If so, evaluate the
c   expression in parentheses.

        elseif (cj .eq. '(') then
          i2 = match (0, i1 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (35, 1)
          lc = i2 - i1 - 1
          linc(1:lc) = linb(i1+1:i2-1)
          id1(1) = i1
          id2(2) = i2
          if (lb .eq. 0) call errmes (36, 1)
          call expres2 (lu, lc, linc, itpy, ly, argy)
          goto 220
        else
          call errmes (37, 0)
          write (6, 3) cj
 3        format ('Illegal character: ',a)
          call abrt
        endif

c   Check if the end of the variable or constant is the end of the statement.

 160    continue
        if (i2 .eq. lb .or. linb(i2+1:lb) .eq. ' ') then

c   If this occurs on the first pass, we are done.

          if (ii .eq. 1) then
            itpx = ita(1)
            lx = lna(1)
            argx(1:lx) = ar(1)(1:lx)
            goto 230

c   If this occurs on the second pass, proceed to evaluate.

          else
            goto 210
          endif
        endif

c   Check if the next character after the variable name is a left parenthesis.

        i3 = nblk (i2 + 1, lb, linb)
        cj = linb(i3:i3)
        if (cj .eq. '(') then
          if (ix .eq. 0) call errmes (38, 1)
          i2 = match (0, i3 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (39, 1)
          k1 = index (linb(i2:lb), '=')
 170      continue

c   Check if the subscripted variable is really a function reference.

          if (kdim(ix) .eq. -1) then
            if (k1 .ne. 0) then
              call errmes (40, 0)
              write (6, 4) nam(1:l1)
 4            format ('A function name may not appear on the LHS of',
     $          ' an assignment statement: ',a)
              call abrt
            endif

c   Generate a function call.  If it is an intrinsic reference and the mixed
c   mode SAFE option is in effect, set IMM = 2 as a flag to inform ARLIST not
c   to revert to mixed mode FAST while evaluating the argument list.

            id1(1) = i1
            id2(2) = i2
            lc = i2 - i3 - 1
            linc(1:lc) = linb(i3+1:i2-1)
            ims = imm
            if (ix .le. mint .and. imm .ge. 1) imm = 2
            call arlist2 (lu, lc, linc, nar, itar, lar, arg)
            imm = ims
            it1 = ita(ii)
            ln1 = lna(ii)
            arg1 = ar(ii)
            call genfun (lu, it1, ln1, arg1, nar, itar, lar, arg, itpy,
     $        ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220

c   Check if the subscripted variable has a dimension.  If not, this is
c   assumed to be an external function reference.

          elseif (kdim(ix) .eq. 0) then
            if (k1 .eq. 0) then

c   If this function name was earlier listed in an external statement and was
c   not explicitly typed, it must now be implicitly typed.

              if (kdec(ix) .eq. 1 .and. mplc(ix) .eq. 1) then
                cj = nam(1:1)
                ij = max (index (alpu, cj), index (alpl, cj))
                ktyp(ix) = impl(ij)
                ita(ii) = ktyp(ix)
              endif
              kdec(ix) = 1
              kdim(ix) = -1
              kt = ktyp(ix)
              write (lu, 5) nam(1:l1), ctp(kt)
 5            format ('CMP*'/'CMP*  Undimensioned variable assumed to',
     $          ' be an external function.'/'CMP*  Name: ',a,4x,
     $          'Type: ',a/'CMP*')
              goto 170
            else
              call errmes (41, 0)
              write (6, 6)
 6            format ('MP variables may not be used in statement',
     $          ' function definitions.'/'Define an external function',
     $          ' for this purpose.')
              call abrt
            endif

c   Otherwise it must be an ordinary array with subscript.  The combination of
c   the variable name and the subscript will now be treated as a variable.
c   IX is set to 0 as a flag indicating that this has been done.

          else
            i4 = iscan (i3, i2, linb)
            if (i4 .ne. 0) then
              call errmes (42, 0)
              write (6, 7) nam
 7            format ('The MP subscript on this variable is not',
     $          ' allowed: ',a)
              call abrt
            endif
            l1 = i2 - i1 + 1
            id2(ii) = i2
            lna(ii) = l1
            ar(ii)(1:l1) = linb(i1:i2)
            ix = 0
            goto 160
          endif
        else

c   The variable does not have a subscript.  Check if it has a dimension.

          if (ix .gt. 0) then
            if (kdim(ix) .gt. 0) then
              call errmes (43, 0)
              write (6, 8) nam(1:l1)
 8            format ('This dimensioned variable is used without a',
     $          ' subscript: ',a)
              call abrt
            endif
          endif
        endif
        i1 = i3

c   Identify the operator.

        do 180 i = 1, nop
          l1 = lop(i) - 1
          if (linb(i1:i1+l1) .eq. lopr(i) .or. linb(i1:i1+l1) .eq.
     $      uopr(i)) goto 190
 180    continue

        call errmes (44, 0)
        write (6, 9) linb(i1:i1)
 9      format ('Illegal operator: ',a)
        call abrt

 190    iop(ii) = i
        i1 = i1 + lop(i)
 200  continue

c   Compare the precedence levels of the two operators.

      if (kop(iop(1)) .lt. kop(iop(2))) then
        i1 = id1(2)
        goto 110
      endif

c   An operation can be performed.

210   call gen (lu, ita, lna, ar, iop(1), itpy, ly, argy)

c   Replace the two operands and the operator with the result in LINB.

 220  if (ly .ne. 0) then
        i1 = id1(1)
        i2 = id2(2)
        l1 = i2 - i1 + 1
        ld = ly - l1
        if (i1 .gt. 0) linc(1:i1-1) = linb(1:i1-1)
        linc(i1:i1+ly-1) = argy(1:ly)
        if (lb .gt. i2) linc(i1+ly:lb+ld) = linb(i2+1:lb)
        lb = lb + ld
        linb(1:lb) = linc(1:lb)
        goto 100
      else
        itpx = itpy
        lx = ly
        argx = argy
      endif

c   Finished at last.

 230  continue
c*
c      write (lu, *) 'exit express, argx = %'//argx(1:lx)//'%'
c      call flush (lu)
c*
      return
      end
      subroutine expres2 (lu, la, lina, itpx, lx, argx)

c   This processes the arithmetic and/or logical expression in LINA, of length
c   = LA.  The result, after evaluation, is placed in ARGX, with type = ITPX,
c   and length = LX.  If the last result was =, then the ARGX is set to blanks,
c   and ITPX and LX are set to zero.  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb, linc
      character*80 ar(2), arg(mar), arg1, argx, argy
      character*16 nam
      dimension itar(mar), lar(mar), ita(2), lna(2), id1(2), id2(2),
     $  iop(2)
      character*1 cj
c*
c      write (lu, *) 'enter expres'
c      call flush (lu)
c*
c   Search for an executable operation (one that is not dependent on the
c   results of operations with higher precedence) in the statement.  The two
c   arguments of the operation, their types and lengths, are identified.

      lb = la
      linb(1:lb) = lina(1:la)

 100  i1 = 1
c*
c      write (lu, *) 'expres lb =', lb
c      write (lu, *) '%'//linb(1:lb)//'%'
c      call flush (lu)
c*
 110  do 200 ii = 1, 2
        i1 = nblk (i1, lb, linb)
        if (i1 .eq. 0) call errmes (29, 1)
        cj = linb(i1:i1)
        ix = 0
c*
c        write (lu, *) 'cj =', cj
c        call flush (lu)
c*
c   Check if this is the start of a numeric constant.

        is1 = index (dig, cj)
        if ((cj .eq. '-' .or. cj .eq. '.') .and. i1 .lt. lb) then
          i2 = nblk (i1 + 1, lb, linb)
          is2 = index (dig, linb(i2:i2))
        else
          is2 = 0
        endif

        if (is1 .ne. 0 .or. is2 .ne. 0) then
          it = numcon (i1, i2, lb, linb)

c   If the mixed mode safe flag is on, then the numeric constant is always
c   of type MP.

          if (imm .ge. 1) then
            if (it .eq. 1) then
              it = 8
            elseif (it .eq. 2 .or. it .eq. 3) then
              it = 9
            endif
          endif
          if (it .le. 3) then
            id1(ii) = i1
            id2(ii) = i2
            ita(ii) = it
            lna(ii) = i2 - i1 + 1
            ar(ii) = linb(i1:i2)
          else
            id1(1) = i1
            id2(2) = i2
            itpy = it
            call gencon (lu, i1, i2, linb, itpy, ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220
          endif

c   Check if string is a variable name.

        elseif (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then

          do 120 j = i1, lb
            cj = linb(j:j)
            if (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then
              goto 120
            elseif (index (dig, cj) .ne. 0 .or. cj .eq. '_') then
              goto 120
            else
              goto 130
            endif
 120      continue

          j = lb + 1

c   Variable or function name has been identified.

 130      i2 = j - 1
          l1 = i2 - i1 + 1
          nam = linb(i1:i2)
          ix = itab (1, 0, nam)
          if (ix .eq. 0) then
            call errmes (30, 0)
            write (6, 1) nam
 1          format ('This Fortran keyword may not appear in an',
     $        ' expression: ',a)
            call abrt
          elseif (ktyp(ix) .eq. 0 .and. kdec(ix) .eq. 0) then
            call errmes (31, 0)
            write (6, 2) nam
 2          format ('This variable has not been typed: ',a)
            call abrt
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = ktyp(ix)
          lna(ii) = l1
          ar(ii)(1:l1) = linb(i1:i2)

c   Check if string is a logical constant.

        elseif (cj .eq. '.' .and. i1 .lt. lb .and.
     $      (linb(i1+1:i1+1) .eq. 'T' .or. linb(i1+1:i1+1) .eq. 't' .or.
     $      linb(i1+1:i1+1) .eq. 'F' .or. linb(i1+1:i1+1) .eq. 'f'))
     $      then
          if (linb(i1:i1+2) .eq. '.T.' .or. linb(i1:i1+2) .eq. '.t.'.or.
     $      linb(i1:i1+2) .eq. '.F.' .or. linb(i1:i1+2) .eq. '.f.')
     $      then
            i2 = i1 + 2
          elseif (linb(i1:i1+5) .eq. '.TRUE.' .or.
     $        linb(i1:i1+5) .eq. '.true.') then
            i2 = i1 + 5
          elseif (linb(i1:i1+6) .eq. '.FALSE.' .or.
     $        linb(i1:i1+6) .eq. '.false.') then
            i2 = i1 + 6
          else
            call errmes (32, 1)
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 7
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by ".

        elseif (cj .eq. '"') then
          j1 = i1
 140      i2 = indx (j1 + 1, lb, '"', linb)
          if (i2 .eq. 0) call errmes (33, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '"') then
            j1 = i2 + 1
            goto 140
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by '.

        elseif (cj .eq. '''') then
          j1 = i1
 150      i2 = indx (j1 + 1, lb, '''', linb)
          if (i2 .eq. 0) call errmes (34, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '''') then
            j1 = i2 + 1
            goto 150
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a unary minus sign (i.e. argument 1 is null).

        elseif (cj .eq. '-') then
          if (ii .eq. 2) goto 110
          id1(1) = i1
          id2(1) = i1
          ita(1) = 0
          lna(1) = 1
          ar(1) = ' '
          iop(1) = 3
          i1 = i1 + 1
          goto 200

c   Check if argument is a logical not operation (i.e. argument 1 is null).

        elseif (cj .eq. '.' .and. i1 + 4 .le. lb) then
          if (linb(i1:i1+4) .eq. '.NOT.' .or. linb(i1:i1+4) .eq.
     $      '.not.') then
            if (ii .eq. 2) goto 110
            id1(1) = i1
            id2(1) = i1
            ita(1) = 0
            lna(1) = 1
            ar(1) = ' '
            iop(1) = 15
            i1 = i1 + 5
            goto 200
          endif

c   Check if the next character is a left parenthesis.  If so, evaluate the
c   expression in parentheses.

        elseif (cj .eq. '(') then
          i2 = match (0, i1 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (35, 1)
          lc = i2 - i1 - 1
          linc(1:lc) = linb(i1+1:i2-1)
          id1(1) = i1
          id2(2) = i2
          if (lb .eq. 0) call errmes (36, 1)
          call expres3 (lu, lc, linc, itpy, ly, argy)
          goto 220
        else
          call errmes (37, 0)
          write (6, 3) cj
 3        format ('Illegal character: ',a)
          call abrt
        endif

c   Check if the end of the variable or constant is the end of the statement.

 160    continue
        if (i2 .eq. lb .or. linb(i2+1:lb) .eq. ' ') then

c   If this occurs on the first pass, we are done.

          if (ii .eq. 1) then
            itpx = ita(1)
            lx = lna(1)
            argx(1:lx) = ar(1)(1:lx)
            goto 230

c   If this occurs on the second pass, proceed to evaluate.

          else
            goto 210
          endif
        endif

c   Check if the next character after the variable name is a left parenthesis.

        i3 = nblk (i2 + 1, lb, linb)
        cj = linb(i3:i3)
        if (cj .eq. '(') then
          if (ix .eq. 0) call errmes (38, 1)
          i2 = match (0, i3 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (39, 1)
          k1 = index (linb(i2:lb), '=')
 170      continue

c   Check if the subscripted variable is really a function reference.

          if (kdim(ix) .eq. -1) then
            if (k1 .ne. 0) then
              call errmes (40, 0)
              write (6, 4) nam(1:l1)
 4            format ('A function name may not appear on the LHS of',
     $          ' an assignment statement: ',a)
              call abrt
            endif

c   Generate a function call.  If it is an intrinsic reference and the mixed
c   mode SAFE option is in effect, set IMM = 2 as a flag to inform ARLIST not
c   to revert to mixed mode FAST while evaluating the argument list.

            id1(1) = i1
            id2(2) = i2
            lc = i2 - i3 - 1
            linc(1:lc) = linb(i3+1:i2-1)
            ims = imm
            if (ix .le. mint .and. imm .ge. 1) imm = 2
            call arlist3 (lu, lc, linc, nar, itar, lar, arg)
            imm = ims
            it1 = ita(ii)
            ln1 = lna(ii)
            arg1 = ar(ii)
            call genfun (lu, it1, ln1, arg1, nar, itar, lar, arg, itpy,
     $        ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220

c   Check if the subscripted variable has a dimension.  If not, this is
c   assumed to be an external function reference.

          elseif (kdim(ix) .eq. 0) then
            if (k1 .eq. 0) then

c   If this function name was earlier listed in an external statement and was
c   not explicitly typed, it must now be implicitly typed.

              if (kdec(ix) .eq. 1 .and. mplc(ix) .eq. 1) then
                cj = nam(1:1)
                ij = max (index (alpu, cj), index (alpl, cj))
                ktyp(ix) = impl(ij)
                ita(ii) = ktyp(ix)
              endif
              kdec(ix) = 1
              kdim(ix) = -1
              kt = ktyp(ix)
              write (lu, 5) nam(1:l1), ctp(kt)
 5            format ('CMP*'/'CMP*  Undimensioned variable assumed to',
     $          ' be an external function.'/'CMP*  Name: ',a,4x,
     $          'Type: ',a/'CMP*')
              goto 170
            else
              call errmes (41, 0)
              write (6, 6)
 6            format ('MP variables may not be used in statement',
     $          ' function definitions.'/'Define an external function',
     $          ' for this purpose.')
              call abrt
            endif

c   Otherwise it must be an ordinary array with subscript.  The combination of
c   the variable name and the subscript will now be treated as a variable.
c   IX is set to 0 as a flag indicating that this has been done.

          else
            i4 = iscan (i3, i2, linb)
            if (i4 .ne. 0) then
              call errmes (42, 0)
              write (6, 7) nam
 7            format ('The MP subscript on this variable is not',
     $          ' allowed: ',a)
              call abrt
            endif
            l1 = i2 - i1 + 1
            id2(ii) = i2
            lna(ii) = l1
            ar(ii)(1:l1) = linb(i1:i2)
            ix = 0
            goto 160
          endif
        else

c   The variable does not have a subscript.  Check if it has a dimension.

          if (ix .gt. 0) then
            if (kdim(ix) .gt. 0) then
              call errmes (43, 0)
              write (6, 8) nam(1:l1)
 8            format ('This dimensioned variable is used without a',
     $          ' subscript: ',a)
              call abrt
            endif
          endif
        endif
        i1 = i3

c   Identify the operator.

        do 180 i = 1, nop
          l1 = lop(i) - 1
          if (linb(i1:i1+l1) .eq. lopr(i) .or. linb(i1:i1+l1) .eq.
     $      uopr(i)) goto 190
 180    continue

        call errmes (44, 0)
        write (6, 9) linb(i1:i1)
 9      format ('Illegal operator: ',a)
        call abrt

 190    iop(ii) = i
        i1 = i1 + lop(i)
 200  continue

c   Compare the precedence levels of the two operators.

      if (kop(iop(1)) .lt. kop(iop(2))) then
        i1 = id1(2)
        goto 110
      endif

c   An operation can be performed.

210   call gen (lu, ita, lna, ar, iop(1), itpy, ly, argy)

c   Replace the two operands and the operator with the result in LINB.

 220  if (ly .ne. 0) then
        i1 = id1(1)
        i2 = id2(2)
        l1 = i2 - i1 + 1
        ld = ly - l1
        if (i1 .gt. 0) linc(1:i1-1) = linb(1:i1-1)
        linc(i1:i1+ly-1) = argy(1:ly)
        if (lb .gt. i2) linc(i1+ly:lb+ld) = linb(i2+1:lb)
        lb = lb + ld
        linb(1:lb) = linc(1:lb)
        goto 100
      else
        itpx = itpy
        lx = ly
        argx = argy
      endif

c   Finished at last.

 230  continue
c*
c      write (lu, *) 'exit express, argx = %'//argx(1:lx)//'%'
c      call flush (lu)
c*
      return
      end
      subroutine expres3 (lu, la, lina, itpx, lx, argx)

c   This processes the arithmetic and/or logical expression in LINA, of length
c   = LA.  The result, after evaluation, is placed in ARGX, with type = ITPX,
c   and length = LX.  If the last result was =, then the ARGX is set to blanks,
c   and ITPX and LX are set to zero.  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb, linc
      character*80 ar(2), arg(mar), arg1, argx, argy
      character*16 nam
      dimension itar(mar), lar(mar), ita(2), lna(2), id1(2), id2(2),
     $  iop(2)
      character*1 cj
c*
c      write (lu, *) 'enter expres'
c      call flush (lu)
c*
c   Search for an executable operation (one that is not dependent on the
c   results of operations with higher precedence) in the statement.  The two
c   arguments of the operation, their types and lengths, are identified.

      lb = la
      linb(1:lb) = lina(1:la)

 100  i1 = 1
c*
c      write (lu, *) 'expres lb =', lb
c      write (lu, *) '%'//linb(1:lb)//'%'
c      call flush (lu)
c*
 110  do 200 ii = 1, 2
        i1 = nblk (i1, lb, linb)
        if (i1 .eq. 0) call errmes (29, 1)
        cj = linb(i1:i1)
        ix = 0
c*
c        write (lu, *) 'cj =', cj
c        call flush (lu)
c*
c   Check if this is the start of a numeric constant.

        is1 = index (dig, cj)
        if ((cj .eq. '-' .or. cj .eq. '.') .and. i1 .lt. lb) then
          i2 = nblk (i1 + 1, lb, linb)
          is2 = index (dig, linb(i2:i2))
        else
          is2 = 0
        endif

        if (is1 .ne. 0 .or. is2 .ne. 0) then
          it = numcon (i1, i2, lb, linb)

c   If the mixed mode safe flag is on, then the numeric constant is always
c   of type MP.

          if (imm .ge. 1) then
            if (it .eq. 1) then
              it = 8
            elseif (it .eq. 2 .or. it .eq. 3) then
              it = 9
            endif
          endif
          if (it .le. 3) then
            id1(ii) = i1
            id2(ii) = i2
            ita(ii) = it
            lna(ii) = i2 - i1 + 1
            ar(ii) = linb(i1:i2)
          else
            id1(1) = i1
            id2(2) = i2
            itpy = it
            call gencon (lu, i1, i2, linb, itpy, ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220
          endif

c   Check if string is a variable name.

        elseif (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then

          do 120 j = i1, lb
            cj = linb(j:j)
            if (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then
              goto 120
            elseif (index (dig, cj) .ne. 0 .or. cj .eq. '_') then
              goto 120
            else
              goto 130
            endif
 120      continue

          j = lb + 1

c   Variable or function name has been identified.

 130      i2 = j - 1
          l1 = i2 - i1 + 1
          nam = linb(i1:i2)
          ix = itab (1, 0, nam)
          if (ix .eq. 0) then
            call errmes (30, 0)
            write (6, 1) nam
 1          format ('This Fortran keyword may not appear in an',
     $        ' expression: ',a)
            call abrt
          elseif (ktyp(ix) .eq. 0 .and. kdec(ix) .eq. 0) then
            call errmes (31, 0)
            write (6, 2) nam
 2          format ('This variable has not been typed: ',a)
            call abrt
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = ktyp(ix)
          lna(ii) = l1
          ar(ii)(1:l1) = linb(i1:i2)

c   Check if string is a logical constant.

        elseif (cj .eq. '.' .and. i1 .lt. lb .and.
     $      (linb(i1+1:i1+1) .eq. 'T' .or. linb(i1+1:i1+1) .eq. 't' .or.
     $      linb(i1+1:i1+1) .eq. 'F' .or. linb(i1+1:i1+1) .eq. 'f'))
     $      then
          if (linb(i1:i1+2) .eq. '.T.' .or. linb(i1:i1+2) .eq. '.t.'.or.
     $      linb(i1:i1+2) .eq. '.F.' .or. linb(i1:i1+2) .eq. '.f.')
     $      then
            i2 = i1 + 2
          elseif (linb(i1:i1+5) .eq. '.TRUE.' .or.
     $        linb(i1:i1+5) .eq. '.true.') then
            i2 = i1 + 5
          elseif (linb(i1:i1+6) .eq. '.FALSE.' .or.
     $        linb(i1:i1+6) .eq. '.false.') then
            i2 = i1 + 6
          else
            call errmes (32, 1)
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 7
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by ".

        elseif (cj .eq. '"') then
          j1 = i1
 140      i2 = indx (j1 + 1, lb, '"', linb)
          if (i2 .eq. 0) call errmes (33, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '"') then
            j1 = i2 + 1
            goto 140
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by '.

        elseif (cj .eq. '''') then
          j1 = i1
 150      i2 = indx (j1 + 1, lb, '''', linb)
          if (i2 .eq. 0) call errmes (34, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '''') then
            j1 = i2 + 1
            goto 150
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a unary minus sign (i.e. argument 1 is null).

        elseif (cj .eq. '-') then
          if (ii .eq. 2) goto 110
          id1(1) = i1
          id2(1) = i1
          ita(1) = 0
          lna(1) = 1
          ar(1) = ' '
          iop(1) = 3
          i1 = i1 + 1
          goto 200

c   Check if argument is a logical not operation (i.e. argument 1 is null).

        elseif (cj .eq. '.' .and. i1 + 4 .le. lb) then
          if (linb(i1:i1+4) .eq. '.NOT.' .or. linb(i1:i1+4) .eq.
     $      '.not.') then
            if (ii .eq. 2) goto 110
            id1(1) = i1
            id2(1) = i1
            ita(1) = 0
            lna(1) = 1
            ar(1) = ' '
            iop(1) = 15
            i1 = i1 + 5
            goto 200
          endif

c   Check if the next character is a left parenthesis.  If so, evaluate the
c   expression in parentheses.

        elseif (cj .eq. '(') then
          i2 = match (0, i1 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (35, 1)
          lc = i2 - i1 - 1
          linc(1:lc) = linb(i1+1:i2-1)
          id1(1) = i1
          id2(2) = i2
          if (lb .eq. 0) call errmes (36, 1)
          call expres4 (lu, lc, linc, itpy, ly, argy)
          goto 220
        else
          call errmes (37, 0)
          write (6, 3) cj
 3        format ('Illegal character: ',a)
          call abrt
        endif

c   Check if the end of the variable or constant is the end of the statement.

 160    continue
        if (i2 .eq. lb .or. linb(i2+1:lb) .eq. ' ') then

c   If this occurs on the first pass, we are done.

          if (ii .eq. 1) then
            itpx = ita(1)
            lx = lna(1)
            argx(1:lx) = ar(1)(1:lx)
            goto 230

c   If this occurs on the second pass, proceed to evaluate.

          else
            goto 210
          endif
        endif

c   Check if the next character after the variable name is a left parenthesis.

        i3 = nblk (i2 + 1, lb, linb)
        cj = linb(i3:i3)
        if (cj .eq. '(') then
          if (ix .eq. 0) call errmes (38, 1)
          i2 = match (0, i3 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (39, 1)
          k1 = index (linb(i2:lb), '=')
 170      continue

c   Check if the subscripted variable is really a function reference.

          if (kdim(ix) .eq. -1) then
            if (k1 .ne. 0) then
              call errmes (40, 0)
              write (6, 4) nam(1:l1)
 4            format ('A function name may not appear on the LHS of',
     $          ' an assignment statement: ',a)
              call abrt
            endif

c   Generate a function call.  If it is an intrinsic reference and the mixed
c   mode SAFE option is in effect, set IMM = 2 as a flag to inform ARLIST not
c   to revert to mixed mode FAST while evaluating the argument list.

            id1(1) = i1
            id2(2) = i2
            lc = i2 - i3 - 1
            linc(1:lc) = linb(i3+1:i2-1)
            ims = imm
            if (ix .le. mint .and. imm .ge. 1) imm = 2
            call arlist4 (lu, lc, linc, nar, itar, lar, arg)
            imm = ims
            it1 = ita(ii)
            ln1 = lna(ii)
            arg1 = ar(ii)
            call genfun (lu, it1, ln1, arg1, nar, itar, lar, arg, itpy,
     $        ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220

c   Check if the subscripted variable has a dimension.  If not, this is
c   assumed to be an external function reference.

          elseif (kdim(ix) .eq. 0) then
            if (k1 .eq. 0) then

c   If this function name was earlier listed in an external statement and was
c   not explicitly typed, it must now be implicitly typed.

              if (kdec(ix) .eq. 1 .and. mplc(ix) .eq. 1) then
                cj = nam(1:1)
                ij = max (index (alpu, cj), index (alpl, cj))
                ktyp(ix) = impl(ij)
                ita(ii) = ktyp(ix)
              endif
              kdec(ix) = 1
              kdim(ix) = -1
              kt = ktyp(ix)
              write (lu, 5) nam(1:l1), ctp(kt)
 5            format ('CMP*'/'CMP*  Undimensioned variable assumed to',
     $          ' be an external function.'/'CMP*  Name: ',a,4x,
     $          'Type: ',a/'CMP*')
              goto 170
            else
              call errmes (41, 0)
              write (6, 6)
 6            format ('MP variables may not be used in statement',
     $          ' function definitions.'/'Define an external function',
     $          ' for this purpose.')
              call abrt
            endif

c   Otherwise it must be an ordinary array with subscript.  The combination of
c   the variable name and the subscript will now be treated as a variable.
c   IX is set to 0 as a flag indicating that this has been done.

          else
            i4 = iscan (i3, i2, linb)
            if (i4 .ne. 0) then
              call errmes (42, 0)
              write (6, 7) nam
 7            format ('The MP subscript on this variable is not',
     $          ' allowed: ',a)
              call abrt
            endif
            l1 = i2 - i1 + 1
            id2(ii) = i2
            lna(ii) = l1
            ar(ii)(1:l1) = linb(i1:i2)
            ix = 0
            goto 160
          endif
        else

c   The variable does not have a subscript.  Check if it has a dimension.

          if (ix .gt. 0) then
            if (kdim(ix) .gt. 0) then
              call errmes (43, 0)
              write (6, 8) nam(1:l1)
 8            format ('This dimensioned variable is used without a',
     $          ' subscript: ',a)
              call abrt
            endif
          endif
        endif
        i1 = i3

c   Identify the operator.

        do 180 i = 1, nop
          l1 = lop(i) - 1
          if (linb(i1:i1+l1) .eq. lopr(i) .or. linb(i1:i1+l1) .eq.
     $      uopr(i)) goto 190
 180    continue

        call errmes (44, 0)
        write (6, 9) linb(i1:i1)
 9      format ('Illegal operator: ',a)
        call abrt

 190    iop(ii) = i
        i1 = i1 + lop(i)
 200  continue

c   Compare the precedence levels of the two operators.

      if (kop(iop(1)) .lt. kop(iop(2))) then
        i1 = id1(2)
        goto 110
      endif

c   An operation can be performed.

210   call gen (lu, ita, lna, ar, iop(1), itpy, ly, argy)

c   Replace the two operands and the operator with the result in LINB.

 220  if (ly .ne. 0) then
        i1 = id1(1)
        i2 = id2(2)
        l1 = i2 - i1 + 1
        ld = ly - l1
        if (i1 .gt. 0) linc(1:i1-1) = linb(1:i1-1)
        linc(i1:i1+ly-1) = argy(1:ly)
        if (lb .gt. i2) linc(i1+ly:lb+ld) = linb(i2+1:lb)
        lb = lb + ld
        linb(1:lb) = linc(1:lb)
        goto 100
      else
        itpx = itpy
        lx = ly
        argx = argy
      endif

c   Finished at last.

 230  continue
c*
c      write (lu, *) 'exit express, argx = %'//argx(1:lx)//'%'
c      call flush (lu)
c*
      return
      end
      subroutine expres4 (lu, la, lina, itpx, lx, argx)

c   This processes the arithmetic and/or logical expression in LINA, of length
c   = LA.  The result, after evaluation, is placed in ARGX, with type = ITPX,
c   and length = LX.  If the last result was =, then the ARGX is set to blanks,
c   and ITPX and LX are set to zero.  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb, linc
      character*80 ar(2), arg(mar), arg1, argx, argy
      character*16 nam
      dimension itar(mar), lar(mar), ita(2), lna(2), id1(2), id2(2),
     $  iop(2)
      character*1 cj
c*
c      write (lu, *) 'enter expres'
c      call flush (lu)
c*
c   Search for an executable operation (one that is not dependent on the
c   results of operations with higher precedence) in the statement.  The two
c   arguments of the operation, their types and lengths, are identified.

      lb = la
      linb(1:lb) = lina(1:la)

 100  i1 = 1
c*
c      write (lu, *) 'expres lb =', lb
c      write (lu, *) '%'//linb(1:lb)//'%'
c      call flush (lu)
c*
 110  do 200 ii = 1, 2
        i1 = nblk (i1, lb, linb)
        if (i1 .eq. 0) call errmes (29, 1)
        cj = linb(i1:i1)
        ix = 0
c*
c        write (lu, *) 'cj =', cj
c        call flush (lu)
c*
c   Check if this is the start of a numeric constant.

        is1 = index (dig, cj)
        if ((cj .eq. '-' .or. cj .eq. '.') .and. i1 .lt. lb) then
          i2 = nblk (i1 + 1, lb, linb)
          is2 = index (dig, linb(i2:i2))
        else
          is2 = 0
        endif

        if (is1 .ne. 0 .or. is2 .ne. 0) then
          it = numcon (i1, i2, lb, linb)

c   If the mixed mode safe flag is on, then the numeric constant is always
c   of type MP.

          if (imm .ge. 1) then
            if (it .eq. 1) then
              it = 8
            elseif (it .eq. 2 .or. it .eq. 3) then
              it = 9
            endif
          endif
          if (it .le. 3) then
            id1(ii) = i1
            id2(ii) = i2
            ita(ii) = it
            lna(ii) = i2 - i1 + 1
            ar(ii) = linb(i1:i2)
          else
            id1(1) = i1
            id2(2) = i2
            itpy = it
            call gencon (lu, i1, i2, linb, itpy, ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220
          endif

c   Check if string is a variable name.

        elseif (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then

          do 120 j = i1, lb
            cj = linb(j:j)
            if (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then
              goto 120
            elseif (index (dig, cj) .ne. 0 .or. cj .eq. '_') then
              goto 120
            else
              goto 130
            endif
 120      continue

          j = lb + 1

c   Variable or function name has been identified.

 130      i2 = j - 1
          l1 = i2 - i1 + 1
          nam = linb(i1:i2)
          ix = itab (1, 0, nam)
          if (ix .eq. 0) then
            call errmes (30, 0)
            write (6, 1) nam
 1          format ('This Fortran keyword may not appear in an',
     $        ' expression: ',a)
            call abrt
          elseif (ktyp(ix) .eq. 0 .and. kdec(ix) .eq. 0) then
            call errmes (31, 0)
            write (6, 2) nam
 2          format ('This variable has not been typed: ',a)
            call abrt
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = ktyp(ix)
          lna(ii) = l1
          ar(ii)(1:l1) = linb(i1:i2)

c   Check if string is a logical constant.

        elseif (cj .eq. '.' .and. i1 .lt. lb .and.
     $      (linb(i1+1:i1+1) .eq. 'T' .or. linb(i1+1:i1+1) .eq. 't' .or.
     $      linb(i1+1:i1+1) .eq. 'F' .or. linb(i1+1:i1+1) .eq. 'f'))
     $      then
          if (linb(i1:i1+2) .eq. '.T.' .or. linb(i1:i1+2) .eq. '.t.'.or.
     $      linb(i1:i1+2) .eq. '.F.' .or. linb(i1:i1+2) .eq. '.f.')
     $      then
            i2 = i1 + 2
          elseif (linb(i1:i1+5) .eq. '.TRUE.' .or.
     $        linb(i1:i1+5) .eq. '.true.') then
            i2 = i1 + 5
          elseif (linb(i1:i1+6) .eq. '.FALSE.' .or.
     $        linb(i1:i1+6) .eq. '.false.') then
            i2 = i1 + 6
          else
            call errmes (32, 1)
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 7
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by ".

        elseif (cj .eq. '"') then
          j1 = i1
 140      i2 = indx (j1 + 1, lb, '"', linb)
          if (i2 .eq. 0) call errmes (33, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '"') then
            j1 = i2 + 1
            goto 140
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by '.

        elseif (cj .eq. '''') then
          j1 = i1
 150      i2 = indx (j1 + 1, lb, '''', linb)
          if (i2 .eq. 0) call errmes (34, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '''') then
            j1 = i2 + 1
            goto 150
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a unary minus sign (i.e. argument 1 is null).

        elseif (cj .eq. '-') then
          if (ii .eq. 2) goto 110
          id1(1) = i1
          id2(1) = i1
          ita(1) = 0
          lna(1) = 1
          ar(1) = ' '
          iop(1) = 3
          i1 = i1 + 1
          goto 200

c   Check if argument is a logical not operation (i.e. argument 1 is null).

        elseif (cj .eq. '.' .and. i1 + 4 .le. lb) then
          if (linb(i1:i1+4) .eq. '.NOT.' .or. linb(i1:i1+4) .eq.
     $      '.not.') then
            if (ii .eq. 2) goto 110
            id1(1) = i1
            id2(1) = i1
            ita(1) = 0
            lna(1) = 1
            ar(1) = ' '
            iop(1) = 15
            i1 = i1 + 5
            goto 200
          endif

c   Check if the next character is a left parenthesis.  If so, evaluate the
c   expression in parentheses.

        elseif (cj .eq. '(') then
          i2 = match (0, i1 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (35, 1)
          lc = i2 - i1 - 1
          linc(1:lc) = linb(i1+1:i2-1)
          id1(1) = i1
          id2(2) = i2
          if (lb .eq. 0) call errmes (36, 1)
          call expres5 (lu, lc, linc, itpy, ly, argy)
          goto 220
        else
          call errmes (37, 0)
          write (6, 3) cj
 3        format ('Illegal character: ',a)
          call abrt
        endif

c   Check if the end of the variable or constant is the end of the statement.

 160    continue
        if (i2 .eq. lb .or. linb(i2+1:lb) .eq. ' ') then

c   If this occurs on the first pass, we are done.

          if (ii .eq. 1) then
            itpx = ita(1)
            lx = lna(1)
            argx(1:lx) = ar(1)(1:lx)
            goto 230

c   If this occurs on the second pass, proceed to evaluate.

          else
            goto 210
          endif
        endif

c   Check if the next character after the variable name is a left parenthesis.

        i3 = nblk (i2 + 1, lb, linb)
        cj = linb(i3:i3)
        if (cj .eq. '(') then
          if (ix .eq. 0) call errmes (38, 1)
          i2 = match (0, i3 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (39, 1)
          k1 = index (linb(i2:lb), '=')
 170      continue

c   Check if the subscripted variable is really a function reference.

          if (kdim(ix) .eq. -1) then
            if (k1 .ne. 0) then
              call errmes (40, 0)
              write (6, 4) nam(1:l1)
 4            format ('A function name may not appear on the LHS of',
     $          ' an assignment statement: ',a)
              call abrt
            endif

c   Generate a function call.  If it is an intrinsic reference and the mixed
c   mode SAFE option is in effect, set IMM = 2 as a flag to inform ARLIST not
c   to revert to mixed mode FAST while evaluating the argument list.

            id1(1) = i1
            id2(2) = i2
            lc = i2 - i3 - 1
            linc(1:lc) = linb(i3+1:i2-1)
            ims = imm
            if (ix .le. mint .and. imm .ge. 1) imm = 2
            call arlist5 (lu, lc, linc, nar, itar, lar, arg)
            imm = ims
            it1 = ita(ii)
            ln1 = lna(ii)
            arg1 = ar(ii)
            call genfun (lu, it1, ln1, arg1, nar, itar, lar, arg, itpy,
     $        ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220

c   Check if the subscripted variable has a dimension.  If not, this is
c   assumed to be an external function reference.

          elseif (kdim(ix) .eq. 0) then
            if (k1 .eq. 0) then

c   If this function name was earlier listed in an external statement and was
c   not explicitly typed, it must now be implicitly typed.

              if (kdec(ix) .eq. 1 .and. mplc(ix) .eq. 1) then
                cj = nam(1:1)
                ij = max (index (alpu, cj), index (alpl, cj))
                ktyp(ix) = impl(ij)
                ita(ii) = ktyp(ix)
              endif
              kdec(ix) = 1
              kdim(ix) = -1
              kt = ktyp(ix)
              write (lu, 5) nam(1:l1), ctp(kt)
 5            format ('CMP*'/'CMP*  Undimensioned variable assumed to',
     $          ' be an external function.'/'CMP*  Name: ',a,4x,
     $          'Type: ',a/'CMP*')
              goto 170
            else
              call errmes (41, 0)
              write (6, 6)
 6            format ('MP variables may not be used in statement',
     $          ' function definitions.'/'Define an external function',
     $          ' for this purpose.')
              call abrt
            endif

c   Otherwise it must be an ordinary array with subscript.  The combination of
c   the variable name and the subscript will now be treated as a variable.
c   IX is set to 0 as a flag indicating that this has been done.

          else
            i4 = iscan (i3, i2, linb)
            if (i4 .ne. 0) then
              call errmes (42, 0)
              write (6, 7) nam
 7            format ('The MP subscript on this variable is not',
     $          ' allowed: ',a)
              call abrt
            endif
            l1 = i2 - i1 + 1
            id2(ii) = i2
            lna(ii) = l1
            ar(ii)(1:l1) = linb(i1:i2)
            ix = 0
            goto 160
          endif
        else

c   The variable does not have a subscript.  Check if it has a dimension.

          if (ix .gt. 0) then
            if (kdim(ix) .gt. 0) then
              call errmes (43, 0)
              write (6, 8) nam(1:l1)
 8            format ('This dimensioned variable is used without a',
     $          ' subscript: ',a)
              call abrt
            endif
          endif
        endif
        i1 = i3

c   Identify the operator.

        do 180 i = 1, nop
          l1 = lop(i) - 1
          if (linb(i1:i1+l1) .eq. lopr(i) .or. linb(i1:i1+l1) .eq.
     $      uopr(i)) goto 190
 180    continue

        call errmes (44, 0)
        write (6, 9) linb(i1:i1)
 9      format ('Illegal operator: ',a)
        call abrt

 190    iop(ii) = i
        i1 = i1 + lop(i)
 200  continue

c   Compare the precedence levels of the two operators.

      if (kop(iop(1)) .lt. kop(iop(2))) then
        i1 = id1(2)
        goto 110
      endif

c   An operation can be performed.

210   call gen (lu, ita, lna, ar, iop(1), itpy, ly, argy)

c   Replace the two operands and the operator with the result in LINB.

 220  if (ly .ne. 0) then
        i1 = id1(1)
        i2 = id2(2)
        l1 = i2 - i1 + 1
        ld = ly - l1
        if (i1 .gt. 0) linc(1:i1-1) = linb(1:i1-1)
        linc(i1:i1+ly-1) = argy(1:ly)
        if (lb .gt. i2) linc(i1+ly:lb+ld) = linb(i2+1:lb)
        lb = lb + ld
        linb(1:lb) = linc(1:lb)
        goto 100
      else
        itpx = itpy
        lx = ly
        argx = argy
      endif

c   Finished at last.

 230  continue
c*
c      write (lu, *) 'exit express, argx = %'//argx(1:lx)//'%'
c      call flush (lu)
c*
      return
      end
      subroutine expres5 (lu, la, lina, itpx, lx, argx)

c   This processes the arithmetic and/or logical expression in LINA, of length
c   = LA.  The result, after evaluation, is placed in ARGX, with type = ITPX,
c   and length = LX.  If the last result was =, then the ARGX is set to blanks,
c   and ITPX and LX are set to zero.  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb, linc
      character*80 ar(2), arg(mar), arg1, argx, argy
      character*16 nam
      dimension itar(mar), lar(mar), ita(2), lna(2), id1(2), id2(2),
     $  iop(2)
      character*1 cj
c*
c      write (lu, *) 'enter expres'
c      call flush (lu)
c*
c   Search for an executable operation (one that is not dependent on the
c   results of operations with higher precedence) in the statement.  The two
c   arguments of the operation, their types and lengths, are identified.

      lb = la
      linb(1:lb) = lina(1:la)

 100  i1 = 1
c*
c      write (lu, *) 'expres lb =', lb
c      write (lu, *) '%'//linb(1:lb)//'%'
c      call flush (lu)
c*
 110  do 200 ii = 1, 2
        i1 = nblk (i1, lb, linb)
        if (i1 .eq. 0) call errmes (29, 1)
        cj = linb(i1:i1)
        ix = 0
c*
c        write (lu, *) 'cj =', cj
c        call flush (lu)
c*
c   Check if this is the start of a numeric constant.

        is1 = index (dig, cj)
        if ((cj .eq. '-' .or. cj .eq. '.') .and. i1 .lt. lb) then
          i2 = nblk (i1 + 1, lb, linb)
          is2 = index (dig, linb(i2:i2))
        else
          is2 = 0
        endif

        if (is1 .ne. 0 .or. is2 .ne. 0) then
          it = numcon (i1, i2, lb, linb)

c   If the mixed mode safe flag is on, then the numeric constant is always
c   of type MP.

          if (imm .ge. 1) then
            if (it .eq. 1) then
              it = 8
            elseif (it .eq. 2 .or. it .eq. 3) then
              it = 9
            endif
          endif
          if (it .le. 3) then
            id1(ii) = i1
            id2(ii) = i2
            ita(ii) = it
            lna(ii) = i2 - i1 + 1
            ar(ii) = linb(i1:i2)
          else
            id1(1) = i1
            id2(2) = i2
            itpy = it
            call gencon (lu, i1, i2, linb, itpy, ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220
          endif

c   Check if string is a variable name.

        elseif (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then

          do 120 j = i1, lb
            cj = linb(j:j)
            if (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then
              goto 120
            elseif (index (dig, cj) .ne. 0 .or. cj .eq. '_') then
              goto 120
            else
              goto 130
            endif
 120      continue

          j = lb + 1

c   Variable or function name has been identified.

 130      i2 = j - 1
          l1 = i2 - i1 + 1
          nam = linb(i1:i2)
          ix = itab (1, 0, nam)
          if (ix .eq. 0) then
            call errmes (30, 0)
            write (6, 1) nam
 1          format ('This Fortran keyword may not appear in an',
     $        ' expression: ',a)
            call abrt
          elseif (ktyp(ix) .eq. 0 .and. kdec(ix) .eq. 0) then
            call errmes (31, 0)
            write (6, 2) nam
 2          format ('This variable has not been typed: ',a)
            call abrt
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = ktyp(ix)
          lna(ii) = l1
          ar(ii)(1:l1) = linb(i1:i2)

c   Check if string is a logical constant.

        elseif (cj .eq. '.' .and. i1 .lt. lb .and.
     $      (linb(i1+1:i1+1) .eq. 'T' .or. linb(i1+1:i1+1) .eq. 't' .or.
     $      linb(i1+1:i1+1) .eq. 'F' .or. linb(i1+1:i1+1) .eq. 'f'))
     $      then
          if (linb(i1:i1+2) .eq. '.T.' .or. linb(i1:i1+2) .eq. '.t.'.or.
     $      linb(i1:i1+2) .eq. '.F.' .or. linb(i1:i1+2) .eq. '.f.')
     $      then
            i2 = i1 + 2
          elseif (linb(i1:i1+5) .eq. '.TRUE.' .or.
     $        linb(i1:i1+5) .eq. '.true.') then
            i2 = i1 + 5
          elseif (linb(i1:i1+6) .eq. '.FALSE.' .or.
     $        linb(i1:i1+6) .eq. '.false.') then
            i2 = i1 + 6
          else
            call errmes (32, 1)
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 7
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by ".

        elseif (cj .eq. '"') then
          j1 = i1
 140      i2 = indx (j1 + 1, lb, '"', linb)
          if (i2 .eq. 0) call errmes (33, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '"') then
            j1 = i2 + 1
            goto 140
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by '.

        elseif (cj .eq. '''') then
          j1 = i1
 150      i2 = indx (j1 + 1, lb, '''', linb)
          if (i2 .eq. 0) call errmes (34, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '''') then
            j1 = i2 + 1
            goto 150
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a unary minus sign (i.e. argument 1 is null).

        elseif (cj .eq. '-') then
          if (ii .eq. 2) goto 110
          id1(1) = i1
          id2(1) = i1
          ita(1) = 0
          lna(1) = 1
          ar(1) = ' '
          iop(1) = 3
          i1 = i1 + 1
          goto 200

c   Check if argument is a logical not operation (i.e. argument 1 is null).

        elseif (cj .eq. '.' .and. i1 + 4 .le. lb) then
          if (linb(i1:i1+4) .eq. '.NOT.' .or. linb(i1:i1+4) .eq.
     $      '.not.') then
            if (ii .eq. 2) goto 110
            id1(1) = i1
            id2(1) = i1
            ita(1) = 0
            lna(1) = 1
            ar(1) = ' '
            iop(1) = 15
            i1 = i1 + 5
            goto 200
          endif

c   Check if the next character is a left parenthesis.  If so, evaluate the
c   expression in parentheses.

        elseif (cj .eq. '(') then
          i2 = match (0, i1 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (35, 1)
          lc = i2 - i1 - 1
          linc(1:lc) = linb(i1+1:i2-1)
          id1(1) = i1
          id2(2) = i2
          if (lb .eq. 0) call errmes (36, 1)
          call expres6 (lu, lc, linc, itpy, ly, argy)
          goto 220
        else
          call errmes (37, 0)
          write (6, 3) cj
 3        format ('Illegal character: ',a)
          call abrt
        endif

c   Check if the end of the variable or constant is the end of the statement.

 160    continue
        if (i2 .eq. lb .or. linb(i2+1:lb) .eq. ' ') then

c   If this occurs on the first pass, we are done.

          if (ii .eq. 1) then
            itpx = ita(1)
            lx = lna(1)
            argx(1:lx) = ar(1)(1:lx)
            goto 230

c   If this occurs on the second pass, proceed to evaluate.

          else
            goto 210
          endif
        endif

c   Check if the next character after the variable name is a left parenthesis.

        i3 = nblk (i2 + 1, lb, linb)
        cj = linb(i3:i3)
        if (cj .eq. '(') then
          if (ix .eq. 0) call errmes (38, 1)
          i2 = match (0, i3 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (39, 1)
          k1 = index (linb(i2:lb), '=')
 170      continue

c   Check if the subscripted variable is really a function reference.

          if (kdim(ix) .eq. -1) then
            if (k1 .ne. 0) then
              call errmes (40, 0)
              write (6, 4) nam(1:l1)
 4            format ('A function name may not appear on the LHS of',
     $          ' an assignment statement: ',a)
              call abrt
            endif

c   Generate a function call.  If it is an intrinsic reference and the mixed
c   mode SAFE option is in effect, set IMM = 2 as a flag to inform ARLIST not
c   to revert to mixed mode FAST while evaluating the argument list.

            id1(1) = i1
            id2(2) = i2
            lc = i2 - i3 - 1
            linc(1:lc) = linb(i3+1:i2-1)
            ims = imm
            if (ix .le. mint .and. imm .ge. 1) imm = 2
            call arlist6 (lu, lc, linc, nar, itar, lar, arg)
            imm = ims
            it1 = ita(ii)
            ln1 = lna(ii)
            arg1 = ar(ii)
            call genfun (lu, it1, ln1, arg1, nar, itar, lar, arg, itpy,
     $        ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220

c   Check if the subscripted variable has a dimension.  If not, this is
c   assumed to be an external function reference.

          elseif (kdim(ix) .eq. 0) then
            if (k1 .eq. 0) then

c   If this function name was earlier listed in an external statement and was
c   not explicitly typed, it must now be implicitly typed.

              if (kdec(ix) .eq. 1 .and. mplc(ix) .eq. 1) then
                cj = nam(1:1)
                ij = max (index (alpu, cj), index (alpl, cj))
                ktyp(ix) = impl(ij)
                ita(ii) = ktyp(ix)
              endif
              kdec(ix) = 1
              kdim(ix) = -1
              kt = ktyp(ix)
              write (lu, 5) nam(1:l1), ctp(kt)
 5            format ('CMP*'/'CMP*  Undimensioned variable assumed to',
     $          ' be an external function.'/'CMP*  Name: ',a,4x,
     $          'Type: ',a/'CMP*')
              goto 170
            else
              call errmes (41, 0)
              write (6, 6)
 6            format ('MP variables may not be used in statement',
     $          ' function definitions.'/'Define an external function',
     $          ' for this purpose.')
              call abrt
            endif

c   Otherwise it must be an ordinary array with subscript.  The combination of
c   the variable name and the subscript will now be treated as a variable.
c   IX is set to 0 as a flag indicating that this has been done.

          else
            i4 = iscan (i3, i2, linb)
            if (i4 .ne. 0) then
              call errmes (42, 0)
              write (6, 7) nam
 7            format ('The MP subscript on this variable is not',
     $          ' allowed: ',a)
              call abrt
            endif
            l1 = i2 - i1 + 1
            id2(ii) = i2
            lna(ii) = l1
            ar(ii)(1:l1) = linb(i1:i2)
            ix = 0
            goto 160
          endif
        else

c   The variable does not have a subscript.  Check if it has a dimension.

          if (ix .gt. 0) then
            if (kdim(ix) .gt. 0) then
              call errmes (43, 0)
              write (6, 8) nam(1:l1)
 8            format ('This dimensioned variable is used without a',
     $          ' subscript: ',a)
              call abrt
            endif
          endif
        endif
        i1 = i3

c   Identify the operator.

        do 180 i = 1, nop
          l1 = lop(i) - 1
          if (linb(i1:i1+l1) .eq. lopr(i) .or. linb(i1:i1+l1) .eq.
     $      uopr(i)) goto 190
 180    continue

        call errmes (44, 0)
        write (6, 9) linb(i1:i1)
 9      format ('Illegal operator: ',a)
        call abrt

 190    iop(ii) = i
        i1 = i1 + lop(i)
 200  continue

c   Compare the precedence levels of the two operators.

      if (kop(iop(1)) .lt. kop(iop(2))) then
        i1 = id1(2)
        goto 110
      endif

c   An operation can be performed.

210   call gen (lu, ita, lna, ar, iop(1), itpy, ly, argy)

c   Replace the two operands and the operator with the result in LINB.

 220  if (ly .ne. 0) then
        i1 = id1(1)
        i2 = id2(2)
        l1 = i2 - i1 + 1
        ld = ly - l1
        if (i1 .gt. 0) linc(1:i1-1) = linb(1:i1-1)
        linc(i1:i1+ly-1) = argy(1:ly)
        if (lb .gt. i2) linc(i1+ly:lb+ld) = linb(i2+1:lb)
        lb = lb + ld
        linb(1:lb) = linc(1:lb)
        goto 100
      else
        itpx = itpy
        lx = ly
        argx = argy
      endif

c   Finished at last.

 230  continue
c*
c      write (lu, *) 'exit express, argx = %'//argx(1:lx)//'%'
c      call flush (lu)
c*
      return
      end
      subroutine expres6 (lu, la, lina, itpx, lx, argx)

c   This processes the arithmetic and/or logical expression in LINA, of length
c   = LA.  The result, after evaluation, is placed in ARGX, with type = ITPX,
c   and length = LX.  If the last result was =, then the ARGX is set to blanks,
c   and ITPX and LX are set to zero.  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb, linc
      character*80 ar(2), arg(mar), arg1, argx, argy
      character*16 nam
      dimension itar(mar), lar(mar), ita(2), lna(2), id1(2), id2(2),
     $  iop(2)
      character*1 cj
c*
c      write (lu, *) 'enter expres'
c      call flush (lu)
c*
c   Search for an executable operation (one that is not dependent on the
c   results of operations with higher precedence) in the statement.  The two
c   arguments of the operation, their types and lengths, are identified.

      lb = la
      linb(1:lb) = lina(1:la)

 100  i1 = 1
c*
c      write (lu, *) 'expres lb =', lb
c      write (lu, *) '%'//linb(1:lb)//'%'
c      call flush (lu)
c*
 110  do 200 ii = 1, 2
        i1 = nblk (i1, lb, linb)
        if (i1 .eq. 0) call errmes (29, 1)
        cj = linb(i1:i1)
        ix = 0
c*
c        write (lu, *) 'cj =', cj
c        call flush (lu)
c*
c   Check if this is the start of a numeric constant.

        is1 = index (dig, cj)
        if ((cj .eq. '-' .or. cj .eq. '.') .and. i1 .lt. lb) then
          i2 = nblk (i1 + 1, lb, linb)
          is2 = index (dig, linb(i2:i2))
        else
          is2 = 0
        endif

        if (is1 .ne. 0 .or. is2 .ne. 0) then
          it = numcon (i1, i2, lb, linb)

c   If the mixed mode safe flag is on, then the numeric constant is always
c   of type MP.

          if (imm .ge. 1) then
            if (it .eq. 1) then
              it = 8
            elseif (it .eq. 2 .or. it .eq. 3) then
              it = 9
            endif
          endif
          if (it .le. 3) then
            id1(ii) = i1
            id2(ii) = i2
            ita(ii) = it
            lna(ii) = i2 - i1 + 1
            ar(ii) = linb(i1:i2)
          else
            id1(1) = i1
            id2(2) = i2
            itpy = it
            call gencon (lu, i1, i2, linb, itpy, ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220
          endif

c   Check if string is a variable name.

        elseif (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then

          do 120 j = i1, lb
            cj = linb(j:j)
            if (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then
              goto 120
            elseif (index (dig, cj) .ne. 0 .or. cj .eq. '_') then
              goto 120
            else
              goto 130
            endif
 120      continue

          j = lb + 1

c   Variable or function name has been identified.

 130      i2 = j - 1
          l1 = i2 - i1 + 1
          nam = linb(i1:i2)
          ix = itab (1, 0, nam)
          if (ix .eq. 0) then
            call errmes (30, 0)
            write (6, 1) nam
 1          format ('This Fortran keyword may not appear in an',
     $        ' expression: ',a)
            call abrt
          elseif (ktyp(ix) .eq. 0 .and. kdec(ix) .eq. 0) then
            call errmes (31, 0)
            write (6, 2) nam
 2          format ('This variable has not been typed: ',a)
            call abrt
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = ktyp(ix)
          lna(ii) = l1
          ar(ii)(1:l1) = linb(i1:i2)

c   Check if string is a logical constant.

        elseif (cj .eq. '.' .and. i1 .lt. lb .and.
     $      (linb(i1+1:i1+1) .eq. 'T' .or. linb(i1+1:i1+1) .eq. 't' .or.
     $      linb(i1+1:i1+1) .eq. 'F' .or. linb(i1+1:i1+1) .eq. 'f'))
     $      then
          if (linb(i1:i1+2) .eq. '.T.' .or. linb(i1:i1+2) .eq. '.t.'.or.
     $      linb(i1:i1+2) .eq. '.F.' .or. linb(i1:i1+2) .eq. '.f.')
     $      then
            i2 = i1 + 2
          elseif (linb(i1:i1+5) .eq. '.TRUE.' .or.
     $        linb(i1:i1+5) .eq. '.true.') then
            i2 = i1 + 5
          elseif (linb(i1:i1+6) .eq. '.FALSE.' .or.
     $        linb(i1:i1+6) .eq. '.false.') then
            i2 = i1 + 6
          else
            call errmes (32, 1)
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 7
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by ".

        elseif (cj .eq. '"') then
          j1 = i1
 140      i2 = indx (j1 + 1, lb, '"', linb)
          if (i2 .eq. 0) call errmes (33, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '"') then
            j1 = i2 + 1
            goto 140
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by '.

        elseif (cj .eq. '''') then
          j1 = i1
 150      i2 = indx (j1 + 1, lb, '''', linb)
          if (i2 .eq. 0) call errmes (34, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '''') then
            j1 = i2 + 1
            goto 150
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a unary minus sign (i.e. argument 1 is null).

        elseif (cj .eq. '-') then
          if (ii .eq. 2) goto 110
          id1(1) = i1
          id2(1) = i1
          ita(1) = 0
          lna(1) = 1
          ar(1) = ' '
          iop(1) = 3
          i1 = i1 + 1
          goto 200

c   Check if argument is a logical not operation (i.e. argument 1 is null).

        elseif (cj .eq. '.' .and. i1 + 4 .le. lb) then
          if (linb(i1:i1+4) .eq. '.NOT.' .or. linb(i1:i1+4) .eq.
     $      '.not.') then
            if (ii .eq. 2) goto 110
            id1(1) = i1
            id2(1) = i1
            ita(1) = 0
            lna(1) = 1
            ar(1) = ' '
            iop(1) = 15
            i1 = i1 + 5
            goto 200
          endif

c   Check if the next character is a left parenthesis.  If so, evaluate the
c   expression in parentheses.

        elseif (cj .eq. '(') then
          i2 = match (0, i1 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (35, 1)
          lc = i2 - i1 - 1
          linc(1:lc) = linb(i1+1:i2-1)
          id1(1) = i1
          id2(2) = i2
          if (lb .eq. 0) call errmes (36, 1)
          call expres7 (lu, lc, linc, itpy, ly, argy)
          goto 220
        else
          call errmes (37, 0)
          write (6, 3) cj
 3        format ('Illegal character: ',a)
          call abrt
        endif

c   Check if the end of the variable or constant is the end of the statement.

 160    continue
        if (i2 .eq. lb .or. linb(i2+1:lb) .eq. ' ') then

c   If this occurs on the first pass, we are done.

          if (ii .eq. 1) then
            itpx = ita(1)
            lx = lna(1)
            argx(1:lx) = ar(1)(1:lx)
            goto 230

c   If this occurs on the second pass, proceed to evaluate.

          else
            goto 210
          endif
        endif

c   Check if the next character after the variable name is a left parenthesis.

        i3 = nblk (i2 + 1, lb, linb)
        cj = linb(i3:i3)
        if (cj .eq. '(') then
          if (ix .eq. 0) call errmes (38, 1)
          i2 = match (0, i3 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (39, 1)
          k1 = index (linb(i2:lb), '=')
 170      continue

c   Check if the subscripted variable is really a function reference.

          if (kdim(ix) .eq. -1) then
            if (k1 .ne. 0) then
              call errmes (40, 0)
              write (6, 4) nam(1:l1)
 4            format ('A function name may not appear on the LHS of',
     $          ' an assignment statement: ',a)
              call abrt
            endif

c   Generate a function call.  If it is an intrinsic reference and the mixed
c   mode SAFE option is in effect, set IMM = 2 as a flag to inform ARLIST not
c   to revert to mixed mode FAST while evaluating the argument list.

            id1(1) = i1
            id2(2) = i2
            lc = i2 - i3 - 1
            linc(1:lc) = linb(i3+1:i2-1)
            ims = imm
            if (ix .le. mint .and. imm .ge. 1) imm = 2
            call arlist7 (lu, lc, linc, nar, itar, lar, arg)
            imm = ims
            it1 = ita(ii)
            ln1 = lna(ii)
            arg1 = ar(ii)
            call genfun (lu, it1, ln1, arg1, nar, itar, lar, arg, itpy,
     $        ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220

c   Check if the subscripted variable has a dimension.  If not, this is
c   assumed to be an external function reference.

          elseif (kdim(ix) .eq. 0) then
            if (k1 .eq. 0) then

c   If this function name was earlier listed in an external statement and was
c   not explicitly typed, it must now be implicitly typed.

              if (kdec(ix) .eq. 1 .and. mplc(ix) .eq. 1) then
                cj = nam(1:1)
                ij = max (index (alpu, cj), index (alpl, cj))
                ktyp(ix) = impl(ij)
                ita(ii) = ktyp(ix)
              endif
              kdec(ix) = 1
              kdim(ix) = -1
              kt = ktyp(ix)
              write (lu, 5) nam(1:l1), ctp(kt)
 5            format ('CMP*'/'CMP*  Undimensioned variable assumed to',
     $          ' be an external function.'/'CMP*  Name: ',a,4x,
     $          'Type: ',a/'CMP*')
              goto 170
            else
              call errmes (41, 0)
              write (6, 6)
 6            format ('MP variables may not be used in statement',
     $          ' function definitions.'/'Define an external function',
     $          ' for this purpose.')
              call abrt
            endif

c   Otherwise it must be an ordinary array with subscript.  The combination of
c   the variable name and the subscript will now be treated as a variable.
c   IX is set to 0 as a flag indicating that this has been done.

          else
            i4 = iscan (i3, i2, linb)
            if (i4 .ne. 0) then
              call errmes (42, 0)
              write (6, 7) nam
 7            format ('The MP subscript on this variable is not',
     $          ' allowed: ',a)
              call abrt
            endif
            l1 = i2 - i1 + 1
            id2(ii) = i2
            lna(ii) = l1
            ar(ii)(1:l1) = linb(i1:i2)
            ix = 0
            goto 160
          endif
        else

c   The variable does not have a subscript.  Check if it has a dimension.

          if (ix .gt. 0) then
            if (kdim(ix) .gt. 0) then
              call errmes (43, 0)
              write (6, 8) nam(1:l1)
 8            format ('This dimensioned variable is used without a',
     $          ' subscript: ',a)
              call abrt
            endif
          endif
        endif
        i1 = i3

c   Identify the operator.

        do 180 i = 1, nop
          l1 = lop(i) - 1
          if (linb(i1:i1+l1) .eq. lopr(i) .or. linb(i1:i1+l1) .eq.
     $      uopr(i)) goto 190
 180    continue

        call errmes (44, 0)
        write (6, 9) linb(i1:i1)
 9      format ('Illegal operator: ',a)
        call abrt

 190    iop(ii) = i
        i1 = i1 + lop(i)
 200  continue

c   Compare the precedence levels of the two operators.

      if (kop(iop(1)) .lt. kop(iop(2))) then
        i1 = id1(2)
        goto 110
      endif

c   An operation can be performed.

210   call gen (lu, ita, lna, ar, iop(1), itpy, ly, argy)

c   Replace the two operands and the operator with the result in LINB.

 220  if (ly .ne. 0) then
        i1 = id1(1)
        i2 = id2(2)
        l1 = i2 - i1 + 1
        ld = ly - l1
        if (i1 .gt. 0) linc(1:i1-1) = linb(1:i1-1)
        linc(i1:i1+ly-1) = argy(1:ly)
        if (lb .gt. i2) linc(i1+ly:lb+ld) = linb(i2+1:lb)
        lb = lb + ld
        linb(1:lb) = linc(1:lb)
        goto 100
      else
        itpx = itpy
        lx = ly
        argx = argy
      endif

c   Finished at last.

 230  continue
c*
c      write (lu, *) 'exit express, argx = %'//argx(1:lx)//'%'
c      call flush (lu)
c*
      return
      end
      subroutine expres7 (lu, la, lina, itpx, lx, argx)

c   This processes the arithmetic and/or logical expression in LINA, of length
c   = LA.  The result, after evaluation, is placed in ARGX, with type = ITPX,
c   and length = LX.  If the last result was =, then the ARGX is set to blanks,
c   and ITPX and LX are set to zero.  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb, linc
      character*80 ar(2), arg(mar), arg1, argx, argy
      character*16 nam
      dimension itar(mar), lar(mar), ita(2), lna(2), id1(2), id2(2),
     $  iop(2)
      character*1 cj
c*
c      write (lu, *) 'enter expres'
c      call flush (lu)
c*
c   Search for an executable operation (one that is not dependent on the
c   results of operations with higher precedence) in the statement.  The two
c   arguments of the operation, their types and lengths, are identified.

      lb = la
      linb(1:lb) = lina(1:la)

 100  i1 = 1
c*
c      write (lu, *) 'expres lb =', lb
c      write (lu, *) '%'//linb(1:lb)//'%'
c      call flush (lu)
c*
 110  do 200 ii = 1, 2
        i1 = nblk (i1, lb, linb)
        if (i1 .eq. 0) call errmes (29, 1)
        cj = linb(i1:i1)
        ix = 0
c*
c        write (lu, *) 'cj =', cj
c        call flush (lu)
c*
c   Check if this is the start of a numeric constant.

        is1 = index (dig, cj)
        if ((cj .eq. '-' .or. cj .eq. '.') .and. i1 .lt. lb) then
          i2 = nblk (i1 + 1, lb, linb)
          is2 = index (dig, linb(i2:i2))
        else
          is2 = 0
        endif

        if (is1 .ne. 0 .or. is2 .ne. 0) then
          it = numcon (i1, i2, lb, linb)

c   If the mixed mode safe flag is on, then the numeric constant is always
c   of type MP.

          if (imm .ge. 1) then
            if (it .eq. 1) then
              it = 8
            elseif (it .eq. 2 .or. it .eq. 3) then
              it = 9
            endif
          endif
          if (it .le. 3) then
            id1(ii) = i1
            id2(ii) = i2
            ita(ii) = it
            lna(ii) = i2 - i1 + 1
            ar(ii) = linb(i1:i2)
          else
            id1(1) = i1
            id2(2) = i2
            itpy = it
            call gencon (lu, i1, i2, linb, itpy, ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220
          endif

c   Check if string is a variable name.

        elseif (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then

          do 120 j = i1, lb
            cj = linb(j:j)
            if (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then
              goto 120
            elseif (index (dig, cj) .ne. 0 .or. cj .eq. '_') then
              goto 120
            else
              goto 130
            endif
 120      continue

          j = lb + 1

c   Variable or function name has been identified.

 130      i2 = j - 1
          l1 = i2 - i1 + 1
          nam = linb(i1:i2)
          ix = itab (1, 0, nam)
          if (ix .eq. 0) then
            call errmes (30, 0)
            write (6, 1) nam
 1          format ('This Fortran keyword may not appear in an',
     $        ' expression: ',a)
            call abrt
          elseif (ktyp(ix) .eq. 0 .and. kdec(ix) .eq. 0) then
            call errmes (31, 0)
            write (6, 2) nam
 2          format ('This variable has not been typed: ',a)
            call abrt
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = ktyp(ix)
          lna(ii) = l1
          ar(ii)(1:l1) = linb(i1:i2)

c   Check if string is a logical constant.

        elseif (cj .eq. '.' .and. i1 .lt. lb .and.
     $      (linb(i1+1:i1+1) .eq. 'T' .or. linb(i1+1:i1+1) .eq. 't' .or.
     $      linb(i1+1:i1+1) .eq. 'F' .or. linb(i1+1:i1+1) .eq. 'f'))
     $      then
          if (linb(i1:i1+2) .eq. '.T.' .or. linb(i1:i1+2) .eq. '.t.'.or.
     $      linb(i1:i1+2) .eq. '.F.' .or. linb(i1:i1+2) .eq. '.f.')
     $      then
            i2 = i1 + 2
          elseif (linb(i1:i1+5) .eq. '.TRUE.' .or.
     $        linb(i1:i1+5) .eq. '.true.') then
            i2 = i1 + 5
          elseif (linb(i1:i1+6) .eq. '.FALSE.' .or.
     $        linb(i1:i1+6) .eq. '.false.') then
            i2 = i1 + 6
          else
            call errmes (32, 1)
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 7
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by ".

        elseif (cj .eq. '"') then
          j1 = i1
 140      i2 = indx (j1 + 1, lb, '"', linb)
          if (i2 .eq. 0) call errmes (33, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '"') then
            j1 = i2 + 1
            goto 140
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by '.

        elseif (cj .eq. '''') then
          j1 = i1
 150      i2 = indx (j1 + 1, lb, '''', linb)
          if (i2 .eq. 0) call errmes (34, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '''') then
            j1 = i2 + 1
            goto 150
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a unary minus sign (i.e. argument 1 is null).

        elseif (cj .eq. '-') then
          if (ii .eq. 2) goto 110
          id1(1) = i1
          id2(1) = i1
          ita(1) = 0
          lna(1) = 1
          ar(1) = ' '
          iop(1) = 3
          i1 = i1 + 1
          goto 200

c   Check if argument is a logical not operation (i.e. argument 1 is null).

        elseif (cj .eq. '.' .and. i1 + 4 .le. lb) then
          if (linb(i1:i1+4) .eq. '.NOT.' .or. linb(i1:i1+4) .eq.
     $      '.not.') then
            if (ii .eq. 2) goto 110
            id1(1) = i1
            id2(1) = i1
            ita(1) = 0
            lna(1) = 1
            ar(1) = ' '
            iop(1) = 15
            i1 = i1 + 5
            goto 200
          endif

c   Check if the next character is a left parenthesis.  If so, evaluate the
c   expression in parentheses.

        elseif (cj .eq. '(') then
          i2 = match (0, i1 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (35, 1)
          lc = i2 - i1 - 1
          linc(1:lc) = linb(i1+1:i2-1)
          id1(1) = i1
          id2(2) = i2
          if (lb .eq. 0) call errmes (36, 1)
          call expres8 (lu, lc, linc, itpy, ly, argy)
          goto 220
        else
          call errmes (37, 0)
          write (6, 3) cj
 3        format ('Illegal character: ',a)
          call abrt
        endif

c   Check if the end of the variable or constant is the end of the statement.

 160    continue
        if (i2 .eq. lb .or. linb(i2+1:lb) .eq. ' ') then

c   If this occurs on the first pass, we are done.

          if (ii .eq. 1) then
            itpx = ita(1)
            lx = lna(1)
            argx(1:lx) = ar(1)(1:lx)
            goto 230

c   If this occurs on the second pass, proceed to evaluate.

          else
            goto 210
          endif
        endif

c   Check if the next character after the variable name is a left parenthesis.

        i3 = nblk (i2 + 1, lb, linb)
        cj = linb(i3:i3)
        if (cj .eq. '(') then
          if (ix .eq. 0) call errmes (38, 1)
          i2 = match (0, i3 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (39, 1)
          k1 = index (linb(i2:lb), '=')
 170      continue

c   Check if the subscripted variable is really a function reference.

          if (kdim(ix) .eq. -1) then
            if (k1 .ne. 0) then
              call errmes (40, 0)
              write (6, 4) nam(1:l1)
 4            format ('A function name may not appear on the LHS of',
     $          ' an assignment statement: ',a)
              call abrt
            endif

c   Generate a function call.  If it is an intrinsic reference and the mixed
c   mode SAFE option is in effect, set IMM = 2 as a flag to inform ARLIST not
c   to revert to mixed mode FAST while evaluating the argument list.

            id1(1) = i1
            id2(2) = i2
            lc = i2 - i3 - 1
            linc(1:lc) = linb(i3+1:i2-1)
            ims = imm
            if (ix .le. mint .and. imm .ge. 1) imm = 2
            call arlist8 (lu, lc, linc, nar, itar, lar, arg)
            imm = ims
            it1 = ita(ii)
            ln1 = lna(ii)
            arg1 = ar(ii)
            call genfun (lu, it1, ln1, arg1, nar, itar, lar, arg, itpy,
     $        ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220

c   Check if the subscripted variable has a dimension.  If not, this is
c   assumed to be an external function reference.

          elseif (kdim(ix) .eq. 0) then
            if (k1 .eq. 0) then

c   If this function name was earlier listed in an external statement and was
c   not explicitly typed, it must now be implicitly typed.

              if (kdec(ix) .eq. 1 .and. mplc(ix) .eq. 1) then
                cj = nam(1:1)
                ij = max (index (alpu, cj), index (alpl, cj))
                ktyp(ix) = impl(ij)
                ita(ii) = ktyp(ix)
              endif
              kdec(ix) = 1
              kdim(ix) = -1
              kt = ktyp(ix)
              write (lu, 5) nam(1:l1), ctp(kt)
 5            format ('CMP*'/'CMP*  Undimensioned variable assumed to',
     $          ' be an external function.'/'CMP*  Name: ',a,4x,
     $          'Type: ',a/'CMP*')
              goto 170
            else
              call errmes (41, 0)
              write (6, 6)
 6            format ('MP variables may not be used in statement',
     $          ' function definitions.'/'Define an external function',
     $          ' for this purpose.')
              call abrt
            endif

c   Otherwise it must be an ordinary array with subscript.  The combination of
c   the variable name and the subscript will now be treated as a variable.
c   IX is set to 0 as a flag indicating that this has been done.

          else
            i4 = iscan (i3, i2, linb)
            if (i4 .ne. 0) then
              call errmes (42, 0)
              write (6, 7) nam
 7            format ('The MP subscript on this variable is not',
     $          ' allowed: ',a)
              call abrt
            endif
            l1 = i2 - i1 + 1
            id2(ii) = i2
            lna(ii) = l1
            ar(ii)(1:l1) = linb(i1:i2)
            ix = 0
            goto 160
          endif
        else

c   The variable does not have a subscript.  Check if it has a dimension.

          if (ix .gt. 0) then
            if (kdim(ix) .gt. 0) then
              call errmes (43, 0)
              write (6, 8) nam(1:l1)
 8            format ('This dimensioned variable is used without a',
     $          ' subscript: ',a)
              call abrt
            endif
          endif
        endif
        i1 = i3

c   Identify the operator.

        do 180 i = 1, nop
          l1 = lop(i) - 1
          if (linb(i1:i1+l1) .eq. lopr(i) .or. linb(i1:i1+l1) .eq.
     $      uopr(i)) goto 190
 180    continue

        call errmes (44, 0)
        write (6, 9) linb(i1:i1)
 9      format ('Illegal operator: ',a)
        call abrt

 190    iop(ii) = i
        i1 = i1 + lop(i)
 200  continue

c   Compare the precedence levels of the two operators.

      if (kop(iop(1)) .lt. kop(iop(2))) then
        i1 = id1(2)
        goto 110
      endif

c   An operation can be performed.

210   call gen (lu, ita, lna, ar, iop(1), itpy, ly, argy)

c   Replace the two operands and the operator with the result in LINB.

 220  if (ly .ne. 0) then
        i1 = id1(1)
        i2 = id2(2)
        l1 = i2 - i1 + 1
        ld = ly - l1
        if (i1 .gt. 0) linc(1:i1-1) = linb(1:i1-1)
        linc(i1:i1+ly-1) = argy(1:ly)
        if (lb .gt. i2) linc(i1+ly:lb+ld) = linb(i2+1:lb)
        lb = lb + ld
        linb(1:lb) = linc(1:lb)
        goto 100
      else
        itpx = itpy
        lx = ly
        argx = argy
      endif

c   Finished at last.

 230  continue
c*
c      write (lu, *) 'exit express, argx = %'//argx(1:lx)//'%'
c      call flush (lu)
c*
      return
      end
      subroutine expres8 (lu, la, lina, itpx, lx, argx)

c   This processes the arithmetic and/or logical expression in LINA, of length
c   = LA.  The result, after evaluation, is placed in ARGX, with type = ITPX,
c   and length = LX.  If the last result was =, then the ARGX is set to blanks,
c   and ITPX and LX are set to zero.  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb, linc
      character*80 ar(2), arg(mar), arg1, argx, argy
      character*16 nam
      dimension itar(mar), lar(mar), ita(2), lna(2), id1(2), id2(2),
     $  iop(2)
      character*1 cj
c*
c      write (lu, *) 'enter expres'
c      call flush (lu)
c*
c   Search for an executable operation (one that is not dependent on the
c   results of operations with higher precedence) in the statement.  The two
c   arguments of the operation, their types and lengths, are identified.

      lb = la
      linb(1:lb) = lina(1:la)

 100  i1 = 1
c*
c      write (lu, *) 'expres lb =', lb
c      write (lu, *) '%'//linb(1:lb)//'%'
c      call flush (lu)
c*
 110  do 200 ii = 1, 2
        i1 = nblk (i1, lb, linb)
        if (i1 .eq. 0) call errmes (29, 1)
        cj = linb(i1:i1)
        ix = 0
c*
c        write (lu, *) 'cj =', cj
c        call flush (lu)
c*
c   Check if this is the start of a numeric constant.

        is1 = index (dig, cj)
        if ((cj .eq. '-' .or. cj .eq. '.') .and. i1 .lt. lb) then
          i2 = nblk (i1 + 1, lb, linb)
          is2 = index (dig, linb(i2:i2))
        else
          is2 = 0
        endif

        if (is1 .ne. 0 .or. is2 .ne. 0) then
          it = numcon (i1, i2, lb, linb)

c   If the mixed mode safe flag is on, then the numeric constant is always
c   of type MP.

          if (imm .ge. 1) then
            if (it .eq. 1) then
              it = 8
            elseif (it .eq. 2 .or. it .eq. 3) then
              it = 9
            endif
          endif
          if (it .le. 3) then
            id1(ii) = i1
            id2(ii) = i2
            ita(ii) = it
            lna(ii) = i2 - i1 + 1
            ar(ii) = linb(i1:i2)
          else
            id1(1) = i1
            id2(2) = i2
            itpy = it
            call gencon (lu, i1, i2, linb, itpy, ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220
          endif

c   Check if string is a variable name.

        elseif (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then

          do 120 j = i1, lb
            cj = linb(j:j)
            if (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then
              goto 120
            elseif (index (dig, cj) .ne. 0 .or. cj .eq. '_') then
              goto 120
            else
              goto 130
            endif
 120      continue

          j = lb + 1

c   Variable or function name has been identified.

 130      i2 = j - 1
          l1 = i2 - i1 + 1
          nam = linb(i1:i2)
          ix = itab (1, 0, nam)
          if (ix .eq. 0) then
            call errmes (30, 0)
            write (6, 1) nam
 1          format ('This Fortran keyword may not appear in an',
     $        ' expression: ',a)
            call abrt
          elseif (ktyp(ix) .eq. 0 .and. kdec(ix) .eq. 0) then
            call errmes (31, 0)
            write (6, 2) nam
 2          format ('This variable has not been typed: ',a)
            call abrt
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = ktyp(ix)
          lna(ii) = l1
          ar(ii)(1:l1) = linb(i1:i2)

c   Check if string is a logical constant.

        elseif (cj .eq. '.' .and. i1 .lt. lb .and.
     $      (linb(i1+1:i1+1) .eq. 'T' .or. linb(i1+1:i1+1) .eq. 't' .or.
     $      linb(i1+1:i1+1) .eq. 'F' .or. linb(i1+1:i1+1) .eq. 'f'))
     $      then
          if (linb(i1:i1+2) .eq. '.T.' .or. linb(i1:i1+2) .eq. '.t.'.or.
     $      linb(i1:i1+2) .eq. '.F.' .or. linb(i1:i1+2) .eq. '.f.')
     $      then
            i2 = i1 + 2
          elseif (linb(i1:i1+5) .eq. '.TRUE.' .or.
     $        linb(i1:i1+5) .eq. '.true.') then
            i2 = i1 + 5
          elseif (linb(i1:i1+6) .eq. '.FALSE.' .or.
     $        linb(i1:i1+6) .eq. '.false.') then
            i2 = i1 + 6
          else
            call errmes (32, 1)
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 7
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by ".

        elseif (cj .eq. '"') then
          j1 = i1
 140      i2 = indx (j1 + 1, lb, '"', linb)
          if (i2 .eq. 0) call errmes (33, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '"') then
            j1 = i2 + 1
            goto 140
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a character constant delimited by '.

        elseif (cj .eq. '''') then
          j1 = i1
 150      i2 = indx (j1 + 1, lb, '''', linb)
          if (i2 .eq. 0) call errmes (34, 1)
          if (i2 .lt. lb .and. linb(i2+1:i2+1) .eq. '''') then
            j1 = i2 + 1
            goto 150
          endif
          id1(ii) = i1
          id2(ii) = i2
          ita(ii) = 6
          lna(ii) = i2 - i1 + 1
          ar(ii) = linb(i1:i2)

c   Check if argument is a unary minus sign (i.e. argument 1 is null).

        elseif (cj .eq. '-') then
          if (ii .eq. 2) goto 110
          id1(1) = i1
          id2(1) = i1
          ita(1) = 0
          lna(1) = 1
          ar(1) = ' '
          iop(1) = 3
          i1 = i1 + 1
          goto 200

c   Check if argument is a logical not operation (i.e. argument 1 is null).

        elseif (cj .eq. '.' .and. i1 + 4 .le. lb) then
          if (linb(i1:i1+4) .eq. '.NOT.' .or. linb(i1:i1+4) .eq.
     $      '.not.') then
            if (ii .eq. 2) goto 110
            id1(1) = i1
            id2(1) = i1
            ita(1) = 0
            lna(1) = 1
            ar(1) = ' '
            iop(1) = 15
            i1 = i1 + 5
            goto 200
          endif

c   Check if the next character is a left parenthesis.  If so, evaluate the
c   expression in parentheses.

        elseif (cj .eq. '(') then
          i2 = match (0, i1 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (35, 1)
          lc = i2 - i1 - 1
          linc(1:lc) = linb(i1+1:i2-1)
          id1(1) = i1
          id2(2) = i2
          if (lb .eq. 0) call errmes (36, 1)
!          call expres (lu, lc, linc, itpy, ly, argy)
!          goto 220

          call errmes (120, 0)
          write (6, 11)
11        format ('Too deeply nested.')
          call abrt

        else
          call errmes (37, 0)
          write (6, 3) cj
 3        format ('Illegal character: ',a)
          call abrt
        endif

c   Check if the end of the variable or constant is the end of the statement.

 160    continue
        if (i2 .eq. lb .or. linb(i2+1:lb) .eq. ' ') then

c   If this occurs on the first pass, we are done.

          if (ii .eq. 1) then
            itpx = ita(1)
            lx = lna(1)
            argx(1:lx) = ar(1)(1:lx)
            goto 230

c   If this occurs on the second pass, proceed to evaluate.

          else
            goto 210
          endif
        endif

c   Check if the next character after the variable name is a left parenthesis.

        i3 = nblk (i2 + 1, lb, linb)
        cj = linb(i3:i3)
        if (cj .eq. '(') then
          if (ix .eq. 0) call errmes (38, 1)
          i2 = match (0, i3 + 1, lb, linb)
          if (i2 .eq. 0) call errmes (39, 1)
          k1 = index (linb(i2:lb), '=')
 170      continue

c   Check if the subscripted variable is really a function reference.

          if (kdim(ix) .eq. -1) then
            if (k1 .ne. 0) then
              call errmes (40, 0)
              write (6, 4) nam(1:l1)
 4            format ('A function name may not appear on the LHS of',
     $          ' an assignment statement: ',a)
              call abrt
            endif

c   Generate a function call.  If it is an intrinsic reference and the mixed
c   mode SAFE option is in effect, set IMM = 2 as a flag to inform ARLIST not
c   to revert to mixed mode FAST while evaluating the argument list.

            id1(1) = i1
            id2(2) = i2
            lc = i2 - i3 - 1
            linc(1:lc) = linb(i3+1:i2-1)
            ims = imm
            if (ix .le. mint .and. imm .ge. 1) imm = 2
!            call arlist (lu, lc, linc, nar, itar, lar, arg)

            call errmes (120, 0)
            write (6, 11)
            call abrt

            imm = ims
            it1 = ita(ii)
            ln1 = lna(ii)
            arg1 = ar(ii)
            call genfun (lu, it1, ln1, arg1, nar, itar, lar, arg, itpy,
     $        ly, argy)
            i1 = nblk (i2 + 1, lb, linb)
            goto 220

c   Check if the subscripted variable has a dimension.  If not, this is
c   assumed to be an external function reference.

          elseif (kdim(ix) .eq. 0) then
            if (k1 .eq. 0) then

c   If this function name was earlier listed in an external statement and was
c   not explicitly typed, it must now be implicitly typed.

              if (kdec(ix) .eq. 1 .and. mplc(ix) .eq. 1) then
                cj = nam(1:1)
                ij = max (index (alpu, cj), index (alpl, cj))
                ktyp(ix) = impl(ij)
                ita(ii) = ktyp(ix)
              endif
              kdec(ix) = 1
              kdim(ix) = -1
              kt = ktyp(ix)
              write (lu, 5) nam(1:l1), ctp(kt)
 5            format ('CMP*'/'CMP*  Undimensioned variable assumed to',
     $          ' be an external function.'/'CMP*  Name: ',a,4x,
     $          'Type: ',a/'CMP*')
              goto 170
            else
              call errmes (41, 0)
              write (6, 6)
 6            format ('MP variables may not be used in statement',
     $          ' function definitions.'/'Define an external function',
     $          ' for this purpose.')
              call abrt
            endif

c   Otherwise it must be an ordinary array with subscript.  The combination of
c   the variable name and the subscript will now be treated as a variable.
c   IX is set to 0 as a flag indicating that this has been done.

          else
            i4 = iscan (i3, i2, linb)
            if (i4 .ne. 0) then
              call errmes (42, 0)
              write (6, 7) nam
 7            format ('The MP subscript on this variable is not',
     $          ' allowed: ',a)
              call abrt
            endif
            l1 = i2 - i1 + 1
            id2(ii) = i2
            lna(ii) = l1
            ar(ii)(1:l1) = linb(i1:i2)
            ix = 0
            goto 160
          endif
        else

c   The variable does not have a subscript.  Check if it has a dimension.

          if (ix .gt. 0) then
            if (kdim(ix) .gt. 0) then
              call errmes (43, 0)
              write (6, 8) nam(1:l1)
 8            format ('This dimensioned variable is used without a',
     $          ' subscript: ',a)
              call abrt
            endif
          endif
        endif
        i1 = i3

c   Identify the operator.

        do 180 i = 1, nop
          l1 = lop(i) - 1
          if (linb(i1:i1+l1) .eq. lopr(i) .or. linb(i1:i1+l1) .eq.
     $      uopr(i)) goto 190
 180    continue

        call errmes (44, 0)
        write (6, 9) linb(i1:i1)
 9      format ('Illegal operator: ',a)
        call abrt

 190    iop(ii) = i
        i1 = i1 + lop(i)
 200  continue

c   Compare the precedence levels of the two operators.

      if (kop(iop(1)) .lt. kop(iop(2))) then
        i1 = id1(2)
        goto 110
      endif

c   An operation can be performed.

210   call gen (lu, ita, lna, ar, iop(1), itpy, ly, argy)

c   Replace the two operands and the operator with the result in LINB.

 220  if (ly .ne. 0) then
        i1 = id1(1)
        i2 = id2(2)
        l1 = i2 - i1 + 1
        ld = ly - l1
        if (i1 .gt. 0) linc(1:i1-1) = linb(1:i1-1)
        linc(i1:i1+ly-1) = argy(1:ly)
        if (lb .gt. i2) linc(i1+ly:lb+ld) = linb(i2+1:lb)
        lb = lb + ld
        linb(1:lb) = linc(1:lb)
        goto 100
      else
        itpx = itpy
        lx = ly
        argx = argy
      endif

c   Finished at last.

 230  continue
c*
c      write (lu, *) 'exit express, argx = %'//argx(1:lx)//'%'
c      call flush (lu)
c*
      return
      end
      subroutine fixsub (k1, k2, ln)

c   This routine prepends '1,' to subscripts MP variables in LINE between K1
c   and K2.  LN is the length of the full line.  It also changes the names of
c   the special constants when found.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 linx
      character*16 nam, namq
      character*1 cj

      j1 = k1
      namq = 'MPNWP'
      iqnwp = itab (0, 0, namq)
      namq = 'DPPIC'
      iqpic = itab (0, 0, namq)
      iqd = iqpic - iqnwp + 1

c   Extract the next character from the line.

 100  if (j1 .gt. k2) goto 130
      j1 = nblk (j1, k2, line)
      cj = line(j1:j1)

c   Check if it is the start of a numeric constant.

      is1 = index (dig, cj)
      if ((cj .eq. '-' .or. cj .eq. '.') .and. j1 .lt. k2) then
        j2 = nblk (j1 + 1, k2, line)
        is2 = index (dig, line(j2:j2))
      else
        is2 = 0
      endif

      if (is1 .ne. 0 .or. is2 .ne. 0) then
        itp = numcon (j1, j2, ln, line)
        j1 = j2 + 1
        goto 100

c   Check if it the start of a name.

      elseif (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then

        do 110 j = j1, k2
          cj = line(j:j)
          if (index (del, cj) .ne. 0) goto 120
 110    continue

        j = k2 + 1

c   The variable has been identified.

 120    i2 = j - 1
        nam = line(j1:i2)
        ix = itab (0, 0, nam)
        if (ix .eq. 0) then
          j1 = i2 + 1
          goto 100
        endif
        itp = ktyp(ix)

c   Check if the variable is the function value.  If so, change its name.

        if (kdim(ix) .eq. -2) then
          lx = ln - i2 + 5
          linx(1:5) = 'MPFVX'
          linx(6:lx) = line(i2+1:ln)
          ld = j1 - i2 + 4
          k2 = k2 + ld
          ln = ln + ld
          line(j1:ln) = linx(1:lx)
          j1 = j1 + 5
          goto 100

c   Check if the variable is a special constant.  If so, change its name.

        elseif (ix .ge. iqnwp .and. ix .le. iqpic) then
          line(j1:i2) = var(ix+iqd)
          kcon(ix) = 1
          j1 = i2 + 1
          goto 100

c   Check if the variable is MP.

        elseif (itp .ge. 8) then

c   Check if this MP variable has a subscript.

          i1 = nblk (i2 + 1, k2, line)
          if (i1 .eq. 0) goto 130
          if (line(i1:i1) .eq. '(') then
            if (kdim(ix) .gt. 0) then
              lx = ln - i1 + 2
              linx(1:2) = '1,'
              linx(3:lx) = line(i1+1:ln)
              line(i1+1:ln+2) = linx(1:lx)
              k2 = k2 + 2
              ln = ln + 2
              j1 = indx (i1 + 1, k2, ')', line)
              if (j1 .eq. 0) call errmes (45, 1)
            endif
          else
            j1 = i2 + 1
            goto 100
          endif
        endif
        j1 = i2 + 1
        goto 100

c   Check if it is the start of a logical constant.

      elseif (cj .eq. '.') then
        i1 = indx (j1 + 1, k2, '.', line)
        if (i1 .eq. 0) call errmes (46, 1)
        j1 = i1 + 1
        goto 100

c   Check if it is the start of a character constant.

      elseif (cj .eq. '"') then
        i1 = indx (j1 + 1, k2, '"', line)
        if (i1 .eq. 0) call errmes (47, 1)
        j1 = i1 + 1
        goto 100
      elseif (cj .eq. '''') then
        i1 = indx (j1 + 1, k2, '''', line)
        if (i1 .eq. 0) call errmes (48, 1)
        j1 = i1 + 1
        goto 100
      endif

c   Check if it is one of the miscellaneous symbols.

      i1 = index (del, cj)
      if (i1 .eq. 0) call errmes (49, 1)
      j1 = j1 + 1
      goto 100

 130  return
      end

      subroutine gen (lu, ita, lna, ar, iop, itpx, lx, argx)

c   This generates code for a single operation.  The two input argument names
c   are in AR, with types in ITA and lengths in LNA.  The operation code is in
c   IOP.  The result (ordinarily a temporary variable name, but empty in case
c   of assignments) is placed in ARGX, with type ITPX and length LX.  LU is
c   the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*80 ar(2), arg1, arg2, arg3, argx
      character*4 tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, getmp
      dimension ita(2), lna(2)

      itp1 = ita(1)
      l1 = lna(1)
      arg1(1:l1) = ar(1)(1:l1)
      itp2 = ita(2)
      l2 = lna(2)
      arg2(1:l2) = ar(2)(1:l2)
c*
c      write (lu, *) 'enter gen ', itp1, itp2, ' ', uopr(iop)
c      write (lu, *) 'args: ', arg1(1:l1), '  ', arg2(1:l2)
c      call flush (lu)
c*
c   Check for character entities with non-character entities.

      if (itp1 .eq. 6 .and. itp2 .ne. 6 .or. itp1 .ne. 6 .and.
     $  itp2 .eq. 6) goto 110

c   Check for logical entities with non-logical entities.

      if (itp1 .eq. 7 .and. itp2 .ne. 7 .or. itp1 .ne. 0 .and.
     $  itp1 .ne. 7 .and. itp2 .eq. 7) goto 110

c   Check for assignments.

      if (iop .eq. 1) then
        itpx = 0
        argx(1:4) = ' '
        lx = 0
        call genasn (lu, itp1, l1, arg1, itp2, l2, arg2)
        goto 100
      endif

c   Handle all other operations here.  The result variable will be a
c   temporary variable and the result type is the max of the two type numbers,
c   except for comparisons and a couple of other mixed mode cases.

      if (iop .le. 6) then
        if (itp1 .eq. 8 .and. (itp2 .eq. 2 .or. itp2 .eq. 3) .or.
     $    itp2 .eq. 8 .and. (itp1 .eq. 2 .or. itp1 .eq. 3)) then
          itpx = 9
        elseif (itp1 .ge. 8 .and. (itp2 .eq. 4 .or. itp2 .eq. 5) .or.
     $      itp2 .ge. 8 .and. (itp1 .eq. 4 .or. itp1 .eq. 5)) then
          itpx = 10
        else
          itpx = max (itp1, itp2)
        endif

c   If the mixed mode safe option is in effect, change IN, SP, DP, CO and DC
c   results to the appropriate MP type.

        if (imm .ge. 1 .and. itp1 .ge. 1) then
          if (itpx .eq. 1) then
            itpx = 8
            itp3 = 8
          elseif (itpx .eq. 2 .or. itpx .eq. 3) then
            itpx = 9
            itp3 = 9
          elseif (itpx .eq. 4 .or. itpx .eq. 5) then
            itpx = 10
            itp3 = 10
          endif
        endif
      else
        itpx = 7
      endif

c   Check if the operation is really the definition of a MPI or MPR constant.

      if (iop .eq. 2 .and. l2 .eq. 1 .and. arg2(1:1) .eq. '0') then
        if (index (dig, arg1(1:1)) .ne. 0 .or. arg1(1:1) .eq. '.' .or.
     $    arg1(1:1) .eq. '-' .or. arg1(1:1) .eq. '+') then
          lina(1:l1) = arg1(1:l1)
          if (itp1 .eq. 1 .or. itp1 .eq. 8) then
            itpx = 8
          elseif (itp1 .eq. 2 .or. itp1 .eq. 3 .or. itp1 .eq. 9) then
            itpx = 9
          endif
          call gencon (lu, 1, l1, lina, itpx, lx, argx)
        else
          itpx = itp1
          lx = l1
          argx(1:lx) = arg1(1:l1)
        endif
        goto 120
      endif
      lx = 4
      argx(1:4) = getmp (itpx)

c   Generate code for non-MP operands.

      if (itp1 .lt. 8 .and. itp2 .lt. 8) then
        if (imm .eq. 0 .or. itp1 .eq. 0 .or. itp1 .eq. 7 .and.
     $    itp2 .eq. 7 .or. iop .ge. 7 .or. iop .eq. 15) then

c   Either mixed mode fast option is in effect, or else the operation is a
c   comparison or logical operation.  Generate a simple one line non-MP
c   statement.

          if (arg1(1:1) .eq. '-') then
            arg3(1:1) = '('
            arg3(2:l1+1) = arg1(1:l1)
            arg3(l1+2:l1+2) = ')'
            arg1(1:l1+2) = arg3(1:l1+2)
            l1 = l1 + 2
          endif
          if (arg2(1:1) .eq. '-') then
            arg3(1:1) = '('
            arg3(2:l2+1) = arg2(1:l2)
            arg3(l2+2:l2+2) = ')'
            arg2(1:l2+2) = arg3(1:l2+2)
            l2 = l2 + 2
          endif
          write (lina, 1) argx(1:4), arg1(1:l1), uopr(iop)(1:lop(iop)),
     $      arg2(1:l2)
 1        format (6x,a,' = ',a,' ',a,' ',a)
          la = 15 + l1 + lop(iop) + l2
          call outlin (lu, 1, la, lina)
          goto 100
        else

c   The mixed mode safe option is in effect.  Convert the left operand to
c   the appropriate MP type so that it will be evaluated using the MP routines.

          tmp1 = getmp (itp3)
          arg3(1:4) = tmp1
          l3 = 4
          call genasn (lu, itp3, l3, arg3, itp1, l1, arg1)

c   If ARG1 is a temporary, release it.  Then set ARG1 = ARG3 and generate
c   code for this operation using the appropriate GENXXX routine.

          if (l1 .eq. 4) then
            if (arg1(1:2) .eq. 'MP') then
              tmp1 = arg1(1:4)
              call rltmp (tmp1)
            endif
          endif
          itp1 = itp3
          l1 = l3
          arg1(1:l1) = arg3(1:l3)
        endif
      endif

c   Check if operation is a plus.

      if (iop .eq. 2) then
        call genadd (lu, itp1, l1, arg1, itp2, l2, arg2, argx)

c   Check if the operation is minus.

      elseif (iop .eq. 3) then
        call gensub (lu, itp1, l1, arg1, itp2, l2, arg2, argx)

c   Check if the operation is exponentiation.

      elseif (iop .eq. 4) then
        call genexp (lu, itp1, l1, arg1, itp2, l2, arg2, argx)

c   Check if the operation is multiplication.

      elseif (iop .eq. 5) then
        call genmul (lu, itp1, l1, arg1, itp2, l2, arg2, argx)

c   Check if the operation is division.

      elseif (iop .eq. 6) then
        call gendiv (lu, itp1, l1, arg1, itp2, l2, arg2, argx)

c   Check if the operation is comparison.

      elseif (iop .ge. 7 .and. iop .le. 12) then
        call gencpr (lu, itp1, l1, arg1, itp2, l2, arg2, iop, argx)
        goto 100
      else
        goto 110
      endif

c   If the result is of type MPI, truncate the result.

      if (itpx .eq. 8) then
        tmp1 = getmp (9)
        write (lu, 2) argx(1:4), argx(1:4), tmp1
 2      format (6x,'CALL MPINFR (',a,', ',a,', ',a,')')
        call rltmp (tmp1)
      endif

c   Release any temporaries among the arguments.

 100  if (l1 .eq. 4) then
        if (arg1(1:2) .eq. 'MP') then
          tmp1 = arg1(1:4)
          call rltmp (tmp1)
        endif
      endif
      if (l2 .eq. 4) then
        if (arg2(1:2) .eq. 'MP') then
          tmp1 = arg2(1:4)
          call rltmp (tmp1)
        endif
      endif
      goto 120

 110  call errmes (50, 0)
      write (6, 3) uopr(iop)(1:lop(iop))
 3    format ('Operation ',a,' is not defined with these operands.')
      call abrt

 120  continue
c*
c      write (lu, *) 'exit gen itpx, lx, argx =', itpx, lx, ' ',
c     $  argx(1:lx)
c      call flush (lu)
c*
      return
      end

      subroutine genadd (lu, itp1, l1, arg1, itp2, l2, arg2, argx)

c   This generates code for an add operation.  The operands are in ARG1 and
c   ARG2, with types ITP1 and ITP2, and with lengths L1 and L2.  The result
c   name is in ARGX (also input).  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*80 arg1, arg2, argx
      character*4 tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, getmp

      if (itp1 .lt. 8) then
        if (itp1 .eq. 3) then

c   (Arg1 is DP) and (Arg2 is MP).

          tmp1 = getmp (9)
          if (itp2 .eq. 10) then
            write (lu, 1) arg2(1:l2), argx(1:4)
 1          format (6x,'CALL MPCEQ (MPNW4, ',a,', ',a,')')
          endif
          write (lu, 2) arg1(1:l1), tmp1
 2        format (6x,'CALL MPDMC (',a,', 0, ',a,')')
          write (lu, 3) tmp1, arg2(1:l2), argx(1:4)
 3        format (6x,'CALL MPADD (',a,', ',a,', ',a,')')
          call rltmp (tmp1)
        elseif (itp1 .eq. 1 .or. itp1 .eq. 2) then

c   (Arg1 is IN or SP) and (Arg2 is MP).

          tmp1 = getmp (3)
          tmp2 = getmp (9)
          if (itp2 .eq. 10) then
            write (lu, 1) arg2(1:l2), argx(1:4)
          endif
          write (lu, 4) tmp1, arg1(1:l1)
 4        format (6x,a,' = ',a)
          write (lu, 2) tmp1, tmp2
          write (lu, 3) tmp2, arg2(1:l2), argx(1:4)
          call rltmp (tmp1)
          call rltmp (tmp2)
        else

c   (Arg1 is CO or DC) and (Arg2 is MP).

          tmp1 = getmp (3)
          tmp2 = getmp (3)
          tmp3 = getmp (9)
          tmp4 = getmp (9)
          tmp5 = getmp (10)
          if (itp1 .eq. 5) then
            write (lu, 5) tmp1, arg1(1:l1)
 5          format (6x,a,' = DREAL (',a,')')
            write (lu, 6) tmp2, arg1(1:l1)
 6          format (6x,a,' = DIMAG (',a,')')
          else
            write (lu, 7) tmp1, arg1(1:l1)
 7          format (6x,a,' = REAL (',a,')')
            write (lu, 8) tmp2, arg1(1:l1)
 8          format (6x,a,' = AIMAG (',a,')')
          endif
          write (lu, 2) tmp1, tmp3
          write (lu, 2) tmp2, tmp4
          write (lu, 9) tmp3, tmp4, tmp5
 9        format (6x,'CALL MPMMPC (',a,', ',a,', MPNW4, ',a,')')
          if (itp2 .eq. 10) then
            write (lu, 10) tmp5, arg2(1:l2), argx(1:4)
 10         format (6x,'CALL MPCADD (MPNW4, ',a,', ',a,', ',a,')')
          else
            tmp6 = getmp (10)
            write (lu, 11) tmp3
 11         format (6x,'CALL MPDMC (0.D0, 0, ',a,')')
            write (lu, 9) arg2(1:l2), tmp3, tmp6
            write (lu, 10) tmp5, tmp6, argx(1:4)
            call rltmp (tmp6)
          endif
          call rltmp (tmp1)
          call rltmp (tmp2)
          call rltmp (tmp3)
          call rltmp (tmp4)
          call rltmp (tmp5)
        endif
      elseif (itp2 .lt. 8) then
        if (itp2 .eq. 3) then

c   (Arg1 is MP) and (Arg2 is DP).

          tmp1 = getmp (9)
          if (itp1 .eq. 10) then
            write (lu, 1) arg1(1:l1), argx(1:4)
          endif
          write (lu, 2) arg2(1:l2), tmp1
          write (lu, 3) arg1(1:l1), tmp1, argx(1:4)
          call rltmp (tmp1)
        elseif (itp2 .eq. 1 .or. itp2 .eq. 2) then

c   (Arg1 is MP) and (Arg2 is IN or SP).

          tmp1 = getmp (3)
          tmp2 = getmp (9)
          if (itp1 .eq. 10) then
            write (lu, 1) arg1(1:l1), argx(1:4)
          endif
          write (lu, 4) tmp1, arg2(1:l2)
          write (lu, 2) tmp1, tmp2
          write (lu, 3) arg1(1:l1), tmp2, argx(1:4)
          call rltmp (tmp1)
          call rltmp (tmp2)
        else

c   (Arg1 is MP) and (Arg2 is CO or DC).

          tmp1 = getmp (3)
          tmp2 = getmp (3)
          tmp3 = getmp (9)
          tmp4 = getmp (9)
          tmp5 = getmp (10)
          if (itp2 .eq. 5) then
            write (lu, 5) tmp1, arg2(1:l2)
            write (lu, 6) tmp2, arg2(1:l2)
          else
            write (lu, 7) tmp1, arg2(1:l2)
            write (lu, 8) tmp2, arg2(1:l2)
          endif
          write (lu, 2) tmp1, tmp3
          write (lu, 2) tmp2, tmp4
          write (lu, 9) tmp3, tmp4, tmp5
          if (itp1 .eq. 10) then
            write (lu, 10) arg1(1:l1), tmp5, argx(1:4)
          else
            tmp6 = getmp (10)
            write (lu, 11) tmp3
            write (lu, 9) arg1(1:l1), tmp3, tmp6
            write (lu, 10) tmp6, tmp5, argx(1:4)
            call rltmp (tmp6)
          endif
          call rltmp (tmp1)
          call rltmp (tmp2)
          call rltmp (tmp3)
          call rltmp (tmp4)
          call rltmp (tmp5)
        endif
      elseif (itp1 .ne. 10 .and. itp2 .ne. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is MPI or MPR).

        write (lina, 3) arg1(1:l1), arg2(1:l2), argx(1:4)
        la = l1 + l2 + 27
        call outlin (lu, 1, la, lina)
      elseif (itp1 .ne. 10 .and. itp2 .eq. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is MPC).

        tmp1 = getmp (9)
        tmp2 = getmp (10)
        write (lu, 11) tmp1
        write (lu, 9) arg1(1:l1), tmp1, tmp2
        write (lu, 10) tmp2, arg2(1:l2), argx(1:4)
        call rltmp (tmp1)
        call rltmp (tmp2)
      elseif (itp1 .eq. 10 .and. itp2 .ne. 10) then

c   (Arg1 is MPC) and (Arg1 is MPI or MPR).

        tmp1 = getmp (9)
        tmp2 = getmp (10)
        write (lu, 11) tmp1
        write (lu, 9) arg2(1:l2), tmp1, tmp2
        write (lu, 10) arg1(1:l1), tmp2, argx(1:4)
        call rltmp (tmp1)
        call rltmp (tmp2)
      else

c   (Arg1 is MPC) and (Arg2 is MPC).

        write (lina, 10) arg1(1:l1), arg2(1:l2), argx(1:4)
        la = l1 + l2 + 35
        call outlin (lu, 1, la, lina)
      endif

      return
      end

      subroutine genasn (lu, itp1, l1, arg1, itp2, l2, arg2)

c   This generates code for an assign operation.  The operands are in ARG1 and
c   ARG2, with types ITP1 and ITP2, and with lengths L1 and L2.  There is no
c   "result" temporary with assignments.  LU is the unit number for output
c   code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*80 arg1, arg2, argx
      character*16 nam, namq
      character*4 tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, getmp

      namq = 'MPNWP'
      iqnwp = itab (0, 0, namq)
      namq = 'DPPIC'
      iqpic = itab (0, 0, namq)
      iqd = iqpic - iqnwp + 1
      isc = iqnwp + 2 * iqd - 1

c   Check if Arg1 is an appropriate name for the result of an assignment
c   (i.e. it must not be special constant or a function name or parameter).

      k = index (arg1, '(')
      if (k .eq. 0) k = 100
      l = min (k - 1, l1, 16)
      nam = arg1(1:l)
      ix = itab (0, 0, nam)
      if (ix .gt. iqpic .and. ix .le. isc .and. lu .eq. 12) then
        ix = ix - iqd
        write (lu, 1) var(ix)(1:lvar(ix))
 1      format ('CMP*'/'CMP*  The parameter definition of this special',
     $    ' constant is ignored: ',a/'CMP*')
        goto 100
      elseif (ktyp(ix) .le. 8 .and. lu .eq. 12) then
        call errmes (107, 0)
        write (6, 2) nam(1:l)
 2      format ('Non-MP parameters may not be defined in a MP',
     $    ' parameter statement: ',a)
        call abrt
      elseif (ix .le. mint .or. kdim(ix) .lt. 0) then
        call errmes (51, 0)
        write (6, 3) nam(1:l)
 3      format ('This name may not appear on the LHS of an assignment',
     $    ' statement: ',a)
        call abrt
      elseif (lu .eq. 12) then

c   If this is a MP parameter definition, set KDIM(IX) to -3 so that it can't
c   be stored into again.

        kdim(ix) = -3
      endif

      if (itp1 .lt. 8) then
        if (itp2 .lt. 8) then

c   (LHS is non-MP) and (RHS is non-MP).

          write (lina, 4) arg1(1:l1), arg2(1:l2)
 4        format (6x,a,' = ',a)
          la = l1 + l2 + 9
          call outlin (lu, 1, la, lina)
        elseif (itp1 .le. 3 .or. itp2 .ne. 10) then

c   (LHS is IN, SP or DP) or (RHS is MPI or MPR).  At least one is not complex.

          tmp1 = getmp (3)
          tmp2 = getmp (1)
          write (lu, 5) arg2(1:l2), tmp1, tmp2
 5        format (6x,'CALL MPMDC (',a,', ',a,', ',a,')')
          write (lu, 6) arg1(1:l1), tmp1, tmp2
 6        format (6x,a,' = ',a,' * 2.D0 ** ',a)
          call rltmp (tmp1)
          call rltmp (tmp2)
        else

c   (LHS is CO or DC) and (RHS is MPC).

          tmp1 = getmp (9)
          tmp2 = getmp (9)
          tmp3 = getmp (3)
          tmp4 = getmp (3)
          tmp5 = getmp (1)
          write (lu, 7) arg2(1:l2), tmp1, tmp2
 7        format (6x,'CALL MPMPCM (MPNW4, ',a,', ',a,', ',a,')')
          write (lu, 5) tmp1, tmp3, tmp5
          write (lu, 6) tmp3, tmp3, tmp5
          write (lu, 5) tmp2, tmp4, tmp5
          write (lu, 6) tmp4, tmp4, tmp5
          write (lu, 8) arg1(1:l1), tmp3, tmp4
 8        format (6x,a,' = DCMPLX (',a,', ',a,')')
          call rltmp (tmp1)
          call rltmp (tmp2)
          call rltmp (tmp3)
          call rltmp (tmp4)
          call rltmp (tmp5)
        endif
      elseif (itp2 .lt. 8) then
        if (itp2 .eq. 3) then

c   (LHS is MP) and (RHS is DP).

          write (lina, 9) arg2(1:l2), arg1(1:l1)
 9        format (6x,'CALL MPDMC (',a,', 0, ',a,')')
          la = l1 + l2 + 24
          call outlin (lu, 1, la, lina)
        elseif (itp1 .ne. 10 .or. itp2 .eq. 1 .or. itp2 .eq. 2) then

c   (LHS is MP) or (RHS is IN or SP).  At least one is not complex.

          tmp1 = getmp (3)
          write (lu, 4) tmp1, arg2(1:l2)
          write (lu, 9) tmp1, arg1(1:l1)
          call rltmp (tmp1)
        else

c   (LHS is MPC) and (RHS is CO or DC).

          tmp1 = getmp (3)
          tmp2 = getmp (3)
          tmp3 = getmp (9)
          tmp4 = getmp (9)
          if (itp2 .eq. 5) then
            write (lu, 10) tmp1, arg2(1:l2)
 10         format (6x,a,' = DREAL (',a,')')
            write (lu, 11) tmp2, arg2(1:l2)
 11         format (6x,a,' = DIMAG (',a,')')
          else
            write (lu, 12) tmp1, arg2(1:l2)
 12         format (6x,a,' = REAL (',a,')')
            write (lu, 13) tmp2, arg2(1:l2)
 13         format (6x,a,' = AIMAG (',a,')')
          endif
          write (lu, 9) tmp1, tmp3
          write (lu, 9) tmp2, tmp4
          write (lu, 14) tmp3, tmp4, arg1(1:l1)
 14       format (6x,'CALL MPMMPC (',a,', ',a,', MPNW4, ',a,')')
          call rltmp (tmp1)
          call rltmp (tmp2)
          call rltmp (tmp3)
          call rltmp (tmp4)
        endif
      elseif (itp1 .ne. 10 .or. itp2 .ne. 10) then

c   (LHS is MPI or MPR) or (RHS is MPI or MPR).  At least one is not MPC.

        write (lina, 15) arg2(1:l2), arg1(1:l1)
 15     format (6x,'CALL MPEQ (',a,', ',a,')')
        la = l1 + l2 + 20
        call outlin (lu, 1, la, lina)
      else

c   (LHS is MPC) and (RHS is MPC).

        write (lina, 16) arg2(1:l2), arg1(1:l1)
 16     format (6x,'CALL MPCEQ (MPNW4, ',a,', ',a,')')
        la = l1 + l2 + 28
        call outlin (lu, 1, la, lina)
      endif
      if (itp1 .eq. 8 .and. itp2 .ne. 1 .and. itp2 .ne. 8) then

c   Truncate the result if (LHS is MPI) and (RHS is not IN or MPI).

        tmp1 = getmp (9)
        write (lina, 17) arg1(1:l1), arg1(1:l1), tmp1
 17     format (6x,'CALL MPINFR (',a,', ',a,', ',a,')')
        la = 2 * l1 + 28
        call outlin (lu, 1, la, lina)
        call rltmp (tmp1)
      elseif (itp1 .eq. 10 .and. itp2 .ne. 4 .and. itp2 .ne. 5 .and.
     $    itp2 .ne. 10) then

c   Zero the imaginary part if (LHS is MPC) and (RHS is not CO or DC or MPC).

        tmp1 = getmp (9)
        write (lu, 18) tmp1
 18     format (6x,'CALL MPDMC (0.D0, 0, ',a,')')
        write (lina, 14) arg1(1:l1), tmp1, arg1(1:l1)
        la = 2 * l1 + 35
        call outlin (lu, 1, la, lina)
        call rltmp (tmp1)
      endif

 100  return
      end

      subroutine gencon (lu, i1, i2, lina, itpy, ly, argy)

c   This generates the code for a MPR constant.  I1 and I2 are the indices
c   of LINA delimiting the constant.  The output MP temporary is placed in
c   ARGY, with length LY.  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb
      character*80 argy
      character*4 num, tmp1, tmp2, tmp3, getmp

      tmp1 = getmp (11)
      tmp2 = getmp (6)
      tmp3 = getmp (1)
      ly = 4
      argy(1:4) = getmp (itpy)
      la = i2 - i1 + 1
      l2 = max (index (lina(i1:i2), 'D'), index (lina(i1:i2), 'd'),
     $  index (lina(i1:i2), 'E'), index (lina(i1:i2), 'e')) - 1
      if (l2 .ge. 0) then
        l1 = la - l2 - 1
      else
        l1 = 0
        l2 = la
      endif
      linb(1:6) = ' '
      linb(7:10) = tmp1
      linb(11:17) = ' = ''10^'
      if (l1 .gt. 0) then
        linb(18:l1+17) = lina(i1+l2+1:i2)
      else
        l1 = 1
        linb(18:18) = '0'
      endif
      linb(l1+18:l1+20) = ' x '
      linb(l1+21:l1+l2+20) = lina(i1:i1+l2-1)
      linb(l1+l2+21:l1+l2+21) = ''''
      l = l1 + l2 + 21
      call outlin (lu, 1, l, linb)
      ls = l1 + l2 + 6
      lsm = max (lsm, ls)
      write (num, '(I3)') ls
      write (lu, 1) tmp1, num(1:3), tmp2, tmp3, tmp3, num(1:3)
 1    format (6x,'READ (',a,', ''(',a,'A1)'' ) (',a,'(',a,'), ',a,
     $  ' = 1, ',a,')')
      write (lu, 2) tmp2, ls, argy(1:4)
 2    format (6x,'CALL MPINPC (',a,', ',i3,', ',a,')')
      call rltmp (tmp3)

      return
      end

      subroutine gencpr (lu, itp1, l1, arg1, itp2, l2, arg2, iop, argx)

c   This generates code for a compare operation.  The operands are in ARG1 and
c   ARG2, with types ITP1 and ITP2, and with lengths L1 and L2.  The result
c   name is in ARGX (also input).  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*80 arg1, arg2, argx
      character*8 andor
      character*4 tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, getmp

c   If one of the operands has a complex type, only .EQ. and .NE. are allowed.

      if (itp1 .eq. 4 .or. itp1 .eq. 5 .or. itp1 .eq. 10 .or.
     $  itp2 .eq. 4 .or. itp2 .eq. 5 .or. itp2 .eq. 10) then
        if (iop .ge. 9) goto 100
        if (iop .eq. 7) then
          andor = '.AND.'
        else
          andor = '.OR.'
        endif
      endif

      if (itp1 .lt. 8) then
        if (itp1 .eq. 3) then
          if (itp2 .ne. 10) then

c   (Arg1 is DP) and (Arg2 is MPI or MPR).

            tmp1 = getmp (9)
            tmp2 = getmp (1)
            write (lu, 1) arg1(1:l1), tmp1
 1          format (6x,'CALL MPDMC (',a,', 0, ',a,')')
            write (lu, 2) tmp1, arg2(1:l2), tmp2
 2          format (6x,'CALL MPCPR (',a,', ',a,', ',a,')')
            write (lu, 3) argx(1:4), tmp2, uopr(iop)(1:lop(iop))
 3          format (6x,a,' = ',a,' ',a,' 0')
            call rltmp (tmp1)
            call rltmp (tmp2)
          else

c   (Arg1 is DP) and (Arg2 is MPC).

            tmp1 = getmp (9)
            tmp2 = getmp (9)
            tmp3 = getmp (9)
            tmp4 = getmp (9)
            tmp5 = getmp (1)
            tmp6 = getmp (1)
            write (lu, 1) arg1(1:l1), tmp1
            write (lu, 4) tmp2
 4          format (6x,'CALL MPDMC (0.D0, 0, ',a,')')
            write (lu, 5) arg2(1:l2), tmp3, tmp4
 5          format (6x,'CALL MPMPCM (MPNW4, ',a,', ',a,', ',a,')')
            write (lu, 2) tmp1, tmp3, tmp5
            write (lu, 2) tmp2, tmp4, tmp6
            write (lu, 6) argx(1:4), tmp5, uopr(iop)(1:lop(iop)),
     $       andor(1:5), tmp6, uopr(iop)(1:lop(iop))
 6          format (6x,a,' = ',a,' ',a,' 0 ',a,' ',a,' ',a,' 0')
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
            call rltmp (tmp4)
            call rltmp (tmp5)
            call rltmp (tmp6)
          endif
        elseif (itp1 .eq. 1 .or. itp1 .eq. 2) then
          if (itp2 .ne. 10) then

c   (Arg1 is IN or SP) and (Arg2 is MPI or MPR).

            tmp1 = getmp (3)
            tmp2 = getmp (9)
            tmp3 = getmp (1)
            write (lu, 7) tmp1, arg1(1:l1)
 7          format (6x,a,' = ',a)
            write (lu, 1) tmp1, tmp2
            write (lu, 2) tmp2, arg2(1:l2), tmp3
            write (lu, 3) argx(1:4), tmp3, uopr(iop)(1:lop(iop))
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
          else

c   (Arg1 is IN or SP) and (Arg2 is MPC).

            tmp1 = getmp (3)
            tmp2 = getmp (9)
            tmp3 = getmp (9)
            tmp4 = getmp (9)
            tmp5 = getmp (9)
            tmp6 = getmp (1)
            tmp7 = getmp (1)
            write (lu, 7) tmp1, arg1(1:l1)
            write (lu, 1) tmp1, tmp2
            write (lu, 4) tmp3
            write (lu, 5) arg2(1:l2), tmp4, tmp5
            write (lu, 2) tmp2, tmp4, tmp6
            write (lu, 2) tmp3, tmp5, tmp7
            write (lu, 6) argx(1:4), tmp6, uopr(iop)(1:lop(iop)),
     $       andor(1:5), tmp7, uopr(iop)(1:lop(iop))
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
            call rltmp (tmp4)
            call rltmp (tmp5)
            call rltmp (tmp6)
            call rltmp (tmp7)
          endif
        elseif (itp1 .eq. 4 .or. itp1 .eq. 5) then
          if (itp2 .ne. 10) then

c   (Arg1 is CO or DC) and (Arg2 is MPI or MPR).

            tmp1 = getmp (3)
            tmp2 = getmp (3)
            tmp3 = getmp (9)
            tmp4 = getmp (1)
            if (itp1 .eq. 5) then
              write (lu, 8) tmp1, arg1(1:l1)
 8            format (6x,a,' = DREAL (',a,')')
              write (lu, 9) tmp2, arg1(1:l1)
 9            format (6x,a,' = DIMAG (',a,')')
            else
              write (lu, 10) tmp1, arg1(1:l1)
 10           format (6x,a,' = REAL (',a,')')
              write (lu, 11) tmp2, arg1(1:l1)
 11           format (6x,a,' = AIMAG (',a,')')
            endif
            write (lu, 1) tmp1, tmp3
            write (lu, 2) tmp3, arg2(1:l2), tmp4
            write (lu, 6) argx(1:4), tmp4, uopr(iop)(1:lop(iop)),
     $       andor(1:5), tmp2, uopr(iop)(1:lop(iop))
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
            call rltmp (tmp4)
          else

c   (Arg1 is CO or DC) and (Arg2 is MPC).

            tmp1 = getmp (3)
            tmp2 = getmp (3)
            tmp3 = getmp (9)
            tmp4 = getmp (9)
            tmp5 = getmp (9)
            tmp6 = getmp (9)
            tmp7 = getmp (1)
            tmp8 = getmp (1)
            if (itp1 .eq. 5) then
              write (lu, 8) tmp1, arg1(1:l1)
              write (lu, 9) tmp2, arg1(1:l1)
            else
              write (lu, 10) tmp1, arg1(1:l1)
              write (lu, 11) tmp2, arg1(1:l1)
            endif
            write (lu, 1) tmp1, tmp3
            write (lu, 1) tmp2, tmp4
            write (lu, 5) arg2(1:l2), tmp5, tmp6
            write (lu, 2) tmp3, tmp5, tmp7
            write (lu, 2) tmp4, tmp6, tmp8
            write (lu, 6) argx(1:4), tmp7, uopr(iop)(1:lop(iop)),
     $       andor(1:5), tmp8, uopr(iop)(1:lop(iop))
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
            call rltmp (tmp4)
            call rltmp (tmp5)
            call rltmp (tmp6)
            call rltmp (tmp7)
            call rltmp (tmp8)
          endif
        endif
      elseif (itp2 .lt. 8) then
        if (itp2 .eq. 3) then
          if (itp1 .ne. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is DP).

            tmp1 = getmp (9)
            tmp2 = getmp (1)
            write (lu, 1) arg2(1:l2), tmp1
            write (lu, 2) arg1(1:l1), tmp1, tmp2
            write (lu, 3) argx(1:4), tmp2, uopr(iop)(1:lop(iop))
            call rltmp (tmp1)
            call rltmp (tmp2)
          else

c   (Arg1 is MPC) and (Arg2 is DP).

            tmp1 = getmp (9)
            tmp2 = getmp (9)
            tmp3 = getmp (9)
            tmp4 = getmp (9)
            tmp5 = getmp (1)
            tmp6 = getmp (1)
            write (lu, 1) arg2(1:l2), tmp1
            write (lu, 4) tmp2
            write (lu, 5) arg1(1:l1), tmp3, tmp4
            write (lu, 2) tmp3, tmp1, tmp5
            write (lu, 2) tmp4, tmp2, tmp6
            write (lu, 6) argx(1:4), tmp5, uopr(iop)(1:lop(iop)),
     $       andor(1:5), tmp6, uopr(iop)(1:lop(iop))
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
            call rltmp (tmp4)
            call rltmp (tmp5)
            call rltmp (tmp6)
          endif
        elseif (itp2 .eq. 1 .or. itp2 .eq. 2) then
          if (itp1 .ne. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is IN or SP).

            tmp1 = getmp (3)
            tmp2 = getmp (9)
            tmp3 = getmp (1)
            write (lu, 7) tmp1, arg2(1:l2)
            write (lu, 1) tmp1, tmp2
            write (lu, 2) arg1(1:l1), tmp2, tmp3
            write (lu, 3) argx(1:4), tmp3, uopr(iop)(1:lop(iop))
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
          else

c   (Arg1 is MPC) and (Arg2 is IN or SP).

            tmp1 = getmp (3)
            tmp2 = getmp (9)
            tmp3 = getmp (9)
            tmp4 = getmp (9)
            tmp5 = getmp (9)
            tmp6 = getmp (1)
            tmp7 = getmp (1)
            write (lu, 7) tmp1, arg2(1:l2)
            write (lu, 1) tmp1, tmp2
            write (lu, 4) tmp3
            write (lu, 5) arg1(1:l1), tmp4, tmp5
            write (lu, 2) tmp4, tmp2, tmp6
            write (lu, 2) tmp5, tmp3, tmp7
            write (lu, 6) argx(1:4), tmp6, uopr(iop)(1:lop(iop)),
     $       andor(1:5), tmp7, uopr(iop)(1:lop(iop))
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
            call rltmp (tmp4)
            call rltmp (tmp5)
            call rltmp (tmp6)
            call rltmp (tmp7)
          endif
        elseif (itp2 .eq. 4 .or. itp2 .eq. 5) then
          if (itp1 .ne. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is CO or DC).

            tmp1 = getmp (3)
            tmp2 = getmp (3)
            tmp3 = getmp (9)
            tmp4 = getmp (1)
            if (itp2 .eq. 5) then
              write (lu, 8) tmp1, arg2(1:l2)
              write (lu, 9) tmp2, arg2(1:l2)
            else
              write (lu, 10) tmp1, arg2(1:l2)
              write (lu, 11) tmp2, arg2(1:l2)
            endif
            write (lu, 1) tmp1, tmp3
            write (lu, 2) arg1(1:l1), tmp3, tmp4
            write (lu, 6) argx(1:4), tmp4, uopr(iop)(1:lop(iop)),
     $       andor(1:5), tmp2, uopr(iop)(1:lop(iop))
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
            call rltmp (tmp4)
          else

c   (Arg1 is MPC) and (Arg2 is CO or DC).

            tmp1 = getmp (3)
            tmp2 = getmp (3)
            tmp3 = getmp (9)
            tmp4 = getmp (9)
            tmp5 = getmp (9)
            tmp6 = getmp (9)
            tmp7 = getmp (1)
            tmp8 = getmp (1)
            if (itp2 .eq. 5) then
              write (lu, 8) tmp1, arg2(1:l2)
              write (lu, 9) tmp2, arg2(1:l2)
            else
              write (lu, 10) tmp1, arg2(1:l2)
              write (lu, 11) tmp2, arg2(1:l2)
            endif
            write (lu, 1) tmp1, tmp3
            write (lu, 1) tmp2, tmp4
            write (lu, 5) arg1(1:l1), tmp5, tmp6
            write (lu, 2) tmp5, tmp3, tmp7
            write (lu, 2) tmp6, tmp4, tmp8
            write (lu, 6) argx(1:4), tmp7, uopr(iop)(1:lop(iop)),
     $       andor(1:5), tmp8, uopr(iop)(1:lop(iop))
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
            call rltmp (tmp4)
            call rltmp (tmp5)
            call rltmp (tmp6)
            call rltmp (tmp7)
            call rltmp (tmp8)
          endif
        endif
      elseif (itp1 .ne. 10 .and. itp2 .ne. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is MPI or MPR).

        tmp1 = getmp (1)
        write (lina, 2) arg1(1:l1), arg2(1:l2), tmp1
        la = l1 + l2 + 27
        call outlin (lu, 1, la, lina)
        write (lu, 3) argx(1:4), tmp1, uopr(iop)(1:lop(iop))
        call rltmp (tmp1)
      elseif (itp1 .eq. 10 .and. itp2 .ne. 10) then

c   (Arg1 is MPC) and (Arg2 is MPI or MPR).

        tmp1 = getmp (9)
        tmp2 = getmp (9)
        tmp3 = getmp (9)
        tmp4 = getmp (1)
        tmp5 = getmp (1)
        write (lu, 5) arg1(1:l1), tmp1, tmp2
        write (lu, 4) tmp3
        write (lu, 2) tmp1, arg2(1:l2), tmp4
        write (lu, 2) tmp2, tmp3, tmp5
        write (lu, 6) argx(1:4), tmp4, uopr(iop)(1:lop(iop)),
     $   andor(1:5), tmp5, uopr(iop)(1:lop(iop))
        call rltmp (tmp1)
        call rltmp (tmp2)
        call rltmp (tmp3)
        call rltmp (tmp4)
        call rltmp (tmp5)
      elseif (itp1 .ne. 10 .and. itp2 .eq. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is MPC).

        tmp1 = getmp (9)
        tmp2 = getmp (9)
        tmp3 = getmp (9)
        tmp4 = getmp (1)
        tmp5 = getmp (1)
        write (lu, 5) arg2(1:l2), tmp1, tmp2
        write (lu, 4) tmp3
        write (lu, 2) arg1(1:l1), tmp1, tmp4
        write (lu, 2) tmp3, tmp2, tmp5
        write (lu, 6) argx(1:4), tmp4, uopr(iop)(1:lop(iop)),
     $   andor(1:5), tmp5, uopr(iop)(1:lop(iop))
        call rltmp (tmp1)
        call rltmp (tmp2)
        call rltmp (tmp3)
        call rltmp (tmp4)
        call rltmp (tmp5)
      else

c   (Arg1 is MPC) AND (Arg2 is MPC).

        tmp1 = getmp (9)
        tmp2 = getmp (9)
        tmp3 = getmp (9)
        tmp4 = getmp (9)
        tmp5 = getmp (1)
        tmp6 = getmp (1)
        write (lu, 5) arg1(1:l1), tmp1, tmp2
        write (lu, 5) arg2(1:l2), tmp3, tmp4
        write (lu, 2) tmp1, tmp3, tmp5
        write (lu, 2) tmp2, tmp4, tmp6
        write (lu, 6) argx(1:4), tmp5, uopr(iop)(1:lop(iop)),
     $   andor(1:5), tmp6, uopr(iop)(1:lop(iop))
        call rltmp (tmp1)
        call rltmp (tmp2)
        call rltmp (tmp3)
        call rltmp (tmp4)
        call rltmp (tmp5)
        call rltmp (tmp6)
      endif
      goto 110


 100  call errmes (52, 0)
      write (6, 12) uopr(iop)(1:lop(iop))
 12   format ('Operation ',a,' is not defined with these operands.')
      call abrt

 110  return
      end

      subroutine gendiv (lu, itp1, l1, arg1, itp2, l2, arg2, argx)

c   This generates code for a divide operation.  The operands are in ARG1 and
c   ARG2, with types ITP1 and ITP2, and with lengths L1 and L2.  The result
c   name is in ARGX (also input).  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*80 arg1, arg2, argx
      character*4 tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, getmp

      if (itp1 .lt. 8) then
        if (itp1 .eq. 3) then
          if (itp2 .ne. 10) then

c   (Arg1 is DP) and (Arg2 is MPI or MPR).

            tmp1 = getmp (9)
            write (lu, 1) arg1(1:l1), tmp1
 1          format (6x,'CALL MPDMC (',a,', 0, ',a,')')
            write (lu, 2) tmp1, arg2(1:l2), argx(1:4)
 2          format (6x,'CALL MPDIV (',a,', ',a,', ',a,')')
            call rltmp (tmp1)
          else

c   (Arg1 is DP) and (Arg2 is MPC).

            tmp1 = getmp (9)
            tmp2 = getmp (9)
            tmp3 = getmp (10)
            write (lu, 1) arg1(1:l1), tmp1
            write (lu, 3) tmp2
 3          format (6x,'CALL MPDMC (0.D0, 0, ',a,')')
            write (lu, 4) tmp1, tmp2, tmp3
 4          format (6x,'CALL MPMMPC (',a,', ',a,', MPNW4, ',a,')')
            write (lu, 5) tmp3, arg2(1:l2), argx(1:4)
 5          format (6x,'CALL MPCDIV (MPNW4, ',a,', ',a,', ',a,')')
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
          endif
        elseif (itp1 .eq. 1 .or. itp1 .eq. 2) then
          if (itp2 .ne. 10) then

c   (Arg1 is IN or SP) and (Arg2 is MPI or MPR).

            tmp1 = getmp (3)
            tmp2 = getmp (9)
            write (lu, 6) tmp1, arg1(1:l1)
 6          format (6x,a,' = ',a)
            write (lu, 1) tmp1, tmp2
            write (lu, 2) tmp2, arg2(1:l2), argx(1:4)
            call rltmp (tmp1)
            call rltmp (tmp2)
          else

c   (Arg1 is IN or SP) and (Arg2 is MPC).

            tmp1 = getmp (3)
            tmp2 = getmp (9)
            tmp3 = getmp (9)
            tmp4 = getmp (10)
            write (lu, 6) tmp1, arg1(1:l1)
            write (lu, 1) tmp1, tmp2
            write (lu, 3) tmp3
            write (lu, 4) tmp2, tmp3, tmp4
            write (lu, 5) tmp4, arg2(1:l2), argx(1:4)
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
            call rltmp (tmp4)
          endif
        elseif (itp1 .eq. 4 .or. itp1 .eq. 5) then
          if (itp2 .ne. 10) then

c   (Arg1 is CO or DP) and (Arg2 is MPI or MPR).

            tmp1 = getmp (3)
            tmp2 = getmp (3)
            tmp3 = getmp (9)
            tmp4 = getmp (9)
            if (itp1 .eq. 5) then
              write (lu, 7) tmp1, arg1(1:l1)
 7            format (6x,a,' = DREAL (',a,')')
              write (lu, 8) tmp2, arg1(1:l1)
 8            format (6x,a,' = DIMAG (',a,')')
            else
              write (lu, 9) tmp1, arg1(1:l1)
 9            format (6x,a,' = REAL (',a,')')
              write (lu, 10) tmp2, arg1(1:l1)
 10           format (6x,a,' = AIMAG (',a,')')
            endif
            write (lu, 1) tmp1, tmp3
            write (lu, 1) tmp2, tmp4
            write (lu, 2) tmp3, arg2(1:l2), argx(1:4)
            write (lu, 11) tmp4, arg2(1:l2), argx(1:4)
 11         format (6x,'CALL MPDIV (',a,', ',a,', ',a,'(MPNWQ+5))')
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
            call rltmp (tmp4)
          else

c   (Arg1 is CO or DP) and (Arg2 is MPC).

            tmp1 = getmp (3)
            tmp2 = getmp (3)
            tmp3 = getmp (9)
            tmp4 = getmp (9)
            tmp5 = getmp (10)
            if (itp1 .eq. 5) then
              write (lu, 7) tmp1, arg1(1:l1)
              write (lu, 8) tmp2, arg1(1:l1)
            else
              write (lu, 9) tmp1, arg1(1:l1)
              write (lu, 10) tmp2, arg1(1:l1)
            endif
            write (lu, 1) tmp1, tmp3
            write (lu, 1) tmp2, tmp4
            write (lu, 4) tmp3, tmp4, tmp5
            write (lu, 12) tmp5, arg2(1:l2), argx(1:4)
 12         format (6x,'CALL MPCDIV (MPNW4, ',a,', ',a,', ',a,')')
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
            call rltmp (tmp4)
            call rltmp (tmp5)
          endif
        endif
      elseif (itp2 .lt. 8) then
        if (itp2 .eq. 3) then
          if (itp1 .ne. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is DP).

            write (lina, 13) arg1(1:l1), arg2(1:l2), argx(1:4)
 13         format (6x,'CALL MPDIVD (',a,', ',a,', 0, ',a,')')
            la = l1 + l2 + 31
            call outlin (lu, 1, la, lina)
          else

c   (Arg1 is MPC) and (Arg2 is DP).

            tmp1 = getmp (9)
            tmp2 = getmp (9)
            write (lu, 14) arg1(1:l1), tmp1, tmp2
 14         format (6x,'CALL MPMPCM (MPNW4, ',a,', ',a,', ',a,')')
            write (lu, 13) tmp1, arg2(1:l2), argx(1:4)
            write (lu, 15) tmp2, arg2(1:l2), argx(1:4)
 15         format (6x,'CALL MPDIVD (',a,', ',a,', 0, ',a,
     $        '(MPNWQ+5))')
            call rltmp (tmp1)
            call rltmp (tmp2)
          endif
        elseif (itp2 .eq. 1 .or. itp2 .eq. 2) then
          if (itp1 .ne. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is IN or SP).

            tmp1 = getmp (3)
            write (lu, 6) tmp1, arg2(1:l2)
            write (lu, 13) arg1(1:l1), tmp1, argx(1:4)
            call rltmp (tmp1)
          else

c   (Arg1 is MPC) and (Arg2 is IN or SP).

            tmp1 = getmp (3)
            tmp2 = getmp (9)
            tmp3 = getmp (9)
            write (lu, 6) tmp1, arg2(1:l2)
            write (lu, 14) arg1(1:l1), tmp2, tmp3
            write (lu, 13) tmp2, tmp1, argx(1:4)
            write (lu, 15) tmp3, tmp1, argx(1:4)
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
          endif
        elseif (itp2 .eq. 4 .or. itp2 .eq. 5) then
          if (itp1 .ne. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is CO or DC).

            tmp1 = getmp (3)
            tmp2 = getmp (3)
            tmp3 = getmp (9)
            tmp4 = getmp (9)
            tmp5 = getmp (10)
            tmp6 = getmp (10)
            if (itp2 .eq. 5) then
              write (lu, 7) tmp1, arg2(1:l2)
              write (lu, 8) tmp2, arg2(1:l2)
            else
              write (lu, 9) tmp1, arg2(1:l2)
              write (lu, 10) tmp2, arg2(1:l2)
            endif
            write (lu, 1) tmp1, tmp3
            write (lu, 1) tmp2, tmp4
            write (lu, 4) tmp3, tmp4, tmp5
            write (lu, 3) tmp3
            write (lu, 4) arg1(1:l1), tmp3, tmp6
            write (lu, 12) tmp6, tmp5, argx(1:4)
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
            call rltmp (tmp4)
            call rltmp (tmp5)
            call rltmp (tmp6)
          else

c   (Arg1 is MPC) and (Arg2 is CO or DC).

            tmp1 = getmp (3)
            tmp2 = getmp (3)
            tmp3 = getmp (9)
            tmp4 = getmp (9)
            tmp5 = getmp (10)
            if (itp2 .eq. 5) then
              write (lu, 7) tmp1, arg2(1:l2)
              write (lu, 8) tmp2, arg2(1:l2)
            else
              write (lu, 9) tmp1, arg2(1:l2)
              write (lu, 10) tmp2, arg2(1:l2)
            endif
            write (lu, 1) tmp1, tmp3
            write (lu, 1) tmp2, tmp4
            write (lu, 4) tmp3, tmp4, tmp5
            write (lu, 12) arg1(1:l1), tmp5, argx(1:4)
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
            call rltmp (tmp4)
            call rltmp (tmp5)
          endif
        endif
      elseif (itp1 .ne. 10 .and. itp2 .ne. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is MPI or MPR).

        write (lina, 2) arg1(1:l1), arg2(1:l2), argx(1:4)
        la = l1 + l2 + 27
        call outlin (lu, 1, la, lina)
      elseif (itp1 .eq. 10 .and. itp2 .ne. 10) then

c   (Arg1 is MPC) and (Arg2 is MPI or MPR).

        tmp1 = getmp (9)
        tmp2 = getmp (9)
        write (lu, 14) arg1(1:l1), tmp1, tmp2
        write (lu, 2) tmp1, arg2(1:l2), argx(1:4)
        write (lu, 11) tmp2, arg2(1:l2), argx(1:4)
        call rltmp (tmp1)
        call rltmp (tmp2)
      elseif (itp1 .ne. 10 .and. itp2 .eq. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is MPC).

        tmp1 = getmp (9)
        tmp2 = getmp (10)
        write (lu, 3) tmp1
        write (lu, 4) arg1(1:l1), tmp1, tmp2
        write (lu, 12) tmp2, arg2(1:l2), argx(1:4)
        call rltmp (tmp1)
        call rltmp (tmp2)
      elseif (itp1 .eq. 10 .and. itp2 .eq. 10) then

c   (Arg1 is MPC) and (Arg2 is MPC).

        write (lina, 12) arg1(1:l1), arg2(1:l2), argx(1:4)
        la = l1 + l2 + 35
        call outlin (lu, 1, la, lina)
      endif

      return
      end

      subroutine genexp (lu, itp1, l1, arg1, itp2, l2, arg2, argx)

c   This generates code for an exponentiation.  The operands are in ARG1 and
c   ARG2, with types ITP1 and ITP2, and with lengths L1 and L2.  The result
c   name is in ARGX (also input).  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*80 arg1, arg2, argx
      character*4 tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, getmp

      iop = 4
      if (itp1 .lt. 8) then
        if (itp1 .eq. 3 .and. itp2 .eq. 8) then

c   (Arg1 is DP) and (Arg2 is MPI).

          kcon(3) = 1
          tmp1 = getmp (9)
          tmp2 = getmp (8)
          tmp3 = getmp (3)
          tmp4 = getmp (1)
          tmp5 = getmp (9)
          tmp6 = getmp (9)
          write (lu, 1) arg1(1:l1), tmp1
 1        format (6x,'CALL MPDMC (',a,', 0, ',a,')')
          write (lu, 2) arg2(1:l2), tmp2
 2        format (6x,'CALL MPEQ (',a,', ',a,')')
          write (lu, 3) tmp2, tmp2
 3        format (6x,'IF (',a,'(2) .EQ. 0. .OR. ',a,'(2) .EQ. 1) THEN')
          write (lu, 4) tmp2, tmp3, tmp4
 4        format (6x,'CALL MPMDC (',a,', ',a,', ',a,')')
          write (lu, 5) tmp4, tmp3, tmp4
 5        format (6x,a,' = NINT (',a,' * 2.D0 ** ',a,')')
          write (lu, 6) tmp1, tmp4, argx(1:4)
 6        format (6x,'CALL MPNPWR (',a,', ',a,', ',a,')')
          write (lu, 7)
 7        format (6x,'ELSE')
          write (lu, 8) tmp1, tmp5
 8        format (6x,'CALL MPLOG (',a,', MPL02, ',a,')')
          write (lu, 9) arg2(1:l2), tmp5, tmp6
 9        format (6x,'CALL MPMUL (',a,', ',a,', ',a,')')
          write (lu, 10) tmp6, argx(1:4)
 10       format (6x,'CALL MPEXP (',a,', MPL02, ',a,')')
          write (lu, 11)
 11       format (6x,'ENDIF')
          call rltmp (tmp1)
          call rltmp (tmp2)
          call rltmp (tmp3)
          call rltmp (tmp4)
          call rltmp (tmp5)
          call rltmp (tmp6)
        elseif (itp1 .eq. 3 .and. itp2 .eq. 9) then

c   (Arg1 is DP) and (Arg2 is MPR).

          kcon(3) = 1
          tmp1 = getmp (9)
          tmp2 = getmp (9)
          tmp3 = getmp (9)
          write (lu, 1) arg1(1:l1), tmp1
          write (lu, 8) tmp1, tmp2
          write (lu, 9) arg2(1:l2), tmp2, tmp3
          write (lu, 10) tmp3, argx(1:4)
          call rltmp (tmp1)
          call rltmp (tmp2)
          call rltmp (tmp3)
        elseif ((itp1 .eq. 1 .or. itp1 .eq. 2) .and. itp2 .eq. 8) then

c   (Arg1 is IN or SP) and (Arg2 is MPI).

          kcon(3) = 1
          tmp1 = getmp (9)
          tmp2 = getmp (8)
          tmp3 = getmp (3)
          tmp4 = getmp (1)
          tmp5 = getmp (9)
          tmp6 = getmp (9)
          write (lu, 12) tmp3, arg1(1:l1)
 12       format (6x,a,' = ',a)
          write (lu, 1) tmp3, tmp1
          write (lu, 2) arg2(1:l2), tmp2
          write (lu, 3) tmp2, tmp2
          write (lu, 4) tmp2, tmp3, tmp4
          write (lu, 5) tmp4, tmp3, tmp4
          write (lu, 6) tmp1, tmp4, argx(1:4)
          write (lu, 7)
          write (lu, 8) tmp1, tmp5
          write (lu, 9) arg2(1:l2), tmp5, tmp6
          write (lu, 10) tmp6, argx(1:4)
          write (lu, 11)
          call rltmp (tmp1)
          call rltmp (tmp2)
          call rltmp (tmp3)
          call rltmp (tmp4)
          call rltmp (tmp5)
          call rltmp (tmp6)
        elseif ((itp1 .eq. 1 .or. itp1 .eq. 2) .and. itp2 .eq. 9) then

c   (Arg1 is IN or SP) and (Arg2 is MPI or MPR).

          kcon(3) = 1
          tmp1 = getmp (3)
          tmp2 = getmp (9)
          tmp3 = getmp (9)
          tmp4 = getmp (9)
          write (lu, 13) tmp1, arg1(1:l1)
 13       format (6x,a,' = ',a)
          write (lu, 1) tmp1, tmp2
          write (lu, 8) tmp2, tmp3
          write (lu, 9) arg2(1:l2), tmp3, tmp4
          write (lu, 10) tmp4, argx(1:4)
          call rltmp (tmp1)
          call rltmp (tmp2)
          call rltmp (tmp3)
          call rltmp (tmp4)
        else
          goto 100
        endif
      elseif (itp2 .lt. 8) then
        if (itp1 .ne. 10 .and. itp2 .eq. 1) then

c   (Arg1 is MPI or MPR) and (Arg2 is IN).

          write (lina, 14) arg1(1:l1), arg2(1:l2), argx(1:4)
 14       format (6x,'CALL MPNPWR (',a,', ',a,', ',a,')')
          la = l1 + l2 + 28
          call outlin (lu, 1, la, lina)
        elseif (itp1 .eq. 10 .and. itp2 .eq. 1) then

c   (Arg1 is MPC) and (Arg2 is IN).  This is the only permissible
c   exponentiation with a MPC operand.

          write (lina, 15) arg1(1:l1), arg2(1:l2), argx(1:4)
 15       format (6x,'CALL MPCPWR (MPNW4, ',a,', ',a,', ',a,')')
          la = l1 + l2 + 35
          call outlin (lu, 1, la, lina)
        elseif (itp1 .ne. 10 .and. (itp2 .eq. 2 .or. itp2 .eq. 3))
     $      then

c   (Arg1 is MPI or MPR) and (Arg2 is SP or DP).

          kcon(3) = 1
          tmp1 = getmp (3)
          tmp2 = getmp (9)
          tmp3 = getmp (9)
          tmp4 = getmp (9)
          write (lu, 13) tmp1(1:4), arg2(1:l2)
          write (lu, 1) tmp1(1:4), tmp2
          write (lu, 8) arg1(1:l1), tmp3
          write (lu, 9) tmp2, tmp3, tmp4
          write (lu, 10) tmp4, argx(1:4)
          call rltmp (tmp1)
          call rltmp (tmp2)
          call rltmp (tmp3)
          call rltmp (tmp4)
        else
          goto 100
        endif
      elseif (itp1 .ne. 10 .and. itp2 .eq. 8) then

c   (Arg1 is MPI or MPR) and (Arg2 is MPI).

        kcon(3) = 1
        tmp1 = getmp (8)
        tmp2 = getmp (3)
        tmp3 = getmp (1)
        tmp4 = getmp (9)
        tmp5 = getmp (9)
        write (lu, 2) arg2(1:l2), tmp1
        write (lu, 3) tmp1, tmp1
        write (lu, 4) tmp1, tmp2, tmp3
        write (lu, 5) tmp3, tmp2, tmp3
        write (lu, 6) arg1(1:l1), tmp3, argx(1:4)
        write (lu, 7)
        write (lu, 8) arg1(1:l1), tmp4
        write (lu, 9) arg2(1:l2), tmp4, tmp5
        write (lu, 10) tmp5, argx(1:4)
        write (lu, 11)
        call rltmp (tmp1)
        call rltmp (tmp2)
        call rltmp (tmp3)
        call rltmp (tmp4)
        call rltmp (tmp5)
      elseif (itp1 .ne. 10 .and. itp2 .eq. 9) then

c  (Arg1 is MPI or MPR) and (Arg2 is MPI or MPR).

        kcon(3) = 1
        tmp1 = getmp (9)
        tmp2 = getmp (9)
        write (lu, 8) arg1(1:l1), tmp1
        write (lu, 9) arg2(1:l2), tmp1, tmp2
        write (lu, 10) tmp2, argx(1:4)
        call rltmp (tmp1)
        call rltmp (tmp2)
      else
        goto 100
      endif
      goto 110

 100  call errmes (53, 0)
      write (6, 16) uopr(iop)(1:lop(iop))
 16   format ('Operation ',a,' is not defined with these operands.')
      call abrt

 110  return
      end

      subroutine genmul (lu, itp1, l1, arg1, itp2, l2, arg2, argx)

c   This generates code for a multiply operation.  The operands are in ARG1 and
c   ARG2, with types ITP1 and ITP2, and with lengths L1 and L2.  The result
c   name is in ARGX (also input).  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*80 arg1, arg2, argx
      character*4 tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, getmp

      if (itp1 .lt. 8) then
        if (itp1 .eq. 3) then
          if (itp2 .ne. 10) then

c   (Arg1 is DP) and (Arg2 is MPI or MPR).

            write (lina, 1) arg2(1:l2), arg1(1:l1), argx(1:4)
 1          format (6x,'CALL MPMULD (',a,', ',a,', 0, ',a,')')
            la = l1 + l2 + 31
            call outlin (lu, 1, la, lina)
          else

c   (Arg1 is DP) and (Arg2 is MPC).

            tmp1 = getmp (9)
            tmp2 = getmp (9)
            write (lu, 2) arg2(1:l2), tmp1, tmp2
 2          format (6x,'CALL MPMPCM (MPNW4, ',a,', ',a,', ',a,')')
            write (lu, 1) tmp1, arg1(1:l1), argx(1:4)
            write (lu, 3) tmp2, arg1(1:l1), argx(1:4)
 3          format (6x,'CALL MPMULD (',a,', ',a,', 0, ',a,
     $        '(MPNWQ+5))')
            call rltmp (tmp1)
            call rltmp (tmp2)
          endif
        elseif (itp1 .eq. 1 .or. itp1 .eq. 2) then
          if (itp2 .ne. 10) then

c   (Arg1 is IN or SP) and (Arg2 is MPI or MPR).

            tmp1 = getmp (3)
            write (lu, 4) tmp1, arg1(1:l1)
 4          format (6x,a,' = ',a)
            write (lu, 1) arg2(1:l2), tmp1, argx(1:4)
            call rltmp (tmp1)
          else

c   (Arg1 is IN or SP) and (Arg2 is MPC).

            tmp1 = getmp (3)
            tmp2 = getmp (9)
            tmp3 = getmp (9)
            write (lu, 4) tmp1, arg1(1:l1)
            write (lu, 2) arg2(1:l2), tmp2, tmp3
            write (lu, 1) tmp2, tmp1, argx(1:4)
            write (lu, 3) tmp3, tmp1, argx(1:4)
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
          endif
        elseif (itp1 .eq. 4 .or. itp1 .eq. 5) then
          if (itp2 .ne. 10) then

c   (Arg1 is CO or DP) and (Arg2 is MPI or MPR).

            tmp1 = getmp (3)
            tmp2 = getmp (3)
            if (itp1 .eq. 5) then
              write (lu, 5) tmp1, arg1(1:l1)
 5            format (6x,a,' = DREAL (',a,')')
              write (lu, 6) tmp2, arg1(1:l1)
 6            format (6x,a,' = DIMAG (',a,')')
            else
              write (lu, 7) tmp1, arg1(1:l1)
 7            format (6x,a,' = REAL (',a,')')
              write (lu, 8) tmp2, arg1(1:l1)
 8            format (6x,a,' = AIMAG (',a,')')
            endif
            write (lu, 1) arg2(1:l2), tmp1, argx(1:4)
            write (lu, 3) arg2(1:l2), tmp2, argx(1:4)
            call rltmp (tmp1)
            call rltmp (tmp2)
          else

c   (Arg1 is CO or DP) and (Arg2 is MPC).

            tmp1 = getmp (3)
            tmp2 = getmp (3)
            tmp3 = getmp (9)
            tmp4 = getmp (9)
            tmp5 = getmp (10)
            if (itp1 .eq. 5) then
              write (lu, 5) tmp1, arg1(1:l1)
              write (lu, 6) tmp2, arg1(1:l1)
            else
              write (lu, 7) tmp1, arg1(1:l1)
              write (lu, 8) tmp2, arg1(1:l1)
            endif
            write (lu, 9) tmp1, tmp3
 9          format (6x,'CALL MPDMC (',a,', 0, ',a,')')
            write (lu, 9) tmp2, tmp4
            write (lu, 10) tmp3, tmp4, tmp5
 10         format (6x,'CALL MPMMPC (',a,', ',a,', MPNW4, ',a,')')
            write (lu, 11) tmp5, arg2(1:l2), argx(1:4)
 11         format (6x,'CALL MPCMUL (MPNW4, ',a,', ',a,', ',a,')')
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
            call rltmp (tmp4)
            call rltmp (tmp5)
          endif
        endif
      elseif (itp2 .lt. 8) then
        if (itp2 .eq. 3) then
          if (itp1 .ne. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is DP).

            write (lina, 1) arg1(1:l1), arg2(1:l2), argx(1:4)
            la = l1 + l2 + 31
            call outlin (lu, 1, la, lina)
          else

c   (Arg1 is MPC) and (Arg2 is DP).

            tmp1 = getmp (9)
            tmp2 = getmp (9)
            write (lu, 2) arg1(1:l1), tmp1, tmp2
            write (lu, 1) tmp1, arg2(1:l2), argx(1:4)
            write (lu, 3) tmp2, arg2(1:l2), argx(1:4)
            call rltmp (tmp1)
            call rltmp (tmp2)
          endif
        elseif (itp2 .eq. 1 .or. itp2 .eq. 2) then
          if (itp1 .ne. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is IN or SP).

            tmp1 = getmp (3)
            write (lu, 4) tmp1, arg2(1:l2)
            write (lu, 1) arg1(1:l1), tmp1, argx(1:4)
            call rltmp (tmp1)
          else

c   (Arg1 is MPC) and (Arg2 is IN or SP).

            tmp1 = getmp (3)
            tmp2 = getmp (9)
            tmp3 = getmp (9)
            write (lu, 4) tmp1, arg2(1:l2)
            write (lu, 2) arg1(1:l1), tmp2, tmp3
            write (lu, 1) tmp2, tmp1, argx(1:4)
            write (lu, 3) tmp3, tmp1, argx(1:4)
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
          endif
        elseif (itp2 .eq. 4 .or. itp2 .eq. 5) then
          if (itp1 .ne. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is CO or DC).

            tmp1 = getmp (3)
            tmp2 = getmp (3)
            if (itp2 .eq. 5) then
              write (lu, 5) tmp1, arg2(1:l2)
              write (lu, 6) tmp2, arg2(1:l2)
            else
              write (lu, 7) tmp1, arg2(1:l2)
              write (lu, 8) tmp2, arg2(1:l2)
            endif
            write (lu, 1) arg1(1:l1), tmp1, argx(1:4)
            write (lu, 3) arg1(1:l1), tmp2, argx(1:4)
            call rltmp (tmp1)
            call rltmp (tmp2)
          else

c   (Arg1 is MPC) and (Arg2 is CO or DC).

            tmp1 = getmp (3)
            tmp2 = getmp (3)
            tmp3 = getmp (9)
            tmp4 = getmp (9)
            tmp5 = getmp (10)
            if (itp2 .eq. 5) then
              write (lu, 5) tmp1, arg2(1:l2)
              write (lu, 6) tmp2, arg2(1:l2)
            else
              write (lu, 7) tmp1, arg2(1:l2)
              write (lu, 8) tmp2, arg2(1:l2)
            endif
            write (lu, 9) tmp1, tmp3
            write (lu, 9) tmp2, tmp4
            write (lu, 10) tmp3, tmp4, tmp5
            write (lu, 11) arg1(1:l1), tmp5, argx(1:4)
            call rltmp (tmp1)
            call rltmp (tmp2)
            call rltmp (tmp3)
            call rltmp (tmp4)
            call rltmp (tmp5)
          endif
        endif
      elseif (itp1 .ne. 10 .and. itp2 .ne. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is MPI or MPR).

        write (lina, 12) arg1(1:l1), arg2(1:l2), argx(1:4)
 12     format (6x,'CALL MPMUL (',a,', ',a,', ',a,')')
        la = l1 + l2 + 27
        call outlin (lu, 1, la, lina)
      elseif (itp1 .eq. 10 .and. itp2 .ne. 10) then

c   (Arg1 is MPC) and (Arg2 is MPI or MPR).

        tmp1 = getmp (9)
        tmp2 = getmp (9)
        write (lu, 2) arg1(1:l1), tmp1, tmp2
        write (lu, 12) tmp1, arg2(1:l2), argx(1:4)
        write (lu, 13) tmp2, arg2(1:l2), argx(1:4)
 13     format (6x,'CALL MPMUL (',a,', ',a,', ',a,'(MPNWQ+5))')
        call rltmp (tmp1)
        call rltmp (tmp2)
      elseif (itp1 .ne. 10 .and. itp2 .eq. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is MPC).

        tmp1 = getmp (9)
        tmp2 = getmp (9)
        write (lu, 2) arg2(1:l2), tmp1, tmp2
        write (lu, 12) arg1(1:l1), tmp1, argx(1:4)
        write (lu, 13) arg1(1:l1), tmp2, argx(1:4)
        call rltmp (tmp1)
        call rltmp (tmp2)
      elseif (itp1 .eq. 10 .and. itp2 .eq. 10) then

c   (Arg1 is MPC) and (Arg2 is MPC).

        write (lina, 11) arg1(1:l1), arg2(1:l2), argx(1:4)
        la = l1 + l2 + 35
        call outlin (lu, 1, la, lina)
      endif

      return
      end

      subroutine gensub (lu, itp1, l1, arg1, itp2, l2, arg2, argx)

c   This generates code for a subtract operation.  The operands are in ARG1 and
c   ARG2, with types ITP1 and ITP2, and with lengths L1 and L2.  The result
c   name is in ARGX (also input).  LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*80 arg1, arg2, argx
      character*4 tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, getmp

      if (itp1 .eq. 0) then

c   Handle the negation of a MP entity.

        if (itp2 .ne. 10) then

c   Arg2 is MPI or MPR.

          write (lu, 1) arg2(1:l2), argx(1:4)
 1        format (6x,'CALL MPEQ (',a,', ',a,')')
          write (lu, 2) argx(1:4), argx(1:4)
 2        format (6x,a,'(1) = - ',a,'(1)')
        else

c   Arg2 is MPC.

          write (lu, 3) arg2(1:l2), argx(1:4)
 3        format (6x,'CALL MPCEQ (MPNW4, ',a,', ',a,')')
          write (lu, 2) argx(1:4), argx(1:4)
          write (lu, 4) argx(1:4), argx(1:4)
 4        format (6x,a,'(MPNWQ+5) = - ',a,'(MPNWQ+5)')
        endif
      elseif (itp1 .lt. 8) then
        if (itp1 .eq. 3) then

c   (Arg1 is DP) and (Arg2 is MP).

          tmp1 = getmp (9)
          if (itp2 .eq. 10) then
            write (lu, 5) arg2(1:l2), argx(1:4)
 5          format (6x,'CALL MPCEQ (MPNW4, ',a,', ',a,')')
          endif
          write (lu, 6) arg1(1:l1), tmp1
 6        format (6x,'CALL MPDMC (',a,', 0, ',a,')')
          write (lu, 7) tmp1, arg2(1:l2), argx(1:4)
 7        format (6x,'CALL MPSUB (',a,', ',a,', ',a,')')
          if (itp2 .eq. 10) write (lu, 4) argx(1:4), argx(1:4)
          call rltmp (tmp1)
        elseif (itp1 .eq. 1 .or. itp1 .eq. 2) then

c   (Arg1 is IN or SP) and (Arg2 is MP).

          tmp1 = getmp (3)
          tmp2 = getmp (9)
          if (itp2 .eq. 10) then
            write (lu, 5) arg2(1:l2), argx(1:4)
          endif
          write (lu, 8) tmp1, arg1(1:l1)
 8        format (6x,a,' = ',a)
          write (lu, 6) tmp1, tmp2
          write (lu, 9) tmp2, arg2(1:l2), argx(1:4)
 9        format (6x,'CALL MPSUB (',a,', ',a,', ',a,')')
          call rltmp (tmp1)
          call rltmp (tmp2)
        else

c   (Arg1 is CO or DC) and (Arg2 is MP).

          tmp1 = getmp (3)
          tmp2 = getmp (3)
          tmp3 = getmp (9)
          tmp4 = getmp (9)
          tmp5 = getmp (10)
          if (itp1 .eq. 5) then
            write (lu, 10) tmp1, arg1(1:l1)
 10         format (6x,a,' = DREAL (',a,')')
            write (lu, 11) tmp2, arg1(1:l1)
 11         format (6x,a,' = DIMAG (',a,')')
          else
            write (lu, 12) tmp1, arg1(1:l1)
 12         format (6x,a,' = REAL (',a,')')
            write (lu, 13) tmp2, arg1(1:l1)
 13         format (6x,a,' = AIMAG (',a,')')
          endif
          write (lu, 6) tmp1, tmp3
          write (lu, 6) tmp2, tmp4
          write (lu, 14) tmp3, tmp4, tmp5
 14       format (6x,'CALL MPMMPC (',a,', ',a,', MPNW4, ',a,')')
          if (itp2 .eq. 10) then
            write (lu, 15) tmp5, arg2(1:l2), argx(1:4)
 15         format (6x,'CALL MPCSUB (MPNW4, ',a,', ',a,', ',a,')')
          else
            tmp6 = getmp (10)
            write (lu, 16) tmp3
 16         format (6x,'CALL MPDMC (0.D0, 0, ',a,')')
            write (lu, 14) arg2(1:l2), tmp3, tmp6
            write (lu, 15) tmp5, tmp6, argx(1:4)
            call rltmp (tmp6)
          endif
          call rltmp (tmp1)
          call rltmp (tmp2)
          call rltmp (tmp3)
          call rltmp (tmp4)
          call rltmp (tmp5)
        endif
      elseif (itp2 .lt. 8) then
        if (itp2 .eq. 3) then

c   (Arg1 is MP) and (Arg2 is DP).

          tmp1 = getmp (9)
          if (itp1 .eq. 10) then
            write (lu, 5) arg1(1:l1), argx(1:4)
          endif
          write (lu, 6) arg2(1:l2), tmp1
          write (lu, 7) arg1(1:l1), tmp1, argx(1:4)
          call rltmp (tmp1)
        elseif (itp2 .eq. 1 .or. itp2 .eq. 2) then

c   (Arg1 is MP) and (Arg2 is IN or SP).

          tmp1 = getmp (3)
          tmp2 = getmp (9)
          if (itp1 .eq. 10) then
            write (lu, 5) arg1(1:l1), argx(1:4)
          endif
          write (lu, 8) tmp1, arg2(1:l2)
          write (lu, 6) tmp1, tmp2
          write (lu, 7) arg1(1:l1), tmp2, argx(1:4)
          call rltmp (tmp1)
          call rltmp (tmp2)
        else

c   (Arg1 is MP) and (Arg2 is CO or DC).

          tmp1 = getmp (3)
          tmp2 = getmp (3)
          tmp3 = getmp (9)
          tmp4 = getmp (9)
          tmp5 = getmp (10)
          if (itp2 .eq. 5) then
            write (lu, 10) tmp1, arg2(1:l2)
            write (lu, 11) tmp2, arg2(1:l2)
          else
            write (lu, 12) tmp1, arg2(1:l2)
            write (lu, 13) tmp2, arg2(1:l2)
          endif
          write (lu, 6) tmp1, tmp3
          write (lu, 6) tmp2, tmp4
          write (lu, 14) tmp3, tmp4, tmp5
          if (itp1 .eq. 10) then
            write (lu, 15) arg1(1:l1), tmp5, argx(1:4)
          else
            tmp6 = getmp (10)
            write (lu, 16) tmp3
            write (lu, 14) arg1(1:l1), tmp3, tmp6
            write (lu, 15) tmp6, tmp5, argx(1:4)
            call rltmp (tmp6)
          endif
          call rltmp (tmp1)
          call rltmp (tmp2)
          call rltmp (tmp3)
          call rltmp (tmp4)
          call rltmp (tmp5)
        endif
      elseif (itp1 .ne. 10 .and. itp2 .ne. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is MPI or MPR).

        write (lina, 9) arg1(1:l1), arg2(1:l2), argx(1:4)
        la = l1 + l2 + 27
        call outlin (lu, 1, la, lina)
      elseif (itp1 .ne. 10 .and. itp2 .eq. 10) then

c   (Arg1 is MPI or MPR) and (Arg2 is MPC).

        tmp1 = getmp (9)
        tmp2 = getmp (10)
        write (lu, 16) tmp1
        write (lu, 14) arg1(1:l1), tmp1, tmp2
        write (lu, 15) tmp2, arg2(1:l2), argx(1:4)
        call rltmp (tmp1)
        call rltmp (tmp2)
      elseif (itp1 .eq. 10 .and. itp2 .ne. 10) then

c   (Arg1 is MPC) and (Arg1 is MPI or MPR).

        tmp1 = getmp (9)
        tmp2 = getmp (10)
        write (lu, 16) tmp1
        write (lu, 14) arg2(1:l2), tmp1, tmp2
        write (lu, 15) arg1(1:l1), tmp2, argx(1:4)
        call rltmp (tmp1)
        call rltmp (tmp2)
      else

c   (Arg1 is MPC) and (Arg2 is MPC).

        write (lina, 15) arg1(1:l1), arg2(1:l2), argx(1:4)
        la = l1 + l2 + 35
        call outlin (lu, 1, la, lina)
      endif

      return
      end

      subroutine genfun (lu, itp1, l1, arg1, nar, itar, lar, arg, itpx,
     $  lx, argx)

c   This generates code for a function reference.  The function name is ARG1,
c   with type ITP1 and length L1.  NAR is the number of arguments.  The
c   argument list is input in ARG, with types in ITAR and lengths in LAR.
c   The result is placed in ARGX, with type in ITPX and length in LX.  LU is
c   the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 liny
      character*80 arg1, arg(mar), argi, argj, argx
      character*16 nam, namq, ucase
      dimension itar(mar), lar(mar)
      character*4 getmp, tmp1
c*
c      write (lu, *) 'enter genfun nar, arg1 =', nar,
c     $  '  %'//arg1(1:l1)//'%'
c      write (lu, '(2i4,2x,a)') (i, itar(i), arg(i)(1:lar(i)),
c     $  i = 1, nar)
c      call flush (lu)
c*
      namq = 'DPNRT'
      iqnrt = itab (0, 0, namq)
      namq = 'MPINT'
      iqint = itab (0, 0, namq)
      namq = 'ABS'
      iqabs = itab (0, 0, namq)
      namq = 'NINT'
      iqnint = itab (0, 0, namq)

c   Check if this function is one of the obsolescent type-specific intrinsics.

      namq = ucase (arg1(1:l1))

      do 100 i = 1, nsf
        if (sfun(i) .eq. namq) then
          call errmes (54, 0)
          write (6, 1) arg1(1:l1)
 1        format ('This type-specific Fortran intrinsic function is',
     $      ' not allowed: ',a/'Replace with the equivalent Fortran',
     $      ' generic function.')
          call abrt
        endif
 100  continue

c   Find the function name in the symbol table.

      ll = min (l1, 16)
      nam = arg1(1:ll)
      ix = itab (0, 0, nam)
      if (ix .eq. 0) call errmes (55, 1)
      if (ix .gt. mint) call chkarg (lu, nam, nar, itar, lar, arg)

c   Check if this is a reference to an intrisic function.

      if (ix .le. mint) then

c   Check if the mixed mode safe option is in effect.

        if (imm .ge. 1) then

c   Convert any non-MP arguments to MP.

          do 110 i = 1, nar
            itpi = itar(i)
            li = lar(i)
            argi(1:li) = arg(i)(1:li)
            if (ix .eq. iqnrt .and. i .eq. 2) goto 110
            if (itpi .eq. 1) then
              itpj = 8
            elseif (itpi .eq. 2 .or. itpi .eq. 3) then
              itpj = 9
            elseif (itpi .eq. 4 .or. itpi .eq. 5) then
              itpj = 10
            else
              goto 110
            endif
            lj = 4
            argj(1:4) = getmp (itpj)
            call genasn (lu, itpj, lj, argj, itpi, li, argi)
            if (li .eq. 4) then
              if (argi(1:2) .eq. 'MP') then
                tmp1 = argi(1:4)
                call rltmp (tmp1)
              endif
            endif

c   Substitute the new argument (a MP temporary) for the old.

            itar(i) = itpj
            lar(i) = 4
            arg(i)(1:4) = argj(1:4)
 110      continue

        endif
        itm = 0

c   Determine if any arguments are of a MP type.

        do 120 i = 1, nar
          if (itar(i) .ge. 8) itm = 1
 120    continue

c   Call INTRIN for intrinsic calls with MP arguments.  Other intrinsic
c   references will be handled in this routine.

        if (itm .ne. 0 .or. ix .ge. iqint) then
          call intrin (lu, itp1, l1, arg1, nar, itar, lar, arg, itpx,
     $      lx, argx)
          goto 150
        elseif (itp1 .eq. -1) then

c   Except for ABS with a complex or double complex argument, and NINT, the
c   result type of a Fortran-77 intrinsic functions with an argument-dependent
c   type is the type of the first argument.

          if (ix .eq. iqabs) then
            if (itar(1) .eq. 4) then
              itpx = 2
            elseif (itar(1) .eq. 5) then
              itpx = 3
            else
              itpx = itar(1)
            endif
          elseif (ix .eq. iqnint) then
            itpx = 1
          else
            itpx = itar(1)
          endif
        else
          itpx = itp1
        endif
      else

c   For all other cases, set the result to be a temporary of type ITP1.

        itpx = itp1
      endif

      if (lu .eq. 12 .and. ix .gt. mint) then
        call errmes (56, 0)
        write (6, 2)
 2      format ('Only intrinsic functions may appear in a parameter',
     $    ' statement.')
        call abrt
      endif

      lx = 4
      argx = getmp (itpx)

c   Check if this is an ordinary function reference of type MP.  If so,
c   generate a call statement.

      if (itp1 .ge. 8) then
        liny(1:11) = '      CALL '
        liny(12:l1+11) = arg1(1:l1)
        liny(l1+12:l1+13) = ' ('
        ly = l1 + 13

c   Append the argument list.

        do 130 i = 1, nar
          li = lar(i)
          liny(ly+1:ly+li) = arg(i)(1:li)
          liny(ly+li+1:ly+li+2) = ', '
          ly = ly + li + 2
 130    continue

c   Set the last argument of the call statement to be the result temporary
c   name.

        liny(ly+1:ly+4) = argx(1:4)
        liny(ly+5:ly+5) = ')'
        ly = ly + 5

c   Check if it is a non-MP function reference.  If so, generate an assignment
c   statement.

      else
        liny(1:6) = ' '
        liny(7:10) = argx(1:4)
        liny(11:13) = ' = '
        liny(14:l1+13) = arg1(1:l1)
        liny(l1+14:l1+15) = ' ('
        ly = l1 + 15

c   Append the argument list.

        do 140 i = 1, nar
          li = lar(i)
          liny(ly+1:ly+li) = arg(i)(1:li)
          liny(ly+li+1:ly+li+2) = ', '
          ly = ly + li + 2
 140    continue

        if (nar .eq. 0) then
          ly = ly + 1
        else
          ly = ly - 1
        endif
        liny(ly:ly) = ')'

      endif

      call outlin (lu, 1, ly, liny)

c   Release any temporaries among the arguments.

 150  do 160 i = 1, nar
        li = lar(i)
        if (li .eq. 4) then
          if (arg(i)(1:2) .eq. 'MP') then
            tmp1 = arg(i)(1:4)
            call rltmp (tmp1)
          endif
        endif
 160  continue
c*
c      write (lu, *) 'exit genfun  itpx, argx =', itpx,
c     $  ' %'//argx(1:lx)//'%'
c      call flush (lu)
c*
      return
      end

      function getmp (itp)

c   This returns a temporary variable name that is of type ITP.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*4 getmp
      character*16 nam

      if (itp .le. 0 .or. itp .gt. 11) then
        call errmes (57, 0)
        write (6, 1)
 1      format ('Improper type input to GETMP.')
        call abrt
      endif

c   The two character types don't need to be put in the table, since they are
c   never retained after definition.

      if (itp .eq. 6) then
        getmp = 'MPA1'
        goto 120
      elseif (itp .eq. 11) then
        getmp = 'MPA2'
        goto 120
      endif

      do 100 i = 1, 9
        if (itmp(i,itp) .eq. 0) goto 110
 100  continue

      call errmes (58, 0)
      write (6, 2)
 2    format ('Statement is too complicated.')
      call abrt

 110  itmp(i,itp) = 1
      write (getmp, 3) ctm(itp), i
 3    format ('MP',a1,i1)

 120  nam = getmp
      ix = itab (2, itp, nam)

      return
      end

      subroutine ifst (ifl, ifs, k1, ln)

c   This handles MP IF and ELSEIF statements.  K1 is the index of the left
c   parenthesis, and LN is the last non-blank character in the statement.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*80 argx
      character*16 linq, ucase
      character*4 tmp1

c   If this is an elseif statement, output an else statement.

      if (ifs .eq. 2) write (11, 1)
1     format (6x,'ELSE')

c   Locate the matching right parenthesis of the left parenthesis.

      j1 = k1
      j2 = match (0, j1 + 1, ln, line)

c   Determine whether the expression inside the parentheses and the expression
c   following the parentheses have MP variables.

      i1 = iscan (j1, j2, line)
      if (i1 .ne. 0) call fixsub (j1, j2, ln)
      j3 = nblk (j2 + 1, ln, line)
      lq = min (j3 + 15, ln)
      linq = ucase (line(j3:lq))
      if (j3 + 3 .eq. ln .and. linq(1:4) .eq. 'THEN') then
        i2 = 0
        ith = 1
      else
        i2 = iscan (j3, ln, line)
        j4 = ln
        if (i2 .ne. 0) call fixsub (j3, j4, ln)
        ith = 0
      endif

c   If the expression inside parentheses has no MP variables, there is no
c   need to process it.

      if (i1 .eq. 0) then
        la = j2
        lina(1:6) = ' '
        lina(7:la) = line(7:la)
        call outlin (11, 1, la, lina)
        write (11, 2)
 2      format (5x,'$  THEN')
      else

c   Process the expression inside parentheses.

        la = j2 - j1 - 1
        lina(1:la) = line(j1+1:j2-1)
        call expres (11, la, lina, itpx, lx, argx)

c   Check if the result of the expression in parentheses is of type logical.

        if (itpx .ne. 7) then
          call errmes (59, 0)
          write (6, 3)
 3        format ('Result of expression in parentheses is not of type',
     $      ' logical.')
          call abrt
        endif

c   Output IF statement with the resulting logical variable.

        write (11, 4) argx(1:lx)
 4      format (6x,'IF (',a,') THEN')

c   Release final logical temporary variable.

        tmp1 = argx(1:lx)
        call rltmp (tmp1)
      endif

c   If the expression following the parentheses is merely THEN, then we are
c   done.

      if (ith .eq. 1) goto 100

c   If the expression following the parentheses has no MP variables, then
c   it does not need to be processed.

      if (i2 .eq. 0) then
        la = ln - j2 + 6
        lina(1:6) = ' '
        lina(7:la) = line(j2+1:ln)
        call outlin (11, 1, la, lina)
      else

c   Process the executable MP expression after the parentheses.

        j1 = j2 + 1
        if (j1 .gt. ln) call errmes (60, 1)
        j1 = nblk (j1, ln, line)
        call exec (j1, ln)
      endif

      write (11, 5)
 5    format (6x,'ENDIF')

 100  return
      end

      subroutine implic (k1, ln)

c   This handles IMPLICIT statements.  K1 and LN are the indices of the
c   first (after 'IMPLICIT') and last non-blank characters in the statement.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*16 linq, ucase
      character*1 cj

c   Check if this is an implicit 'none' statement.

      j1 = k1
      if (j1 + 3 .eq. ln) then
        linq = ucase (line(j1:ln))
        if (linq(1:4) .eq. 'NONE') then

          do 100 i = 1, 26
            if (impl(i) .le. 7) impl(i) = 0
 100      continue

          goto 140
        endif
      endif

 110  it = ntype (j1, ln)
      j1 = indx (j1, ln, '(', line)
      if (it .eq. 0 .or. j1 .eq. 0) call errmes (61, 1)

c   Process a normal implicit statement.

 120  j1 = nblk (j1 + 1, ln, line)
      cj = line(j1:j1)
      if (cj .eq. '(') goto 120
      if (cj .eq. ',') goto 120
      if (cj .eq. ')') then
        if (j1 .eq. ln) goto 140
        j1 = nblk (j1 + 1, ln, line)
        if (line(j1:j1) .eq. ',') then
          j1 = nblk (j1 + 1, ln, line)
          goto 110
        endif
      endif
      i1 = max (index (alpl, cj), index (alpu, cj))
      if (j1 .eq. 0 .or. i1 .eq. 0) call errmes (62, 1)
      if (impl(i1) .le. 7) impl(i1) = it
      j2 = nblk (j1 + 1, ln, line)
      cj = line(j2:j2)
      if (cj .eq. '-') then
        j2 = nblk (j2 + 1, ln, line)
        cj = line(j2:j2)
        i2 = max (index (alpl, cj), index (alpu, cj))
        if (i2 .eq. 0) call errmes (63, 1)

        do 130 i = i1, i2
          if (impl(i) .le. 7) impl(i) = it
 130    continue

        j1 = j2
      endif
      goto 120

 140  return
      end

      function indx (k1, k2, cx, lina)

c   Finds the index of the first instance of character CX in LINA between
c   positions K1 and K2.

      character*2000 lina
      character*1 cx

      do 100 i = k1, k2
        if (lina(i:i) .eq. cx) goto 110
 100  continue

      i = 0
 110  indx = i

      return
      end

      subroutine inimp

c   This generates declaration and initialization code for MP routines.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*80 lin
      character*16 nam
      character*8 num0, num1, num2, num3, num4, numx

c   Optionally print out the symbol table for this subprogram.
c*
c      write (6, '(4i4,2x,a)') (i, ktyp(i), kdim(i), lvar(i),
c     $  '%'//var(i)(1:lvar(i))//'%', i = mint + 1, nvar)
c*
c   Place the character and MP dimensions into character variables.

      write (numx, '(I8)') mxp + 4
      i1 = nblk (1, 8, numx)
      n1 = 9 - i1
      num0 = numx(i1:8)
      write (numx, '(I8)') mxp + 5
      i1 = nblk (1, 8, numx)
      n1 = 9 - i1
      num1 = numx(i1:8)
      write (numx, '(I8)') 2 * mxp + 8
      i1 = nblk (1, 8, numx)
      n2 = 9 - i1
      num2 = numx(i1:8)
      write (numx, '(I8)') int (7.225 * mxp) + 100
      i1 = nblk (1, 8, numx)
      n3 = 9 - i1
      num3 = numx(i1:8)
      write (numx, '(I8)') lsm + 4
      i1 = nblk (1, 8, numx)
      n4 = 9 - i1
      num4 = numx(i1:8)

c   If this is the main program, make sure that at least one MPR temporary
c   is defined.

      if (istp .eq. 1) then
        nam = 'MPM1'
        ix = itab (2, 9, nam)
      endif

c   Insert declarations variables that do not yet have valid declarations.
c   These include, for example, the temporaries generated by GETMP.

      do 100 i = mint + 1, nvar
        kdm = kdim(i)
        ktp = ktyp(i)
        lv = lvar(i)
        nam = var(i)
        if (kdec(i) .eq. 0) then
          if (ktp .eq. 1) then
            write (6, 1) nam(1:lv)
 1          format (6x,'INTEGER ',a)
          elseif (ktp .eq. 2) then
            write (6, 2) nam(1:lv)
 2          format (6x,'REAL ',a)
          elseif (ktp .eq. 3) then
            write (6, 3) nam(1:lv)
 3          format (6x,'DOUBLE PRECISION ',a)
          elseif (ktp .eq. 4) then
            write (6, 4) nam(1:lv)
 4          format (6x,'COMPLEX ',a)
          elseif (ktp .eq. 5) then
            write (6, 5) nam(1:lv)
 5          format (6x,'DOUBLE COMPLEX ',a)
          elseif (ktp .eq. 6) then
            write (6, 6) nam(1:lv), num3(1:n3)
 6          format (6x,'CHARACTER*1 ',a,'(',a,')')
          elseif (ktp .eq. 7) then
            write (6, 7) nam(1:lv)
 7          format (6x, 'LOGICAL ',a)

c   For MP dimensioned variables, only declare the type as real.

          elseif (kdm .gt. 0) then
            write (6, 8) nam(1:lv)
 8          format (6x,'REAL ',a)

c   For MP scalar variables, declare as real with the appropriate dimension.

          elseif (ktp .eq. 8 .or. ktp .eq. 9) then
            write (6, 9) nam(1:lv), num0(1:n1)
 9          format (6x,'REAL ',a,'(',a,')')
          elseif (ktp .eq. 10) then
            write (6, 9) nam(1:lv), num2(1:n2)
          elseif (ktp .eq. 11) then
            write (6, 10) num4(1:n4), nam(1:lv)
 10         format (6x,'CHARACTER*',a,' ',a)
          endif
        endif

c   If the variable is a parameter, save it.

        if (kdm .eq. -3) write (6, 11) nam(1:lv)
 11     format (6x,'SAVE ',a)
 100  continue

c   Insert declarations for MPNWQ, MPNW4, MPL02, MPL10 and MPPIC if any is
c   required in this subprogram.

      if (istp .eq. 1 .or. kcon(1) .ne. 0 .or. kcon(3) .ne. 0 .or.
     $  kcon(4) .ne. 0 .or. kcon(5) .ne. 0)
     $  write (6, 12) (num1(1:n1), i = 1, 3)
 12   format (6x,'INTEGER MPNWQ, MPNW4'/6x,'REAL MPL02, MPL10, MPPIC'/
     $  6x,'COMMON /MPTCON/ MPNWQ, MPNW4, MPL02(',a,'), MPL10(',a,
     $  '),'/5x,'$  MPPIC(',a,')')

c   Insert declaration for MPEPS if it is required.

      if (kcon(2) .ne. 0) write (6, 13)
 13   format (6x,'REAL MPEPS(8)')

c   If the scratch space directive is in effect, insert allocation.

      if (istp .eq. 1 .and. mss .ne. 0) write (6, 14) mss
 14   format (6x,'REAL MPSS'/6x,'COMMON /MPCOM3/ MPSS(',i8,')')

c   If the precision level is very high, insert allocation for DP scratch.

      if (mxp .ge. 1016) write (6, 15) mxp + 8
 15   format (6x,'DOUBLE PRECISION MPDS'/
     $  6x,'COMMON /MPCOM4/ MPDS(',i8,')')

c   If any MP parameters have been defined, define a flag.

      if (mpa .gt. 0) write (6, 16)
 16   format (6x,'INTEGER MPPAR'/6x,'SAVE MPPAR'/6x,'DATA MPPAR /0/')

c   This is the end of the declaration section.

      write (6, 17)
 17   format ('C')

c   Check if this is the main program.

      if (istp .eq. 1) then

c   Insert calls to set the precision level and scratch space.

        write (6, 18) mxp + 1
 18     format (6x,'CALL MPSETP (''NW'', ',i6,')')
        if (mss .ne. 0) write (6, 19) mss
 19     format (6x,'CALL MPSETP (''IMS'', ',i8,')')

c   Insert code to compute MPNWQ, MPNW4, MPL02, MPL10 and MPPIC.

        write (6, 20) mxp, mxp + 4
 20     format (
     $    6x,'MPNWQ = ',i6/
     $    6x,'MPNW4 = ',i6/
     $    6x,'CALL MPDMC (2.D0, 0, MPM1)'/
     $    6x,'CALL MPLOG (MPM1, MPL02, MPL02)'/
     $    6x,'CALL MPDMC (10.D0, 0, MPM1)'/
     $    6x,'CALL MPLOG (MPM1, MPL02, MPL10)'/
     $    6x,'CALL MPPI (MPPIC)')
        write (6, 18) mxp
      endif

c   Set value for MPEPS if required.

      if (kcon(2) .ne. 0) write (6, 21) eps(1)(1:lep(1)),
     $    eps(2)(1:lep(2))
 21   format (6x,'CALL MPDMC (',a,', ',a,', MPEPS)')

c   If MP parameters have been defined, insert code here.

      if (mpa .gt. 0) then
        write (6, 22)
 22     format (6x,'IF (MPPAR .EQ. 0) THEN')

 110    read (12, '(A)', end = 120) lin
        l1 = lnblk (lin)
        write (6, '(A)') lin(1:l1)
        goto 110

 120    write (6, 23)
 23     format (6x,'MPPAR = 1'/6x,'ENDIF')
      endif

      write (6, 24)
 24   format ('CMP<')

      return
      end

      subroutine intrin (lu, itp1, l1, arg1, nar, itar, lar, arg, itpx,
     $  lx, argx)

c   This generates code for MP intrinsic functions.  The function name is ARG1,
c   with type ITP1 and length L1.  NAR is the number of arguments.  The
c   argument list is input in ARG, with types in ITAR and lengths in LAR.
c   The result is placed in ARGX, with type in ITPX and length in LX.
c   LU is the unit number for output code.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*80 arg1, arg(mar), argx, argz
      character*16 nam
      dimension itar(mar), lar(mar)
      character*4 getmp, tmp1, tmp2, tmp3, tmp4, tmp5
c*
c      write (lu, *) 'enter intrin, nar, arg1 =', nar,
c     $  ' %'//arg1(1:l1)//'%'
c      write (lu, '(2i4,2x,a)') (i, itar(i), arg(i)(1:lar(i)), i = 1,nar)
c      call flush (lu)
c*
      ll = min (l1, 16)
      nam = arg1(1:ll)
      ix = itab (0, 0, nam)
      ktp = ktyp(ix)

c   For functions with an argument-dependent result type, the type of the
c   result is the type of the first argument, except for ABS with a MPC
c   argument and NINT.

      if (ktp .ne. -1) then
        itpx = ktp
      elseif (ix .eq. 11 .and. itar(1) .eq. 10) then
        itpx = 9
      elseif (ix .eq. 39) then
        itpx = 8
      else
        itpx = itar(1)
      endif
      lx = 4
      argx = getmp (itpx)

      goto (
     $  100, 110, 120, 440, 130, 140, 150, 160, 450, 170,
     $  180, 190, 200, 210, 220, 440, 450, 450, 230, 450,
     $  450, 240, 450, 250, 260, 270, 280, 290, 300, 310,
     $  320, 330, 340, 350, 360, 370, 380, 390, 400, 410,
     $   420, 430, 460, 460) ix - 10

c   It is ABS.

 100  if (nar .ne. 1) goto 450
      la1 = lar(1)
      if (itar(1) .ne. 10) then
        write (lu, 1) arg(1)(1:la1), argx(1:4)
 1      format (6x,'CALL MPEQ (',a,', ',a,')')
        write (lu, 2) argx(1:4), argx(1:4)
 2      format (6x,a,'(1) = ABS (',a,'(1))')
      else
        tmp1 = getmp (9)
        tmp2 = getmp (9)
        tmp3 = getmp (9)
        tmp4 = getmp (9)
        write (lu, 3) arg(1)(1:la1), tmp1, tmp2
 3      format (6x,'CALL MPMPCM (MPNW4, ',a,', ',a,', ',a,')')
        write (lu, 4) tmp1, tmp1, tmp3
 4      format (6x,'CALL MPMUL (',a,', ',a,', ',a,')')
        write (lu, 4) tmp2, tmp2, tmp4
        write (lu, 5) tmp3, tmp4, tmp1
 5      format (6x,'CALL MPADD (',a,', ',a,', ',a,')')
        write (lu, 6) tmp1, argx(1:4)
 6      format (6x,'CALL MPSQRT (',a,', ',a,')')
        call rltmp (tmp1)
        call rltmp (tmp2)
        call rltmp (tmp3)
        call rltmp (tmp4)
      endif
      goto 470

c   It is ACOS.

 110  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      kcon(5) = 1
      la1 = lar(1)
      tmp1 = getmp (9)
      tmp2 = getmp (9)
      tmp3 = getmp (9)
      write (lu, 7) tmp1
 7    format (6x,'CALL MPDMC (1.D0, 0, ',a,')')
      write (lina, 4) arg(1)(1:la1), arg(1)(1:la1), tmp2
      la = 2 * la1 + 27
      call outlin (lu, 1, la, lina)
      write (lu, 8) tmp1, tmp2, tmp3
 8    format (6x,'CALL MPSUB (',a,', ',a,', ',a,')')
      write (lu, 6) tmp3, tmp1
      write (lu, 9) arg(1)(1:la1), tmp1, argx(1:4)
 9    format (6x,'CALL MPANG (',a,', ',a,', MPPIC, ',a,')')
      call rltmp (tmp1)
      call rltmp (tmp2)
      call rltmp (tmp3)
      goto 470

c   It is AINT.

 120  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      la1 = lar(1)
      tmp1 = getmp (9)
      write (lu, 10) arg(1)(1:la1), argx(1:4), tmp1
 10   format (6x,'CALL MPINFR (',a,', ',a,', ',a,')')
      call rltmp (tmp1)
      goto 470

c   It is ANINT.

 130  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      la1 = lar(1)
      write (lu, 11) arg(1)(1:la1), argx(1:4)
 11   format (6x,'CALL MPNINT (',a,', ',a,')')
      goto 470

c   It is ASIN.

 140  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      kcon(5) = 1
      la1 = lar(1)
      tmp1 = getmp (9)
      tmp2 = getmp (9)
      tmp3 = getmp (9)
      write (lu, 7) tmp1
      write (lina, 4) arg(1)(1:la1), arg(1)(1:la1), tmp2
      la = 2 * la1 + 27
      call outlin (lu, 1, la, lina)
      write (lu, 8) tmp1, tmp2, tmp3
      write (lu, 6) tmp3, tmp1
      write (lu, 9) tmp1, arg(1)(1:la1), argx(1:4)
      call rltmp (tmp1)
      call rltmp (tmp2)
      call rltmp (tmp3)
      goto 470

c   It is ATAN.

 150  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      kcon(5) = 1
      la1 = lar(1)
      tmp1 = getmp (9)
      write (lu, 7) tmp1
      write (lu, 9) tmp1, arg(1)(1:la1), argx(1:4)
      call rltmp (tmp1)
      goto 470

c   It is ATAN2.

 160  if (nar .ne. 2 .or. itar(1) .ne. 9 .or. itar(2) .ne. 9) goto 450
      kcon(5) = 1
      la1 = lar(1)
      la2 = lar(2)
      write (lina, 9) arg(2)(1:la2), arg(1)(1:la1), argx(1:4)
      la = la1 + la2 + 34
      call outlin (lu, 1, la, lina)
      goto 470

c   It is CMPLX (convert MPC to CO).

 170  if (nar .ne. 1 .or. itar(1) .ne. 10) goto 450
      write (lu, 12)
 12   format ('CMP*'/'CMP*  Note: The result of CMPLX with an MP',
     $  ' argument has type CO.'/'CMP*  If an MPC result is',
     $  ' required, use DPCMPL or an assignment statement.'/'CMP*')
      la1 = lar(1)
      tmp1 = getmp (9)
      tmp2 = getmp (9)
      tmp3 = getmp (3)
      tmp4 = getmp (3)
      tmp5 = getmp (1)
      write (lu, 3) arg(1)(1:la1), tmp1, tmp2
      write (lu, 13) tmp1, tmp3, tmp5
 13   format (6x,'CALL MPMDC (',a,', ',a,', ',a,')')
      write (lu, 14) tmp3, tmp3, tmp5
 14   format (6x,a,' = ',a,' * 2.D0 ** ',a)
      write (lu, 13) tmp2, tmp4, tmp5
      write (lu, 14) tmp4, tmp4, tmp5
      write (lu, 15) argx(1:4), tmp3, tmp4
 15   format (6x,a,' = DCMPLX (',a,', ',a,')')
      call rltmp (tmp1)
      call rltmp (tmp2)
      call rltmp (tmp3)
      call rltmp (tmp4)
      call rltmp (tmp5)
      goto 470

c   It is CONJG.

 180  if (nar .ne. 1 .or. itar(1) .ne. 10) goto 450
      la1 = lar(1)
      write (lu, 16) arg(1)(1:la1), argx(1:4)
 16   format (6x,'CALL MPCEQ (MPNW4, ',a,', ',a,')')
      write (lu, 17) argx(1:4), argx(1:4)
 17   format (6x,a,'(MPNWQ+5) = - ',a,'(MPNWQ+5)')
      goto 470

c   It is COS.

 190  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      kcon(5) = 1
      la1 = lar(1)
      tmp1 = getmp (9)
      write (lu, 18) arg(1)(1:la1), argx(1:4), tmp1
 18   format (6x,'CALL MPCSSN (',a,', MPPIC, ',a,', ',a,')')
      call rltmp (tmp1)
      goto 470

c   It is COSH.

 200  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      kcon(3) = 1
      la1 = lar(1)
      tmp1 = getmp (9)
      write (lu, 19) arg(1)(1:la1), argx(1:4), tmp1
 19   format (6x,'CALL MPCSSH (',a,', MPL02, ',a,', ',a,')')
      call rltmp (tmp1)
      goto 470

c   It is DBLE (convert MPI or MPR to DP).

 210  if (nar .ne. 1) goto 450
      write (lu, 20)
 20   format ('CMP*'/'CMP*  Note: The result of DBLE with an MP',
     $  ' argument has type DP.'/'CMP*  If an MPR result is',
     $  ' required, use DPREAL or an assignment statement.'/'CMP*')
      la1 = lar(1)
      tmp1 = getmp (3)
      tmp2 = getmp (1)
      write (lu, 13) arg(1)(1:la1), tmp1, tmp2
      write (lu, 14) argx(1:4), tmp1, tmp2
      call rltmp (tmp1)
      call rltmp (tmp2)
      goto 470

c   It is DCMPLX (convert MPC to DC).

 220  if (nar .ne. 1 .or. itar(1) .ne. 10) goto 450
      write (lu, 21)
 21   format ('CMP*'/'CMP*  Note: The result of DCMPLX with an MP',
     $  ' argument has type DC.'/'CMP*  If an MPC result is',
     $  ' required, use DPCMPL or an assignment statement.'/'CMP*')
      la1 = lar(1)
      tmp1 = getmp (9)
      tmp2 = getmp (9)
      tmp3 = getmp (3)
      tmp4 = getmp (3)
      tmp5 = getmp (1)
      write (lu, 3) arg(1)(1:la1), tmp1, tmp2
      write (lu, 13) tmp1, tmp3, tmp5
      write (lu, 14) tmp3, tmp3, tmp5
      write (lu, 13) tmp2, tmp4, tmp5
      write (lu, 14) tmp4, tmp4, tmp5
      write (lu, 15) argx(1:4), tmp3, tmp4
      call rltmp (tmp1)
      call rltmp (tmp2)
      call rltmp (tmp3)
      call rltmp (tmp4)
      call rltmp (tmp5)
      goto 470

c   It is EXP.

 230  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      kcon(3) = 1
      la1 = lar(1)
      write (lu, 22) arg(1)(1:la1), argx(1:4)
 22   format (6x,'CALL MPEXP (',a,', MPL02, ',a,')')
      goto 470

c   It is INT (convert MPI or MPR to IN).

 240  if (nar .ne. 1) goto 450
      write (lu, 23)
 23   format ('CMP*'/'CMP*  Note: The result of INT with an MP',
     $  ' argument has type IN.'/'CMP*  If an MPI result is',
     $  ' required, use MPINT or an assignment statement.'/'CMP*')
      la1 = lar(1)
      tmp1 = getmp (3)
      tmp2 = getmp (1)
      write (lu, 13) arg(1)(1:la1), tmp1, tmp2
      write (lu, 14) argx(1:4), tmp1, tmp2
      call rltmp (tmp1)
      call rltmp (tmp2)
      goto 470

c   It is LOG.

 250  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      kcon(3) = 1
      la1 = lar(1)
      write (lu, 24) arg(1)(1:la1), argx(1:4)
 24   format (6x,'CALL MPLOG (',a,', MPL02, ',a,')')
      goto 470

c   It is LOG10.

 260  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      kcon(4) = 1
      la1 = lar(1)
      tmp1 = getmp (9)
      write (lu, 24) arg(1)(1:la1), tmp1
      write (lu, 25) tmp1, 'MPL10', argx(1:4)
 25   format (6x,'CALL MPDIV (',a,', ',a,', ',a,')')
      call rltmp (tmp1)
      goto 470

c   It is MAX.

 270  if (nar .ne. 2 .or. itar(1) .ne. itar(2) .or. itar(1) .eq. 10)
     $  goto 450
      la1 = lar(1)
      la2 = lar(2)
      tmp1 = getmp (1)
      write (lina, 26) arg(1)(1:la1), arg(2)(1:la2), tmp1
 26   format (6x,'CALL MPCPR (',a,', ',a,', ',a,')')
      la = la1 + la2 + 27
      call outlin (lu, 1, la, lina)
      write (lu, 27) tmp1, arg(1)(1:la1), argx(1:4), arg(2)(1:la2),
     $  argx(1:4)
 27   format (6x,'IF (',a,' .GE. 0) THEN'/
     $  8x,'CALL MPEQ (',a,', ',a,')'/6x,'ELSE'/
     $  8x,'CALL MPEQ (',a,', ',a,')'/6x,'ENDIF')
      call rltmp (tmp1)
      goto 470

c   It is MIN.

 280  if (nar .ne. 2 .or. itar(1) .ne. itar(2) .or. itar(1) .eq. 10)
     $  goto 450
      la1 = lar(1)
      la2 = lar(2)
      tmp1 = getmp (1)
      write (lina, 26) arg(1)(1:la1), arg(2)(1:la2), tmp1
      la = la1 + la2 + 27
      call outlin (lu, 1, la, lina)
      write (lu, 27) tmp1, arg(2)(1:la2), argx(1:4), arg(1)(1:la1),
     $  argx(1:4)
      call rltmp (tmp1)
      goto 470

c   It is MOD.

 290  if (nar .ne. 2 .or. itar(1) .ne. itar(2) .or. itar(1) .eq. 10)
     $  goto 450
      la1 = lar(1)
      la2 = lar(2)
      tmp1 = getmp (9)
      tmp2 = getmp (8)
      tmp3 = getmp (9)
      write (lina, 25) arg(1)(1:la1), arg(2)(1:la2), tmp1
      la = la1 + la2 + 27
      call outlin (lu, 1, la, lina)
      write (lu, 10) tmp1, tmp2, tmp3
      write (lu, 4) arg(2)(1:la2), tmp2, tmp1
      write (lu, 8) arg(1)(1:la1), tmp1, argx(1:4)
      call rltmp (tmp1)
      call rltmp (tmp2)
      call rltmp (tmp3)
      goto 470

c   It is NINT.

 300  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      la1 = lar(1)
      write (lu, 11) arg(1)(1:la1), argx(1:4)
      goto 470

c   It is REAL (convert MPI or MPR to SP).

 310  if (nar .ne. 1) goto 450
      write (lu, 28)
 28   format ('CMP*'/'CMP*  Note: The result of REAL with an MP',
     $  ' argument has type SP.'/'CMP*  If an MPR result is',
     $  ' required, use DPREAL or an assignment statement.'/'CMP*')
      la1 = lar(1)
      tmp1 = getmp (3)
      tmp2 = getmp (1)
      write (lu, 13) arg(1)(1:la1), tmp1, tmp2
      write (lu, 14) argx(1:4), tmp1, tmp2
      call rltmp (tmp1)
      call rltmp (tmp2)
      goto 470

c   It is SIGN.

 320  if (nar .ne. 2 .or. itar(1) .ne. itar(2) .or. itar(1) .eq. 10)
     $  goto 450
      la1 = lar(1)
      la2 = lar(2)
      tmp1 = getmp (9)
      write (lu, 1) arg(1)(1:la1), argx(1:4)
      write (lu, 1) arg(2)(1:la2), tmp1
      write (lu, 29) tmp1, argx(1:4), argx(1:4), argx(1:4), argx(1:4)
 29   format (6x,'IF (',a,'(1) .GE. 0.) THEN'/
     $  6x,a,'(1) = ABS (',a,'(1))'/6x,'ELSE'/
     $  6x,a,'(1) = - ABS (',a,'(1))'/6x,'ENDIF')
      call rltmp (tmp1)
      goto 470

c   It is SIN.

 330  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      kcon(5) = 1
      la1 = lar(1)
      tmp1 = getmp (9)
      write (lu, 18) arg(1)(1:la1), tmp1, argx(1:4)
      call rltmp (tmp1)
      goto 470

c   It is SINH.

 340  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      kcon(3) = 1
      la1 = lar(1)
      tmp1 = getmp (9)
      write (lu, 19) arg(1)(1:la1), tmp1, argx(1:4)
      call rltmp (tmp1)
      goto 470

c   It is SQRT.

 350  if (nar .ne. 1 .or. itar(1) .eq. 8) goto 450
      la1 = lar(1)
      if (itar(1) .eq. 9) then
        write (lu, 6) arg(1)(1:la1), argx(1:4)
      elseif (itar(1) .eq. 10) then
        write (lu, 30) arg(1)(1:la1), argx(1:4)
 30     format (6x,'CALL MPCSQR (MPNW4, ',a,', ',a,')')
      endif
      goto 470

c   It is TAN.

 360  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      kcon(5) = 1
      la1 = lar(1)
      tmp1 = getmp (9)
      tmp2 = getmp (9)
      write (lu, 18) arg(1)(1:la1), tmp1, tmp2
      write (lu, 25) tmp2, tmp1, argx(1:4)
      call rltmp (tmp1)
      call rltmp (tmp2)
      goto 470

c   It is TANH.

 370  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      kcon(3) = 1
      la1 = lar(1)
      tmp1 = getmp (9)
      tmp2 = getmp (9)
      write (lu, 19) arg(1)(1:la1), tmp1, tmp2
      write (lu, 25) tmp2, tmp1, argx(1:4)
      call rltmp (tmp1)
      call rltmp (tmp2)
      goto 470

c   It is MPINT.

 380  if (nar .ne. 1 .or. itar(1) .ne. 9) goto 450
      la1 = lar(1)
      tmp1 = getmp (9)
      write (lu, 10) arg(1)(1:la1), argx(1:4), tmp1
      call rltmp (tmp1)
      goto 470

c   It is DPCMPL.

 390  if (nar .ne. 2 .or. itar(1) .ne. 9 .or. itar(2) .ne. 9) goto 450
      la1 = lar(1)
      la2 = lar(2)
      write (lina, 31) arg(1)(1:la1), arg(2)(1:la2), argx(1:4)
 31   format (6x,'CALL MPMMPC (',a,', ',a,', MPNW4, ',a,')')
      la = la1 + la2 + 35
      call outlin (lu, 1, la, lina)
      goto 470

c   It is DPIMAG.

 400  if (nar .ne. 1 .or. itar(1) .ne. 10) goto 450
      la1 = lar(1)
      tmp1 = getmp (9)
      write (lu, 3) arg(1)(1:la1), tmp1, argx(1:4)
      call rltmp (tmp1)
      goto 470

c   It is DPREAL.

 410  if (nar .ne. 1 .or. itar(1) .ne. 10) goto 450
      la1 = lar(1)
      write (lu, 1) arg(1)(1:la1), argx(1:4)
      goto 470

c   It is DPNRT.

 420  if (nar .ne. 2 .or. itar(1) .ne. 9 .or. itar(2) .ne. 1) goto 450
      la1 = lar(1)
      la2 = lar(2)
      write (lina, 32) arg(1)(1:la1), arg(2)(1:la2), argx(1:4)
 32   format (6x,'CALL MPNRT (',a,', ',a,', ',a,')')
      la = la1 + la2 + 27
      call outlin (lu, 1, la, lina)
      goto 470

c   It is DPRAND.

 430  if (nar .ne. 0) goto 450
      write (lu, 33) argx(1:4)
 33   format (6x,'CALL MPRAND (',a,')')
      goto 470

 440  call errmes (64, 0)
      write (6, 34) arg1(1:l1)
 34   format ('This intrinsic function is not defined with this MP',
     $  ' argument type: ',a)
      call abrt

 450  call errmes (65, 0)
      write (6, 35) arg1(1:l1)
 35   format ('This intrinsic function has an improper argument list: ',
     $  a)
      call abrt

 460  call errmes (66, 0)
      write (6, 36) arg1(1:l1)
 36   format ('This subroutine name may not be used as a function: ',a)
      call abrt

 470  continue
c*
c      write (lu, *) 'exit intrin  itpx, argx =', itpx,
c     $  ' %'//argx(1:lx)//'%'
c      call flush (lu)
c*
      return
      end

      function ipfsb (k1, ln)

c   This checks to see if the statement is a program, subroutine, function
c   or block data statement. K1 and LN are the indices of the first and last
c   non-blank characters in the statement.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*16 linq, nam, ucase

      j1 = k1
      lq = min (j1 + 15, ln)
      linq = ucase (line(j1:lq))
      if (linq(1:7) .eq. 'PROGRAM') then
        if (iex .ne. 0) goto 100
        istp = 1
        ipfsb = 1
        j1 = nblk (j1 + 7, ln, line)
        if (j1 .eq. 0) goto 110
        fnam = line(j1:ln)
        lsar = 0
        call outlin (11, 1, ln, line)
      elseif (linq(1:10) .eq. 'SUBROUTINE') then
        if (iex .ne. 0) goto 100
        istp = 2
        ipfsb = 1
        j1 = nblk (j1 + 10, ln, line)
        if (j1 .eq. 0) goto 110
        j3 = indx (j1, ln, '(', line)
        if (j3 .ne. 0) then
          j2 = j3 - 1
        else
          j2 = ln
        endif
        fnam = line(j1:j2)
        if (j3 .ne. 0) then
          lsar = ln - j3 - 1
          sarg(1:lsar) = line(j3+1:ln-1)
        else
          lsar = 0
          sarg(1:1) = ' '
        endif
        call outlin (11, 1, ln, line)
      elseif (linq(1:8) .eq. 'FUNCTION') then
        if (iex .ne. 0) goto 100
        istp = 3
        js = j1
        j1 = nblk (j1 + 8, ln, line)
        j3 = indx (j1, ln, '(', line)
        if (j1 .eq. 0 .or. j3 .eq. 0) goto 110
        j2 = j3 - 1
        fnam = line(j1:j2)
        ix = itab (0, 0, fnam)
        lsar = ln - j3 - 1
        sarg(1:lsar) = line(j3+1:ln-1)

c   If the function name is MP, then change to subroutine statement.

        if (ix .gt. mint .and. ktyp(ix) .ge. 8) then
          kdim(ix) = -2
          call outlin (11, 2, ln, line)
          lina(1:js-1) = line(1:js-1)
          lina(js:js+9) = 'SUBROUTINE'
          lina(js+10:ln+1) = line(js+8:ln-1)
          if (lsar .eq. 0 .or. sarg(1:lsar) .eq. ' ') then
            lina(ln+2:ln+7) = 'MPFVX)'
            la = ln + 7
          else
            lina(ln+2:ln+9) = ', MPFVX)'
            la = ln + 9
          endif
          nam = 'MPFVX'
          ix = itab (2, ktyp(ix), nam)
          call outlin (11, 1, la, lina)
          write (11, 1)
 1        format ('CMP<')
        else
          call outlin (11, 1, ln, line)
        endif
        ipfsb = 1
      elseif (linq(1:10) .eq. 'BLOCK DATA') then
        if (iex .ne. 0) goto 100
        istp = 4
        ipfsb = 1
        call outlin (11, 1, ln, line)
      else
        ipfsb = 0
      endif
      goto 130

 100  call errmes (67, 0)
      write (6, 2)
 2    format ('A declarative statement may not appear after an',
     $  ' executable statement.')
      call abrt

 110  call errmes (68, 0)
      write (6, 3)
 3    format ('The subprogram name or argument list is missing.')
      call abrt

 120  call errmes (69, 0)
      write (6, 4)
 4    format ('Too many program units in this file.')
      call abrt

 130  return
      end

      function iscan (k1, ln, lina)

c   This scans LINE between positions K1 and LN for variable names and
c   enters new ones into the table.  The result is 1 if a MP constant or
c   special constant name is found, 2 if a MP variable is found, and 0
c   otherwise.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*16 nam
      character*1 cj

      j1 = k1
      iscan = 0

c   Extract the next character from the line.

 100  if (j1 .gt. ln) goto 130
      j1 = nblk (j1, ln, lina)
      cj = lina(j1:j1)

c   Check if it is the start of a numeric constant.

      is1 = index (dig, cj)
      if ((cj .eq. '-' .or. cj .eq. '.') .and. j1 .lt. ln) then
        j2 = nblk (j1 + 1, ln, lina)
        is2 = index (dig, lina(j2:j2))
      else
        is2 = 0
      endif

      if (is1 .ne. 0 .or. is2 .ne. 0) then
        itp = numcon (j1, j2, ln, lina)
        j1 = j2 + 1
        goto 100

c   Check if it the start of a name.

      elseif (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then

        do 110 j = j1, ln
          cj = lina(j:j)
          if (index (del, cj) .ne. 0) goto 120
 110    continue

        j = ln + 1
 120    i2 = j - 1
        nam = lina(j1:i2)
        ix = itab (1, 0, nam)
        if (ix .eq. 0) then
          j1 = i2 + 1
          goto 100
        endif
        itp = ktyp(ix)
        if (itp .lt. 8) kdec(ix) = 1

c   Check if the variable is a special constant.

        if (ix .ge. 1 .and. ix .le. 5) then
          iscan = max (iscan, 1)

c   Check if the variable is MP.

        elseif (itp .ge. 8) then
          iscan = 2
        endif
        j1 = i2 + 1
        goto 100

c   Check if it is the start of a logical constant.

      elseif (cj .eq. '.') then
        i1 = indx (j1 + 1, ln, '.', lina)
        if (i1 .eq. 0) call errmes (70, 1)
        j1 = i1 + 1
        goto 100

c   Check if it is the start of a character constant.

      elseif (cj .eq. '"') then
        i1 = indx (j1 + 1, ln, '"', lina)
        if (i1 .eq. 0) call errmes (71, 1)
        j1 = i1 + 1
        goto 100
      elseif (cj .eq. '''') then
        i1 = indx (j1 + 1, ln, '''', lina)
        if (i1 .eq. 0) call errmes (72, 1)
        j1 = i1 + 1
        goto 100
      endif

c   Check if it is one of the miscellaneous symbols.

      i1 = index (del, cj)
      if (i1 .eq. 0) then
        call errmes (73, 0)
        write (6, 1) cj
 1      format ('Illegal character: ',a)
        call abrt
      endif
      j1 = j1 + 1
      goto 100

 130  continue
      return
      end

      function itab (ic, itp, nam)

c   This routine searches for NAM in the variable table and returns its index
c   if found.  If it is not found, the action depends on the value of IC as
c   follows:
c     IC
c     0   Name is not entered in table and zero is returned.
c     1   New names are entered in table.  Four letter names starting with DP
c         or MP are NOT allowed.  The entry in KTYP is set to ITP if ITP is
c         nonzero, otherwise to the implicit type.
c     2   New names are entered in table.  Four letter names starting with DP
c         or MP are allowed.  The entry in KTYP is set to ITP if ITP is
c         nonzero, otherwise to the implicit type.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*16 nam, namx, ucase
      character*1 cj

c   Set NAMX = NAM with upper case alphabetics and check table.

      namx = ucase (nam)

      do 100 i = 1, nvar
        if (var(i) .eq. namx) then
          itab = i
          goto 120
        endif
 100  continue

c   NAMX is not in table.

      if (ic .eq. 0) then
        itab = 0
        goto 120
      endif

c   Check if NAMX is a common Fortran keyword -- if so, don't enter in table.

      do 110 i = 1, nky
        if (namx .eq. keyw(i)) then
          itab = 0
          goto 120
        endif
 110  continue

c   Check if it is a four-letter name starting with DP or MP.

      l1 = lnblk (namx)
      if (ic .eq. 1 .and. l1 .eq. 4) then
        if (namx(1:2) .eq. 'DP' .or. namx(1:2) .eq. 'MP') then
          call errmes (74, 0)
          write (6, 1)
 1        format ('Four-letter names starting with DP or MP are not',
     $      ' allowed.')
          call abrt
        endif
      endif

c   Add NAMX to table.

      nvar = nvar + 1
      if (nvar .gt. mvar) then
        call errmes (75, 0)
        write (6, 2)
 2      format ('Subprogram has too many variables.')
        call abrt
      endif

      var(nvar) = namx
      kdec(nvar) = 0
      kdim(nvar) = 0
      lvar(nvar) = l1

c   If ITP is greater than zero, set the type of the new variable to ITP.
c   If ITP is zero, set the type to the implicit type.  The type is stored
c   in the array KTYP.  Whether explicit or implicit is stored in MPLC.

      if (itp .gt. 0) then
        ktyp(nvar) = itp
        mplc(nvar) = 0
      else
        cj = namx(1:1)
        i1 = index (alpu, cj)
        if (i1 .eq. 0) call errmes (76, 1)
        ktyp(nvar) = impl(i1)
        mplc(nvar) = 1
      endif
      itab = nvar

 120  return
      end

      function lnblk (lin)
      character*(*) lin

c   This finds the index of the last non-blank character in LIN.

      ln = len (lin)

      do 100 i = ln, 1, -1
        if (lin(i:i) .ne. ' ') goto 110
 100  continue

      i = 0
 110  lnblk = i

      return
      end

      function match (ic, k1, la, lina)

c   This finds the location (up to LA) in LINA of the right parenthesis that
c   matches the left parenthesis at location K1 - 1.  If IC is nonzero, a comma
c   will also will be accepted as a terminator.  Parentheses or commas in
c   character constants are ignored.

      character*2000 lina
      character*1 cj

      j1 = k1
      ip = 0
      match = 0

 100  if (j1 .gt. la) goto 110
      j1 = nblk (j1, la, lina)
      if (j1 .eq. 0) goto 110
      cj = lina(j1:j1)
      if (cj .eq. ')' .or. (cj .eq. ',' .and. ic .ne. 0 .and.
     $  ip .eq. 0)) then
        if (ip .eq. 0) then
          match = j1
          goto 110
        else
          ip = ip - 1
          j1 = j1 + 1
          goto 100
        endif
      elseif (cj .eq. '(') then
        ip = ip + 1
        j1 = j1 + 1
        goto 100
      elseif (cj .eq. '"') then
        j1 = indx (j1 + 1, la, '"', lina)
        if (j1 .eq. 0) call errmes (77, 1)
        j1 = j1 + 1
        goto 100
      elseif (cj .eq. '''') then
        j1 = indx (j1 + 1, la, '''', lina)
        if (j1 .eq. 0) call errmes (78, 1)
        j1 = j1 + 1
        goto 100
      else
        j1 = j1 + 1
        goto 100
      endif

 110  continue

      return
      end

      subroutine mpdec (ln)

c   This checks to see if a comment is a MP directive.  LN is the index of
c   the last non-blank character.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
c>   Set CON to be the Log_10 BDX, where BDX is the radix in MPFUN.
c    For IEEE and other systems for which BDX = 2^24, CON = 7.224719896D0.
c    For Cray non-IEEE systems, BDX = 2^22 and CON = 6.622659905D0.

      double precision con, cl2, rlt, t1
      parameter (con = 7.224719896d0, rlt = 3.3219280948873621d0)
      character*16 linq, numx, numy, ucase

      j1 = nblk (5, ln, line)

      lq = min (j1 + 15, ln)
      linq = ucase (line(j1:lq))
      if (linq(1:15) .eq. 'PRECISION LEVEL') then
        if (mxp .ne. 0 .or. istp .ne. 0) goto 110
        if (iex .ne. 0) goto 140
        numx = line(j1+15:ln)
        read (numx, '(BN,I16)', err = 120) k
        if (k .le. 0) goto 120
        mxp = (k + 7) / con
        mpp = con * mxp
        t1 = rlt * (7 - k)
        n1 = t1 - 1.d0
        t1 = 2.d0 ** (t1 - n1)
        write (numy, '(F14.12,''D0'')') t1
        i1 = nblk (1, 16, numy)
        lep(1) = 17 - i1
        eps(1) = numy(i1:16)
        write (numy, '(I10)') n1
        i1 = nblk (1, 10, numy)
        lep(2) = 11 - i1
        eps(2) = numy(i1:10)
      elseif (linq(1:13) .eq. 'SCRATCH SPACE') then
        if (mxp .eq. 0) goto 100
        if (mss .ne. 0 .or. istp .ne. 0) goto 110
        if (iex .ne. 0) goto 140
        numx = line(j1+13:ln)
        read (numx , '(BN,I16)', err = 120) k
        if (k .le. 0) goto 120
        mss = k
      elseif (linq(1:8) .eq. 'IMPLICIT') then
        if (mxp .eq. 0) goto 100
        if (istp .eq. 0) goto 110
        if (iex .ne. 0) goto 140
        k1 = nblk (j1 + 8, ln, line)
        call implic (k1, ln)
        mpt = 1
      elseif (linq(1:10) .eq. 'TYPE ERROR') then
        if (mxp .eq. 0) goto 100
        if (iex .ne. 0) goto 140
        j1 = nblk (j1 + 10, ln, line)
        lq = min (j1 + 15, ln)
        linq = ucase (line(j1:lq))
        if (linq(1:2) .eq. 'ON') then
          ite = 1
        elseif (linq(1:3) .eq. 'OFF') then
          ite = 0
        else
          call errmes (79, 0)
        endif
      elseif (linq(1:10) .eq. 'MIXED MODE') then
        if (mxp .eq. 0) goto 100
        if (iex .ne. 0) goto 140
        j1 = nblk (j1 + 10, ln, line)
        lq = min (j1 + 15, ln)
        linq = ucase (line(j1:lq))
        if (linq(1:4) .eq. 'SAFE') then
          imm = 1
        elseif (linq(1:4) .eq. 'FAST') then
          imm = 0
        else
          call errmes (80, 0)
        endif
      elseif (linq(1:16) .eq. 'OUTPUT PRECISION') then
        if (mxp .eq. 0) goto 100
        if (iex .ne. 0) goto 140
        numx = line(j1+16:ln)
        read (numx, '(BN,I16)', err = 120) k
        if (k .le. 0) goto 120
        mpp = k
      elseif (linq(1:7) .eq. 'EPSILON') then
        if (mxp .eq. 0) goto 100
        if (iex .ne. 0) goto 140
        k1 = nblk (j1 + 7, ln, line)
        i1 = max (indx (k1, ln, 'E', line), indx (k1, ln, 'e', line))
        if (i1 .eq. 0) call errmes (81, 1)
        numx = line(k1:i1-1)
        read (numx, '(F16.0)') t1
        numx = line(i1+1:ln)
        read (numx, '(BN,I16)') n1
        if (t1 .le. 0 .or. n1 .ge. 0) call errmes (82, 1)
        t1 = rlt * (n1 + log10 (t1))
        n1 = t1 - 1.d0
        t1 = 2.d0 ** (t1 - n1)
        write (numy, '(F14.12,''D0'')') t1
        i1 = nblk (1, 16, numy)
        lep(1) = 17 - i1
        eps(1) = numy(i1:16)
        write (numy, '(I10)') n1
        i1 = nblk (1, 10, numy)
        lep(2) = 11 - i1
        eps(2) = numy(i1:10)
      else

c   Check for MP type declarative.

        it = ntype (j1, ln)
        if (it .ge. 8) then
          if (mxp .eq. 0) goto 100
          if (iex .ne. 0) goto 140
          call type (it, j1, ln)
          mpt = 1
          if (it .eq. 10) kcon(1) = 1
        else
          goto 130
        endif
      endif
      goto 150

 100  call errmes (83, 0)
      write (6, 1)
 1    format ('Precision level has not yet been declared.')
      call abrt

 110  call errmes (84, 0)
      write (6, 2)
 2    format ('Improper placement of MP directive.')
      call abrt

 120  call errmes (85, 0)
      write (6, 3)
 3    format ('Improper integer constant.')
      call abrt

 130  call errmes (86, 0)
      write (6, 4)
 4    format ('Unrecognized CMP+ directive.')
      call abrt

 140  call errmes (87, 0)
      write (6, 5)
 5    format ('A declarative statement may not appear after an',
     $  ' executable statement.')
      call abrt

 150  return
      end

      function nblk (k1, k2, lin)

c   This finds the index of the first non-blank character in LIN between
c   positions K1 and K2.  LIN may be of any character type.

      character*(*) lin

      do 100 i = k1, k2
        if (lin(i:i) .ne. ' ') goto 110
 100  continue

      i = 0
 110  nblk = i

      return
      end

      function ntype (k1, k2)

c   Identifies type declarations in type statements or implicit statements
c   and repositions pointer one past end of declarative.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*16 charq, linq, ucase
      parameter (charq = '*0123456789')

      lq = min (k1 + 15, k2)
      linq = ucase (line(k1:lq))
      if (linq(1:7) .eq. 'INTEGER') then
        ntype = 1
        k1 = k1 + 6
100     k1 = nblk (k1 + 1, k2, line)
        if (index (charq, line(k1:k1)) .ne. 0) goto 100
      elseif (linq(1:5) .eq. 'REAL ' .or. linq(1:5) .eq. 'REAL*') then
        ntype = 2
        k1 = nblk (k1 + 4, k2, line)
        if (line(k1:k1) .eq. '*') then
          k1 = nblk (k1 + 1, k2, line)
          if (line(k1:k1) .eq. '8') ntype = 3
          k1 = k1 + 1
        endif
      elseif (linq(1:6) .eq. 'DOUBLE') then
        ntype = 3
        k1 = nblk (k1 + 6, k2, line)
        lq = min (k1 + 15, k2)
        linq = ucase (line(k1:lq))
        if (linq(1:9) .eq. 'PRECISION') then
          k1 = k1 + 9
        elseif (linq(1:7) .eq. 'COMPLEX') then
          ntype = 5
          k1 = k1 + 7
        endif
      elseif (linq(1:7) .eq. 'COMPLEX') then
        ntype = 4
        k1 = nblk (k1 + 7, k2, line)
        if (line(k1:k1) .eq. '*') then
          k1 = nblk (k1 + 1, k2, line)
          if (line(k1:k1+1) .eq. '16') then
            ntype = 5
            k1 = k1 + 2
          else
            k1 = k1 + 1
          endif
        endif
      elseif (linq(1:9) .eq. 'CHARACTER') then
        ntype = 6
        k1 = k1 + 8
110     k1 = nblk (k1 + 1, k2, line)
        if (line(k1:k1+2) .eq. '(*)') then
          k1 = nblk (k1 + 3, k2, line)
        elseif (index (charq, line(k1:k1)) .ne. 0) then
          goto 110
        endif
      elseif (linq(1:7) .eq. 'LOGICAL') then
        ntype = 7
        k1 = k1 + 6
120     k1 = nblk (k1 + 1, k2, line)
        if (index (charq, line(k1:k1)) .ne. 0) goto 120
      elseif (linq(1:14) .eq. 'MULTIP INTEGER') then
        ntype = 8
        k1 = nblk (k1 + 14, k2, line)
      elseif (linq(1:11) .eq. 'MULTIP REAL') then
        ntype = 9
        k1 = nblk (k1 + 11, k2, line)
      elseif (linq(1:14) .eq. 'MULTIP COMPLEX') then
        ntype = 10
        k1 = nblk (k1 + 14, k2, line)
      else
        ntype = 0
      endif

      return
      end

      function numcon (k1, k2, la, lina)

c   This parses numeric constants, returning the type of the constant.
c   K1 is the index of the start of the constant, and K2 is the index of
c   the end (an output value).  LA is the index of the last non-blank
c   character in LINA.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*1 cj

c   IB =  1 if the previous character was a blank, 0 otherwise.
c   ID =  1 if a digit has occurred, 0 otherwise.  Reset to 0 after D or E.
c   IP =  1 if a period has occurred, 0 otherwise.
c   IS =  1 if a sign has occurred, 0 otherwise.  Reset to 0 after D or E.
c   IT =  The type number of the constant (1, 2, 3 or 9).
c   IX =  1 if a D or E has occurred, 0 otherwise.

      ib = 0
      id = 0
      ip = 0
      is = 0
      it = 1
      ix = 0

      do 100 j = k1, la
        cj = lina(j:j)
        if (index (dig, cj) .ne. 0) then
          id = 1
          goto 100
        elseif (cj .eq. '.') then
          if (ip .ne. 0 .or. ix .ne. 0 .or. j .eq. ib + 1) goto 110
          ip = 1
          it = 2
          goto 100
        elseif (cj .eq. 'D' .or. cj .eq. 'd') then
          if (ix .eq. 1) call errmes (88, 1)
          id = 0
          is = 0
          it = 3
          ix = 1
          goto 100
        elseif (cj .eq. 'E' .or. cj .eq. 'e') then
          if (ix .eq. 1) call errmes (89, 1)
          id = 0
          is = 0
          it = 2
          ix = 1
          goto 100
        elseif (cj .eq. '+' .or. cj .eq. '-') then
          if (id .ne. 0 .or. is .ne. 0) goto 110
          if (ip .ne. 0 .and. ix .eq. 0) call errmes (90, 1)
          is = 1
          goto 100
        elseif (cj .eq. ' ') then
          ib = j
          goto 100
        else
          goto 110
        endif
 100  continue

      j = la + 1

c   Numeric constant has been parsed.  Trim any trailing blanks.

 110  k2 = j - 1
 120  if (k2 .ge. k1 .and. lina(k2:k2) .eq. ' ') then
        k2 = k2 - 1
        goto 120
      endif
      numcon = it

      return
      end

      subroutine outlin (lu, ic, la, lina)

c   This outputs Fortran statements.  If IC is 0, LINA is a comment line up
c   to 80 characters long.  If IC is 1, LINA is a possibly multiline Fortran
c   statement.  If IC is 2, LINA is a possibly multiline Fortran statement that
c   will be output with 'CMP>' at the start of each line.

      character*2000 lina

      if (ic .eq. 0) then
        i1 = max (la, 1)
        write (lu, 1) lina(1:i1)
 1      format (a)
      elseif (ic .eq. 1) then
        i1 = min (la, 72)
        write (lu, 1) lina(1:i1)

        do 100 i = 73, la, 66
          i1 = min (i + 65, la)
          write (lu, 2) lina(i:i1)
 2        format (5x,'$',a)
 100    continue

      elseif (ic .eq. 2) then
        i1 = min (la, 72)
        write (lu, 3) lina(7:i1)
 3      format ('CMP>    ',a)

        do 110 i = 73, la, 66
          i1 = min (i + 65, la)
          write (lu, 4) lina(i:i1)
 4        format ('CMP>   $',a)
 110    continue

      endif

      return
      end

      subroutine param (k1, ln)

c   This processes parameter statements.  K1 is the index of the first
c   character after 'PARAMETER'.  LN is the index of the last non-blank
c   character.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*80 arg(mar), argx
      dimension itar(mar), lar(mar)

c   Use FIXSUB to change names of special constants.

      k2 = ln
      call fixsub (k1, k2, ln)

c   Use ARLIST to process each separate expression.

      j1 = nblk (k1, ln, line)
      if (line(j1:j1) .ne. '(' .or. line(ln:ln) .ne. ')')
     $  call errmes (91, 1)
      la = ln - j1 - 1
      lina(1:la) = line(j1+1:ln-1)
      call arlist (12, la, lina, nar, itar, lar, arg)
      if (nar .eq. 0) call errmes (92, 1)
      mpa = mpa + nar

c   Make sure that all expressions were assignments.

      do 100 i = 1, nar
        if (lar(i) .ne. 0) call errmes (93, 1)
 100  continue

      return
      end

      subroutine rdwr (irw, k1, k2, ln)

c   This processes read and write statements, depending on whether IRW is
c   1 or 2.  K1 and K2 and the indices of the parentheses.  LN is the index
c   of the last non-blank character.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina
      character*80 arg(mar), argk
      character*16 unit, nam
      character*8 numx, numy
      dimension itar(mar), lar(mar)
      character*4 tmp1, tmp2, tmp3, getmp

c   Place the output precision parameter into a character variable.

      write (numy, '(I8)') mpp
      i1 = nblk (1, 8, numy)
      lx = 9 - i1
      numx = numy(i1:8)

c   Determine unit number.

      i1 = indx (k1, ln, ',', line)
      if (i1 .ne. 0 .and. i1 .lt. k2) then
        i2 = i1
      else
        i2 = k2
      endif
      lun = i2 - k1 - 1
      unit(1:lun) = line(k1+1:i2-1)

c   Check if there is a * for the format.

      if (i2 .lt. k2) then
        i2 = nblk (i2 + 1, k2, line)
        if (line(i2:k2-1) .ne. '*') then
          call errmes (94, 0)
          write (6, 1)
 1        format ('This form of read/write statement is not allowed',
     $      ' with MP variables.')
          call abrt
        endif
      else

c   There is no star or format number -- this is a binary I/O statement.
c   Check to make sure there are no parentheses.

        i1 = indx (k2 + 1, ln, '(', line)
        if (i1 .ne. 0) then
          call errmes (95, 0)
          write (6, 2)
 2        format ('Only entire arrays may be output with binary MP',
     $      ' I/O.')
          call abrt
        endif
        call outlin (11, 1, ln, line)
        goto 120
      endif

c   Form a list of the arguments.

      j1 = nblk (k2 + 1, ln, line)
      la = ln - j1 + 1
      lina(1:la) = line(j1:ln)
      call arlist (11, la, lina, nar, itar, lar, arg)

c   Process the statement.

      do 100 k = 1, nar
        argk = arg(k)
        lk = lar(k)
        itk = itar(k)

c   Set NAM to be the argument without subscripts.

        i1 = index (argk(1:lk), '(')
        if (i1 .eq. 0) then
          i2 = lk
        else
          i2 = i1 - 1
        endif
        i2 = min (i2, 16)
        nam = argk(1:i2)

c   Check if the argument is a constant or a variable.

        if (max (index (alpl, nam(1:1)), index (alpu, nam(1:1))) .gt. 0)
     $    then

c   Check if the variable has subscripts and if it is dimensioned.

          ix = itab (0, 0, nam)
          if (ix .eq. 0) then
            call errmes (96, 0)
            write (6, 3) nam
 3          format ('This Fortran keyword may not appear in a',
     $        ' read/write statement: ',a)
            call abrt
          elseif (i1 .eq. 0 .and. kdim(ix) .ne. 0 .and.
     $        kdim(ix) .ne. -3) then
            call errmes (97, 0)
            write (6, 4) nam
 4          format ('Dimensioned variables must be subscripted in this',
     $        ' form of read/write: ',a)
            call abrt
          endif
        endif

c   Check if it is read or a write.

        if (irw .eq. 1) then
          if (itk .lt. 8) then

c   Read an ordinary non-MP variable.

            write (11, 5) unit(1:lun), argk(1:lk)
 5          format (6x,'READ (',a,', *) ',a)
          elseif (itk .ne. 10) then

c   Read a MPI or MPR variable, possibly on multiple lines.

            tmp1 = getmp (6)
            write (11, 6) unit(1:lun), argk(1:lk), tmp1
 6          format (6x,'CALL MPINP (',a,', ',a,', ',a,')')
          else

c   Read a MPC variable.

            tmp1 = getmp (6)
            tmp2 = getmp (9)
            tmp3 = getmp (9)
            write (11, 6) unit(1:lun), tmp2, tmp1
            write (11, 6) unit(1:lun), tmp3, tmp1
            write (11, 7) tmp2, tmp3, argk(1:lk)
 7          format (6x,'CALL MPMMPC (',a,', ',a,', MPNW4, ',a,')')
            call rltmp (tmp2)
            call rltmp (tmp3)
          endif
        else
          if (itk .eq. 1) then

c   Write an integer variable.

            write (11, 8) unit(1:lun), argk(1:lk)
 8          format (6x,'WRITE (',a,', ''(I12)'') ',a)
          elseif (itk .ge. 2 .and. itk .le. 5) then

c   Write an SP, DP, CO or DC variable.

            write (11, 9) unit(1:lun), argk(1:lk)
 9          format (6x,'WRITE (',a,', ''(1P2D25.15)'') ',a)
          elseif (itk .eq. 6) then

c   Write a character variable.

            write (11, 10) unit(1:lun), argk(1:lk)
 10         format (6x,'WRITE (',a,', ''(A)'') ',a)
          elseif (itk .eq. 7) then

c   Write a logical variable.

            write (11, 11) unit(1:lun), argk(1:lk)
 11         format (6x,'WRITE (',a,', ''(L4)'') ',a)
          elseif (itk .ne. 10) then

c   Write a MPI or MPR variable.

            tmp1 = getmp (6)
            write (11, 12) unit(1:lun), arg(k)(1:lar(k)), numx(1:lx),
     $        tmp1
 12         format (6x,'CALL MPOUT (',a,', ',a,', ',a,', ',a,')')
          else

c   Read a MPC variable.

            tmp1 = getmp (6)
            tmp2 = getmp (9)
            tmp3 = getmp (9)
            write (11, 13) argk(1:lk), tmp2, tmp3
 13         format (6x,'CALL MPMPCM (MPNW4, ',a,', ',a,', ',a,')')
            write (11, 12) unit(1:lun), tmp2, numx(1:lx), tmp1
            write (11, 12) unit(1:lun), tmp3, numx(1:lx), tmp1
            call rltmp (tmp2)
            call rltmp (tmp3)
          endif
        endif
 100  continue

c   Release any temporaries among the arguments.

      do 110 i = 1, nar
        li = lar(i)
        if (li .eq. 4) then
          if (arg(i)(1:2) .eq. 'MP') then
            tmp1 = arg(i)(1:4)
            call rltmp (tmp1)
          endif
        endif
 110  continue

      goto 120

 120  return
      end

      subroutine rltmp (tmp)

c   This releases temporary variable TMP for future use.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*4 tmp
      character*1 cx

      read (tmp, 1, err = 110) cx, k
 1    format (2x,a1,i1)

      do 100 i = 1, ntyp
        if (ctm(i) .eq. cx) goto 120
 100  continue

 110  call errmes (98, 0)
      write (6, 2) tmp
 2    format ('RLTMP could not find temporary variable in table : ',a/
     $  'Please contact the author.')
      call abrt

 120  itmp(k,i) = 0

      return
      end

      subroutine type (itp, k1, ln)

c   This processes type statements by delimiting variable names, inserting in
c   table if required (with types set to ITP) and placing any previously
c   declared MP variables in a separate statement, with dimensions corrected.
c   If ITP = 0, this is a flag that the statement being processed is an
c   external directive, and no types are set.  K1 and LN are the indices of the
c   first (after the type name) and last non-blank characters in the statement.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+
      character*2000 lina, linb
      character*16 nam, namq
      character*1 cj
      character*8 dim1, dim2, dimy

c  Output the original line as a comment, unless (1) this is a MP directive or
c  (2) this is a non-MP type statement and no MP type directives have yet
c  appeared in this routine.

      if (itp .lt. 8 .and. mpt .ne. 0) call outlin (11, 2, ln, line)

c   Form the start of LINA and LINB.

      ka = 12
      na = 0
      lina(1:ka-1) = '      REAL '
      kb = k1
      nb = 0
      linb(1:kb-1) = line(1:kb-1)
      linb(kb:kb) = ' '
      kb = kb + 1

c   Place the MP dimension into the character variables DIM1 and DIM2.

      write (dimy, '(I8)') mxp + 4
      i1 = nblk (1, 8, dimy)
      ld1 = 9 - i1
      dim1 = dimy(i1:8)
      write (dimy, '(I8)') 2 * mxp + 8
      i1 = nblk (1, 8, dimy)
      ld2 = 9 - i1
      dim2 = dimy(i1:8)
      j1 = k1

c   Extract the next character from the line.

 100  if (j1 .gt. ln) goto 130
      j1 = nblk (j1, ln, line)
      cj = line(j1:j1)

c   Check if it the start of a name.

      if (max (index (alpl, cj), index (alpu, cj)) .ne. 0) then

        do 110 j = j1, ln
          cj = line(j:j)
          if (max (index (alpl, cj), index (alpu, cj)) .ne. 0) goto 110
          if (index (dig, cj) .ne. 0 .or. cj .eq. '_') goto 110
          if (index (del, cj) .ne. 0) goto 120
          call errmes (99, 1)
 110    continue

        j = ln + 1
 120    j2 = j - 1
        nam = line(j1:j2)

c   Add entry to variable table.  With a few exceptions, it should not already
c   be in the table.  Exceptions: the function name, variables in the argument
c   list, variables in previous MP and conventional explicit type statements,
c   the special constants and the special functions.

        ix = itab (0, 0, nam)
        if (ix .ne. 0 .and. (itp .ge. 8 .or. itp .gt. 0. and.
     $    itp .lt. 8 .and. mplc(ix) .eq. 1)) then
          call errmes (100, 0)
          write (6, 1) nam
 1        format ('This reserved or previously defined name may not',
     $      ' appear in a type'/'statement: ',a)
          call abrt
        endif
        ix = itab (1, itp, nam)
        if (ix .eq. 0) then
          call errmes (101, 0)
          write (6, 2) nam
 2        format ('This Fortran keyword may not appear in a type',
     $      ' statement: ',a)
          call abrt
        endif
        namq = 'DPCSSH'
        iq = itab (0, 0, namq)
        if (itp .eq. 0 .and. mplc(ix) .eq. 1 .and. ix .gt. iq)
     $    ktyp(ix) = 0
        ktp = ktyp(ix)

c   If this is a CMP+ directive, there should be no dimensions.

        if (itp .ge. 8) then
          i1 = index (line(j2:ln), '(')
          if (i1 .ne. 0) then
            call errmes (102, 0)
            write (6, 3)
 3          format ('MP type directives may not specify dimensions.')
            call abrt
          endif
          j1 = j2 + 1
          goto 100
        endif

c   Check if this is a MP variable with a dimension.  If so, copy it to LINA.
c   If it is a MP variable without a dimension, copy it to neither line.

        if (itp .ne. 0 .and. ktp .ge. 8) then
          if (j2 .lt. ln) then
            j3 = nblk (j2 + 1, ln, line)
            cj = line(j3:j3)
            if (cj .eq. '(') then
              kdec(ix) = 1
              na = na + 1
              lina(ka:ka+j2-j1) = line(j1:j2)
              ka = ka + j2 - j1 + 1
              j1 = j3

c   Insert MP dimension as the first dimension.

              lina(ka:ka) = '('
              if (ktp .lt. 10) then
                lina(ka+1:ka+ld1) = dim1(1:ld1)
                ka = ka + ld1 + 1
              else
                lina(ka+1:ka+ld2) = dim2(1:ld2)
                ka = ka + ld2 + 1
              endif
              kdim(ix) = 1
              lina(ka:ka) = ','
              ka = ka + 1
              j2 = match (0, j1 + 1, ln, line)
              if (j2 .eq. 0) call errmes (103, 1)
              i1 = iscan (j1, j2, line)
              if (i1 .ne. 0) then
                call errmes (104, 0)
                write (6, 4) nam
 4              format ('The MP dimension on this variable is not',
     $            ' allowed: ',a)
                call abrt
              endif
              lina(ka:ka+j2-j1-1) = line(j1+1:j2)
              ka = ka + j2 - j1
              lina(ka:ka+1) = ', '
              ka = ka + 2
            endif
          endif

c   Otherwise this is an ordinary variable -- copy to LINB.

        else
          kdec(ix) = 1
          nb = nb + 1
          linb(kb:kb+j2-j1) = line(j1:j2)
          kb = kb + j2 - j1 + 1
          if (j1 .lt. ln) then
            j3 = nblk (j2 + 1, ln, line)
            cj = line(j3:j3)
            if (cj .eq. '(') then
              kdim(ix) = 1
              linb(kb:kb) = '('
              kb = kb + 1
              j1 = j3
              j2 = match (0, j1 + 1, ln, line)
              i1 = iscan (j1, j2, line)
              if (i1 .ne. 0) then
                call errmes (105, 0)
                write (6, 4) nam
                call abrt
              endif
              linb(kb:kb+j2-j1-1) = line(j1+1:j2)
              kb = kb + j2 - j1
            endif
          endif
          linb(kb:kb+1) = ', '
          kb = kb + 2
        endif
        j1 = j2 + 1
        goto 100

c   The only other character that should appear here is a comma.

      elseif (cj .eq. ',') then
        j1 = j1 + 1
        goto 100
      else
        call errmes (106, 1)
      endif

c   Output LINA and LINB, provided there is something to output.

 130  if (na .gt. 0) call outlin (11, 1, ka - 3, lina)
      if (nb .gt. 0) call outlin (11, 1, kb - 3, linb)
      if (itp .lt. 8 .and. mpt .ne. 0) write (11, 5)
 5    format ('CMP<')

      return
      end

      function ucase (nam)

c   This routine returns the character string NAM with upper case alphabetics.
c+
      parameter (mar = 40, mvar = 400, mifl = 10, mint = 54, msub = 100,
     $  ndo = 50, nky = 45, nop = 15, nsf = 50, ntyp = 10)
      character*2000 line, sarg
      character*26 alpl, alpu
      character*16 eps, fnam, keyw, sfun, snam, var
      character*12 del
      character*10 dig
      character*8 ctp, lopr, uopr
      character*1 ctm
      common /cmp1/ iex, imm, istp, ite, kdo, lct, lsar, lsm, mpa, mpp,
     $  mpt, mss, mxp, nsub, nvar
      common /cmp2/ idon(ndo), ifbl(mifl), impl(26), imps(26), 
     $  itmp(9,ntyp), ktyp(mvar), kcon(5), kdec(mvar), kdim(mvar), 
     $  kstp(0:mar,msub), lvar(mvar), kop(nop), lop(nop), lep(2), 
     $  mplc(mvar), nars(msub)
      common /cmp3/ alpl, alpu, del, dig, fnam, line, sarg
      common /cmp4/ ctm(ntyp), ctp(ntyp), eps(2), keyw(nky), lopr(nop),
     $  sfun(nsf), snam(msub), uopr(nop), var(mvar)
c+

      character*16 namx, ucase
      character*(*) nam
      character*1 cj

      namx = ' '
      lq = len (nam)

      do 100 j = 1, lq
        cj = nam(j:j)
        i1 = index (alpl, cj)
        if (i1 .gt. 0) then
          namx(j:j) = alpu(i1:i1)
        else
          namx(j:j) = cj
        endif
 100  continue

      ucase = namx
      return
      end
