#include "actor.h"

Actor::Actor():
  speed_(0, 0)
{}

Actor::~Actor() {

}

void Actor::LoadSprite(const std::string & path) {
  texture_.loadFromFile(path);
  sprite_.setTexture(texture_);
}

void Actor::GameUpdate() {
  sprite_.move(speed_.x, speed_.y);
}

void Actor::SetSpeed(float x, float y) {
  speed_.x = x;
  speed_.y = y;
}

void Actor::SetPos(float x, float y) {
  sprite_.setPosition(x, y);
}

sf::Vector2f Actor::GetPos() const {
  return sprite_.getPosition();
}

sf::IntRect Actor::GetIntRect() const {
  return sprite_.getTextureRect();
}



void Actor::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(sprite_, states);
}
