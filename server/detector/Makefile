CC = @g++
CFLAGS = -std=c++11 -DDEBUG

OPENCV_LIBS = $(shell for lib in $(OPENCV_PATH)/lib/*.so; do echo $$lib; done;)

DETECTOR_SRCS = detector.cpp features.cpp edges.cpp geometry.cpp util.cpp
HASHER_SRCS = hasher.cpp util.cpp

# TODO(sghiaus): Split the makefile into objects for incremental building.

all: detector hasher

detector: $(DETECTOR_SRCS)
	$(CC) $(CFLAGS) -I$(OPENCV_PATH)/include -I../.. -o detector $(DETECTOR_SRCS) $(OPENCV_LIBS) -L$(OPENCV_PATH)/lib

hasher: $(HASHER_SRCS)
	$(CC) $(CFLAGS) -I$(OPENCV_PATH)/include -I../.. -o hasher $(HASHER_SRCS) $(OPENCV_LIBS) -L$(OPENCV_PATH)/lib
