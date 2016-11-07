#pragma once

#include <SFML/Window/Event.hpp>

#define theKeyboard Keyboard::GetInstance()

class Keyboard {
 public:
  static Keyboard &GetInstance();

  void HandleKeyPress();
  void HandleKeyRelease();

  bool ShouldAttack()    const;
  bool ShouldMoveLeft()  const;
  bool ShouldMoveRight() const;

 protected:
  Keyboard();

 private:
  static Keyboard *_keyboard;

  bool should_left_;
  bool should_right_;
  bool should_attack_;
};
