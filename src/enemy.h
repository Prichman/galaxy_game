#pragma once

#include "attack_actor.h"

class Enemy : public AttackActor {
 friend class EnemiesLine;
  
 public:
  Enemy(int row, int pos_num);
};
