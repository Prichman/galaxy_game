target = galaxy_game

objects = actor.o attack_actor.o bullet.o enemies_line.o enemy.o game_event.o \
					game_event_manager.o game_manager.o hero.o keyboard.o storage.o

CXXFLAGS = -Wall -g

LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

all : $(target) reloc

$(target) : $(objects) main.o
	$(CXX) $(LDFLAGS) $^ -o $(target)

attack_actor.o : game_event.h \
								 game_event_manager.h

enemies_line.o : enemy.h game_event.h game_event_manager.h storage.h

game_event.o : actor.h

enemy.o : storage.h

game_manager.o : actor.h bullet.h enemy.h keyboard.h game_event_manager.h \
								 game_event.h storage.h

main.o : game_manager.h

$(objects) : %.o : %.h
$(objects) main.o : %.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $<

reloc : 
	mkdir -p ../build
	mv $(target) ../build

clean :
	-rm -rf ../build $(objects) main.o

.PHONY : clean reloc
