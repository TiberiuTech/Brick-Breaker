#include "GameAnimation.h"
#include <iostream>
#include <algorithm> 

GameAnimation::GameAnimation(sf::RenderWindow& window, const std::string& text, float duration, sf::Font* f)
    : duration(duration), font(f) {
    animationText.setFont(*font);

    
    sf::Vector2u windowSize = window.getSize();
    float x = windowSize.x / 2.0f - animationText.getLocalBounds().width / 2.0f;
    float y = windowSize.y / 2.0f - animationText.getLocalBounds().height / 2.0f;
    animationText.setPosition(x, y);

    animationText.setString(text);
    animationText.setCharacterSize(24);
    animationText.setFillColor(sf::Color::White);
}

void GameAnimation::update(float deltaTime) {
   
    animationText.move(0, -50 * deltaTime);

 
    sf::Uint8 alpha = static_cast<sf::Uint8>(std::max(0.f, 255.f - (animationClock.getElapsedTime().asSeconds() / duration) * 255.f));
    animationText.setFillColor(sf::Color(
        animationText.getFillColor().r,
        animationText.getFillColor().g,
        animationText.getFillColor().b,
        alpha
    ));
}

void GameAnimation::draw(sf::RenderWindow& window) {
    window.draw(animationText);
}

bool GameAnimation::isFinished() const {
    return animationClock.getElapsedTime().asSeconds() >= duration;
}
