ifeq ($(CONFIG),debug)
	OPT = - Wall -O2 -g
else
	OPT =
endif

all: heuristic

heuristic: heuristic.cpp Makefile
	g++ heuristic.cpp -framework OpenCL -oHEURISTIC $(OPT)

clean:
	rm -f heuristic

# apple  -framework OpenCL
#linux   -lOpenCL

#-Ofast
#-O2
#-O3