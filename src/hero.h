#pragma once

#include "attack_actor.h"

class Hero : public AttackActor {
 public:
  Hero();

  void AddLife();
  void SubLife();
  sf::Uint32 GetLivesCount() const;

  void AddScore(int score);
  sf::Uint32 GetScore() const;

 private:
  sf::Uint32 lives_count_;
  sf::Uint32 score_;
};
