#include "attack_actor.h"

#include "game_event_manager.h"

AttackActor::AttackActor() : can_fire_(true) {}

AttackActor::~AttackActor() {}

void AttackActor::Attack() {
  if (!CanFire())
    return;

  theEventManager.PushEvent(kAttack, this);
  can_fire_ = false;
}

void AttackActor::AllowFire() {
  can_fire_ = true;
}

bool AttackActor::CanFire() const {
  return can_fire_;
}
