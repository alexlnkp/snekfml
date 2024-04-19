
extern "C" {
    #include <unistd.h>
}

#include "game.hpp"

// A hack so the compiler trusts me for once
#define INLINE __attribute__((always_inline)) inline

#define PRINT(ARG) write(STDOUT_FILENO, ARG, sizeof(ARG))

struct {
    sf::Color White = {255, 255, 255};
    sf::Color Green = {0, 255, 0};
} Colors;

#define GREEN Colors.Green
#define WHITE Colors.White

// Here's how the grid should look like at the current params (each [ ] is 20x20 px in size) 1:8 scale
// [ ] [ ] [ ] [ ]
// [ ] [ ] [ ] [ ]
// [ ] [ ] [ ] [ ]
// [ ] [ ] [ ] [ ]

#define SNAKE_HEAD_WIDTH  GRID_X_RESOLUTION
#define SNAKE_HEAD_HEIGHT GRID_Y_RESOLUTION

constexpr uint8_t SNAKE_SEGMENT_WIDTH  = (SNAKE_HEAD_WIDTH - 4);
constexpr uint8_t SNAKE_SEGMENT_HEIGHT = (SNAKE_HEAD_WIDTH - 4);

constexpr uint8_t FRAMERATE = 60;

Snek::Snek() {
    mWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake");
    mWindow.setFramerateLimit(FRAMERATE);

    InitSnakeHead();
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

INLINE void Snek::handleEvents() {
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


// Retrieve the real position (in pixels) in the window based on the x,y position on the grid
// For example, with x,y like this (x=2, y=2) (grid scale 1:8, `[ ]` represents a single segment of a grid which is set by GAME_RESOLUTION and is 20 by default):
//
// ```
// [ ] [ ] [ ] [ ]
// [ ] [ ] [ ] [ ]
// [ ] [ ] [P] [ ]
// [ ] [ ] [ ] [ ]
// ```
//
// P being the position on the grid we want to grab real position of.
// We'll get {x * 20, y * 20} i.e. {60, 60}
INLINE std::pair<uint16_t, uint16_t> GetGridPos(uint8_t x, uint8_t y) {
    return {x * GAME_RESOLUTION, y * GAME_RESOLUTION};
}

INLINE void Snek::updateGame() {

}

INLINE void Snek::InitSnakeHead() {
    sf::RectangleShape SnakeHead({SNAKE_HEAD_WIDTH, SNAKE_HEAD_HEIGHT});
    auto MidPos = GetGridPos(15, 15);
    SnakeHead.setPosition(MidPos.first, MidPos.second);
    SnakeHead.setFillColor(WHITE);

    snake.push_back(SnakeHead);
}

INLINE void Snek::AddSegmentToSnake() {
    sf::RectangleShape snakeSeg({SNAKE_SEGMENT_WIDTH, SNAKE_SEGMENT_HEIGHT});
    snakeSeg.setOrigin({SNAKE_SEGMENT_WIDTH + 4, SNAKE_SEGMENT_HEIGHT + 4});
    snakeSeg.setFillColor(WHITE);

    snake.push_back(snakeSeg);
}

INLINE void Snek::DrawSnake() {
    for (sf::RectangleShape snakeSeg : snake) {
        mWindow.draw(snakeSeg);
    }
}

INLINE void Snek::drawGame() {
    
    mWindow.clear();

#ifdef DEBUG
    DrawDebugGrid(mWindow);
#endif

    // Draw all here

    Snek::DrawSnake();
    
    mWindow.display();
}