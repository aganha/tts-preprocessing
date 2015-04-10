
AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS =-c -O2 -Wall 
AM_LDFLAGS =-L/usr/local/lib -Wno-unused-but-set-variable -Wno-unused-variable -Wno-write-strings -Wno-sign-compare -Wno-unused-result -Wno-uninitialized


bin_PROGRAMS = Traductor
Traductor_SOURCES = main.cpp diccitem.cpp diccitem.h Avl.h Avl2.h Comparable.h
Traductor_LDADD =-lboost_filesystem -lboost_regex -lfreeling  -ldl


