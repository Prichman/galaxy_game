#include "game_manager.h"

#include <cstdlib>
#include <ctime>

#include <iostream>

#include "actor.h"
#include "bullet.h"
#include "enemy_bullet.h"
#include "enemy.h"
#include "game_event_manager.h"
#include "keyboard.h"
#include "storage.h"

static const int kFpsSec = 16;
static const int kKillScore = 100;

GameManager::GameManager()
    : game_mode_(kGame),
      running_(false),
      win_(false) {
  main_window_.create(sf::VideoMode(theStorage.screen_width(),
                      theStorage.screen_height()), "Galaxy game",
                      sf::Style::Close);

  // Initialize
  theKeyboard;
  theEventManager;
  theStorage;

  srand(time(NULL));

  LoadBackground();
  CreateGui();
}

GameManager::~GameManager() {
  CleanUp();
}

void GameManager::StartGame() {
  sf::Time sleep_time;
  const sf::Time fps_step = sf::milliseconds(kFpsSec);
  clock_.restart();
  running_ = true;

  std::cout << "Game started" << std::endl;
  while (running_) {
    clock_.restart();

    HandleEvents();
    TickAndRender();
    sleep_time = fps_step - clock_.getElapsedTime();
    if (sleep_time.asMilliseconds() > 0)
      sf::sleep(sleep_time);
  }
}

void GameManager::Pause() {
  running_ = false;
}

void GameManager::Resume() {
  running_ = true;
}

void GameManager::HandleEvents() {
  sf::Event event;

  while (main_window_.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        CloseMainWindow();
        break;
      case sf::Event::KeyPressed:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
          CloseMainWindow();
        theKeyboard.HandleKeyPress(event.key.code);
        break;
      case sf::Event::KeyReleased:
        theKeyboard.HandleKeyRelease(event.key.code);
        break;
      default:
        break;
    }
  }
}

void GameManager::GameUpdate() {
  // Move phase
  // TODO: change digits to const.
  const float hero_hspeed = 2.f;
  if (!hero_.resurrecting()) {
    bool need_move_left = theKeyboard.IsPressed(sf::Keyboard::Left);
    bool need_move_right = theKeyboard.IsPressed(sf::Keyboard::Right);
    

    if (!(need_move_left && need_move_right)) {
      if (need_move_left) {
        hero_.SetSpeed(-hero_hspeed, 0);
      } else if (need_move_right) {
        hero_.SetSpeed(hero_hspeed, 0);
      }
    }

    if (theKeyboard.IsPressed(sf::Keyboard::Space)) {
      hero_.Attack();
    }
  }

  // Update all actors.
  hero_.GameUpdate();
  enemies_line_.GameUpdate();
  for (auto *bullet : hero_bullets_)
     bullet->GameUpdate();

  for (auto *bullet : enemies_bullets_)
     bullet->GameUpdate();

  // Collision phase.
  // If found create event and remove bullet.
  FindCollisions();

  // Game events phase.
  HandleGameEvents(); 
}

void GameManager::TickAndRender() {
  main_window_.clear(sf::Color::Black);
  switch (game_mode_) { 
    case kStart:
      break;
    case kGame:
      GameUpdate();
      Render();
      break;
    case kEnd:
      if (win_) {
        ShowGoodEnding();
      } else {
        ShowBadEnding();
      }
      break;
  }
  main_window_.display();
}

void GameManager::Render() {
  DrawBackground();
  DrawActors();
  DrawGui();
}


void GameManager::FindCollisions() {
  // 3. Enemy and enemy's bullet. 
  // 4. Restoring hero and enemy's bullet.
  // We don't need to handle these collisions.

  // 1. Enemy and hero's bullet.
  for (auto it = hero_bullets_.begin(); it != hero_bullets_.end(); ++it) {
    sf::FloatRect bullet_rect = (*it)->bounding_rect();
    if (bullet_rect.top < enemies_line_.GetBottom()) {
      if (enemies_line_.KillEnemy(bullet_rect)) {
        delete *it;
        *it = nullptr;
      }
    }
  }

  // Remove empty bullets.
  for (int i = 0, n = hero_bullets_.size(); i < n; ++i) {
    if (hero_bullets_[i] == nullptr) {
      hero_bullets_.erase(hero_bullets_.begin() + i);
      --i;
      --n;
    }
  }

  // 2. Hero and enemy's bullet.
  if (!hero_.resurrecting()) {
    sf::FloatRect hero_rect = hero_.bounding_rect();
    for (auto it = enemies_bullets_.begin(); it != enemies_bullets_.end(); ++it) {
      sf::FloatRect enemy_rect = (*it)->bounding_rect();
      if (hero_rect.intersects(enemy_rect)) {
        theEventManager.PushEvent(kKill, &hero_); 
        delete *it;
        *it = nullptr;
        enemies_bullets_.erase(it);

        break;
      }
    }
  }
}

