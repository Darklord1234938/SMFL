#include "Gui.h"

Gui::Image::Image(sf::Vector2f _pos, sf::Vector2f _size, sf::String _sprite)
{
    pos = sf::Vector2f(_pos.x -= (_size.x / 2), _pos.y -= (_size.y / 2));
    body.setPosition(sf::Vector2f(pos));
    body.setSize(_size);
    size = _size;
    sprite.loadFromFile(_sprite);
    body.setTexture(&sprite);
}

sf::RectangleShape Gui::Image::Draw()
{
    return body;
}

void Gui::Button::Hover()
{
    body.setFillColor(sf::Color(125, 125, 125));
}

bool Gui::Button::Action()
{
    body.setFillColor(sf::Color(10, 10, 10));
    return true;
}

bool Gui::Button::Action(bool _b)
{
    body.setFillColor(sf::Color(10, 10, 10));
    return _b;
}
