#pragma once
#include <SFML/Graphics.hpp>

class GameAnimation {
public:
    
    GameAnimation(sf::RenderWindow& window, const std::string& text, float duration, sf::Font* font);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool isFinished() const;

private:
    sf::Text animationText;
    sf::Clock animationClock;
    sf::Font* font;
    float duration;
};
