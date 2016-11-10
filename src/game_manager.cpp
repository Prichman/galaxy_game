#include "game_manager.h"

#include <cstdlib>

#include "actor.h"
#include "bullet.h"
#include "enemy.h"

#include "keyboard.h"
#include "game_event_manager.h"
#include "game_event.h"
#include "storage.h"

static const int FPS_SEC      = 16;
static const int windowWidth  = 800;
static const int windowHeight = 600;

GameManager::GameManager()
    : running_(false) {
  main_window_.create(sf::VideoMode(windowWidth, windowHeight), "Galaxy game",
                      sf::Style::Close);

  // Initialize
  theKeyboard;
  theEventManager;
  theStorage;

  LoadBackground();
}

void GameManager::StartGame() {
  sf::Time sleep_time;
  const sf::Time fps_step = sf::milliseconds(FPS_SEC);
  clock_.restart();
  running_ = true;

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
        running_ = false;
        main_window_.close();
        break;
      case sf::Event::KeyPressed:
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
  if (theKeyboard.ShouldMoveLeft()) {

  } else if (theKeyboard.ShouldMoveRight()) {

  } else if (theKeyboard.ShouldAttack()) {

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
  FindCollisions();

  // Game events phase.
  // TODO: !!!
  GameEvent *game_event = nullptr;
  while ((game_event = theEventManager.GetEvent()) != nullptr) {
    switch (game_event->GetType()) {
      case ATTACK: {
        // Create bullet.
        Bullet *bullet = new Bullet;

        sf::IntRect hero_rect = hero_.GetIntRect();
        float bullet_x = hero_rect.left + hero_rect.width / 2;
        float bullet_y = hero_rect.top + hero_rect.height +
                         bullet->GetIntRect().height;
        bullet->SetPos(bullet_x, bullet_y);
        break;
      }
      case KILL: {
        // Remove player or enemy and bullet.
        Actor *parent = game_event->GetParent();
        if (dynamic_cast<Enemy *>(parent) == nullptr) {
          // Collision 2, hero and enemy's bullet.
          hero_.Die();
        } else {
          // Collision 1, enemy and hero's bullet.
          int killing_score = 100; // TODO: change to const.
          hero_.AddScore(killing_score);
        }
        break;
      }
      case DEATH: {
        // Bullet is out of screen.
        Bullet *bullet = dynamic_cast<Bullet *>(game_event->GetParent());
        delete bullet;
        break;
      }
    }
    delete game_event;
    game_event = nullptr;
  }
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
    sf::IntRect bullet_rect = (*it)->GetIntRect();
    if (bullet_rect.top >= enemies_line_.GetBottom()) {
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
    }
  }

  // 2. Hero and enemy's bullet.
  sf::IntRect hero_rect = hero_.GetIntRect();
  for (auto it = enemies_bullets_.begin(); it != enemies_bullets_.end(); ++it) {
    sf::IntRect enemy_rect = (*it)->GetIntRect();
    if (hero_rect.intersects(enemy_rect)) {
      GameEvent *event = new GameEvent(KILL, &hero_);
      theEventManager.PushEvent(event);
      
      delete *it;
      *it = nullptr;
      enemies_bullets_.erase(it);

      break;
    }
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
