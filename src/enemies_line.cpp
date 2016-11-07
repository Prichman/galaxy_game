#include "enemies_line.h"

#include "enemy.h"

static const int verticalMoveTime = 20 * 60;

EnemiesLine::EnemiesLine():
vertical_ticker_(0),
up(true) {
  // TODO: change after first run.
  float h_e = 20;
  float h_s = 5;
  
  bottom_ = 3 * (h_s + h_e);
}

EnemiesLine::~EnemiesLine() {
  for (int i = 0, n = enemies_.size(); i < n; ++i) {
    for (int j = 0, m = enemies_[i].size(); j < m; ++j) {
      delete enemies_[i][j];
      enemies_[i][j] = nullptr;
    }
  }
  enemies_.clear();
}

float EnemiesLine::GetBottom() const {
  return bottom_;
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
    for (int i = 0, n = enemies_.size(); i < n; ++i) {
      for (int j = 0, m = enemies_[i].size(); j < m; ++j) {
        enemies_[i][j]->SetSpeed(0, vspeed);
      }
    }
  }

  for (int i = 0, n = enemies_.size(); i < n; ++i) {
    for (int j = 0, m = enemies_[i].size(); j < m; ++j) {
      enemies_[i][j]->GameUpdate();
    }
  }
}


void
EnemiesLine::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  for (int i = 0, n = enemies_.size(); i < n; ++i) {
    for (int j = 0, m = enemies_[i].size(); j < m; ++j) {
      enemies_[i][j]->draw(target, states);
    }
  }
}


void EnemiesLine::CreateEnemies() {
  int n = 3,
      m = 10;
  
  enemies_.resize(n, std::vector<Enemy *>(m, nullptr));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      enemies_[i][j] = new Enemy(i, j);
    }
  }
}
