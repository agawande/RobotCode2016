PROGRAM      = gpio
TEST_PROGRAM = test_$(PROGRAM)

CC = g++

ROOT_DIR    = /home/pi/workspace/robot
BIN_DIR     = $(ROOT_DIR)/bin
INCLUDE_DIR = $(ROOT_DIR)/include
LIB_DIR     = $(ROOT_DIR)/lib

CPPFLAGS    = -I. -I$(INCLUDE_DIR)
LDFLAGS     = -L$(LIB_DIR) -Wl,-rpath="/usr/local/lib"

OBJS      = $(PROGRAM).o

TEST_OBJS = $(TEST_PROGRAM).o 

DEPS = $(PROGRAM).hpp $(PROGRAM).h

$(TEST_PROGRAM) : $(TEST_OBJS) $(DEPS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) $^ -o $(TEST_PROGRAM) $(PROGRAM).o /usr/local/lib/*wiring*

$(PROGRAM).o : $(DEPS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) -c $(PROGRAM).cpp

$(TEST_PROGRAM).o: $(OBJS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) $(DEPS) -c $(TEST_PROGRAM).cpp 

install: 
	cp *.h $(INCLUDE_DIR)
	cp *.hpp $(INCLUDE_DIR)
	cp $(PROGRAM).o $(LIB_DIR)

clean:
	rm *.o	
	rm *.gch
