#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <random>
#include <iomanip>
#include "game.hpp"

Snek::Snek(int_least64_t seed) noexcept {
    srand(seed);
    mWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake");
    mWindow.setFramerateLimit(FRAMERATE);
    Snake_Head.setSize({SNAKE_HEAD_WIDTH, SNAKE_HEAD_HEIGHT});
    InitSnakeHead(Snake_Head, _Snake_Head_S);

    // God bless public domain fonts
    if (!MyFont.loadFromFile(FONT_FILE_PATH)) {
        exit(-1);
        return;
    }
    
    Text.setFont(MyFont);
    Text.setCharacterSize(FONT_SIZE);
    Text.setLetterSpacing(2.f);
    Text.setPosition(5, WINDOW_HEIGHT - FONT_SIZE - 5);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(7) << Score;
    Text.setString(ss.str());

}

Snek::~Snek() {

}

int Snek::mainLoop() noexcept {
    while (mWindow.isOpen()) {
        handleEvents();
        updateGame();
        drawGame();
    }

    return 0;
}

inline FASTINL void Snek::KeyHandler(sf::Keyboard::Key key, SnakeHead_S &_Snake_Head_S) noexcept {
    switch(key) {

    case sf::Keyboard::Up:    _Snake_Head_S.velocity.Y = -GAME_RESOLUTION; _Snake_Head_S.velocity.X =   0; break;
    case sf::Keyboard::Down:  _Snake_Head_S.velocity.Y =  GAME_RESOLUTION; _Snake_Head_S.velocity.X =   0; break;

    case sf::Keyboard::Left:  _Snake_Head_S.velocity.X = -GAME_RESOLUTION; _Snake_Head_S.velocity.Y =   0; break;
    case sf::Keyboard::Right: _Snake_Head_S.velocity.X =  GAME_RESOLUTION; _Snake_Head_S.velocity.Y =   0; break; 
        
    default: break;
    }
}

inline FASTINL void Snek::UpdateScore() noexcept {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(7) << Score;
    Text.setString(ss.str());
}

