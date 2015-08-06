include $(top_srcdir)/common.mk

TESTS = check_taladoresogts
check_PROGRAMS = check_taladoresogts
check_taladoresogts_SOURCES = check_taladoresogts.c $(top_builddir)/src/taladoresogts.h
check_taladoresogts_CFLAGS = @CHECK_CFLAGS@ -g3 -I$(ZLOG_DIR)/include  -Wall -I$(CACACOMUN_DIR) -std=c99 #-DDEBUG
check_taladoresogts_LDADD =  $(top_builddir)/src/libtaladoresogts.la @CHECK_LIBS@ -lpthread -L$(CACACOMUN_DIR) -lcacacomun  -L$(ZLOG_DIR)/lib -lzlog 
check_taladoresogts_LDFLAGS= -Wl,-stack_size -Wl,4000000

