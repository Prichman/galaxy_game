#include "bullet.h"

#include "game_event.h"
#include "game_event_manager.h"
#include "storage.h"

Bullet::Bullet(const sf::IntRect &actor_rect, bool is_enemy)
    : hero_(!is_enemy) {
  LoadSprite("resources/bullet.png");

  float x, y;
  float vspeed = theStorage.bullet_vspeed();
  sf::IntRect own_rect = GetIntRect();
  x = actor_rect.left + (actor_rect.width - own_rect.width) / 2;
  if (is_enemy) {
    y = actor_rect.top - actor_rect.height - own_rect.height;
  } else {
    y = actor_rect.top + GetIntRect().height;
    vspeed = -vspeed;
  }
  SetPos(x, y);
  SetSpeed(0, vspeed);
}

void Bullet::GameUpdate() {
  Actor::GameUpdate();

  sf::Vector2f pos = GetPos();
  // Check if bullet is out of screen.
  if (pos.y > theStorage.screen_height() ||
      pos.y < 0) {
    GameEvent *event = new GameEvent(DEATH, static_cast<Actor *>(this));
    theEventManager.PushEvent(event);
  }
}

bool Bullet::hero() const {
  return hero_;
}
