#pragma once

#include "actor.h"

class AttackActor : public Actor {
 public:
  AttackActor();

  void Attack();

  void AllowFire();
  bool CanFire() const;

 protected:
  bool can_fire_;
};
