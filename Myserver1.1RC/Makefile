cCC = gcc

ELF = server
ROOTSRC = $(wildcard *.c)
ROOTOBJ = $(ROOTSRC:%.c = %.o)



$ELF:$(ROOTOBJ)
	$(CC) -g  -o $(ELF) $(ROOTOBJ) -lpthread -lsqlite3 -lncurses
clean:
	rm -rf *.o main	
