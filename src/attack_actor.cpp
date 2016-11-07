#include "attack_actor.h"

#include "game_event.h"
#include "game_event_manager.h"

AttackActor::AttackActor() {}

void AttackActor::Attack() {
  GameEvent *event = new GameEvent(ATTACK, static_cast<Actor *>(this));
  theEventManager.PushEvent(event);
  can_fire_ = false;
}

void AttackActor::AllowFire() {
  can_fire_ = true;
}

bool AttackActor::CanFire() const {
  return can_fire_;
}
