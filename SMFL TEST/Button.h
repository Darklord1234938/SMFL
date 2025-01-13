#pragma once
#include <SFML/Graphics.hpp>
#include "Image.h"

struct Button : Image {
private:

public:
    Button();

    Button(sf::Vector2f _pos, sf::Vector2f _size, sf::String _sprite) : Image(_pos, _size, _sprite);

   void Hover();

   bool Action();

    bool Action(bool _b);
};
