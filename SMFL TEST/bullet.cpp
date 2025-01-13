#include "bullet.h"

bullet::bullet(sf::Vector2f _pos, sf::Vector2f _size, float _speed)
{
    body.setFillColor(sf::Color::Blue);
    pos = sf::Vector2f(_pos.x -= (_size.x / 2), _pos.y -= (_size.y / 2));
    body.setPosition(sf::Vector2f(pos));
    body.setSize(_size);
    size = _size;
    speed = _speed;
}

void bullet::Update()
{
    pos.y -= speed;
    body.setPosition(pos);
}

sf::RectangleShape bullet::Draw()
{
    return body;
}

sf::FloatRect bullet::GetBounds()
{
    return body.getGlobalBounds();
}
