#include "game_event.h"

#include "actor.h"

GameEvent::GameEvent(GameEventType type, Actor * parent):
  type_(type),
  parent_(parent) {}



GameEventType GameEvent::GetType() const {
  return type_;
}

Actor *GameEvent::GetParent() {
  return parent_;
}
