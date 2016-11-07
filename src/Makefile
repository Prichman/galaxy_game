target = galaxy_game

objects = main.o

CXXFLAGS = -Wall -g

LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

all : $(target) reloc

$(target) : $(objects)
	$(CXX) $(LDFLAGS) $^ -o $(target)

main.o : main.cpp
	$(CXX) $(CXXFLAGS) -c $<

reloc : 
	mkdir -p ../build
	mv $(target) ../build

clean :
	-rm -rf ../build $(objects)

.PHONY : clean reloc
