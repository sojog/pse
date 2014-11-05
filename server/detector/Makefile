CC = g++
CFLAGS = -std=c++11

OPENCV_LIBS = $(shell for lib in $(OPENCV_PATH)/lib/*.so; do echo $$lib; done;)

all: detector

detector: detector.cpp
	$(CC) $(CFLAGS) -I$(OPENCV_PATH)/include -I../.. -o detector detector.cpp $(OPENCV_LIBS) -L$(OPENCV_PATH)/lib