void GameManager::HandleGameEvents() {
  GameEvent *game_event = nullptr;
  while ((game_event = theEventManager.GetEvent()) != nullptr) {
    switch (game_event->type()) {
      case kAttack: {
        // Create bullet.
        sf::FloatRect actor_rect = game_event->parent()->bounding_rect(); 
        bool is_enemy =
            dynamic_cast<Hero *>(game_event->parent()) == nullptr;
        if (is_enemy) {
          Enemy *enemy = dynamic_cast<Enemy *>(game_event->parent());
          sf::Vector2i enemy_pos = enemies_line_.GetEnemyPos(enemy);
          
          EnemyBullet *bullet = new EnemyBullet(actor_rect, enemy, enemy_pos);
          enemies_bullets_.push_back(bullet);
        } else {
          Bullet *bullet = new Bullet(actor_rect, &hero_);
          hero_bullets_.push_back(bullet);
        }
        break;
      }
      case kKill: {
        // Remove player or enemy.
        Actor *parent = game_event->parent();
        if (parent == nullptr || dynamic_cast<Hero *>(parent) == nullptr) {
           // Collision 1, enemy and hero's bullet.
          hero_.AddScore(kKillScore);
          hero_.AllowFire();
        } else {
          // Collision 2, hero and enemy's bullet.
          hero_.Die();
        }
        break;
      }
      case kDeath: {
        // Bullet is out of screen.
        Bullet *bullet = dynamic_cast<Bullet *>(game_event->parent());
        EnemyBullet *enemy_bullet =
            dynamic_cast<EnemyBullet *>(game_event->parent());
        bool is_enemy = enemy_bullet != nullptr;
        if (is_enemy) {
          // Enemy.
          sf::Vector2i enemy_pos    = enemy_bullet->pos_in_table();
          auto it = std::find(enemies_bullets_.begin(), enemies_bullets_.end(),
                              enemy_bullet);
          enemies_bullets_.erase(it);
          enemies_line_.AllowFire(enemy_pos);
        } else {
          // Hero.
          
          auto it = 
              std::find(hero_bullets_.begin(), hero_bullets_.end(), bullet);
          if (it != hero_bullets_.end()) {
              hero_bullets_.erase(it);
          } else {
            std::cout << "Hello, I'm bug!" << std::endl;
            EnemyBullet *enemy_bullet = dynamic_cast<EnemyBullet *>(bullet);
            if (enemy_bullet != nullptr) {
              std::cout << "My real father is your enemy :<" << std::endl;
              sf::Vector2i pos_in_table = enemy_bullet->pos_in_table();
              std::cout << "Its coordinates: x = " << pos_in_table.x <<
                           ", y = " << pos_in_table.y << std::endl;
            }
          }
          hero_.AllowFire();
        }
        delete bullet;
        break;
      }
      case kLoss: {
        // asd;
        win_ = false;
        game_mode_ = kEnd;
        break;
      }
      case kWin:{
        // asd;
        win_ = true;
        game_mode_ = kEnd;
        break;
      }
    }
    delete game_event;
    game_event = nullptr;
  }
}

void GameManager::LoadBackground() {
  if (!background_texture_.loadFromFile("resources/background.png")) {
    printf("Can't load background file\n");
    exit(1);
  }
  background_.setTexture(background_texture_);
}

void GameManager::LoadFont() {
  if (!font_.loadFromFile("resources/my_font.ttf"))
    std::cout << "Can't load font" << std::endl;
}

