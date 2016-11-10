#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <string>

class Actor : public sf::Drawable {
 public:
  Actor();
  virtual ~Actor();

  virtual void GameUpdate();
  
  void LoadSprite(const std::string & path);

  void SetPos(float x, float y);
  sf::Vector2f GetPos() const;

  void SetSpeed(float x, float y);
  sf::Vector2f GetSpeed() const;
  sf::IntRect GetIntRect() const;

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  sf::Vector2f speed_;

  sf::Sprite  sprite_;
  sf::Texture texture_;
};
