#include "keyboard.h"

#include <SFML/Window/Keyboard.hpp>

Keyboard *Keyboard::_keyboard = nullptr;

Keyboard::Keyboard():
  should_left_  (false),
  should_right_ (false),
  should_attack_(false)
{}

Keyboard &Keyboard::GetInstance() {
  if (_keyboard == nullptr)
    _keyboard = new Keyboard;

  return *_keyboard;
}


void Keyboard::HandleKeyPress() {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
      should_left_ = true;
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
      should_right_ = true;
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
      should_attack_ = true;
  } 
}

void Keyboard::HandleKeyRelease() {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
      should_left_ = false;
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
      should_right_ = false;
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
      should_attack_ = false;
  }
}

bool Keyboard::ShouldAttack() const {
  return should_attack_;
}

bool Keyboard::ShouldMoveLeft()  const {
  return should_left_;
}

bool Keyboard::ShouldMoveRight() const {
  return should_right_;
}
