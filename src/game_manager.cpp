#include "game_manager.h"

#include "actor.h"
#include "bullet.h"

#include "keyboard.h"
#include "game_event_manager.h"
#include "game_event.h"

#include "info.h"

static const int FPS_SEC      = 16;
static const int windowWidth  = 800;
static const int windowHeight = 600;

GameManager::GameManager() {
  main_window_.create(sf::VideoMode(windowWidth, windowHeight), "SFML works!",
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

  while (main_window_.isOpen()) {
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

}

void GameManager::Resume() {

}

void GameManager::HandleEvents() {
  sf::Event event;

  while (main_window_.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
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
  sf::IntRect hero_rect = hero_.GetIntRect();
  for (auto it = enemies_bullets_.begin(); it != enemies_bullets_.end(); ++it) {
    sf::IntRect enemy_rect = (*it)->GetIntRect();
    if (hero_rect.intersects(enemy_rect)) {
      // Kill hero.
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
