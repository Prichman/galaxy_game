#include "game_manager.h"

#include <cstdlib>
#include <ctime>

#include <iostream>

#include "actor.h"
#include "bullet.h"
#include "enemy_bullet.h"
#include "enemy.h"
#include "game_event.h"
#include "game_event_manager.h"
#include "keyboard.h"
#include "storage.h"

static const int kFpsSec = 16;
static const int kKillScore = 100;

GameManager::GameManager()
    : running_(false) {
  main_window_.create(sf::VideoMode(theStorage.screen_width(),
                      theStorage.screen_height()), "Galaxy game",
                      sf::Style::Close);

  // Initialize
  theKeyboard;
  theEventManager;
  theStorage;

  srand(time(NULL));

  LoadBackground();
}

void GameManager::StartGame() {
  sf::Time sleep_time;
  const sf::Time fps_step = sf::milliseconds(kFpsSec);
  clock_.restart();
  running_ = true;

  std::cout << "Game started" << std::endl;
  while (running_) {
    elapsed_ = clock_.restart();

    HandleEvents();
    GameUpdate();
    Render();
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
        theKeyboard.HandleKeyPress();
        break;
      case sf::Event::KeyReleased:
        theKeyboard.HandleKeyRelease();
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
  if (theKeyboard.ShouldMoveLeft()) {
    hero_.SetSpeed(-hero_hspeed, 0);
  } else if (theKeyboard.ShouldMoveRight()) {
    hero_.SetSpeed(hero_hspeed, 0);
  } else if (theKeyboard.ShouldAttack()) {
    hero_.Attack();
  }

  // Update all actors.
  hero_.GameUpdate();
  enemies_line_.GameUpdate();
  for (auto it = hero_bullets_.begin(); it != hero_bullets_.end(); ++it) {
    (*it)->GameUpdate();
  }
  for (auto it = enemies_bullets_.begin(); it != enemies_bullets_.end(); ++it) {
    (*it)->GameUpdate();
  }

  // Collision phase.
  // If found create event and remove bullet.
  FindCollisions();

  // Game events phase.
  HandleGameEvents(); 
}

void GameManager::Render() {
  main_window_.clear();
  
  DrawBackground();
  DrawActors();
  DrawGui();

  main_window_.display();
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
  sf::FloatRect hero_rect = hero_.bounding_rect();
  for (auto it = enemies_bullets_.begin(); it != enemies_bullets_.end(); ++it) {
    sf::FloatRect enemy_rect = (*it)->bounding_rect();
    if (hero_rect.intersects(enemy_rect)) {
      GameEvent *event = new GameEvent(kKill, &hero_);
      theEventManager.PushEvent(event); 
      delete *it;
      *it = nullptr;
      enemies_bullets_.erase(it);

      break;
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
        bool is_enemy = (dynamic_cast<Hero *>(game_event->parent()) ==
                         nullptr);
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
        if (bullet->parent()->bounding_rect().top <
            theStorage.screen_height() / 2) {
          // Enemy.
          EnemyBullet *enemy_bullet = 
              dynamic_cast<EnemyBullet *>(bullet);

          sf::Vector2i enemy_pos = enemy_bullet->pos_in_table();
          auto it = std::find(enemies_bullets_.begin(), enemies_bullets_.end(),
                              enemy_bullet);
          enemies_bullets_.erase(it);
          enemies_line_.AllowFire(enemy_pos);
        } else {
          // Hero.
          
          auto it = std::find(hero_bullets_.begin(), hero_bullets_.end(),
                              bullet);
          hero_bullets_.erase(it);
          hero_.AllowFire();
          
        }
        delete bullet;
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

void GameManager::DrawBackground() {
  main_window_.draw(background_);
}

void GameManager::DrawActors() {
  main_window_.draw(hero_);
  main_window_.draw(enemies_line_);
  for (auto it = enemies_bullets_.begin(); it != enemies_bullets_.end(); ++it)
    main_window_.draw(**it);
  for (auto it = hero_bullets_.begin(); it != hero_bullets_.end(); ++it)
    main_window_.draw(**it);
}

void GameManager::DrawGui() {
  // TODO: 
}


void GameManager::CloseMainWindow() {
  main_window_.close();
  running_ = false;
  std::cout << "Game ended" << std::endl;
}
