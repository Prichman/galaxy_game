#pragma once

#include "actor.h"

#include <vector>

class Enemy;

class EnemiesLine : public Actor {
 public:
  EnemiesLine();
  virtual ~EnemiesLine();

  virtual void GameUpdate() override;

  float GetBottom() const;
  sf::Vector2i GetEnemyPos(Enemy *enemy);

  bool KillEnemy(sf::FloatRect bullet_rect);

  void AllowFire(sf::Vector2i pos_in_table);

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 private:
  void CreateEnemies();
  
  void CheckBorders();
  bool CanMove();
  Enemy *FindLeft();
  Enemy *FindRight();
  
  std::vector<std::vector<Enemy *>> enemies_;

  float bottom_;
  unsigned int vertical_ticker_;
  unsigned int attack_ticker_;
  bool up;
  bool moving_left_;

  int left_enemy_;
  int right_enemy_;
};
