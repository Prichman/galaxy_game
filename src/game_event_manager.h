#pragma once

#include <stack>

class GameEvent;

#define theEventManager GameEventManager::GetInstance()

class GameEventManager {
 public:
  static GameEventManager &GetInstance();
  void PushEvent(GameEvent *event);
  GameEvent *GetEvent();

 protected:
  GameEventManager();

 private:
  static GameEventManager *manager_;
  std::stack<GameEvent *> events_;
};
