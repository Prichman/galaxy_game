#pragma once

#include "actor.h"

class Bullet : public Actor {
 public:
  Bullet(const sf::FloatRect &actor_rect, Actor *parent);
  virtual ~Bullet();

  void GameUpdate() override;

  virtual Actor *parent();

 private:
  Actor *parent_;
};
