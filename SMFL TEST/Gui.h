#pragma once
#include <SFML/Graphics.hpp>

namespace Gui {
    struct Image {
    protected:
        sf::Vector2f size, pos;
        sf::Texture sprite;
        sf::RectangleShape body;

    public:
        Image() {}

        Image(sf::Vector2f _pos, sf::Vector2f _size, sf::String _sprite);

        sf::RectangleShape Draw();
    };

    struct Button : Image {
    private:

    public:
        Button() {}

        Button(sf::Vector2f _pos, sf::Vector2f _size, sf::String _sprite) : Image(_pos, _size, _sprite) { }

        void Hover();

        bool Action();

        bool Action(bool _b);
    };
}
