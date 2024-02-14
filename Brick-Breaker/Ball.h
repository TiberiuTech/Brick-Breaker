#pragma once
#include <SFML/Graphics.hpp>

class Ball {
private:
    sf::CircleShape shape;
    sf::Vector2f direction;
    float speed;

public:
    Ball() = default; 
    Ball(float x, float y);

    void setPosition(float x, float y);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void reverseY();
    void reverseX();
    void resetDirection();
    void setVelocityX(float velX);

    sf::FloatRect getGlobalBounds() const;
    const sf::CircleShape& getShape() const;
};