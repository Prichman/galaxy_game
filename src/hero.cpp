#include "hero.h"

#include <string>

static const int livesCount = 3;
static const int restoreTime = 500;
static const int dt = 16;

Hero::Hero():
 lives_count_(livesCount),
 score_(0),
 restore_(false) {
   LoadSprite("resources/hero.png");
}

void Hero::GameUpdate() {
  if (restore_) {
    // Restore the hero.
    restore_time_ -= dt;

    // TODO: change animation.

    if (restore_time_ <= 0) {
      restore_ = false;
    }
  }
  
  AttackActor::GameUpdate();
}

void Hero::AddLife() {
  ++lives_count_;
}

sf::Uint32 Hero::GetLivesCount() const {
  return lives_count_;
}

void Hero::AddScore(int score) {
  score_ += score;
}

sf::Uint32 Hero::GetScore() const {
  return score_;
}

void Hero::Die() {
  SubLife();
  if (lives_count_ > 0)
    Revive();
  else {
    // TODO: add ending.
  }
}

bool Hero::isResurrecting() const {
  return restore_;
}



void Hero::SubLife() {
  --lives_count_;
}

void Hero::Revive() {
  restore_time_ = restoreTime;
  restore_ = true;
}
