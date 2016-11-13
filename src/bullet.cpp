#include "bullet.h"

#include "game_event_manager.h"
#include "storage.h"

  Bullet::Bullet(const sf::FloatRect &actor_rect, Actor *parent)
    : parent_(parent) {
  LoadSprite("resources/bullet.png");

  float x, y;
  float vspeed = theStorage.bullet_vspeed();
  sf::FloatRect own_rect = bounding_rect();
  x = actor_rect.left + (actor_rect.width - own_rect.width) / 2;
  // Check if parent is Enemy. Enemies are placed in 
  // first height half of screen.
  if ((actor_rect.top - actor_rect.height) <
      (theStorage.screen_height() / 2)) {
    // Enemy.
    y = actor_rect.top + actor_rect.height + own_rect.height;
  } else {
    // Hero.
    y = actor_rect.top - bounding_rect().height;
    vspeed = -vspeed;
  }
  SetPos(x, y);
  SetSpeed(0, vspeed);
}

Bullet::~Bullet() {}

void Bullet::GameUpdate() {
  Actor::GameUpdate();

  sf::Vector2f pos = GetPos();
  // Check if bullet is out of screen.
  if (pos.y > theStorage.screen_height() ||
      pos.y < 0) {
    theEventManager.PushEvent(kDeath, this);
  }
}

Actor *Bullet::parent() {
  return parent_;
}
