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
  bool KillEnemy(sf::IntRect bullet_rect);

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 private:
  void CreateEnemies();
  
  std::vector<std::vector<Enemy *>> enemies_;

  float bottom_;
  unsigned int vertical_ticker_;
  bool up;

  int left_enemy_;
  int right_enemy_;
};
