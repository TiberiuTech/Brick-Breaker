#include "Paddle.h"

Paddle::Paddle(float x, float y) : speed(350.f), velocity(0.f) {
    shape.setPosition(x, y);
    shape.setSize({ 100.f, 15.f });
    shape.setFillColor(sf::Color::Red);
}

void Paddle::moveLeft(float deltaTime) {
    velocity = -speed;
}

void Paddle::moveRight(float deltaTime) {
    velocity = speed;
}

void Paddle::move(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity = -speed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity = speed;
    }
    else {
        velocity = 0.f; 
    }

    shape.move(velocity * deltaTime, 0.f);

    if (shape.getPosition().x < 0) {
        shape.setPosition(0, shape.getPosition().y);
        velocity = 0; 
    }
    else if (shape.getPosition().x + shape.getSize().x > 800) {
        shape.setPosition(800 - shape.getSize().x, shape.getPosition().y);
        velocity = 0; 
    }
}

void Paddle::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Paddle::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

const sf::RectangleShape& Paddle::getShape() const {
    return shape;
}

float Paddle::getWidth() const {
    return shape.getSize().x;
}

const sf::Vector2f& Paddle::getPosition() const {
    return shape.getPosition();
}