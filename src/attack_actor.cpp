#include "attack_actor.h"

#include "game_event.h"
#include "game_event_manager.h"

AttackActor::AttackActor() : can_fire_(true) {}

void AttackActor::Attack() {
  if (!CanFire())
    return;

  GameEvent *event = new GameEvent(kAttack, this);
  theEventManager.PushEvent(event);
  can_fire_ = false;
}

void AttackActor::AllowFire() {
  can_fire_ = true;
}

bool AttackActor::CanFire() const {
  return can_fire_;
}
