###########################################################
#
# Makefile of Error Injector (Bin Wang B.Wang@qub.ac.uk)
#
########################################################

CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = error_generator.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

error_generator: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o
