#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <random>

#include "game.hpp"

Snek::Snek(int_least64_t seed) {
    srand(seed);
    mWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake");
    mWindow.setFramerateLimit(FRAMERATE);
    Snake_Head.setSize({SNAKE_HEAD_WIDTH, SNAKE_HEAD_HEIGHT});
    InitSnakeHead(Snake_Head, _Snake_Head_S);
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

void Snek::KeyHandler(sf::Keyboard::Key key, SnakeHead_S &_Snake_Head_S) {
    switch(key) {

    case sf::Keyboard::Up:    _Snake_Head_S.velocity.Y = -GAME_RESOLUTION; _Snake_Head_S.velocity.X =   0; break;
    case sf::Keyboard::Down:  _Snake_Head_S.velocity.Y =  GAME_RESOLUTION; _Snake_Head_S.velocity.X =   0; break;

    case sf::Keyboard::Left:  _Snake_Head_S.velocity.X = -GAME_RESOLUTION; _Snake_Head_S.velocity.Y =   0; break;
    case sf::Keyboard::Right: _Snake_Head_S.velocity.X =  GAME_RESOLUTION; _Snake_Head_S.velocity.Y =   0; break; 
        
    default: break;
    }
}

INLINE void Snek::handleEvents() {
    while (mWindow.pollEvent(Event)) {
        switch (Event.type) {
        case sf::Event::Closed: mWindow.close(); goto exit_loop;
        case sf::Event::KeyPressed: KeyHandler(Event.key.code, _Snake_Head_S); goto exit_loop;
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
    if (T_Updater.getElapsedTime().asMilliseconds() >= 125.f) {
        UpdateSnek(_Snake_Head_S, SnakeTail);
        SetFruitInstance(Fruit::GetFruitInstance());
        P_FruitRect = *PTR_FruitInstance->GetFruitRect();
        T_Updater.restart();
    }
}

INLINE void Snek::UpdateSnek(SnakeHead_S &_Snake_Head_S, std::vector<sf::RectangleShape> &SnakeTail) {
    MoveSnake(Snake_Head, _Snake_Head_S);

    FruitCollision(SnakeTail);
    
    uint16_t i = 0;
    while (i < SnakeTail.size()) {
        // Terrible hack. Need fixing ASAP
        if (i >= 1) SnakeTail.at(i - 1).setPosition(SnakeTail.at(i).getPosition());
        SnakeTail.at(i).setPosition(PrevSnakeHeadPosition.first, PrevSnakeHeadPosition.second);
        i++;
    }
}

INLINE void Snek::FruitCollision(std::vector<sf::RectangleShape> &SnakeTail) {
    if (PTR_FruitInstance == nullptr) return;

    // For some reason without this check a segfault is guaranteed...
    // Why? Who even knows! Compiling with `-Og -g` never segfaulted here, even without this check
    if (_Snake_Head_S.velocity.X | _Snake_Head_S.velocity.Y) {
        if (_Snake_Head_S.position == PTR_FruitInstance->GetFruitPosition()) {
            GRDEBUG(printf("NOMNOMNOM\n"))
            PTR_FruitInstance->GenerateNewFruitPosition();
            
            AddSegmentToSnake(SnakeTail);
        }
    }
}

INLINE void Snek::InitSnakeHead(sf::RectangleShape &Snake_Head, SnakeHead_S &_Snake_Head_S) {
    Snake_Head.setPosition(GRID_MID_POS_X * GAME_RESOLUTION, GRID_MID_POS_Y * GAME_RESOLUTION);
    Snake_Head.setFillColor(WHITE);

    _Snake_Head_S.velocity.X = 0;
    _Snake_Head_S.velocity.Y = 0;
    _Snake_Head_S.position.first = GRID_MID_POS_X;
    _Snake_Head_S.position.second = GRID_MID_POS_Y;
}

INLINE void Snek::MoveSnake(sf::RectangleShape &Snake_Head, SnakeHead_S &_Snake_Head_S) {
    int8_t velX = _Snake_Head_S.velocity.X;
    int8_t velY = _Snake_Head_S.velocity.Y;

    PrevSnakeHeadPosition.first  = Snake_Head.getPosition().x;
    PrevSnakeHeadPosition.second = Snake_Head.getPosition().y;

    Snake_Head.move(velX, velY);

    _Snake_Head_S.position.first  += SIGN(velX);
    _Snake_Head_S.position.second += SIGN(velY);
}

INLINE void Snek::AddSegmentToSnake(std::vector<sf::RectangleShape> &SnakeTail) {
    sf::RectangleShape snakeSeg({SNAKE_SEGMENT_WIDTH, SNAKE_SEGMENT_HEIGHT});
    snakeSeg.setOrigin(SNAKE_SEGMENT_ANCHOR.first, SNAKE_SEGMENT_ANCHOR.second);
    snakeSeg.setPosition(PrevSnakeHeadPosition.first, PrevSnakeHeadPosition.second);
    snakeSeg.setFillColor(WHITE);

    SnakeTail.push_back(snakeSeg);
}

INLINE void Snek::DrawSnake() {
    mWindow.draw(Snake_Head);
    for (sf::RectangleShape snakeSeg : SnakeTail) {
        mWindow.draw(snakeSeg);
    }
}

INLINE void Snek::drawGame() {
    mWindow.clear();

    GRDEBUG(DrawDebugGrid(mWindow))

    // This alone made me suffer more than the whole project so far.
    Fruit::DrawFruit(mWindow, P_FruitRect);
    
    Snek::DrawSnake();

    mWindow.display();
}

#pragma region Fruit

Fruit* Fruit::S_PTR_FruitInstanceOrigin = nullptr;

INLINE sf::RectangleShape* Fruit::GetFruitRect() {
    return &F_FruitRect;
}

void Fruit::GenerateNewFruitPosition() {
GenRandPos:
    F_PosX = rand() % (GRID_X_RESOLUTION - 2) + 1;
    F_PosY = rand() % (GRID_Y_RESOLUTION - 2) + 1;

    // Failsafe so the fruit can't spawn in the dead middle
    if (F_PosX == GRID_MID_POS_X && F_PosY == GRID_MID_POS_Y) goto GenRandPos;

    auto GridPosFruit = GetGridPos(F_PosX, F_PosY);
    F_FruitRect.setPosition(GridPosFruit.first, GridPosFruit.second);
}

Fruit::Fruit() {
    GRDEBUG(printf("A fruit is initialised at "))
    GRDEBUG(printf("[X%d; Y%d]\n", F_PosX, F_PosY))

    F_FruitRect = sf::RectangleShape({FRUIT_WIDTH, FRUIT_HEIGHT});
    F_FruitRect.setFillColor(GREEN);
    
    F_FruitRect.setOrigin(FRUIT_ANCHOR.first, FRUIT_ANCHOR.second);

    GenerateNewFruitPosition();
}

Fruit::~Fruit() {

}

INLINE void Snek::SetFruitInstance(Fruit* fruit) {
    PTR_FruitInstance = fruit;
}

INLINE void Fruit::DrawFruit(sf::RenderWindow &mWindow, sf::RectangleShape F_FruitRect) {
    mWindow.draw(F_FruitRect);
}

Fruit* Fruit::GetFruitInstance() {
    S_PTR_FruitInstanceOrigin = (S_PTR_FruitInstanceOrigin == nullptr) ? new Fruit() : S_PTR_FruitInstanceOrigin;
    return S_PTR_FruitInstanceOrigin;
}

// Return the GRID POSITION X=[0;32], Y=[0;32]
INLINE std::pair<uint8_t, uint8_t> Fruit::GetFruitPosition() const {
    return std::make_pair<uint8_t, uint8_t>(F_PosX - 1, F_PosY - 1);
}

#pragma endregion