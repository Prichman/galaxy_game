#include "actor.h"

#include <iostream>

Actor::Actor()
    : speed_(0, 0) {}

Actor::~Actor() {

}

void Actor::LoadSprite(const std::string &path) {
  if(!texture_.loadFromFile(path)) {
    std::cout << "Can't load texture from \"" << path << "\"" << std::endl;
    return;
  }
  sprite_.setTexture(texture_);
}

void Actor::GameUpdate() {
  sprite_.move(speed_.x, speed_.y);
}

void Actor::SetSpeed(float x, float y) {
  speed_.x = x;
  speed_.y = y;
}

sf::Vector2f Actor::GetSpeed() const {
  return speed_;
}

void Actor::SetPos(float x, float y) {
  sprite_.setPosition(x, y);
}

sf::Vector2f Actor::GetPos() const {
  return sprite_.getPosition();
}

sf::FloatRect Actor::bounding_rect() const {
  return sprite_.getGlobalBounds();
}



void Actor::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(sprite_, states);
}
