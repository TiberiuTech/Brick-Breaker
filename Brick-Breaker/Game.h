#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "GameAnimation.h"
#include <vector>

class Game {
public:
    Game();
    void run();

private:
    enum class GameState { NotStarted, Tutorial, Running, Paused, GameOver };
    int lives;
    sf::Text livesText;

    sf::Image icon;

    bool win;

    std::vector<std::string> tutorialTexts = { "Tutorial Page 1", "Tutorial Page 2", "Tutorial Page 3" };
    int tutorialPage = 0;
    std::string currentTutorialText;
    unsigned int currentTutorialTextIndex = 0;
    const float typingSpeed = 0.1f; 
    float typingAnimationTime = 0.0f;

    float playButtonScale = 1.0f;
    float playButtonScaleSpeed = 0.5f; 
    bool playButtonScalingUp = true; 

    sf::Vector2f brickBreakerTextInitialPosition;
    float animationTime = 0.0f;
    const float animationAmplitude = 5.0f;
    const float animationSpeed = 2.0f;

    sf::Text tutorialTitleText;
    std::size_t tutorialTitleTextIndex;
    float tutorialTitleAnimationTime;
    bool isTutorialTitleDisplayed;

    void initialize();
    void handleInput();
    void update(float deltaTime);
    void draw();
    void handleCollision();
    void restart();
    void incrementScore();
    bool intersects(const sf::FloatRect& a, const sf::FloatRect& b);
    void setupButtons();
    sf::Text createText(const sf::RectangleShape& rect, const sf::String& string, const sf::Font& font, unsigned int size);

    sf::Texture leftTexture, rightTexture;
    sf::Sprite leftSprite, rightSprite;
    sf::Texture enterKeyTexture;
    sf::Sprite enterKeySprite;
    sf::Texture escKeyTexture;
    sf::Sprite escKeySprite;
    sf::Text pauseInstructionText;
    sf::FloatRect pauseTextRect = pauseInstructionText.getLocalBounds();
    sf::FloatRect pauseTextBounds = pauseInstructionText.getLocalBounds();
    sf::Texture brickTexture;
    sf::Text brickBreakerText;
    sf::Texture backgroundTexture;

    sf::RenderWindow window;
    GameState gameState;

    Paddle paddle;
    Ball ball;
    int score;
    sf::Font font;
    sf::Text scoreText;
    sf::Time elapsed;
    sf::Text timerText;
    sf::RectangleShape retryButton;
    sf::Text retryText;
    sf::Text gameOverText;

    sf::RectangleShape playButton;
    sf::Text playText;
    sf::RectangleShape levelButton;
    sf::Text levelText;

    const int NUM_BRICKS_X = 16;
    const int NUM_BRICKS_Y = 5;
    std::vector<std::vector<Brick>> bricks;
    std::vector<GameAnimation> animations;
    sf::Clock clock;
};
