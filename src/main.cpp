#include <iostream>

using namespace std;

#include <SFML/Graphics.hpp>

#include "game_manager.h"

int main() {
  GameManager *manager = new GameManager;
  manager->StartGame();

  return 0;
}
