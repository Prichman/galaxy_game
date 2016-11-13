#include "enemy.h"

#include <string>

#include "storage.h"

Enemy::Enemy(int row, int pos_num) {
  
  std::string path_prefix = "resources/enemy_",
              path_num = std::to_string(1), // TODO: change 1 to row
              path_end = ".png";
  
  std::string path = path_prefix + path_num + path_end;

  LoadSprite(path);

  sf::Vector2f starting_pos;
  starting_pos.x = theStorage.hmargin() + 
      (bounding_rect().width + theStorage.hspace()) * pos_num;
  starting_pos.y = theStorage.vmargin() +
      (bounding_rect().height + theStorage.vspace()) * row;

  SetPos(starting_pos.x, starting_pos.y);
  SetSpeed(-theStorage.enemy_hspeed(), 0);
}


void Enemy::GameUpdate() {
  AttackActor::GameUpdate();

  if (speed_.y != 0)
    speed_.y = 0;
}
