##############################################################################
#  example/Makefile
#
#  This software is provided for research purposes only.
#  Commercial usage requires license agreement.
#
#  This work was supported by the Director, Office of Science, Division
#  of Mathematical, Information, and Computational Sciences of the
#  U.S. Department of Energy under contract number DE-AC03-76SF00098.
# 
#  Copyright (c) 2001-2002-2003-2004
#  
#  Modified:       Jul 14, 2004
#  Version:        1.0
#
##############################################################################

TARGET = mathinit mathtool

all: $(TARGET)

###### Fortran Test Programs ######
F90 = xlf90

F90FLAGS = -O3 -qnoescape -qstrict

LD = xlf90

F90MOD	= mpfun90.o mpmod90.o globdata.o

mathinit:  $(F90MOD) mathinit.o second.o
	$(LD) -o $@ mathinit.o second.o $(F90MOD)

mathtool:  $(F90MOD) mathtool.o pslqsub.o quadsub.o rootsub.o zetapz.o second.o
	$(LD) -o $@ mathtool.o pslqsub.o quadsub.o rootsub.o zetapz.o second.o $(F90MOD)


###### General Rules ######
.f.o:
	$(F90) $(F90FLAGS) -c $<

###### Clean Up ######
clean: 
	rm -f *.o *.mod *~ *.d work.pc work.pcl $(TARGET)


