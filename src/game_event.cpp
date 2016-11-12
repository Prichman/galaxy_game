#include "game_event.h"

#include "actor.h"

GameEvent::GameEvent(GameEventType type, Actor *parent)
  : type_(type),
    parent_(parent) {}


GameEventType GameEvent::type() const {
  return type_;
}

Actor *GameEvent::parent() {
  return parent_;
}