inline FASTINL void Snek::handleEvents() noexcept {
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
inline FASTINL std::pair<uint16_t, uint16_t> GetGridPos(uint8_t x, uint8_t y) noexcept {
    return std::make_pair<uint16_t, uint16_t>(x * GAME_RESOLUTION, y * GAME_RESOLUTION);
}

inline FASTINL void Snek::updateGame() noexcept {
    if (T_Updater.getElapsedTime().asMilliseconds() >= 125.f) {
        UpdateSnek(_Snake_Head_S, SnakeTail);
        SetFruitInstance(Fruit::GetFruitInstance());
        P_FruitRect = *PTR_FruitInstance->GetFruitRect();
        T_Updater.restart();
    }
}

/**
 * Updates the snake's position and handles collision with the fruit.
 *
 * @param _Snake_Head_S the snake's head struct
 * @param SnakeTail the vector of snake segments
 *
 * @throws None
 */
inline FASTINL void Snek::UpdateSnek(SnakeHead_S &_Snake_Head_S, std::vector<sf::RectangleShape> &SnakeTail) noexcept {
    if ((CurrentGameState == GameOver) | (CurrentGameState == Pause)) return;

    MoveSnake(Snake_Head, _Snake_Head_S);
    TailCollision(Snake_Head, SnakeTail);

    FruitCollision(SnakeTail, Score);
    
    for (uint16_t i = 1; i < SnakeTail.size(); i++) {
        SnakeTail.at(i - 1).setPosition(SnakeTail.at(i).getPosition());
    }
    if (!SnakeTail.empty())
        SnakeTail.back().setPosition(PrevSnakeHeadPosition.first, PrevSnakeHeadPosition.second);
}

/**
 * Checks if the snake's head collides with the fruit.
 * If a collision is detected - the fruit's position is updated, a new segment is added to the snake; 
 * And a debug message is printed.
 *
 * @param SnakeTail a reference to a vector of sf::RectangleShape objects representing the snake's tail
 *
 * @throws None
 */
inline FASTINL void Snek::FruitCollision(std::vector<sf::RectangleShape> &SnakeTail, uint_least64_t &Score) noexcept {
    if (PTR_FruitInstance == nullptr) return;

    // For some reason without this check a segfault is guaranteed...
    // Why? Who even knows! Compiling with `-Og -g` never segfaulted here, even without this check
    if (_Snake_Head_S.velocity.X | _Snake_Head_S.velocity.Y) {
        if (!(_Snake_Head_S.position.first ^ PTR_FruitInstance->GetFruitPosition().first) & !(_Snake_Head_S.position.second ^ PTR_FruitInstance->GetFruitPosition().second)) {
            GRDEBUG(printf("NOMNOMNOM\n"))
            Score += FRUIT_POINTS_WORTH;
            UpdateScore();
            PTR_FruitInstance->GenerateNewFruitPosition();
            
            AddSegmentToSnake(SnakeTail);
        }
    }
}

inline FASTINL void Snek::TailCollision(sf::RectangleShape &Snake_Head, std::vector<sf::RectangleShape> &SnakeTail) noexcept {
    for (sf::RectangleShape &_Snake_Tail_Segment : SnakeTail) {
        if (Snake_Head.getPosition() == _Snake_Tail_Segment.getPosition()) {
            GRDEBUG(printf("GAME OVER\n"))
            CurrentGameState = GameOver;
        }
    }
}

inline FASTINL void Snek::InitSnakeHead(sf::RectangleShape &Snake_Head, SnakeHead_S &_Snake_Head_S) noexcept {
    Snake_Head.setPosition(GRID_MID_POS_X * GAME_RESOLUTION, GRID_MID_POS_Y * GAME_RESOLUTION);
    Snake_Head.setFillColor(WHITE);

    _Snake_Head_S.velocity.X = _Snake_Head_S.velocity.Y = 0;

    _Snake_Head_S.position.first = GRID_MID_POS_X;
    _Snake_Head_S.position.second = GRID_MID_POS_Y;
}

/**
 * Moves the snake's head by the specified delta.
 *
 * @param Snake_Head the sf::RectangleShape object of the snake's head
 * @param _Snake_Head_S the snake's head structure that holds the snake's velocity
 *
 * @throws None
 */
inline FASTINL void Snek::MoveSnake(sf::RectangleShape &Snake_Head, SnakeHead_S &_Snake_Head_S) noexcept {
    int8_t velX = _Snake_Head_S.velocity.X;
    int8_t velY = _Snake_Head_S.velocity.Y;

    PrevSnakeHeadPosition.first  = Snake_Head.getPosition().x;
    PrevSnakeHeadPosition.second = Snake_Head.getPosition().y;

    Snake_Head.move(velX, velY);

    _Snake_Head_S.position.first  += SIGN(velX);
    _Snake_Head_S.position.second += SIGN(velY);
}

/**
 * Adds a new segment to the snake's tail.
 *
 * @param SnakeTail The vector of `sf::RectangleShape` objects representing the snake's tail.
 * 
 * @throws None
 */
inline FASTINL void Snek::AddSegmentToSnake(std::vector<sf::RectangleShape> &SnakeTail) const noexcept {
    sf::RectangleShape snakeSeg({SNAKE_SEGMENT_WIDTH, SNAKE_SEGMENT_HEIGHT});
    snakeSeg.setOrigin(SNAKE_SEGMENT_ANCHOR.first, SNAKE_SEGMENT_ANCHOR.second);
    snakeSeg.setPosition(PrevSnakeHeadPosition.first, PrevSnakeHeadPosition.second);
    snakeSeg.setFillColor(WHITE);

    SnakeTail.push_back(snakeSeg);
}

inline FASTINL void Snek::DrawSnake(sf::RenderWindow &mWindow) const noexcept {
    mWindow.draw(Snake_Head);
    for (sf::RectangleShape snakeSeg : SnakeTail) {
        mWindow.draw(snakeSeg);
    }
}

inline FASTINL void Snek::drawGame() noexcept {
    mWindow.clear();

    GRDEBUG(DrawDebugGrid(mWindow))

    // This alone made me suffer more than the whole project so far.
    Fruit::DrawFruit(mWindow, P_FruitRect);
    
    Snek::DrawSnake(mWindow);

    mWindow.draw(Text);

    mWindow.display();
}

#pragma region Fruit

Fruit* Fruit::S_PTR_FruitInstanceOrigin = nullptr;

inline FASTINL sf::RectangleShape* Fruit::GetFruitRect() noexcept {
    return &F_FruitRect;
}

/**
 * Generates a new position for the fruit.
 *
 * @throws None
 */
inline FASTINL void Fruit::GenerateNewFruitPosition() noexcept {
GenRandPos:
    F_PosX = rand() % (GRID_X_RESOLUTION - 2) + 1;
    F_PosY = rand() % (GRID_Y_RESOLUTION - 2) + 1;

    // Failsafe so the fruit can't spawn in the dead middle
    if (F_PosX == GRID_MID_POS_X && F_PosY == GRID_MID_POS_Y) goto GenRandPos;

    auto GridPosFruit = GetGridPos(F_PosX, F_PosY);
    F_FruitRect.setPosition(GridPosFruit.first, GridPosFruit.second);
}

Fruit::Fruit() noexcept {
    GRDEBUG(printf("A fruit is initialised at [X%d; Y%d]\n", F_PosX, F_PosY))

    F_FruitRect = sf::RectangleShape({FRUIT_WIDTH, FRUIT_HEIGHT});
    F_FruitRect.setFillColor(GREEN);
    
    F_FruitRect.setOrigin(FRUIT_ANCHOR.first, FRUIT_ANCHOR.second);

    GenerateNewFruitPosition();
}

Fruit::~Fruit() {

}

/**
 * Sets the instance of the Fruit class for the Snek class.
 *
 * @param fruit A pointer to the Fruit instance.
 *
 * @throws None
 */
inline FASTINL void Snek::SetFruitInstance(Fruit* fruit) noexcept {
    PTR_FruitInstance = fruit;
}

/**
 * Draws the fruit on the given render window.
 *
 * @param mWindow The render window to draw the fruit on
 * @param F_FruitRect The rectangle shape of the fruit to be drawn
 *
 * @throws None
 */
inline FASTINL void Fruit::DrawFruit(sf::RenderWindow &mWindow, sf::RectangleShape F_FruitRect) noexcept {
    mWindow.draw(F_FruitRect);
}

/**
 * Returns a singleton instance of the Fruit class. If the instance does not exist, it creates a new one.
 *
 * @return A pointer to the singleton Fruit instance.
 */
inline FASTINL Fruit* Fruit::GetFruitInstance() noexcept {
    S_PTR_FruitInstanceOrigin = (S_PTR_FruitInstanceOrigin == nullptr) ? new Fruit() : S_PTR_FruitInstanceOrigin;
    return S_PTR_FruitInstanceOrigin;
}

/**
 * @brief Returns the fruit's position on the game grid
 *
 * The function returns a pair of (X, Y) coordinates representing the position
 * of the fruit on the game grid. The X and Y coordinates are in the range
 * [0;32] where 0 represents the leftmost/topmost position on the grid, and 31
 * represents the rightmost/bottommost position.
 *
 * @return A pair of (X, Y) coordinates representing the position of the fruit
 * on the game grid.
 */
inline FASTINL std::pair<uint8_t, uint8_t> Fruit::GetFruitPosition() const noexcept {
    return std::make_pair(F_PosX - 1, F_PosY - 1);
}

#pragma endregion