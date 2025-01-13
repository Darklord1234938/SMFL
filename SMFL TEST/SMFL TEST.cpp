#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <random>
#include "InputS.h"
#include "bullet.h"
#include "Gui.h"

const int WITDH = 500, HIEGHT = 900;

int enemiesKilled = 0;
bool newGame = true, credits = false, haveToClose = false;


struct Player {
private:
    
    float speed = 5;
    float currentTime, timeMax = 1, decTime = 0.1;
    sf::Vector2f size, pos;
    sf::Texture sprite;
    sf::RectangleShape body;
    std::vector<bullet> bullets;

    void Move() {
        float x = Input::Axis("Horizontal");
        pos.x += 10 * x;
    }

    void Bounds() {
        pos.x = (pos.x + (size.x / 2) > WITDH) ? 0 - (size.x / 2) : (pos.x + (size.x / 2) < 0) ? WITDH - (size.x / 2) : pos.x;
    }

    void Shoot() {
        currentTime -= decTime;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && currentTime < 0) {
            bullet b = bullet(sf::Vector2f(pos.x + size.x / 2, pos.y + size.y / 4), sf::Vector2f(10, 10), 15);
            bullets.push_back(b);
            currentTime = timeMax;
        }
    }

public:
    Player() {}

    Player(sf::Vector2f _pos, sf::Vector2f _size, float _speed, sf::String _sprite) {
        currentTime = timeMax;
        pos = sf::Vector2f(_pos.x -= (_size.x / 2), _pos.y -= (_size.y / 2));
        body.setPosition(sf::Vector2f(pos));
        body.setSize(_size);
        size = _size;
        speed = _speed;
        sprite.loadFromFile(_sprite);
        body.setTexture(&sprite);
    }

    void Update() {
        Shoot();
        for (int i = 0; i < GetBullets(); i++) {
            bullets[i].Update();
        }
        Move();
        Bounds();
        body.setPosition(pos);
    }

    sf::RectangleShape Draw() {
        return body;
    }

    bullet GetBullet(int i) {
        return bullets[i];
    }

    int GetBullets() {
        return bullets.size();
    }

    void RemoveBullet(int index) {
        if (index >= 0 && index < bullets.size()) {
            bullets.erase(bullets.begin() + index);
        }
    }
};

struct Enemy {
private:
    float speed = 5;
    sf::Texture sprite;
    sf::Vector2f size, pos;
    sf::RectangleShape body;

    void Bounds() {
        if (pos.x + (size.x / 2) > WITDH) {
            pos.x = 0 - (size.x / 2);  // If the right edge goes beyond the screen, move it to the left edge
        }
        else if (pos.x + (size.x / 2) < 0) {
            pos.x = WITDH - (size.x / 2);  // If the left edge goes beyond the screen, move it to the right edge
        }
    }

public:
    bool straightdownEnemy = false, crossEnemy = false, sidewaysEnemy = false;
    bool choise = true; // Only randomize when choise is true

    Enemy() {}

    Enemy(sf::Vector2f _pos, sf::Vector2f _size, float _speed, sf::String _sprite) {
        pos = sf::Vector2f(_pos.x -= (_size.x / 2), _pos.y -= (_size.y / 2));
        body.setPosition(sf::Vector2f(300 - 40, 0));
        body.setSize(_size);
        size = _size;
        speed = _speed;
        sprite.loadFromFile(_sprite);
        body.setTexture(&sprite);
    }

    void Randomchoise() {
        if (choise) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, 2);  // Randomly choose movement type

            int randomChoice = distrib(gen);

            switch (randomChoice) {
            case 0:
                straightdownEnemy = true;
                crossEnemy = sidewaysEnemy = false;  // Ensure other movement types are disabled
                break;
            case 1:
                crossEnemy = true;
                straightdownEnemy = sidewaysEnemy = false;
                break;
            case 2:
                sidewaysEnemy = true;
                straightdownEnemy = crossEnemy = false;
                break;
            default:
                break;
            }
            choise = false;  // Prevent immediate re-randomizing
        }
    }

    void Update() {
        if (straightdownEnemy) {
            pos.y += speed;  // Move straight down
        }
        if (crossEnemy) {
            pos.y += speed;
            pos.x += speed;  // Cross movement (diagonal)
        }
        if (sidewaysEnemy) {
            pos.y += speed;
            pos.x -= speed;  // Move sideways (diagonal)
        }

        if (pos.y >= HIEGHT) {
            ResetPosition();  // Reset the enemy if it goes off-screen
        }

        Bounds();
        body.setPosition(pos);
    }

    sf::RectangleShape Draw() {
        return body;
    }

    sf::FloatRect GetBounds() {
        return body.getGlobalBounds();
    }

    void ResetPosition() {
        pos = sf::Vector2f(rand() % WITDH, rand() % (HIEGHT / 4));  // Random reset at top of screen
        body.setPosition(pos);
        choise = true;  // Allow randomization after reset
        Randomchoise(); // Ensure the movement is randomized again after reset
    }
};



sf::String toString(int integer)
{
    char numstr[10]; // enough to hold all numbers up to 32-bits
    sprintf_s(numstr, "%i", integer);
    return numstr;
}


