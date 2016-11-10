#include "enemies_line.h"

#include "enemy.h"
#include "game_event.h"
#include "game_event_manager.h"
#include "storage.h"

static const int verticalMoveTime = 20 * 60;
static const int rowCount     = 3;
static const int columnCount  = 10;

EnemiesLine::EnemiesLine()
    : vertical_ticker_(0),
      up(true) {  
  bottom_ = theStorage.vmargin() - theStorage.hspace() +
      (theStorage.enemy_size() + theStorage.hspace()) * 3;
}

EnemiesLine::~EnemiesLine() {
  for (int i = 0; i < rowCount; ++i) {
    for (int j = 0; j < columnCount; ++j) {
      delete enemies_[i][j];
      enemies_[i][j] = nullptr;
    }
  }
  enemies_.clear();
}

void EnemiesLine::GameUpdate() {
  ++vertical_ticker_;

  // Check if need vertical move.
  if (vertical_ticker_ == verticalMoveTime) {
    vertical_ticker_ = 0;
    
    float vspeed = 2.0;
    if (up)
      vspeed = -vspeed;
    up = !up;
    bottom_ += vspeed;
   
    for (int i = 0; i < rowCount; ++i) {
      for (int j = left_enemy_; j <= right_enemy_; ++j) {
        if (enemies_[i][j] != nullptr)
          enemies_[i][j]->SetSpeed(theStorage.enemy_hspeed(), vspeed);
      }
    }
  }

  // Usual update.
  // Common behavior.
  
  
  for (int i = 0; i < rowCount; ++i) {
    for (int j = left_enemy_; j < right_enemy_; ++j) {
      if (enemies_[i][j] != nullptr)
        enemies_[i][j]->GameUpdate();
    }
  }
}

float EnemiesLine::GetBottom() const {
  return bottom_;
}

bool EnemiesLine::KillEnemy(sf::IntRect bullet_rect) { 
  bool result = false;

  // TODO: test this (x,y).
  int y = (bullet_rect.top - theStorage.vmargin()) /
      (theStorage.vspace() + theStorage.enemy_size());
  int x = (bullet_rect.left - theStorage.hmargin()) /
      (theStorage.hspace() + theStorage.enemy_size());

  bool inter_left = enemies_[y][x]->GetIntRect().intersects(bullet_rect);
  if (inter_left) {
    GameEvent *event = new GameEvent(KILL, nullptr);
    theEventManager.PushEvent(event);
    result = true;
    delete enemies_[y][x];
    enemies_[y][x] = nullptr;
  }

  // Out of range check.
  if (x < columnCount - 1 && !result) {
    bool inter_right = enemies_[y][x + 1]->GetIntRect().intersects(bullet_rect);
    if (inter_right) {
      GameEvent *event = new GameEvent(KILL, nullptr);
      theEventManager.PushEvent(event); 
      result = true;
      delete enemies_[y][x + 1];
      enemies_[y][x + 1] = nullptr;
    }
  }

  if (result)
    CheckBorders();
  return result;
}


void
EnemiesLine::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  for (int i = 0; i < rowCount; ++i) {
    for (int j = 0; j < columnCount; ++j) {
      enemies_[i][j]->draw(target, states);
    }
  }
}


void EnemiesLine::CreateEnemies() {
  enemies_.resize(rowCount, std::vector<Enemy *>(columnCount, nullptr));
  for (int i = 0; i < rowCount; ++i) {
    for (int j = 0; j < columnCount; ++j) {
      enemies_[i][j] = new Enemy(i, j);
    }
  }
  left_enemy_ = 0;
  right_enemy_ = 9;
}

void EnemiesLine::CheckBorders() {
  bool left   = true,
       right  = true;
  for (int i = 0; i < rowCount; ++i) {
    if (enemies_[left_enemy_][i] != nullptr)
      left = false;

    if (enemies_[right_enemy_][i] != nullptr)
      right = false;
  }
  if (!left)
    ++left_enemy_;
  if (!right)
    ++right_enemy_;

  if (left > right) {
    // TODO: end of game.
  }
}
