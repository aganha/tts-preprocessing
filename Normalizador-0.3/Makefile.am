
AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS =-c -O2 -Wall -Wno-unused-but-set-variable -Wno-unused-variable -Wno-write-strings -Wno-sign-compare -Wno-unused-result -Wno-uninitialized -O2 -Wall  -std=gnu++0x 

AM_LDFLAGS =-L/usr/local/lib 

bin_PROGRAMS = Normalizador
Normalizador_SOURCES = main.cpp utility.cpp misc.cpp fst_string.cpp diccitem.cpp funciones.cpp Avl.h Avl2.h Comparable.h funciones.h utilfst.h diccitem.h
Normalizador_LDADD =-lfst -lboost_filesystem -lboost_regex -lfreeling  -ldl


