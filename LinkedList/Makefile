
#/******************************************/
#/*                                        */
#/*        Alexander Agdgomlishvili        */
#/*                                        */
#/*         cdevelopment@mail.com          */
#/*                                        */
#/******************************************/

src = list.c test.c

headers = list.h

libObjects = list.o

objects = $(libObjects) test.o

CC = gcc -g -W -O2 $(Cflags)

LIB = liblist.a

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
