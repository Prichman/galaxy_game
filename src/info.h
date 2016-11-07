#pragma once

struct GameInfo {
  int enemies_line;
  int enemies_count;
  int hero_lives;
  int score;
};

static GameInfo game_info = {10, 30, 3, 0};
