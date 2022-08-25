CFLAGS = -Wall -ggdb

LDFLAGS = -lncurses 
BIN = poke

CC=g++
DEPS = 
OBJ = parsing.o user_interface.o character.o map_cell.o character_behaviour.o world.o heap.o map.o  path_finding.o 

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJ)
	$(CC)  $^ $(CFLAGS) $(LDFLAGS) -o $@ 

clean:
	rm -f *~ *.o $(BIN) core vgcore* .nfs*