int main() {
    bool isGameOver = false;
    int points =0;
    sf::Font font;

    if (font.loadFromFile("font/LowerResolution.ttf"))
    {
        // error...
    }

    sf::Text txt;
    txt.setFillColor(sf::Color::White);

    sf::Text healthText;
    healthText.setFont(font);
    healthText.setFillColor(sf::Color::Green);  // Bright green color for health
    healthText.setCharacterSize(24);  // Same size as points
    healthText.setPosition(WITDH - 150, 10);  // Position at the top-right corner

    sf::RenderWindow window(sf::VideoMode(WITDH, HIEGHT), "gal");

    Player player = Player(sf::Vector2f(window.getSize().x / 2, window.getSize().y - window.getSize().y / 8), sf::Vector2f(80, 150), 5, "Canon.png");
    std::vector<Enemy> enemies;
    Enemy enemy(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 8), sf::Vector2f(30, 30), 5, "alien.png");
    enemies.push_back(enemy);
    

    txt.setFont(font);
    int health = 100; // Initial health value
    int bIndex = 0;
    bool kdp = false, kup = false, kap = false;

    while (window.isOpen()) {

        window.setFramerateLimit(30);
        window.setSize(sf::Vector2u(WITDH, HIEGHT));

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (!newGame && !credits) {

            if (!isGameOver) {


                // Update health text
                sf::String healthStr = toString(health);
                healthText.setString("Health " + healthStr);

                sf::String pointsText = toString(points);
                txt.setString("points " + pointsText);

                for (int i = 0; i < player.GetBullets(); i++) {
                    for (int j = 0; j < enemies.size(); j++) {
                        if (player.GetBullet(i).GetBounds().intersects(enemies[j].GetBounds())) {
                            enemies[j].ResetPosition();  // Reset enemy when hit
                            player.RemoveBullet(i); // Remove the bullet
                            enemiesKilled++;  // Increment the kill counter
                            points++;
                            if (enemiesKilled % 5 == 0) {

                                enemies.push_back(enemy); // Spawn a new enemy every 5 kills
                            }
                            break;
                        }

                        if (player.Draw().getGlobalBounds().intersects(enemies[j].GetBounds())) {
                            // Player collides with enemy, reset enemy and reduce health
                            enemies[j].ResetPosition();
                            enemiesKilled++;
                            health -= 10;
                            if (health <= 0) {
                                // Close the game if health reaches 0
                                //window.close();

                                //
                                isGameOver = true;

                            }
                        }
                    }
                }

                player.Update();
                for (int i = 0; i < enemies.size(); i++) {
                    enemies[i].Randomchoise();
                    enemies[i].Update();
                }

                window.clear();
                window.draw(player.Draw());
                window.draw(txt);  // Drawing points
                window.draw(healthText);  // Drawing health

                for (int i = 0; i < enemies.size(); i++) {
                    window.draw(enemies[i].Draw());
                }

                for (int i = 0; i < player.GetBullets(); i++) {
                    window.draw(player.GetBullet(i).Draw());
                }

                window.display();
            }
            else {
                enemies.clear();

                window.clear();

                Gui::Image gameOverGui(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2), sf::Vector2f(600, 600), "Game Over.png");
                window.draw(gameOverGui.Draw());

                points = 0;

                window.display();


                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                    isGameOver = false;
                    health = 100;

                    enemies.push_back(enemy);
                    window.clear();
                    window.display();
                }
            }
        }
        else if(newGame && !credits) {
            window.clear();

            Gui::Image titleGui(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2), sf::Vector2f(500, 900), "Space war title.png");
            Gui::Button bttnStart(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2 - 30), sf::Vector2f(300, 100), "start.png");
            Gui::Button bttnCredits(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2 + 110), sf::Vector2f(300, 100), "Credits.png");
            Gui::Button bttnExit(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2 + 250), sf::Vector2f(300, 100), "Exits.png");
            Gui::Button bttns[3] { bttnStart, bttnCredits, bttnExit };

            bttns[bIndex].Hover();
            window.draw(titleGui.Draw());
            window.draw(bttns[0].Draw());
            window.draw(bttns[1].Draw());
            window.draw(bttns[2].Draw());

            points = 0;

            window.display();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !kdp && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !kdp && !sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                bIndex++;
                kdp = true;
            }
            else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                kdp = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !kup && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !kup && !sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                bIndex--;
                kup = true;
            }
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                kup = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !kap) {
                switch (bIndex)
                {case 0:
                    newGame = bttnStart.Action(false);
                break;
                case 1:
                    credits = bttnCredits.Action(true);
                    break;
                default:
                    haveToClose = bttnExit.Action();
                    break;
                }
                kap = true;
            }
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                kap = false;
            }

            if (bIndex > 2) {
                bIndex = 0;
            }
            else if(bIndex < 0)
            {
                bIndex = 2;
            }
        }
        else {
            window.clear();

            Gui::Image titleGui(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2), sf::Vector2f(500, 900), "credit page.png");
            Gui::Button bttnBack(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2 + 250), sf::Vector2f(300, 100), "Exits.png");

            bttnBack.Hover();
            window.draw(titleGui.Draw());
            window.draw(bttnBack.Draw());

            window.display();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !kap) {
                credits = bttnBack.Action(false);
                kap = true;
            }
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                kap = false;
                
            }

            if (bIndex > 2) {
                bIndex = 0;
            }
            else if (bIndex < 0)
            {
                bIndex = 2;
            }
        }

        if (haveToClose) {
            window.close();
        }
    }
}
