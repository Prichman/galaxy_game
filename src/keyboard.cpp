#include "keyboard.h"

#include <SFML/Window/Keyboard.hpp>

Keyboard *Keyboard::_keyboard = nullptr;

Keyboard::Keyboard() : current_action_(ACT_NO) {}

Keyboard &Keyboard::GetInstance() {
  if (_keyboard == nullptr)
    _keyboard = new Keyboard;

  return *_keyboard;
}


void Keyboard::HandleKeyPress() {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
    current_action_ = ACT_MOVE_LEFT;
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
    current_action_ = ACT_MOVE_RIGHT;
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
    current_action_ = ACT_ATTACK;
  } else {
    current_action_ = ACT_NO;
  }
}

void Keyboard::HandleKeyRelease() {
  current_action_ = ACT_NO;
}

bool Keyboard::ShouldAttack() const {
  return current_action_ == ACT_ATTACK;
}

bool Keyboard::ShouldMoveLeft()  const {
  return current_action_ == ACT_MOVE_LEFT;
}

bool Keyboard::ShouldMoveRight() const {
  return current_action_ == ACT_MOVE_RIGHT;
}
