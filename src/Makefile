target = galaxy_game

objects = 								\
		actor.o 							\
		attack_actor.o 				\
		bullet.o 							\
		enemies_line.o 				\
		enemy.o 							\
		enemy_bullet.o 				\
		game_event.o 					\
		game_event_manager.o 	\
		game_manager.o 				\
		hero.o keyboard.o 		\
		storage.o

objects_all = $(objects) main.o

CXXFLAGS = -Wall -g

LDFLAGS = -lsfml-graphics \
				 	-lsfml-window \
					-lsfml-system

all : $(target) reloc

$(target) : $(objects_all)
	$(CXX) $(LDFLAGS) $^ -o $(target)

attack_actor.o :game_event_manager.h

bullet.o :  game_event_manager.h  \
					  storage.h

enemies_line.o :  enemy.h 							\
									game_event_manager.h 	\
									storage.h

game_event.o : actor.h

enemy.o : storage.h

game_manager.o : 				\
	actor.h 							\
	bullet.h 							\
	enemy.h 							\
	enemy_bullet.h 				\
	keyboard.h 						\
	game_event_manager.h 	\
	storage.h

hero.o : 	game_event_manager.h \
					storage.h			

main.o : game_manager.h

$(objects) : %.o : %.h
$(objects) main.o : %.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $<

reloc : 
	mkdir -p ../build
	mv $(target) ../build
	cp -r ../resources ../build

clean :
	-rm -rf ../build *.o

.PHONY : clean reloc
