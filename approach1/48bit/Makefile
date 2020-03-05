ifeq ($(CONFIG),debug)
	OPT = - Wall -O0 -g
else
	OPT =
endif

all: heuristic

heuristic: heuristic.cpp Makefile
	g++ heuristic.cpp  -framework OpenCL -oHEURISTIC $(OPT)

clean:
	rm -f heuristic

# OSX -framework OpenCL
# LINUX -lOpenCL