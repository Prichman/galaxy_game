#include "game_manager.h"

#include <cstdlib>

#include <iostream>

#include "actor.h"
#include "bullet.h"
#include "enemy.h"

#include "keyboard.h"
#include "game_event_manager.h"
#include "game_event.h"
#include "storage.h"

static const int FPS_SEC      = 16;

GameManager::GameManager()
    : running_(false) {
  main_window_.create(sf::VideoMode(theStorage.screen_width(),
                      theStorage.screen_height()), "Galaxy game",
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
  if (theKeyboard.ShouldMoveLeft()) {
    hero_.SetSpeed(-0.8, 0);
  } else if (theKeyboard.ShouldMoveRight()) {
    hero_.SetSpeed(0.8, 0);
  } else if (theKeyboard.ShouldAttack()) {
    GameEvent *event = new GameEvent(ATTACK, &hero_);
    theEventManager.PushEvent(event);
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

void GameManager::HandleGameEvents() {
  GameEvent *game_event = nullptr;
  while ((game_event = theEventManager.GetEvent()) != nullptr) {
    switch (game_event->GetType()) {
      case ATTACK: {
        // Create bullet.
        std::cout << "Someone attacks" << std::endl;
        sf::IntRect actor_rect = game_event->GetParent()->GetIntRect(); 
        bool is_enemy = (dynamic_cast<Hero *>(game_event->GetParent()) ==
                         nullptr);
        std::cout << "This one is " << (is_enemy ? "enemy" : "hero");
        std::cout << std::endl;
        Bullet *bullet = new Bullet(actor_rect, is_enemy);
        if (is_enemy) {
          enemies_bullets_.push_back(bullet);
        } else {
          hero_bullets_.push_back(bullet);
        }
        break;
      }
      case KILL: {
        // Remove player or enemy.
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
        std::vector<Bullet *> *bullets; // for needed container.
        if (bullet->hero()) {
          bullets = &hero_bullets_;

        } else {
          bullets = &enemies_bullets_;
        }
        auto bul_it = std::find(bullets->begin(), bullets->end(),
                                 bullet);
        if (bul_it != bullets->end())
          bullets->erase(bul_it);

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
