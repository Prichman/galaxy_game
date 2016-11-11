#pragma once

#include "actor.h"

class Bullet : public Actor {
 public:
  Bullet(const sf::IntRect &actor_rect, bool is_enemy);

  void GameUpdate() override;

  bool hero() const;

 private:
  bool hero_;
};
