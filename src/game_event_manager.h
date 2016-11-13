#pragma once

#include <stack>

#include "game_event.h"

#define theEventManager GameEventManager::GetInstance()

class GameEventManager {
 public:
  static GameEventManager &GetInstance();
  void PushEvent(GameEventType type, Actor *event_parent);
  GameEvent *GetEvent();

 protected:
  GameEventManager();

 private:
  static GameEventManager *manager_;
  std::stack<GameEvent *> events_;
};
