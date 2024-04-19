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
    inline void handleEvents();
    inline void updateGame();
    inline void drawGame();

private:
    inline void DrawSnake();
};