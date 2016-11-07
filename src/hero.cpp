#include "hero.h"

Hero::Hero() {

}

void Hero::AddLife() {
  ++lives_count_;
}

void Hero::SubLife() {
  --lives_count_;
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
