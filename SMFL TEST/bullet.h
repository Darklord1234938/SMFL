#pragma once
#include <SFML/Graphics.hpp>

struct bullet {
private:
    float speed = 5;
    sf::Vector2f size, pos;
    sf::RectangleShape body;

public:
    bullet() {}

    bullet(sf::Vector2f _pos, sf::Vector2f _size, float _speed);

    void Update();

    sf::RectangleShape Draw();

    sf::FloatRect GetBounds();
};

