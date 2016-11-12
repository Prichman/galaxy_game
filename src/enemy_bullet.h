#pragma once

#include "bullet.h"

class EnemyBullet : public Bullet {
 public:
  EnemyBullet(const sf::FloatRect &enemy_rect, Actor *parent, sf::Vector2i pos_in_table);

  sf::Vector2i pos_in_table() const;

 private:
  sf::Vector2i pos_in_table_;
};
