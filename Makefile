
#/******************************************/
#/*                                        */
#/*        Alexander Agdgomlishvili        */
#/*                                        */
#/*         cdevelopment@mail.com          */
#/*                                        */
#/******************************************/

src = clist.c test.c

headers = clist.h

libObjects = clist.o

objects = $(libObjects) test.o

CC = gcc -g -W -O2 $(Cflags)

LIB = libclist.a

TARGET = test

all :  LIBRARY EXECUTABLE

clean :
	rm -rvf *.o *.log *.txt $(LIB) $(TARGET)

%.o : %.c
	$(CC) -c -o $@ $<

LIBRARY : $(libObjects)
	ar rcs $(LIB) $(libObjects) 
		
EXECUTABLE : $(objects)
	$(CC) $(Cflags) $(objects) -o $(TARGET) $(Lflags) $(libraries)
		
STRIP : 
	strip -s $(TARGET)

ECHO_STRING :
	@echo "\n\n-------------------------------------------------------------\n\n"
