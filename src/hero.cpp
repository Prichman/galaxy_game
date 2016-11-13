#include "hero.h"

#include <iostream>
#include <string>

#include "game_event_manager.h"
#include "storage.h"

static const int kLivesCount    = 1;
static const int kRestoreTime   = 3 * 60;
static const int kInvisibleTime = 1 * 60;
static const int dt             = 16;

Hero::Hero()
    : lives_count_(kLivesCount),
      score_(0),
      restore_(false),
      invisible_ticker_(-1) {
   LoadSprite("resources/hero_1.png");
   LoadAdditionalSprite();
   current_sprite_ = &sprite_;

   SetPos((theStorage.screen_width() - bounding_rect().width) / 2,
          theStorage.screen_height() - theStorage.hero_vmargin());
}

void Hero::GameUpdate() {
  if (restore_) {
    // Restore the hero.
    --restore_time_;
    if (restore_time_ <= 0) {
      restore_ = false;
      current_sprite_ = &sprite_;
    }
  }
  
  sf::Vector2f pos = GetPos();

  // Check out of screen moving.
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

sf::Int32 Hero::GetLivesCount() const {
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
  if (lives_count_ >= 0) {
    Revive();
  } else {
    theEventManager.PushEvent(kLoss, nullptr);
  }
}

bool Hero::resurrecting() const {
  return restore_;
}

void Hero::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(*current_sprite_, states);
}


void Hero::SubLife() {
  --lives_count_;
}

void Hero::Revive() {
  restore_time_     = kRestoreTime;
  restore_          = true;
  
  current_sprite_ = &trans_sprite_;
}

void Hero::LoadAdditionalSprite() {
  trans_texture_.create(sprite_.getGlobalBounds().width,
                        sprite_.getGlobalBounds().height);
  trans_sprite_.setTexture(trans_texture_);
  trans_sprite_.setColor(sf::Color(0, 0, 0, 255));  // Absoolutly transparent.
}
