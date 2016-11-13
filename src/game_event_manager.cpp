#include "game_event_manager.h"

GameEventManager *GameEventManager::manager_ = nullptr;

GameEventManager::GameEventManager() {}

GameEventManager &GameEventManager::GetInstance() {
  if (manager_ == nullptr)
    manager_ = new GameEventManager;

  return *manager_;
}

void GameEventManager::PushEvent(GameEventType type, Actor *event_parent) {
  GameEvent *event = new GameEvent(type, event_parent);
  events_.push(event);
}

GameEvent *GameEventManager::GetEvent() {
  GameEvent *event;
  if (events_.size() > 0) {
    event = events_.top();
    events_.pop();
  } else {
    event = nullptr;
  }
  return event;
}
