# CSCI 2021 Project 5 Makefile
AN = p5
CLASS = 2021

CFLAGS = -Wall -Werror -g -Og -Wno-unused-result  # remove warning for system() calls
CC     = gcc $(CFLAGS)
SHELL  = /bin/bash
CWD    = $(shell pwd | sed 's/.*\///g')

PROGRAMS = \
	el_malloc.o \
	el_demo \
	test_el_malloc \


all : $(PROGRAMS)

clean :
	rm -f $(PROGRAMS) *.o vgcore.* a.out

help :
	@echo 'Typical usage is:'
	@echo '  > make                          # build all programs'
	@echo '  > make clean                    # remove all compiled items'
	@echo '  > make zip                      # create a zip file for submission'
	@echo '  > make prob1                    # built targets associated with problem 1'
	@echo '  > make test-prob1 testnum=5     # run problem 1 test #5 only'
	@echo '  > make test                     # run all tests'

# @echo '  > make test-prob2               # run test for problem 2'


############################################################
# 'make zip' to create project zip for submission
zip : clean clean-tests
	rm -f $(AN)-code.zip
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi


# EL MALLOC
el_malloc.o : el_malloc.c el_malloc.h
	$(CC) -c $<

el_demo : el_demo.c el_malloc.o
	$(CC) -o $@ $^

test_el_malloc : test_el_malloc.c el_malloc.o
	$(CC) -o $@ $^

# TESTING TARGETS
test: test-prob1

test-setup :
	@chmod u+rx testy

test-prob1: el_demo test_el_malloc test-setup el_demo
	./testy test_el_malloc.org $(testnum)

clean-tests :
	rm -rf test-results test_el_malloc

