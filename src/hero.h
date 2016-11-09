#pragma once

#include "attack_actor.h"

class Hero : public AttackActor {
 public:
  Hero();

  void GameUpdate() override;

  void AddLife();
  sf::Uint32 GetLivesCount() const;

  void AddScore(int score);
  sf::Uint32 GetScore() const;

  void Die();
  bool isResurrecting() const;

 private:
  void SubLife();
  void Revive();

  void LoadAdditionalSprite();

  sf::Uint32 lives_count_;
  sf::Uint32 score_;

  bool restore_;
  int restore_time_;

  sf::Sprite  trans_sprite_;
  sf::Texture trans_texture_;
};
