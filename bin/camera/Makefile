PROGRAM      = camera
TEST_PROGRAM = test_$(PROGRAM)

CC = g++

ROOT_DIR    = /home/pi/workspace/robot
BIN_DIR     = $(ROOT_DIR)/bin
INCLUDE_DIR = $(ROOT_DIR)/include
LIB_DIR     = $(ROOT_DIR)/lib
USR_LIB     = /usr/local/lib

CPPFLAGS    = -I. -I$(INCLUDE_DIR) -I/usr/local/include/opencv -I/usr/local/include/raspicam -I/usr/local/include
LDFLAGS     = -L$(LIB_DIR) -L/usr/lib -L$(USR_LIB) -L/opt/vc/lib -lpthread -lmmal -lmmal_core -lmmal_util

OBJS = $(PROGRAM).o 

TEST_OBJS = $(OBJS) $(TEST_PROGRAM).o 

DEPS = $(PROGRAM).hpp

$(TEST_PROGRAM) : $(TEST_OBJS) $(DEPS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o $(TEST_PROGRAM) $(TEST_PROGRAM).o $(PROGRAM).o $(USR_LIB)/*opencv* $(USR_LIB)/libraspicam_still_robot_cv.a 

$(PROGRAM).o: $(DEPS)
	$(CC) $(CPPFLAGS) -c $(PROGRAM).cpp 

$(TEST_PROGRAM).o: $(DEPS) $(OBJS)
	$(CC) $(CPPFLAGS) -c $(TEST_PROGRAM).cpp

install: 
	cp *.hpp $(INCLUDE_DIR)
	cp $(PROGRAM).o $(LIB_DIR)

clean:
	rm *.o	
	rm *.gch
