#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <random>

#include "game.hpp"

Snek::Snek(int_least64_t seed) {
    srand(seed);
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
        updateGame();
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

INLINE void Snek::updateGame() {
    if (tenTimer.getElapsedTime().asMilliseconds() >= 125.f) {
        UpdateSnek(_Snake_Head, snake);
        Fruit NewFruit = *Fruit::GetFruitInstance();
        tenTimer.restart();
    }
}

INLINE void Snek::UpdateSnek(SnakeHead &_Snake_Head, std::vector<sf::RectangleShape> &snake) {
    MoveSnake(snake.back(), _Snake_Head);

    if (snake.size() < 2) return;

    // Iterate through the snake segments, excluding the head
    for (size_t i = snake.size() - 2; i != 0; --i) {
        snake.at(i).setPosition(snake.at(i + 1).getPosition());
    }
}

INLINE void Snek::InitSnakeHead(sf::RectangleShape &Snake_Head) {
    auto MidPos = GetGridPos(15, 15);
    Snake_Head.setPosition(MidPos.first, MidPos.second);
    Snake_Head.setFillColor(WHITE);

    _Snake_Head.velocity.X = 0;
    _Snake_Head.velocity.Y = 0;
    _Snake_Head.position.first = GRID_X_RESOLUTION >> 1;
    _Snake_Head.position.second = GRID_X_RESOLUTION >> 1;

    snake.push_back(Snake_Head);
}

INLINE void Snek::MoveSnake(sf::RectangleShape &snake, SnakeHead &_Snake_Head) {
    snake.move(_Snake_Head.velocity.X, _Snake_Head.velocity.Y);
}

INLINE void Snek::AddSegmentToSnake(std::vector<sf::RectangleShape> &snake) {
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

    GRDEBUG(DrawDebugGrid(mWindow))

    Snek::DrawSnake();
    
    mWindow.display();
}

#pragma region Fruit

Fruit* Fruit::instance = nullptr;

Fruit::Fruit() {
    posX = rand() % GRID_X_RESOLUTION;
    posY = rand() % GRID_Y_RESOLUTION;

    GRDEBUG(printf("A fruit is initialised at "))
    GRDEBUG(printf("[X%d; Y%d]\n", posX, posY))
}

Fruit::~Fruit() {

}

Fruit* Fruit::GetFruitInstance() {
    instance = (!instance) ? new Fruit() : instance;
    return instance;
}

INLINE std::pair<uint8_t, uint8_t> Fruit::GetFruitPosition() const {
    return { posX, posY };
}

#pragma endregion