# YOU WILL MOST LIKELY NEED TO CHANGE THE LIBSPEC variable to suit your
# compiler setup.

# Specify the compiler to use
CC=gcc
# Specify the assembler to use
AS=as
# Specity the linker to use
LD=ld
# Added in debugging data 
CCFLAGS=-g
# Added in debugging data 
LDFLAGS=-g


# List the object files involved in this project
OBJS= console.o	main.o realm.o

# The default 'target' (output) is main.elf and it depends on the object files being there.
# These object files are linked together to create main.elf
main.elf : $(OBJS)
	gcc $(OBJS) $(LDFLAGS) -o microrealms	
	@echo "Assuming all went well..."
	@echo "run the program with ./microrealms"	
# The object file main.o depends on maiqn.c.  main.c is compiled to make main.o
main.o: main.c
	$(CC) -c $(CCFLAGS) main.c -o main.o

console.o: console.c
	$(CC) -c $(CCFLAGS) console.c -o console.o

realm.o: realm.c
	$(CC) -c $(CCFLAGS) realm.c -o realm.o


# if someone types in 'make clean' then remove all object files and executables
# associated wit this project
clean: 
	rm $(OBJS) 
	rm microrealms
