
#include "game.hpp"

struct {
    sf::Color White = {255, 255, 255};
    sf::Color Green = {0, 255, 0};
} Colors;

#define GREEN Colors.Green
#define WHITE Colors.White

#define GAME_RESOLUTION 20 // 20 pixels per segment. all of the elements should be drawn on the grid of 20x20 pixels

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define SNAKE_SEGMENT_WIDTH 20
#define SNAKE_SEGMENT_HEIGHT 20

Snek::Snek() {
    mWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake");
}

Snek::~Snek() {

}

int Snek::mainLoop() {
    while (mWindow.isOpen()) {
        handleEvents();
        updateGame();
        drawGame();
    }

    return 0;
}

__attribute__((always_inline))
inline void Snek::handleEvents() {
    while (mWindow.pollEvent(Event)) {
        switch (Event.type) {
        case sf::Event::Closed:
            mWindow.close(); goto exit_loop;
        default:
            goto exit_loop; // could just be a break; instead, not sure
        }
    }
exit_loop:;
}

__attribute__((always_inline))
inline void Snek::updateGame() {

}

__attribute__((always_inline))
inline void Snek::DrawSnake() {
    sf::RectangleShape snakeSeg({SNAKE_SEGMENT_WIDTH, SNAKE_SEGMENT_HEIGHT});
    snakeSeg.setOrigin({SNAKE_SEGMENT_WIDTH >> 1, SNAKE_SEGMENT_HEIGHT >> 1});
    snakeSeg.setFillColor(WHITE);
    snakeSeg.setPosition({WINDOW_WIDTH >> 1, WINDOW_HEIGHT >> 1});
    mWindow.draw(snakeSeg);
}

__attribute__((always_inline))
inline void Snek::drawGame() {
    
    mWindow.clear();

    // Draw all here

    Snek::DrawSnake();
    

    mWindow.display();
}
