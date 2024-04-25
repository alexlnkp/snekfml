#pragma once

#include "gameconfig.h"

// A enum that contains the current state of the game
enum GameState : uint8_t { GameOver, Menu, Game, Pause };

struct Velocity {
    int8_t X;
    int8_t Y;
};

struct SnakeSegment {
    std::pair<uint8_t, uint8_t> position; // X,Y grid position [0;32]
};

struct SnakeHead_S {
    std::pair<uint8_t, uint8_t> position; // X,Y grid position [0;32]
    Velocity velocity;
};

class Fruit {
private:
    uint8_t F_PosX = 0; 
    uint8_t F_PosY = 0;
    static Fruit* S_PTR_FruitInstanceOrigin;
    sf::RectangleShape F_FruitRect;

    // A hack so that Fruit can only be initialised by `Fruit X = Fruit::GetFruitInstance();`
    // This way, there is NO way that there can be multiple fruits at the same time.
    Fruit() noexcept;

public:
    ~Fruit() noexcept;
    inline FASTINL static Fruit* GetFruitInstance() noexcept;
    inline FASTINL void DrawFruit(sf::RenderWindow &mWindow) noexcept;
    inline FASTINL void GenerateNewFruitPosition() noexcept;
    inline FASTINL std::pair<uint8_t, uint8_t> GetFruitPosition() const noexcept;

    inline FASTINL sf::RectangleShape* GetFruitRect() noexcept;
};

class Snek {
private:
    sf::Clock T_Updater;

    sf::RenderWindow mWindow;
    sf::Event Event;

    SnakeHead_S _Snake_Head_S = {};

    std::pair<uint16_t, uint16_t> PrevSnakeHeadPosition;

    sf::RectangleShape Snake_Head;
    std::vector<sf::RectangleShape> SnakeTail;

    GameState CurrentGameState = Game;

    sf::Font MyFont;
    sf::Text ScoreText;
    sf::Text GameOverText;

    uint_least64_t Score = 0;

public:
    Snek(int_least64_t seed) noexcept;
    ~Snek() noexcept;

    int mainLoop() noexcept;

private:
    inline FASTINL void handleEvents() noexcept;
    inline FASTINL void updateGame() noexcept;
    inline FASTINL void drawGame() noexcept;

    inline FASTINL void KeyHandler(sf::Keyboard::Key key, SnakeHead_S &_Snake_Head_S) noexcept;

private:
    inline FASTINL void DrawSnake(sf::RenderWindow &mWindow) const noexcept;

    inline FASTINL uint8_t MoveSnake(sf::RectangleShape &Snake_Head, SnakeHead_S &_Snake_Head_S) noexcept;

    inline FASTINL void InitSnakeHead(sf::RectangleShape &Snake_Head, SnakeHead_S &_Snake_Head_S) noexcept;
    inline FASTINL void AddSegmentToSnake(std::vector<sf::RectangleShape> &SnakeTail) const noexcept;

    inline FASTINL void UpdateSnek(SnakeHead_S &_Snake_Head_S, std::vector<sf::RectangleShape> &SnakeTail) noexcept;

    inline FASTINL void FruitCollision(std::vector<sf::RectangleShape> &SnakeTail, uint_least64_t &Score) noexcept;
    inline FASTINL void TailCollision(sf::RectangleShape &Snake_Head, std::vector<sf::RectangleShape> &SnakeTail) noexcept;

    inline FASTINL void UpdateScore() noexcept;
    inline FASTINL void GameOverHandler() noexcept;
};

inline FASTINL std::pair<uint16_t, uint16_t> GetGridPos(uint8_t x, uint8_t y) noexcept;

#pragma region Debug

GRDEBUG(
    inline __attribute__((always_inline)) void DrawDebugGrid(sf::RenderWindow &mWindow) noexcept {
        for (int i = 0; i < GRID_X_RESOLUTION; i++) {
            for (int j = 0; j < GRID_Y_RESOLUTION; j++) {
                sf::RectangleShape grSeg({10, 10});
                grSeg.setFillColor(sf::Color(100, 100, 100, 50));
                auto GridPos = GetGridPos(i, j);
                grSeg.setPosition({GridPos.first, GridPos.second});
                mWindow.draw(grSeg);
            }
        }
    }
)
#pragma endregion