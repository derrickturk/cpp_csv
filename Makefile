CXX=g++
CXXFLAGS=-std=c++20 $(if $(DEBUG),-g,-O2) -Wall -Wextra -Werror -pedantic -static

HEADERS=csv.h named.h parse.h
SOURCES=csv.cpp main.cpp

csv.exe: $(HEADERS) $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)
