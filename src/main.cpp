#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>

std::vector<sf::RectangleShape> make_turtles(std::vector<sf::Color> colors, int num) {
  std::vector<sf::RectangleShape> turtles;
  for(int i = 0; i < num; i++) {
    sf::RectangleShape turt(sf::Vector2f(33, 33));
    turt.setPosition(sf::Vector2f(10,  25 + (i * (33 * 1.5))));
    turt.setFillColor(colors[std::rand() % colors.size()]);
    turtles.push_back(turt);
  }
  return turtles;
}

sf::RectangleShape* winner;
bool hasWon = false;
bool started = false;

std::vector<sf::RectangleShape> generate_map() {
  std::vector<sf::RectangleShape> shapes;
  for(int x = 0; x < 33; x++) {
    for(int y = 0; y < 33; y++) {
      sf::RectangleShape shape(sf::Vector2f(16, 16));
      shape.setPosition(x * 16, y * 16);
      if (x > 2 && x < 5)
        shape.setFillColor((std::rand() % 2 == 1 ? sf::Color(55, 55, 55) : sf::Color(45, 45, 45)));
      else if(x > 27 && x < 30)
        shape.setFillColor((std::rand() % 2 == 1 ? sf::Color(233, 233, 233) : sf::Color(245, 245, 245)));
      else
        shape.setFillColor((std::rand() % 2 == 1 ? sf::Color(183, 237, 255) : sf::Color(164, 213, 229)));
      shapes.push_back(shape);
    }
  }
  return shapes;
}



int main(void) {
  // dimensions = 33 x 33 (each is 16x16)
  std::srand(std::time( NULL ));
  sf::RenderWindow window(sf::VideoMode(528, 528), "cLogo Turtles");
  window.setSize(sf::Vector2u(528 * 2, 528 * 2));
  window.setFramerateLimit(60);
  sf::Texture turtleTexture;
  if(!turtleTexture.loadFromFile("res/turtle.png")) {
    std::cerr << "Cannot load turtle texture!" << std::endl; return 1;
  }

  std::vector<sf::Color> colors;
  for(int i = 0; i < 100; i++)
    colors.push_back(sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255));

  auto turtles = make_turtles(colors, 10);
  auto map = generate_map();
  while(window.isOpen()){
    sf::Event event;
    while(window.pollEvent(event)){
      if(event.type == sf::Event::Closed)
        window.close();
      else if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::S) {
          started = true;
        } else if(event.key.code == sf::Keyboard::R) {
          for(auto it = turtles.begin(); it != turtles.end(); it++){
            it->setFillColor(sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255));
            auto pos = it->getPosition();
            it->setPosition(10, pos.y);
          }
          hasWon = false;
          winner = NULL;
          started = false;
        }
      }
    }
    window.clear(sf::Color(233, 233, 233));
    // logic lol xd!!
    if(started) {
      for(auto it = turtles.begin(); it != turtles.end(); it++){
        it->move(std::rand() % 6, 0);
        if(it->getPosition().x > 432) {
          for(auto z = turtles.begin(); z != turtles.end(); z++) {
            if(z != it)
              z->setFillColor(sf::Color(0, 0, 0, 0));
          }
          hasWon = true;
          winner = &*it;
          started = false;
        }
      }
    }
    std::for_each(map.begin(), map.end(), [&window](sf::RectangleShape &shape){
          window.draw(shape);
        });
    for(auto it = turtles.begin(); it != turtles.end(); it++){
      if(hasWon && (&*it != winner))
        window.draw(*it);
      else if(hasWon) {
        sf::Transform trans;
        auto pos = it->getPosition();
        window.draw(*it, trans);
      } else {
        window.draw(*it);
      }
    }
    window.display();
  }
  return 0;
}
