#include <SFML/Graphics.hpp>

#include "game_manager.h"

int main() {
  GameManager *manager = new GameManager;
  manager->StartGame();

  delete manager;
  manager = nullptr;

  return 0;
}
