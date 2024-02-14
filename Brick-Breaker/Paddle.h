#pragma once
#include <SFML/Graphics.hpp>

class Paddle {
private:
    sf::RectangleShape shape;
    float speed;
    float velocity; 

public:
    Paddle() = default;
    Paddle(float x, float y);

    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void move(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getGlobalBounds() const;
    const sf::RectangleShape& getShape() const;
    float getWidth() const;
    const sf::Vector2f& getPosition() const;

    void increaseSize(float width, float height) {
        shape.setSize(sf::Vector2f(shape.getSize().x + width,
            shape.getSize().y + height));
    }
};
