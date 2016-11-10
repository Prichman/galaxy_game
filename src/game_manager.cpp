#include "game_manager.h"

#include "actor.h"
#include "bullet.h"

#include "keyboard.h"
#include "game_event_manager.h"
#include "game_event.h"

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

  LoadBackground();
  CreatePlayer();
  CreateEnemies();
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

  for (auto it = actors_.begin(); it != actors_.end(); ++it) {
    (*it)->GameUpdate();
  }


  // Collision phase.

  // Game events phase.
  GameEvent *game_event;
  while ((game_event = theEventManager.GetEvent()) != nullptr) {
    switch (game_event->GetType()) {
      case ATTACK:
        // Create bullet.
        break;
      case KILL:
        // Remove player or enemy and bullet.
        break;
      case DEATH:
        // Remove buller.
        break;
    }
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
        // TODO: add score and remove bullet from set.
        break;
      }
    }
  }

  // 2. Hero and enemy's bullet.
  sf::IntRect hero_rect = hero_.GetIntRect();
  for (auto it = enemies_bullets_.begin(); it != enemies_bullets_.end(); ++it) {
    sf::IntRect enemy_rect = (*it)->GetIntRect();
    if (hero_rect.intersects(enemy_rect)) {
      GameEvent *event = new GameEvent(KILL, &hero_);
      theEventManager.PushEvent(event);
      break;
    }
  }  
}

void GameManager::LoadBackground() {
  // TESTME: --
  background_texture_.loadFromFile("resources/background.png");
  background_.setTexture(background_texture_);
}

void GameManager::CreatePlayer() {
  // TODO: 
}

void GameManager::CreateEnemies() {

}

void GameManager::DrawBackground() {
  main_window_.draw(background_);
}

void GameManager::DrawActors() {
  for (auto it = actors_.begin(); it != actors_.end(); ++it) {
    main_window_.draw(**it);
  }
  main_window_.draw(hero_);
  for (auto it = enemies_bullets_.begin(); it != enemies_bullets_.end(); ++it)
    main_window_.draw(**it);
  for (auto it = hero_bullets_.begin(); it != hero_bullets_.end(); ++it)
    main_window_.draw(**it);
}

void GameManager::DrawGui() {

}
