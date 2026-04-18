CXX = g++
CXXFLAGS = -g -Wall -std=c++11
DEPS = src/tokenizer.h src/fsm.h src/tcp.h
OBJ = src/main.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

all: fsm

format:
	./format.sh

fsm: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f $(OBJ) fsm
