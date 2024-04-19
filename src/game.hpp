#pragma once

#include <SFML/Graphics.hpp>

class Snek {
private:
    sf::RenderWindow mWindow;
    sf::Event Event;

public:
    Snek();
    ~Snek();

    int mainLoop();

private:
    void handleEvents();
    void updateGame();
    void drawGame();

private:
    void DrawSnake();
};