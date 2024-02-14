#include "Game.h"
#include <iostream>
#include <SFML/Window.hpp>

Game::Game()
    : window(sf::VideoMode(800, 600), "Brick Breaker", sf::Style::Titlebar | sf::Style::Close),
    gameState(GameState::NotStarted)
{
    initialize();
}

void Game::initialize() {
    paddle = Paddle(350, 550);
    ball = Ball(paddle.getPosition().x, paddle.getPosition().y - ball.getShape().getRadius() - paddle.getShape().getSize().y / 2.f);

    sf::Image windowIcon;
    if (windowIcon.loadFromFile("C:\\Users\\User\\source\\repos\\TiberiuManolescu\\Brick\\Brick\\profile.jpg")) {
        window.setIcon(windowIcon.getSize().x, windowIcon.getSize().y, windowIcon.getPixelsPtr());
    }

    if (!font.loadFromFile("arial.ttf")) {
        window.close();
        return;
    }
    if (!escKeyTexture.loadFromFile("C:\\Users\\User\\source\\repos\\TiberiuManolescu\\Brick\\Brick\\esc.jpg")) {
        std::cerr << "Failed to load ESC key image!" << std::endl;
        window.close();
        return;
    }
    escKeySprite.setTexture(escKeyTexture);

    if (!enterKeyTexture.loadFromFile("C:\\Users\\User\\source\\repos\\TiberiuManolescu\\Brick\\Brick\\enter.jpg")) {
        std::cerr << "Failed to load enter key image!" << std::endl;
        window.close();
        return;
    }
    enterKeySprite.setTexture(enterKeyTexture);

    if (!backgroundTexture.loadFromFile("C:\\Users\\User\\source\\repos\\TiberiuManolescu\\Brick\\Brick\\background.jpg")) {
        std::cerr << "Failed to load background texture!" << std::endl;
        window.close();
        return;
    }

    if (!brickTexture.loadFromFile("C:\\Users\\User\\source\\repos\\TiberiuManolescu\\Brick\\Brick\\brick.jpg")) {
        std::cerr << "Failed to load brick texture!" << std::endl;
        window.close();
        return;
    }
    if (!leftTexture.loadFromFile("C:\\Users\\User\\source\\repos\\TiberiuManolescu\\Brick\\Brick\\left.jpg")) {
        std::cerr << "Failed to load left image!" << std::endl;
        window.close();
        return;
    }
    leftSprite.setTexture(leftTexture);

    if (!rightTexture.loadFromFile("C:\\Users\\User\\source\\repos\\TiberiuManolescu\\Brick\\Brick\\right.jpg")) {
        std::cerr << "Failed to load right image!" << std::endl;
        window.close();
        return;
    }
    rightSprite.setTexture(rightTexture);

    brickBreakerText.setFont(font);
    brickBreakerText.setString("Brick Breaker");
    brickBreakerText.setCharacterSize(48);
    brickBreakerText.setFillColor(sf::Color::White);
    brickBreakerText.setOrigin(brickBreakerText.getGlobalBounds().width / 2, brickBreakerText.getGlobalBounds().height / 2);
    brickBreakerText.setPosition(window.getSize().x / 2.f, window.getSize().y / 4.f);

    score = 0;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setPosition(10, 570);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setString("Score: 0");

    elapsed = sf::seconds(0);
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setPosition(680, 570);
    timerText.setFillColor(sf::Color::White);
    timerText.setString("0s");

    setupButtons();

    for (int i = 0; i < NUM_BRICKS_X; ++i) {
        bricks.emplace_back(std::vector<Brick>());
        for (int j = 0; j < NUM_BRICKS_Y; ++j) {
            bricks[i].emplace_back(i * 50.0f, j * 20.0f, brickTexture);
        }
    }
}

