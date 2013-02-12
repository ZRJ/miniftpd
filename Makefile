.PHONY:clean
CC=gcc
CFLAGS=-Wall -g -std=gnu99
BIN=miniftpd
OBJS=main.o sysutil.o session.o privparent.o ftpproto.o str.o
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)