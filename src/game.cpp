
extern "C" {
    #include <unistd.h>
}

#include "game.hpp"

#include <SFML/System.hpp>
#include <random>

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
    sf::RectangleShape Snake_Head({SNAKE_HEAD_WIDTH, SNAKE_HEAD_HEIGHT});
    InitSnakeHead(Snake_Head);
}

Snek::~Snek() {

}

int Snek::mainLoop() {
    while (mWindow.isOpen()) {
        currentTime = deltaClock.getElapsedTime();
        deltaTime = ( currentTime - prevDeltaTime );
        prevDeltaTime = currentTime;

        handleEvents();
        updateGame(_Snake_Head, snake);
        drawGame();
    }

    return 0;
}

void Snek::KeyHandler(sf::Keyboard::Key key, SnakeHead &_Snake_Head) {
    switch(key) {

    case sf::Keyboard::Up:    _Snake_Head.velocity.Y = -GAME_RESOLUTION; _Snake_Head.velocity.X =   0; break;
    case sf::Keyboard::Down:  _Snake_Head.velocity.Y =  GAME_RESOLUTION; _Snake_Head.velocity.X =   0; break;

    case sf::Keyboard::Left:  _Snake_Head.velocity.X = -GAME_RESOLUTION; _Snake_Head.velocity.Y =   0; break;
    case sf::Keyboard::Right: _Snake_Head.velocity.X =  GAME_RESOLUTION; _Snake_Head.velocity.Y =   0; break; 
        
    default: break;
    }
}

INLINE void Snek::handleEvents() {
    while (mWindow.pollEvent(Event)) {
        switch (Event.type) {
        case sf::Event::Closed: mWindow.close(); goto exit_loop;
        case sf::Event::KeyPressed: KeyHandler(Event.key.code, _Snake_Head); goto exit_loop;
        default: goto exit_loop; // could just be a break; instead, not sure
        }
    }
exit_loop:;
}


// Retrieve the real position (in pixels) in the window based on the x,y position on the grid
// For example, with (x=2, y=2) (grid scale 1:8, `[ ]` represents a single segment of a grid which is set by GAME_RESOLUTION and is 20 by default):
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

INLINE void Snek::updateGame(SnakeHead &_Snake_Head, std::vector<sf::RectangleShape> &snake) {
    // Update the snake's head position

    if (tenTimer.getElapsedTime().asMilliseconds() >= 500.f) {
        sf::RectangleShape& head = snake.at(snake.size() - 1);
        head.move(_Snake_Head.velocity.X, _Snake_Head.velocity.Y);
        tenTimer.restart();
    }

    // // Iterate through the snake segments, excluding the head
    // for (size_t i = snake.size() - 1; i != 0; --i) {
    //     sf::RectangleShape& currentSegment = snake.at(i);
    //     sf::RectangleShape& previousSegment = snake.at(i + 1);

    //     currentSegment.setPosition(previousSegment.getPosition());
    // }

    // // Update the snake's head position in the snake segments list
    // snake.at(snake.size() - 1).setPosition(head.getPosition().x * GAME_RESOLUTION, head.getPosition().y * GAME_RESOLUTION);

    
}

INLINE void Snek::InitSnakeHead(sf::RectangleShape &Snake_Head) {
    auto MidPos = GetGridPos(15, 15);
    Snake_Head.setPosition(MidPos.first, MidPos.second);
    Snake_Head.setFillColor(WHITE);

    _Snake_Head.velocity.X = 0;
    _Snake_Head.velocity.Y = 0;
    _Snake_Head.position.first = 15;
    _Snake_Head.position.second = 15;

    snake.push_back(Snake_Head);
}

INLINE void Snek::MoveSnake(std::vector<sf::RectangleShape> &snake, uint16_t X, uint16_t Y) {
    
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

    Snek::DrawSnake();
    
    mWindow.display();
}