void Game::setupButtons() {
    playButton.setSize({ 150.f, 50.f });
    playButton.setPosition((window.getSize().x - 150) / 2.0f + 34, window.getSize().y / 2.0f + 120);
    playButton.setFillColor(sf::Color::Red);
    playText = createText(playButton, "Play", font, 45);

    retryButton.setSize({ 150.f, 50.f });
    retryButton.setPosition(static_cast<float>(window.getSize().x - 150) / 2.0f, static_cast<float>(window.getSize().y / 2) + 50.0f);
    retryButton.setFillColor(sf::Color::Blue);
    retryText = createText(retryButton, "Retry", font, 45);
}

sf::Text Game::createText(const sf::RectangleShape& rect, const sf::String& string, const sf::Font& font, unsigned int size) {
    sf::Text text;
    text.setFont(font);
    text.setString(string);
    text.setCharacterSize(size);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(rect.getPosition().x + rect.getSize().x / 2.0f, rect.getPosition().y + rect.getSize().y / 2.0f));

    return text;
}

void Game::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (retryButton.getGlobalBounds().contains(mousePos) && gameState == GameState::GameOver) {
                restart();
            }

            if (gameState == GameState::NotStarted) {
                if (playButton.getGlobalBounds().contains(mousePos)) {
                    gameState = GameState::Tutorial;
                    tutorialPage = 0;
                    currentTutorialText = tutorialTexts[tutorialPage];
                    currentTutorialTextIndex = 0;
                    typingAnimationTime = 0.0f;
                }
                if (levelButton.getGlobalBounds().contains(mousePos)) {
                    window.close();
                }
            }
        }

        if (gameState == GameState::Running) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                paddle.moveLeft(clock.getElapsedTime().asSeconds());
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                paddle.moveRight(clock.getElapsedTime().asSeconds());
            }
        }

        if (gameState == GameState::Tutorial && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            tutorialPage++;
            if (tutorialPage >= tutorialTexts.size()) {
                gameState = GameState::Running; 
            }
            else {
                currentTutorialText = tutorialTexts[tutorialPage];
                currentTutorialTextIndex = 0;
                typingAnimationTime = 0.0f;
            }
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            if (gameState == GameState::Running) {
                gameState = GameState::Paused; 
            }
            else if (gameState == GameState::Paused) {
                gameState = GameState::Running;
            }
        }
    }
}

void Game::update(float deltaTime) {
    paddle.move(deltaTime);
    ball.update(deltaTime);
    handleCollision();

    if (gameState == GameState::Paused || gameState == GameState::GameOver) {
        return; 
    }

    for (auto& animation : animations) {
        animation.update(deltaTime);
    }
    bool allBricksDestroyed = true;
    for (const auto& row : bricks) {
        for (const auto& brick : row) {
            if (!brick.isDestroyed()) {
                allBricksDestroyed = false;
                break;
            }
        }
        if (!allBricksDestroyed) break;
    }

    if (allBricksDestroyed) {
        gameState = GameState::GameOver;
        win = true; 
    }
    animations.erase(std::remove_if(animations.begin(), animations.end(),
        [](const GameAnimation& animation) {
            return animation.isFinished();
        }), animations.end());

    if (playButtonScalingUp) {
        playButtonScale += playButtonScaleSpeed * deltaTime;
        if (playButtonScale > 1.2f) { 
            playButtonScalingUp = false;
        }
    }
    else {
        playButtonScale -= playButtonScaleSpeed * deltaTime;
        if (playButtonScale < 0.8f) { 
            playButtonScalingUp = true;
        }
    }
    playButton.setScale(playButtonScale, playButtonScale);
    playText.setScale(playButtonScale, playButtonScale); 
 
    if (gameState == GameState::Tutorial) {
        typingAnimationTime += deltaTime;
        if (typingAnimationTime >= typingSpeed && currentTutorialTextIndex < currentTutorialText.length()) {
            currentTutorialTextIndex++;
            typingAnimationTime = 0.0f;
        }
    }
}

