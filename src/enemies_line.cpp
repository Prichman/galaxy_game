#include "enemies_line.h"

#include "enemy.h"
#include "game_event.h"
#include "game_event_manager.h"

static const int verticalMoveTime = 20 * 60;
static const int rowCount     = 3;
static const int columnCount  = 10;

EnemiesLine::EnemiesLine()
    : vertical_ticker_(0),
      up(true) {
  // TODO: change after first run.
  float h_e = 20;
  float h_s = 5;
  
  bottom_ = 3 * (h_s + h_e);
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
      for (int j = 0; j < columnCount; ++j) {
        enemies_[i][j]->SetSpeed(0, vspeed);
      }
    }
  }

  for (int i = 0; i < rowCount; ++i) {
    for (int j = 0; j < columnCount; ++j) {
      enemies_[i][j]->GameUpdate();
    }
  }
}

float EnemiesLine::GetBottom() const {
  return bottom_;
}

bool EnemiesLine::KillEnemy(sf::IntRect bullet_rect) { 
  // TODO: move this to separate file.
  const float left_margin = 40;
  const float enemy_size  = 12;
  const float hspace      = 30;
  const float vspace      = 20;
  const float top_margin  = vspace;

  // TODO: test this (x,y).
  int y = (bullet_rect.top - top_margin) / (vspace + enemy_size);
  int x = (bullet_rect.left - left_margin) / (hspace + enemy_size);

  bool inter_left = enemies_[y][x]->GetIntRect().intersects(bullet_rect);
  if (inter_left) {
    GameEvent *event = new GameEvent(KILL, enemies_[y][x]);
    theEventManager.PushEvent(event);
    return true;
  }

  // Out of range check.
  if (x < columnCount - 1) {
    bool inter_right = enemies_[y][x + 1]->GetIntRect().intersects(bullet_rect);
    if (inter_right) {
      GameEvent *event = new GameEvent(KILL, enemies_[y][x + 1]);
      theEventManager.PushEvent(event); 
      return true;
    }
  }

  return false;
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
