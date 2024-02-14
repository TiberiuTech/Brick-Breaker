#include "Ball.h"

Ball::Ball(float x, float y) : direction(1.f, -1.f), speed(300.f) {
    shape.setPosition(x, y);
    shape.setRadius(5.f);
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
}

void Ball::setPosition(float x, float y) {
    shape.setPosition(x, y);
}

void Ball::update(float deltaTime) {
    shape.move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);

   
    if (shape.getPosition().x <= 0 || shape.getPosition().x >= 800) {
        reverseX();
    }

    
    if (shape.getPosition().y <= 0) {
        reverseY();
    }
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Ball::reverseY() {
    direction.y = -direction.y;
}

void Ball::reverseX() {
    direction.x = -direction.x;
}

void Ball::resetDirection() {
    direction = sf::Vector2f(1.f, -1.f);
}

void Ball::setVelocityX(float velX) {
    direction.x = velX;
}

sf::FloatRect Ball::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

const sf::CircleShape& Ball::getShape() const {
    return shape;
}