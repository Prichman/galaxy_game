#pragma once

class Actor;

enum GameEventType {
  kAttack,
  kKill,
  kDeath,
  kLoss,
  kWin
};

class GameEvent {
 public:
  GameEvent(GameEventType type, Actor *parent);

  GameEventType type() const;
  Actor *parent();

 private:
  GameEventType type_;
  Actor *parent_;
};
