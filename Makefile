# Makefile for testing and installation
LOGLEVEL?=3
CC=cc
CFLAGS=-Wall -g
PREFIX?=/usr/local
.PHONY: all clean install

all: test_ert_log
test_ert_log: test_ert_log.c ert_log.h
	$(CC) $(CFLAGS) -DLOGLEVEL=$(LOGLEVEL) $< -o $@
	./$@
clean:
	rm -f test_ert_log

install:
	install -d $(PREFIX)/include
	install -m 444 ert_log.h $(PREFIX)/include
	install -d $(PREFIX)/man/man0
	install -m 444 ert_log.h.0 $(PREFIX)/man/man0
	install -d $(PREFIX)/man/man7
	install -m 444 ert_log.h.0 $(PREFIX)/man/man7/ert_log.h.7
	install -m 444 ert_log.h.0 $(PREFIX)/man/man7/log_err.7
	install -m 444 ert_log.h.0 $(PREFIX)/man/man7/log_warn.7
	install -m 444 ert_log.h.0 $(PREFIX)/man/man7/log_info.7
	install -m 444 ert_log.h.0 $(PREFIX)/man/man7/log_debug.7
