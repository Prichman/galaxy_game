#pragma once

#include <SFML/Graphics.hpp>

#include "actor.h"
#include "enemies_line.h"
#include "hero.h"

enum GameMode {
  kStart,
  kGame,
  kEnd
};

class Actor;
class Bullet;
class EnemyBullet;

class GameManager {
 public:
  GameManager();

  void StartGame();
  void Pause();
  void Resume();

  void HandleEvents();

  void GameUpdate();
  void TickAndRender();
  void Render();

 private:
  void FindCollisions();
  void HandleGameEvents();
  
  void LoadBackground();
  void LoadFont();
  void CreateGui();

  void DrawBackground();
  void DrawActors();
  void DrawGui();
  void DrawFirstLineGui();
  void DrawSecondLineGui();

  void ShowStartScreen();
  void ShowGoodEnding();
  void ShowBadEnding();

  void CloseMainWindow();

  GameMode game_mode_;

  sf::RenderWindow main_window_;
  sf::Clock clock_;

  bool running_;
  bool win_;

  sf::Sprite  background_;
  sf::Texture background_texture_;

  std::vector<EnemyBullet *>  enemies_bullets_;
  std::vector<Bullet *>       hero_bullets_;
  EnemiesLine                 enemies_line_;
  Hero                        hero_;

  sf::Font font_;
  sf::Text text_;

  sf::Sprite  heart_sprite_;
  sf::Texture heart_texture_;

  float lives_first_x_;
  float lives_second_x_;
  float score_first_x_;
  float score_second_x_;
};
