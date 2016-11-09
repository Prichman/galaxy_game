#pragma once

#include <SFML/Graphics.hpp>

#include <set>

#include "actor.h"
#include "enemies_line.h"
#include "hero.h"

class Actor;
class Bullet;

class GameManager {
 public:
  GameManager();

  void StartGame();
  void Pause();
  void Resume();

  void HandleEvents();

  void GameUpdate();
  void Render();

 private:
  void FindCollisions();
  
  void LoadBackground();
  void CreatePlayer();
  void CreateEnemies();

  void DrawBackground();
  void DrawActors();
  void DrawGui();

  sf::RenderWindow main_window_;

  bool running_;

  sf::Sprite  background_;
  sf::Texture background_texture_;

  sf::Clock clock_;
  sf::Time  elapsed_;

  std::set<Bullet *>  enemies_bullets_;
  std::set<Bullet *>  hero_bullets_;
  std::set<Actor *>   actors_;
  EnemiesLine         enemies_line_;
  Hero                hero_;
};
