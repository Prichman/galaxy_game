#include <iostream>

using namespace std;

#include <SFML/Graphics.hpp>

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!", 
                          sf::Style::Close);
  
  sf::CircleShape shape(100.f);
  shape.setOrigin(100.f, 100.f);
  shape.setPosition(200.f, 200.f);
  shape.setFillColor(sf::Color::Green);

  cout << "Galaxy" << endl << endl;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed: {
          cout << "Now we are closing..." << endl;
          window.close();
          break;
        }
        case sf::Event::KeyPressed:
        case sf::Event::KeyReleased: {
          // some action
        }
        default:
          break;
      }
    }

    window.clear(sf::Color::White);
    
    window.draw(shape);

    window.display();
  }

  return 0;
}