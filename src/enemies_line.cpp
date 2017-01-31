#include "enemies_line.h"

#include "enemy.h"
#include "game_event_manager.h"
#include "storage.h"

static const int kVerticalMoveTime  = 5 * 60;
static const int kAttackTime        = 0.5 * 60; // Allow change this
                                              // to test your skill.
static const int kRowCount          = 3;
static const int kColumnCount       = 10;

EnemiesLine::EnemiesLine()
    : vertical_ticker_(0),
      attack_ticker_(0),
      up(false),
      moving_left_(true) {  
  CreateEnemies();
  bottom_ = theStorage.vmargin() - theStorage.hspace() +
      (FindLeft()->bounding_rect().height + theStorage.hspace()) * 3;
}

EnemiesLine::~EnemiesLine() {
  for (int i = 0; i < kRowCount; ++i) {
    for (int j = 0; j < kColumnCount; ++j) {
      delete enemies_[i][j];
      enemies_[i][j] = nullptr;
    }
  }
  enemies_.clear();
}

void EnemiesLine::GameUpdate() {

  // Vertical move
  ++vertical_ticker_;
  // Check if need vertical move.
  if (vertical_ticker_ == kVerticalMoveTime) {
    vertical_ticker_ = 0;
    
    float vspeed = 5.0;
    if (up)
      vspeed = -vspeed;
    up = !up;
    bottom_ += vspeed;
   
    for (int i = 0; i < kRowCount; ++i) {
      for (int j = left_enemy_; j <= right_enemy_; ++j) {
        if (enemies_[i][j] != nullptr)
          enemies_[i][j]->SetSpeed(enemies_[i][j]->GetSpeed().x, vspeed);
      }
    }
  }

  // Enemies attack
  ++attack_ticker_;

  if (attack_ticker_ == kAttackTime) {
    attack_ticker_ = 0;
    int attacker_x = random() % kColumnCount;
    int attacker_y = random() % kRowCount;
    if (enemies_[attacker_y][attacker_x] != nullptr) 
      enemies_[attacker_y][attacker_x]->Attack();
  }

  // Usual update.
  // Common behavior. 
  // Check out-of-screen moving.
  if (!CanMove()) {
    float enemy_speed = theStorage.enemy_hspeed();
    // Check prev direction of move.
    if (!moving_left_)
      enemy_speed = -enemy_speed;
    moving_left_ = !moving_left_;
    for (int i = 0; i < kRowCount; ++i) {
      for (int j = left_enemy_; j <= right_enemy_; ++j)
        if (enemies_[i][j] != nullptr) {
          enemies_[i][j]->SetSpeed(enemy_speed, 0);
        }
    }
  }
  
  for (int i = 0; i < kRowCount; ++i) {
    for (int j = left_enemy_; j <= right_enemy_; ++j) {
      if (enemies_[i][j] != nullptr)
        enemies_[i][j]->GameUpdate();
    }
  }
}

float EnemiesLine::GetBottom() const {
  return bottom_;
}

sf::Vector2i EnemiesLine::GetEnemyPos(Enemy *enemy) {
  for (int i = 0; i < kRowCount; ++i) {
    for (int j = left_enemy_; j <= right_enemy_; ++j) {
      if (enemies_[i][j] == enemy)
        return sf::Vector2i(j, i);
    }
  }
  return sf::Vector2i(-1, -1);
}

bool EnemiesLine::KillEnemy(sf::FloatRect bullet_rect) { 
  bool result = false;

  Enemy *left_enemy = FindLeft();

  int y = (bullet_rect.top - theStorage.vmargin()) /
      (theStorage.vspace() + left_enemy->bounding_rect().height);
  int x = left_enemy_ + (bullet_rect.left - left_enemy->bounding_rect().left) /
      (theStorage.hspace() + left_enemy->bounding_rect().width);
  
  if (x < 0 || x > right_enemy_ || y < 0)
    return false;


  bool inter_left;
  if (enemies_[y][x] == nullptr)
    inter_left = false;
  else
    inter_left = enemies_[y][x]->bounding_rect().intersects(bullet_rect);
  if (inter_left) {
    theEventManager.PushEvent(kKill, nullptr);
    result = true;
    delete enemies_[y][x];
    enemies_[y][x] = nullptr;
  }

  // Out of range check.
  if (x < kColumnCount - 1 && !result) {
    bool inter_right;
    if (enemies_[y][x + 1] == nullptr)
      inter_right = false;
    else
      inter_right =
          enemies_[y][x + 1]->bounding_rect().intersects(bullet_rect);
    if (inter_right) {
      theEventManager.PushEvent(kKill, nullptr);
      result = true;
      delete enemies_[y][x + 1];
      enemies_[y][x + 1] = nullptr;
    }
  }

  if (result)
    CheckBorders();
  return result;
}

void EnemiesLine::AllowFire(sf::Vector2i pos_in_table) {
  if (enemies_[pos_in_table.y][pos_in_table.x] != nullptr)
    enemies_[pos_in_table.y][pos_in_table.x]->AllowFire();
}

void
EnemiesLine::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  for (int i = 0; i < kRowCount; ++i) {
    for (int j = 0; j < kColumnCount; ++j) {
      if (enemies_[i][j] != nullptr)
        enemies_[i][j]->draw(target, states);
    }
  }
}


void EnemiesLine::CreateEnemies() {
  enemies_.resize(kRowCount, std::vector<Enemy *>(kColumnCount, nullptr));
  for (int i = 0; i < kRowCount; ++i) {
    for (int j = 0; j < kColumnCount; ++j) {
      enemies_[i][j] = new Enemy(i, j);
    }
  }
  left_enemy_ = 0;
  right_enemy_ = 9;
}

void EnemiesLine::CheckBorders() {
  bool left   = true,
       right  = true;
  while (left || right) {
    left = true;
    right = true;
    for (int i = 0; i < kRowCount; ++i) {
      if (enemies_[i][left_enemy_] != nullptr)
        left = false;

      if (enemies_[i][right_enemy_] != nullptr)
        right = false;
    }
    if (left)
      ++left_enemy_;
    if (right)
      --right_enemy_;

    if (left_enemy_ > right_enemy_) {
      theEventManager.PushEvent(kWin, nullptr);
    }
  }
}

bool EnemiesLine::CanMove() {
  if (moving_left_) {
    Enemy *left_enemy = FindLeft();
    if (left_enemy != nullptr) {
      if (left_enemy->GetPos().x < 1)
        return false;
    }
  } else {
    Enemy *right_enemy = FindRight();
    if (right_enemy != nullptr) {
      if (right_enemy->GetPos().x + right_enemy->bounding_rect().width > 
          theStorage.screen_width() - 1)
        return false;
    }
  }

  return true;
}


Enemy *EnemiesLine::FindLeft() {
  for (int i = 0; i < kRowCount; ++i) {
    if (enemies_[i][left_enemy_] != nullptr)
      return enemies_[i][left_enemy_];
  }
  return nullptr;
}

Enemy *EnemiesLine::FindRight() {
  for (int i = 0; i < kRowCount; ++i) {
    if (enemies_[i][right_enemy_] != nullptr)
      return enemies_[i][right_enemy_];
  }
  return nullptr;
}
