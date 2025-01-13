#include "InputS.h"

int Input::Axis(std::string _axe)
{
    return (_axe == "Horizontal") ?
        (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) ? -1 :
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ? -1 :
        (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) ? 1 :
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) ? 1 : 0 :
        (_axe == "Vertical") ?
        (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) ? -1 :
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) ? -1 :
        (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) ? 1 :
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ? 1 : 0 : 0;
}
