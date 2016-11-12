#pragma once

#include "actor.h"

class Bullet : public Actor {
 public:
  Bullet(const sf::FloatRect &actor_rect, Actor *parent);

  void GameUpdate() override;

  Actor *parent();

 private:
  Actor *parent_;
};
