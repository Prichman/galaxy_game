#pragma once

class Actor;

enum GameEventType {
  ATTACK,
  KILL,
  DEATH
};

class GameEvent {
 public:
  GameEvent(GameEventType type, Actor * parent);

  GameEventType GetType() const;
  Actor *GetParent();

 private:
  GameEventType type_;
  Actor *parent_;
};
