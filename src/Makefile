IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) -g

ODIR=obj
LDIR =../lib

LIBS= -lpthread

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ_CLIENT = client.o 
OBJ_CLIENT = $(patsubst %,$(ODIR)/%,$(_OBJ_CLIENT))

_OBJ_SERVER = server.o
OBJ_SERVER = $(patsubst %,$(ODIR)/%,$(_OBJ_SERVER))

EXECUTABLE = server client

ALL: $(EXECUTABLE)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ_SERVER)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

client: $(OBJ_CLIENT)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
