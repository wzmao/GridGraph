
ROOT_DIR= $(shell pwd)
TARGETS= bin/preprocess bin/bfs bin/wcc bin/pagerank bin/spmv bin/mis bin/radii bin/sssp bin/gettotalv bin/txt2map bin/add_weights

CXX?= g++
CXXFLAGS?= -O3 -Wall -std=c++11 -g -fopenmp -I$(ROOT_DIR)
HEADERS= $(shell find . -name '*.hpp')

all: $(TARGETS)

bin/preprocess: tools/preprocess.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SYSLIBS)

bin/bfs: examples/bfs.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SYSLIBS)

bin/wcc: examples/wcc.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SYSLIBS)

bin/pagerank: examples/pagerank.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SYSLIBS)

bin/spmv: examples/spmv.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SYSLIBS)

bin/mis: examples/mis.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SYSLIBS)

bin/radii: examples/radii.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SYSLIBS)

bin/sssp: examples/sssp.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SYSLIBS)

bin/gettotalv: tools/gettotalv.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SYSLIBS)

bin/txt2map: tools/txt2map.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SYSLIBS)

bin/add_weights: tools/add_weights.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SYSLIBS)

clean:
	rm -rf $(TARGETS)

