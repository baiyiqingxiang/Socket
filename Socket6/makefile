CC=gcc
CFLAGS=-g -Wall -std=c11
all:echosrv2.o echocli.o echocli2.o basicfun.o
	gcc -o echosrv2 echosrv2.o basicfun.o
	gcc -o echocli echocli.o basicfun.o 
	gcc -o echocli2 echocli2.o basicfun.o
echosrv2.o echocli.o echocli2.o: basicfun.h
.PHONY:clean
clean:
	$(RM) *.o echosrv2 echocli echocli2
