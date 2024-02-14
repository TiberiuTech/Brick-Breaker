#include "Brick.h"

Brick::Brick(float x, float y, const sf::Texture& texture) : destroyed(false) {
    shape.setPosition(x, y);
    shape.setSize({ 50.f, 20.f });
    shape.setOutlineThickness(1.f);
    shape.setOutlineColor(sf::Color::White);
    shape.setTexture(&texture); 
}



void Brick::setTexture(const sf::Texture& texture) {
    shape.setTexture(&texture);
}

void Brick::draw(sf::RenderWindow& window) {
    if (!this->destroyed)
        window.draw(this->shape);
}

sf::FloatRect Brick::getGlobalBounds() {
    return this->shape.getGlobalBounds();
}

bool Brick::isDestroyed() const {
    return this->destroyed;
}

sf::RectangleShape& Brick::getShape() {
    return this->shape;
}

void Brick::destroy() {
    this->destroyed = true;
}