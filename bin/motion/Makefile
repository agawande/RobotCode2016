PROGRAM      = motion
TEST_PROGRAM = test_$(PROGRAM)

CC = g++

USR_LIB     = /usr/local/lib

ROOT_DIR    = /home/pi/workspace/robot

BIN_DIR     = $(ROOT_DIR)/bin
INCLUDE_DIR = $(ROOT_DIR)/include
LIB_DIR     = $(ROOT_DIR)/lib

VPATH       = $(BIN_DIR)/i2cdispatcher:$(BIN_DIR)/messageformatter:$(LIB_DIR)

CPPFLAGS    = -I. -I$(INCLUDE_DIR)
LDFLAGS     = -L$(LIB_DIR) -lwiringPi -L/opt/vc/lib -lpthread -lmmal -lmmal_core -lmmal_util

OBJS = $(PROGRAM).o i2cdispatcher.o messageformatter.o coursereader.o coordinate.o laserprocessor.o gpio.o camera.o distancelookup.o

TEST_OBJS = $(OBJS) $(TEST_PROGRAM).o 

DEPS = $(PROGRAM).hpp $(INCLUDE_DIR)/i2cdispatcher.hpp $(INCLUDE_DIR)/messageformatter.hpp\
                      $(INCLUDE_DIR)/coursereader.hpp $(INCLUDE_DIR)/coordinate.hpp\
                      $(INCLUDE_DIR)/laserprocessor.hpp $(INCLUDE_DIR)/gpio.hpp $(INCLUDE_DIR)/camera.hpp\

$(TEST_PROGRAM) : $(TEST_OBJS) $(DEPS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) $^ -o $(TEST_PROGRAM) $(USR_LIB)/*opencv* $(USR_LIB)/*robot*.a

$(PROGRAM).o: $(DEPS)
	$(CC) $(CPPFLAGS) -c $(PROGRAM).cpp

$(TEST_PROGRAM).o: $(DEPS) $(OBJS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) $(DEPS) -c $(TEST_PROGRAM).cpp $(OJBS)

install: 
	cp *.hpp $(INCLUDE_DIR)
	cp $(PROGRAM).o $(LIB_DIR)

clean:
	rm *.o	
	rm *.gch
