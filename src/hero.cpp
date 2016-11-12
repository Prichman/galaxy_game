#include "hero.h"

#include <iostream>
#include <string>

#include "storage.h"

static const int livesCount = 3;
static const int restoreTime = 500;
static const int dt = 16;

Hero::Hero()
    : lives_count_(livesCount),
      score_(0),
      restore_(false) {
   LoadSprite("resources/hero_1.png");
   LoadAdditionalSprite();

   SetPos((theStorage.screen_width() - bounding_rect().width) / 2,
          theStorage.screen_height() - theStorage.vmargin());
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
  
  sf::Vector2f pos = GetPos();


  bool can_update = true;
  if (speed_.x < 0 && pos.x <= 1)
    can_update = false;

  if (speed_.x > 0 &&
           pos.x >= theStorage.screen_width() - bounding_rect().width - 1)
    can_update = false;

  if (can_update)
    AttackActor::GameUpdate();

  SetSpeed(0, 0);
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
  if (lives_count_ > 0) {
    Revive();
  } else {
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
  restore_      = true;
}

void Hero::LoadAdditionalSprite() {
  std::string trans_texture_path = "resources/hero_1_trans.png";
  if (!trans_texture_.loadFromFile(trans_texture_path)) {
    std::cout << "Can't load texture from \"" << trans_texture_path << "\"";
    std::cout << std::endl;
    return;
  }
  trans_sprite_.setTexture(trans_texture_);
}
