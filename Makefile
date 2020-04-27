
#/******************************************/
#/*                                        */
#/*        Alexander Agdgomlishvili        */
#/*                                        */
#/*         cdevelopment@mail.com          */
#/*                                        */
#/******************************************/

src = main.c clist.c

headers = clist.h

libObjects = clist.o

objects = $(libObjects) main.o

CC = gcc -g -W -O2 $(Cflags)

LIB = libclist.a

TARGET = test

all :  LIBRARY EXECUTABLE

clean :
	rm -rvf *.o *.png $(LIB) $(TARGET)

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


