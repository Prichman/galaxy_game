#pragma once

#include <SFML/Window/Event.hpp>

#define theKeyboard Keyboard::GetInstance()

enum Action {
  ACT_NO,
  ACT_MOVE_LEFT,
  ACT_MOVE_RIGHT,
  ACT_ATTACK
};

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

  Action current_action_;
};