void GameManager::CreateGui() {
  LoadFont();

  text_.setFont(font_);
  text_.setCharacterSize(24);
  text_.setFillColor(sf::Color::White);

  // Calculation gui elements' positions.
  float lives_width;
  float score_width;
  const float kConst = 20;

  lives_first_x_ = 2;
  
  text_.setString("lives");
  lives_width = text_.getGlobalBounds().width;
  text_.setString("score");
  score_width = text_.getGlobalBounds().width;

  lives_second_x_ = theStorage.screen_width() - lives_width - lives_first_x_;

  score_first_x_ = lives_first_x_ + lives_width + kConst;
  score_second_x_ = lives_second_x_ - (score_width + kConst);

  // Loading heart sprite.
  if (!heart_texture_.loadFromFile("resources/heart.png")) {
    std::cout << "Can't load heart texture" << std::endl;
    return;
  }
  heart_sprite_.setTexture(heart_texture_);
  heart_sprite_.scale(1.4, 1.4);
}

void GameManager::DrawBackground() {
  main_window_.draw(background_);
}

void GameManager::DrawActors() {
  for (auto it = enemies_bullets_.begin(); it != enemies_bullets_.end(); ++it)
    main_window_.draw(**it);
  for (auto it = hero_bullets_.begin(); it != hero_bullets_.end(); ++it)
    main_window_.draw(**it);
  main_window_.draw(hero_);
  main_window_.draw(enemies_line_);
}

void GameManager::DrawGui() {
  DrawFirstLineGui();
  DrawSecondLineGui();
}

void GameManager::DrawFirstLineGui() {
  float y = 2;

  // 1st player.
  text_.setString("lives");
  text_.setPosition(lives_first_x_, y); 
  main_window_.draw(text_);
 
  text_.setPosition(score_first_x_, y);
  text_.setString("score");
  main_window_.draw(text_);
 
  // 2nd player.
  text_.setString("lives");
  text_.setPosition(lives_second_x_, y);
  main_window_.draw(text_);

  text_.setString("score");
  text_.setPosition(score_second_x_, y);
  main_window_.draw(text_);
}

void GameManager::DrawSecondLineGui() {
  const int hmargin = 10;
  const int y = 40;
  const float offset_x = hmargin + heart_sprite_.getGlobalBounds().width;
  int lives_count = hero_.GetLivesCount();
  
  heart_sprite_.setPosition(2, y);
  for (int i = 0; i < lives_count; ++i) {
    main_window_.draw(heart_sprite_);
    heart_sprite_.move(offset_x, 0);
  }
  
  sf::Uint32 score = hero_.GetScore();
  text_.setString(std::to_string(score));
  text_.setPosition(score_first_x_, y - 10);

  main_window_.draw(text_);
  // TODO: add second player.
}

void GameManager::ShowStartScreen() {

}

void GameManager::ShowGoodEnding() {
  // TODO: change this.
  float y;
  
  text_.setCharacterSize(50);
  text_.setString("You win!");
  y = theStorage.screen_height() / 2 - text_.getGlobalBounds().height;
  text_.setPosition(
      (theStorage.screen_width() - text_.getGlobalBounds().width) / 2, y);
  main_window_.draw(text_);

  text_.setCharacterSize(24);
  text_.setString("Your score: " + std::to_string(hero_.GetScore()));
  text_.setPosition(
      (theStorage.screen_width() - text_.getGlobalBounds().width) / 2, y + 50);
  main_window_.draw(text_); 

  
  text_.setString("Press Esc to exit");
  text_.setPosition(
      (theStorage.screen_width() - text_.getGlobalBounds().width) / 2, y + 80);
  main_window_.draw(text_); 
}

void GameManager::ShowBadEnding() {
  float y;
  
  text_.setCharacterSize(50);
  text_.setString("Game over");
  y = theStorage.screen_height() / 2 - text_.getGlobalBounds().height;
  text_.setPosition(
      (theStorage.screen_width() - text_.getGlobalBounds().width) / 2, y);
  main_window_.draw(text_);

  text_.setCharacterSize(24);
  text_.setString("Press Esc to exit");
  text_.setPosition(
      (theStorage.screen_width() - text_.getGlobalBounds().width) / 2, y + 50);
  main_window_.draw(text_);
}

void GameManager::CleanUp() {
  for (auto *bullet : enemies_bullets_) {
    delete bullet;
    bullet = nullptr;
  }
  for (auto *bullet : hero_bullets_) {
    delete bullet;
    bullet = nullptr;
  }
}

void GameManager::CloseMainWindow() {
  main_window_.close();
  running_    = false;

  std::cout << "Game ended" << std::endl;
}
