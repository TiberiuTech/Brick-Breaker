#pragma once
#include <SFML/Graphics.hpp>

class Brick {
private:
    sf::RectangleShape shape;
    bool destroyed;

public:
    Brick(float x, float y, const sf::Texture& texture);


    void setTexture(const sf::Texture& texture);

    void draw(sf::RenderWindow& window);
    sf::FloatRect getGlobalBounds();
    bool isDestroyed() const;
    sf::RectangleShape& getShape();
    void destroy();
};