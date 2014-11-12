CC = @g++
CFLAGS = -std=c++11 -DDEBUG

OPENCV_LIBS = $(shell for lib in $(OPENCV_PATH)/lib/*.so; do echo $$lib; done;)

SRCS = detector.cpp feature_matcher.cpp edges.cpp geometry.cpp

all: detector

detector: $(SRCS)
	$(CC) $(CFLAGS) -I$(OPENCV_PATH)/include -I../.. -o detector $(SRCS) $(OPENCV_LIBS) -L$(OPENCV_PATH)/lib
