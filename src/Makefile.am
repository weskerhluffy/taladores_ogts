## Process this file with automake to produce Makefile.in

include $(top_srcdir)/common.mk

#AM_CFLAGS = -Wall -g -I$(ZLOG_DIR)/include -I$(CACACOMUN_DIR)  -O  -ansi -fno-asm -lm -static -DONLINE_JUDGE 
AM_CFLAGS = -Wall -g -I$(ZLOG_DIR)/include -I$(CACACOMUN_DIR)   -ansi -fno-asm -lm -static 
lib_LTLIBRARIES = libtaladoresogts.la 
libtaladoresogts_la_SOURCES = taladoresogts.c taladoresogts.h

bin_PROGRAMS = main
main_SOURCES = main.c
#main_LDADD =  -L$(ZLOG_DIR)/lib -lzlog -lpthread 
#main_LDFLAGS= -Wl,--stack,0x4000000
#main_LDFLAGS= -Wl,-stack_size -Wl,4000000


