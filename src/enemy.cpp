#include "enemy.h"

#include <string>

Enemy::Enemy(int row, int pos_num) {
  sf::Vector2f starting_pos;
 
  // TODO: change if need.
  float left_margin = 40;
  float enemy_size  = 12;
  float hspace      = 5;
  float vspace      = 10;
  float top_margin  = vspace;
  
  starting_pos.x = left_margin + (enemy_size + hspace) * pos_num;
  starting_pos.y = top_margin + (enemy_size + vspace) * row;


  std::string path_prefix = "resources/enemy_",
              path_num = std::to_string(1), // TODO: change 1 to row
              path_end = ".png";
  
  std::string path = path_prefix + path_num + path_end;

  LoadSprite(path);

  SetPos(starting_pos.x, starting_pos.y);
}
