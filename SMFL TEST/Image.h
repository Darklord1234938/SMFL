#pragma once
#include <SFML/Graphics.hpp>
struct Image {
protected:
    sf::Vector2f size, pos;
    sf::Texture sprite;
    sf::RectangleShape body;

public:
    Image();

    Image(sf::Vector2f _pos, sf::Vector2f _size, sf::String _sprite) {  }

    sf::RectangleShape Draw();
};
