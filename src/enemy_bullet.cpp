#include "enemy_bullet.h"

EnemyBullet::EnemyBullet(const sf::FloatRect &enemy_rect, Actor *parent,
                         sf::Vector2i pos_in_table)
    : Bullet(enemy_rect, parent),
      pos_in_table_(pos_in_table) {}

EnemyBullet::~EnemyBullet() {}

sf::Vector2i EnemyBullet::pos_in_table() const {
  return pos_in_table_;
}
