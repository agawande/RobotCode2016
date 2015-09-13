CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

% : %.cpp
	g++ $(CFLAGS) $< $(LIBS) -o $@

#g++ example.cpp -lopencv_core -lopencv_videoio -lopencv_highgui
