# *************************************************************
# FileName: Makefile
# Author:	Glen Gougeon
# Class:	CS362 Software Engineering II
# Created:	11-29-2019
# Last Mod:	11-29-2019
#
# Assignment:	Final Project Part B Unit Testing
#
# Description:	Includes a rules to compile and run unit tests
#
# *************************************************************

CC = gcc # compiler

# FLAGS
CFLAGS = -Wall # Warnings All
CFLAGS += -fpic
CFLAGS += -pedantic
CFLAGS += -coverage # used by gcov
CFLAGS += -lm # req'd. for floor() in rngs.c
CFLAGS += -std=c99 # c99 standard of c-language

# COVERAGE
COV = gcov
COV_FLAGS = -f
COV_FLAGS += -b
rngs.o: rngs.c rngs.h
	${CC} ${CFLAGS}-g -c rngs.c

dominion.o: dominion.c dominion.h dominion_helpers.h rngs.o
	${CC} ${CFLAGS} -g -c dominion.c

interface.o: interface.c interface.h
	${CC} ${CFLAGS} -g -c interface.c

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

runtests: testDrawCard
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out
# ***********************************************************************************

bug1unittest: bug1unittest.c dominion.o rngs.o interface.o
	${CC} ${CFLAGS} -g -o bug1unittest bug1unittest.c dominion.o rngs.o interface.o

# ***********************************************************************************

bug1b_unittest: bug1b_unittest.c dominion.o rngs.o interface.o
	${CC} ${CFLAGS} -g -o bug1b_unittest bug1b_unittest.c dominion.o rngs.o interface.o

# ***********************************************************************************

bug2unittest: bug2unittest.c dominion.o rngs.o interface.o
	${CC} ${CFLAGS} -g -o bug2unittest bug2unittest.c dominion.o rngs.o interface.o

# ***********************************************************************************

bug3unittest: bug3unittest.c dominion.o rngs.o interface.o
	${CC} ${CFLAGS} -g -o bug3unittest bug3unittest.c dominion.o rngs.o interface.o

# ***********************************************************************************

bug4unittest: bug4unittest.c dominion.o rngs.o interface.o
	${CC} ${CFLAGS} -g -o bug4unittest bug4unittest.c dominion.o rngs.o interface.o

# ***********************************************************************************

bug5unittest: bug5unittest.c dominion.o rngs.o interface.o
	${CC} ${CFLAGS} -g -o bug5unittest bug5unittest.c dominion.o rngs.o interface.o

# ***********************************************************************************

bug6unittest: bug6unittest.c dominion.o rngs.o interface.o
	${CC} ${CFLAGS} -g -o bug6unittest bug6unittest.c dominion.o rngs.o interface.o

# ***********************************************************************************

bug7unittest: bug7unittest.c dominion.o rngs.o interface.o
	${CC} ${CFLAGS} -g -o bug7unittest bug7unittest.c dominion.o rngs.o interface.o

# ***********************************************************************************

bug8unittest: bug8unittest.c dominion.o rngs.o interface.o
	${CC} ${CFLAGS} -g -o bug8unittest bug8unittest.c dominion.o rngs.o interface.o

# ***********************************************************************************

bug9unittest: bug9unittest.c dominion.o rngs.o interface.o
	${CC} ${CFLAGS} -g -o bug9unittest bug9unittest.c dominion.o rngs.o interface.o

# ***********************************************************************************

bug10unittest: bug10unittest.c dominion.o rngs.o interface.o
	${CC} ${CFLAGS} -g -o bug10unittest bug10unittest.c dominion.o rngs.o interface.o

# ***********************************************************************************

unittestresults: bug1unittest bug1b_unittest bug2unittest bug3unittest bug4unittest\
	bug5unittest bug6unittest bug7unittest bug8unittest  bug9unittest bug10unittest
	./bug1unittest &> unittestresults.out
	./bug1b_unittest >> unittestresults.out
	./bug2unittest >> unittestresults.out
	./bug3unittest >> unittestresults.out
	./bug4unittest >> unittestresults.out
	./bug5unittest >> unittestresults.out
	./bug6unittest >> unittestresults.out
	./bug7unittest >> unittestresults.out
	./bug8unittest >> unittestresults.out
	./bug9unittest >> unittestresults.out
	./bug10unittest >> unittestresults.out
	${COV} ${COV_FLAGS} dominion.c >> unittestresults.out
	${COV} ${COV_FLAGS} bug1unittest.c >> unittestresults.out
	${COV} ${COV_FLAGS} bug1b_unittest.c >> unittestresults.out
	${COV} ${COV_FLAGS} bug2unittest.c >> unittestresults.out
	${COV} ${COV_FLAGS} bug3unittest.c >> unittestresults.out
	${COV} ${COV_FLAGS} bug4unittest.c >> unittestresults.out
	${COV} ${COV_FLAGS} bug5unittest.c >> unittestresults.out
	${COV} ${COV_FLAGS} bug6unittest.c >> unittestresults.out
	${COV} ${COV_FLAGS} bug7unittest.c >> unittestresults.out
	${COV} ${COV_FLAGS} bug8unittest.c >> unittestresults.out
	${COV} ${COV_FLAGS} bug9unittest.c >> unittestresults.out
	${COV} ${COV_FLAGS} bug10unittest.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	cat bug1unittest.c.gcov >> unittestresults.out
	cat bug1b_unittest.c.gcov >> unittestresults.out
	cat bug2unittest.c.gcov >> unittestresults.out
	cat bug3unittest.c.gcov >> unittestresults.out
	cat bug4unittest.c.gcov >> unittestresults.out
	cat bug5unittest.c.gcov >> unittestresults.out
	cat bug6unittest.c.gcov >> unittestresults.out
	cat bug7unittest.c.gcov >> unittestresults.out
	cat bug8unittest.c.gcov >> unittestresults.out
	cat bug9unittest.c.gcov >> unittestresults.out
	cat bug10unittest.c.gcov >> unittestresults.out

# ***********************************************************************************

#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
playdom: playdom.c dominion.o rngs.o
	${CC} ${CFLAGS} -g -o playdom playdom.c dominion.o rngs.o

player: player.c dominion.o rngs.o interface.o
	${CC} ${CFLAGS} -g -o player player.c dominion.o rngs.o interface.o

all: playdom player unittestresults

# ***********************************************************************************
clean:
	rm -f *.o playdom player *.gcov *.gcda *.gcno *.so *.out *.exe
	rm -f playdom player testDrawCard
	rm -f bug1unittest bug1b_unittest bug2unittest bug3unittest bug4unittest
	rm -f bug5unittest bug6unittest bug7unittest bug8unittest
	rm -f bug9unittest bug10unittest bug11unittest
