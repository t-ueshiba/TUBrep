#
#  $Id: Makefile,v 1.20 2008-10-15 00:50:32 ueshiba Exp $
#
#################################
#  User customizable macros	#
#################################
DEST		= $(LIBDIR)
INCDIR		= $(HOME)/include/TU/Brep
INCDIRS		= -I. -I$(HOME)/include

NAME		= $(shell basename $(PWD))

CPPFLAGS	= -DTUBrepPP_DEBUG
CFLAGS		= -g
ifeq ($(CCC), icpc)
  CFLAGS	= -O3
  ifeq ($(OSTYPE), darwin)
    CPPFLAGS   += -DSSE3 -axP -ip -parallel
  else
    CPPFLAGS   += -DSSE2 -xN -ip
  endif
endif
CCFLAGS		= $(CFLAGS)

LINKER		= $(CCC)

#########################
#  Macros set by mkmf	#
#########################
SUFFIX		= .cc:sC
EXTHDRS		= /home/ueshiba/include/TU/Array++.h \
		/home/ueshiba/include/TU/Geometry++.h \
		/home/ueshiba/include/TU/Normalize.h \
		/home/ueshiba/include/TU/Object++.cc \
		/home/ueshiba/include/TU/Object++.h \
		/home/ueshiba/include/TU/Vector++.h \
		/home/ueshiba/include/TU/types.h \
		TU/Brep/Brep++.h
HDRS		= Brep++.h
SRCS		= Geometry.cc \
		HalfEdge.cc \
		Loop.cc \
		Neighbor.cc \
		PointB.cc \
		TUBrep++.inst.cc \
		TUBrep++.sa.cc
OBJS		= Geometry.o \
		HalfEdge.o \
		Loop.o \
		Neighbor.o \
		PointB.o \
		TUBrep++.inst.o \
		TUBrep++.sa.o

#########################
#  Macros used by RCS	#
#########################
REV		= $(shell echo $Revision: 1.20 $	|		\
		  sed 's/evision://'		|		\
		  awk -F"."					\
		  '{						\
		      for (count = 1; count < NF; count++)	\
			  printf("%d.", $$count);		\
		      printf("%d", $$count + 1);		\
		  }')

include $(PROJECT)/lib/l.mk
###
Geometry.o: TU/Brep/Brep++.h /home/ueshiba/include/TU/Object++.h \
	/home/ueshiba/include/TU/types.h \
	/home/ueshiba/include/TU/Geometry++.h \
	/home/ueshiba/include/TU/Vector++.h \
	/home/ueshiba/include/TU/Array++.h \
	/home/ueshiba/include/TU/Normalize.h
HalfEdge.o: TU/Brep/Brep++.h /home/ueshiba/include/TU/Object++.h \
	/home/ueshiba/include/TU/types.h \
	/home/ueshiba/include/TU/Geometry++.h \
	/home/ueshiba/include/TU/Vector++.h \
	/home/ueshiba/include/TU/Array++.h \
	/home/ueshiba/include/TU/Normalize.h
Loop.o: TU/Brep/Brep++.h /home/ueshiba/include/TU/Object++.h \
	/home/ueshiba/include/TU/types.h \
	/home/ueshiba/include/TU/Geometry++.h \
	/home/ueshiba/include/TU/Vector++.h \
	/home/ueshiba/include/TU/Array++.h \
	/home/ueshiba/include/TU/Normalize.h
Neighbor.o: TU/Brep/Brep++.h /home/ueshiba/include/TU/Object++.h \
	/home/ueshiba/include/TU/types.h \
	/home/ueshiba/include/TU/Geometry++.h \
	/home/ueshiba/include/TU/Vector++.h \
	/home/ueshiba/include/TU/Array++.h \
	/home/ueshiba/include/TU/Normalize.h
PointB.o: TU/Brep/Brep++.h /home/ueshiba/include/TU/Object++.h \
	/home/ueshiba/include/TU/types.h \
	/home/ueshiba/include/TU/Geometry++.h \
	/home/ueshiba/include/TU/Vector++.h \
	/home/ueshiba/include/TU/Array++.h \
	/home/ueshiba/include/TU/Normalize.h
TUBrep++.inst.o: TU/Brep/Brep++.h /home/ueshiba/include/TU/Object++.h \
	/home/ueshiba/include/TU/types.h \
	/home/ueshiba/include/TU/Geometry++.h \
	/home/ueshiba/include/TU/Vector++.h \
	/home/ueshiba/include/TU/Array++.h \
	/home/ueshiba/include/TU/Normalize.h \
	/home/ueshiba/include/TU/Object++.cc
TUBrep++.sa.o: TU/Brep/Brep++.h /home/ueshiba/include/TU/Object++.h \
	/home/ueshiba/include/TU/types.h \
	/home/ueshiba/include/TU/Geometry++.h \
	/home/ueshiba/include/TU/Vector++.h \
	/home/ueshiba/include/TU/Array++.h \
	/home/ueshiba/include/TU/Normalize.h
