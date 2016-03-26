PROGRAM      = robot

CC = g++

ROOT_DIR    = /home/pi/workspace/robot

BIN_DIR     = $(ROOT_DIR)/bin
INCLUDE_DIR = $(ROOT_DIR)/include
LIB_DIR     = $(ROOT_DIR)/lib

VPATH       = $(BIN_DIR)/i2cdispatcher:$(BIN_DIR)/messageformatter:$(LIB_DIR):$(BIN_DIR)/clamper\
              $(BIN_DIR)/conveyor $(BIN_DIR)/sorter

CPPFLAGS    = -I. -I$(INCLUDE_DIR)
LDFLAGS     = -L$(LIB_DIR)

OBJS = $(PROGRAM).o i2cdispatcher.o messageformatter.o conveyor.o clamper.o sorter.o

DEPS = $(INCLUDE_DIR)/i2cdispatcher.hpp $(INCLUDE_DIR)/messageformatter.hpp\
       $(INCLUDE_DIR)/clamper.hpp $(INCLUDE_DIR)/conveyor.hpp $(INCLUDE_DIR)/cage.hpp\
       $(INCLUDE_DIR)/sorter.hpp

$(PROGRAM) : $(OBJS) $(DEPS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) $^ -o $(PROGRAM)

install: 
	sudo cp $(PROGRAM) /usr/local/bin

clean:
	rm *.o
