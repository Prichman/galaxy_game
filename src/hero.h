#pragma once

#include "attack_actor.h"

class Hero : public AttackActor {
 public:
  Hero();

  void GameUpdate() override;

  void AddLife();
  sf::Int32 GetLivesCount() const;

  void AddScore(int score);
  sf::Uint32 GetScore() const;

  void Die();
  bool resurrecting() const;
  bool invisible() const;

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 private:
  void SubLife();
  void Revive();

  void LoadAdditionalSprite();

  sf::Int32 lives_count_;
  sf::Uint32 score_;
  sf::Vector2f starting_pos_;

  bool restore_;
  int restore_time_;
  int invisible_ticker_;

  sf::Sprite *current_sprite_;
  sf::Sprite  trans_sprite_;
  sf::Texture trans_texture_;
};
