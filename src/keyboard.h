#pragma once

#include <set>

#include <SFML/Window/Event.hpp>

#define theKeyboard Keyboard::GetInstance()

class Keyboard {
 public:
  static Keyboard &GetInstance();

  void HandleKeyPress(sf::Keyboard::Key key);
  void HandleKeyRelease(sf::Keyboard::Key key);

  bool IsPressed(sf::Keyboard::Key key);

 protected:
  Keyboard();

 private:
  static Keyboard *keyboard_;

  std::set<sf::Keyboard::Key> keys_pressed_;
};
