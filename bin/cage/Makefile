PROGRAM      = cage
TEST_PROGRAM = test_$(PROGRAM)

CC = g++

ROOT_DIR    = /home/pi/workspace/robot
BIN_DIR     = $(ROOT_DIR)/bin
INCLUDE_DIR = $(ROOT_DIR)/include
LIB_DIR     = $(ROOT_DIR)/lib

VPATH       = $(BIN_DIR)/i2cdispatcher:$(BIN_DIR)/messageformatter:$(LIB_DIR):$(BIN_DIR)/coupling

CPPFLAGS    = -I. -I$(INCLUDE_DIR)
LDFLAGS     = -L$(LIB_DIR)

OBJS      = i2cdispatcher.o messageformatter.o conveyor.o coupling.o

TEST_OBJS = $(OBJS) $(TEST_PROGRAM).o 

DEPS = $(PROGRAM).hpp $(INCLUDE_DIR)/i2cdispatcher.hpp $(INCLUDE_DIR)/messageformatter.hpp\
                      $(INCLUDE_DIR)/cage.hpp $(INCLUDE_DIR)/coupling.hpp

$(TEST_PROGRAM) : $(TEST_OBJS) $(DEPS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) $^ -o $(TEST_PROGRAM)

$(TEST_PROGRAM).o: $(DEPS) $(OBJS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) $(DEPS) -c $(TEST_PROGRAM).cpp $(OJBS)

install: 
	cp *.hpp $(INCLUDE_DIR)

clean:
	rm *.o	
	rm *.gch