void Game::draw() {
    window.clear(sf::Color::Black);

    sf::Text tutorialTitle, instructionText;
    sf::FloatRect titleRect;
    
    sf::Sprite enterKeySprite; 
    enterKeySprite.setTexture(enterKeyTexture);

    if (gameState == GameState::Tutorial) {
        switch (tutorialPage) {
        case 0:
            instructionText.setFont(font);
            instructionText.setString("To continue press Enter");
            instructionText.setCharacterSize(24);
            instructionText.setFillColor(sf::Color::White);
            instructionText.setPosition(window.getSize().x / 2.0f - instructionText.getLocalBounds().width / 2.0f, tutorialTitle.getPosition().y + 120.f);
            window.draw(instructionText);

            enterKeySprite.setScale(0.5f, 0.5f); 

           
            enterKeySprite.setPosition(
                window.getSize().x / 2.f - enterKeySprite.getGlobalBounds().width / 2.f,
                window.getSize().y - enterKeySprite.getGlobalBounds().height - 120.f);
            window.draw(enterKeySprite);
            break;
        case 1:
           
            tutorialTitle.setFont(font);
            tutorialTitle.setString("Tutorial");
            tutorialTitle.setCharacterSize(48);
            tutorialTitle.setFillColor(sf::Color::White);
            titleRect = tutorialTitle.getLocalBounds();
            tutorialTitle.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
            tutorialTitle.setPosition(window.getSize().x / 2.0f, 30.f);
            window.draw(tutorialTitle);

            
            instructionText.setFont(font);
            instructionText.setString("To control Paddle press Left and Right");
            instructionText.setCharacterSize(24);
            instructionText.setFillColor(sf::Color::White);
            instructionText.setPosition(window.getSize().x / 2.0f - instructionText.getLocalBounds().width / 2.0f, tutorialTitle.getPosition().y + 100.f);
            window.draw(instructionText);

            leftSprite.setScale(0.45f, 0.45f);
            leftSprite.setPosition(window.getSize().x / 2.f - leftSprite.getGlobalBounds().width - 10, window.getSize().y - leftSprite.getGlobalBounds().height - 120);
            window.draw(leftSprite);

            rightSprite.setScale(0.45f, 0.45f);  
            rightSprite.setPosition(window.getSize().x / 2.f + 10, window.getSize().y - rightSprite.getGlobalBounds().height - 120);
            window.draw(rightSprite);

            break;
        case 2:
            escKeySprite.setScale(0.5f, 0.5f);
            escKeySprite.setPosition(
                window.getSize().x / 2.f - escKeySprite.getGlobalBounds().width / 2.f,
                window.getSize().y / 2.f - escKeySprite.getGlobalBounds().height / 2.f - 10.f 
            );
            window.draw(escKeySprite);

            
            instructionText.setFont(font);
            instructionText.setString("To pause press ESC");
            instructionText.setCharacterSize(24);
            instructionText.setFillColor(sf::Color::White);
            instructionText.setPosition(window.getSize().x / 2.0f - instructionText.getLocalBounds().width / 2.0f, tutorialTitle.getPosition().y + 100.f);
            window.draw(instructionText);
            break;
        default:
            window.clear(sf::Color::Black); 
            break;
        }
            
            sf::Text tutorialTitle;
            tutorialTitle.setFont(font);
            tutorialTitle.setString("Tutorial");
            tutorialTitle.setCharacterSize(48);
            tutorialTitle.setFillColor(sf::Color::White);
            sf::FloatRect textRect = tutorialTitle.getLocalBounds();
            tutorialTitle.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            tutorialTitle.setPosition(window.getSize().x / 2.0f, 30.f);
            window.draw(tutorialTitle);
    }
    if (gameState == GameState::Paused) {
        sf::Text pauseText;
        pauseText.setFont(font);
        pauseText.setString("Pause");
        pauseText.setCharacterSize(48);
        pauseText.setFillColor(sf::Color::White);

        sf::FloatRect pauseTextRect = pauseText.getLocalBounds();
        pauseText.setOrigin(pauseTextRect.left + pauseTextRect.width / 2.0f,
            pauseTextRect.top + pauseTextRect.height / 2.0f);
        pauseText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

        window.draw(pauseText);
    }
    if (gameState == GameState::NotStarted) {
        window.draw(brickBreakerText);
    }
    if (gameState == GameState::GameOver) {
        sf::Text finalScoreText;
    finalScoreText.setFont(font);
    finalScoreText.setString("Score: " + std::to_string(score));
    finalScoreText.setCharacterSize(24);
    finalScoreText.setFillColor(sf::Color::White);
    finalScoreText.setPosition(10, window.getSize().y - finalScoreText.getLocalBounds().height - 10); 
    window.draw(finalScoreText);

    sf::Text finalTimeText;
    finalTimeText.setFont(font);
    finalTimeText.setString("Time: " + std::to_string(static_cast<int>(elapsed.asSeconds())) + "s");
    finalTimeText.setCharacterSize(24);
    finalTimeText.setFillColor(sf::Color::White);
    finalTimeText.setPosition(window.getSize().x - finalTimeText.getLocalBounds().width - 10,
                              window.getSize().y - finalTimeText.getLocalBounds().height - 10); 
    window.draw(finalTimeText);

        sf::Text endGameText;
        endGameText.setFont(font);
        if (win) {
            endGameText.setString("WIN");
            endGameText.setFillColor(sf::Color::Green);
        }
        else {
            endGameText.setString("GAME OVER");
            endGameText.setFillColor(sf::Color::Red);
        }
        endGameText.setCharacterSize(40);
        sf::FloatRect endGameTextRect = endGameText.getLocalBounds();
        endGameText.setOrigin(endGameTextRect.left + endGameTextRect.width / 2.0f,
            endGameTextRect.top + endGameTextRect.height / 2.0f);
        endGameText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

        window.draw(endGameText);
        window.draw(retryButton);
        window.draw(retryText);
    }
    if (gameState == GameState::Running) {
 
        window.draw(paddle.getShape());
        window.draw(ball.getShape());

        for (auto& row : bricks) {
            for (auto& brick : row) {
                if (!brick.isDestroyed()) {
                    brick.draw(window);
                }
            }
        }

        for (auto& animation : animations) {
            animation.draw(window);
        }
        window.draw(scoreText);
        window.draw(timerText);
    }
    else if (gameState == GameState::GameOver) {
  
        window.draw(gameOverText);
        window.draw(retryButton);
        window.draw(retryText);
    }

    else if (gameState == GameState::NotStarted) {
  
        sf::Sprite backgroundSprite(backgroundTexture);
        window.draw(backgroundSprite);
        window.draw(playButton);
        window.draw(playText);
    }

    window.display();
}
void Game::run() {

        while (window.isOpen()) {
            handleInput();
            float deltaTime = clock.restart().asSeconds();
            if (gameState == GameState::Running) {
                update(deltaTime);
                elapsed += sf::seconds(deltaTime);
                timerText.setString(std::to_string(static_cast<int>(elapsed.asSeconds())) + "s");
            }
            draw();
        }
    }

void Game::handleCollision() {
    for (auto& row : bricks) {
        for (auto& brick : row) {
            if (!brick.isDestroyed() && intersects(ball.getGlobalBounds(), brick.getGlobalBounds())) {
                ball.reverseY();
                brick.destroy();
                incrementScore();
                animations.emplace_back(window, "+1", 0.5f, &font);
            }
        }
    }

    if (ball.getGlobalBounds().top + ball.getGlobalBounds().height > window.getSize().y) {
        gameState = GameState::GameOver;
        ball.resetDirection();
    }

    if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
        ball.reverseY();
    }
}

bool Game::intersects(const sf::FloatRect& a, const sf::FloatRect& b) {
    return a.intersects(b);
}

void Game::incrementScore() {
    score++;
    scoreText.setString("Score: " + std::to_string(score));
}

void Game::restart() {
    initialize();
    gameState = GameState::NotStarted;
    window.setTitle("Brick Breaker");
}
