CXX= g++ -g -std=c++20

FILES= main.o bmp.o filter.o
EXE = ImageEditor

all: $(EXE)

$(EXE): $(FILES)
	$(CXX)	$^ -o $@

main.o: main.cpp bmp.hpp
	$(CXX) -c $< -o $@

bmp.o: bmp.cpp bmp.hpp
	$(CXX) -c $< -o $@

filter.o: filter.cpp filter.hpp
	$(CXX) -c $< -o $@

.PHONY: all clean

clean:
	rm -f $(FILES) $(EXE)