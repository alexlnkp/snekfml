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
    return std::make_pair<uint16_t, uint16_t>(x * GAME_RESOLUTION, y * GAME_RESOLUTION);
}

INLINE void Snek::updateGame() {
    if (tenTimer.getElapsedTime().asMilliseconds() >= 125.f) {
        UpdateSnek(_Snake_Head, snake);
        SetFruitInstance(Fruit::GetFruitInstance());
        _fruit_rect = *fruit_instance->GetFruitRect();
        tenTimer.restart();
    }
}

INLINE void Snek::UpdateSnek(SnakeHead &_Snake_Head, std::vector<sf::RectangleShape> &snake) {
    MoveSnake(snake.back(), _Snake_Head);

    FruitCollision();

    if (snake.size() < 2) return;

    // Iterate through the snake segments, excluding the head
    for (size_t i = snake.size() - 2; i != 0; --i) {
        snake.at(i).setPosition(snake.at(i + 1).getPosition());
    }
}

INLINE void Snek::FruitCollision() const {
    if (fruit_instance == nullptr) return;

    if (_Snake_Head.position == fruit_instance->GetFruitPosition()) {
        printf("NOMNOMNOM\n");
    }
}

INLINE void Snek::InitSnakeHead(sf::RectangleShape &Snake_Head) {
    auto MidPos = GetGridPos(15, 15);
    Snake_Head.setPosition(MidPos.first, MidPos.second);
    Snake_Head.setFillColor(WHITE);

    _Snake_Head.velocity.X = 0;
    _Snake_Head.velocity.Y = 0;
    _Snake_Head.position.first = (GRID_X_RESOLUTION >> 1) - 1;
    _Snake_Head.position.second = (GRID_Y_RESOLUTION >> 1) - 1;

    snake.push_back(Snake_Head);
}

INLINE void Snek::MoveSnake(sf::RectangleShape &snake, SnakeHead &_Snake_Head) {
    int8_t velX = _Snake_Head.velocity.X;
    int8_t velY = _Snake_Head.velocity.Y;
    snake.move(velX, velY);
    _Snake_Head.position.first  += SIGN(velX);
    _Snake_Head.position.second += SIGN(velY);
}

INLINE void Snek::AddSegmentToSnake(std::vector<sf::RectangleShape> &snake) {
    sf::RectangleShape snakeSeg({SNAKE_SEGMENT_WIDTH, SNAKE_SEGMENT_HEIGHT});
    snakeSeg.setOrigin(SNAKE_SEGMENT_ANCHOR.first, SNAKE_SEGMENT_ANCHOR.second);
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

    // This alone made me suffer more than the whole project so far.
    Fruit::DrawFruit(mWindow, _fruit_rect);
    
    Snek::DrawSnake();

    mWindow.display();
}

#pragma region Fruit

Fruit* Fruit::instance = nullptr;

INLINE sf::RectangleShape* Fruit::GetFruitRect() {
    return &fruit_rect;
}

Fruit::Fruit() {
    posX = rand() % (GRID_X_RESOLUTION - 1);
    posY = rand() % (GRID_Y_RESOLUTION - 1);
    
    GRDEBUG(printf("A fruit is initialised at "))
    GRDEBUG(printf("[X%d; Y%d]\n", posX, posY))

    fruit_rect = sf::RectangleShape({FRUIT_WIDTH, FRUIT_HEIGHT});
    fruit_rect.setFillColor(GREEN);
    auto GridPosFruit = GetGridPos(posX, posY);
    fruit_rect.setPosition(GridPosFruit.first, GridPosFruit.second);

    fruit_rect.setOrigin(FRUIT_ANCHOR.first, FRUIT_ANCHOR.second);

}

Fruit::~Fruit() {

}

INLINE void Snek::SetFruitInstance(Fruit* fruit) {
    fruit_instance = fruit;
}

INLINE void Fruit::DrawFruit(sf::RenderWindow &mWindow, sf::RectangleShape fruit_rect) {
    mWindow.draw(fruit_rect);
}

Fruit* Fruit::GetFruitInstance() {
    instance = (instance == nullptr) ? new Fruit() : instance;
    return instance;
}

// Return the GRID POSITION X=[0;32], Y=[0;32]
INLINE std::pair<uint8_t, uint8_t> Fruit::GetFruitPosition() const {
    return std::make_pair<uint8_t, uint8_t>(posX - 1, posY - 1);
}

#pragma endregion