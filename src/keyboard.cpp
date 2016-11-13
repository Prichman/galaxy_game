#include "keyboard.h"

#include <SFML/Window/Keyboard.hpp>

Keyboard *Keyboard::keyboard_ = nullptr;

Keyboard::Keyboard() {}

Keyboard &Keyboard::GetInstance() {
  if (keyboard_ == nullptr)
    keyboard_ = new Keyboard;

  return *keyboard_;
}


void Keyboard::HandleKeyPress(sf::Keyboard::Key key) {
  keys_pressed_.insert(key);
}

void Keyboard::HandleKeyRelease(sf::Keyboard::Key key) {
  auto it = keys_pressed_.find(key);
  if (it != keys_pressed_.end())
    keys_pressed_.erase(it);
}

bool Keyboard::IsPressed(sf::Keyboard::Key key) {
  return keys_pressed_.find(key) != keys_pressed_.end();
}
