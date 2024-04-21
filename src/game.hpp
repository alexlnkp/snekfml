#pragma once

#include "gameconfig.h"

// A enum that contains the current state of the game
enum GameState : uint8_t { Menu, Game, Pause, GameOver };

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
    ~Fruit();
    static Fruit* GetFruitInstance() noexcept;
    inline static void DrawFruit(sf::RenderWindow &mWindow, sf::RectangleShape F_FruitRect) noexcept;
    void GenerateNewFruitPosition() noexcept;
    inline std::pair<uint8_t, uint8_t> GetFruitPosition() const noexcept;

    inline sf::RectangleShape* GetFruitRect() noexcept;
};

class Snek {
private:
    sf::Clock T_Updater;

    sf::RenderWindow mWindow;
    sf::Event Event;

    Fruit* PTR_FruitInstance;
    sf::RectangleShape P_FruitRect;

    SnakeHead_S _Snake_Head_S = {};

    std::pair<uint16_t, uint16_t> PrevSnakeHeadPosition;

    sf::RectangleShape Snake_Head;
    std::vector<sf::RectangleShape> SnakeTail;

    GameState CurrentGameState = Game;

    sf::Font MyFont;
    sf::Text Text;

    uint_least64_t Score = 0;

public:
    Snek(int_least64_t seed) noexcept;
    ~Snek();

    int mainLoop() noexcept;

private:
    inline void handleEvents() noexcept;
    inline void updateGame() noexcept;
    inline void drawGame() noexcept;

    void KeyHandler(sf::Keyboard::Key key, SnakeHead_S &_Snake_Head_S) noexcept;

private:
    inline void DrawSnake(sf::RenderWindow &mWindow) const noexcept;

    inline void MoveSnake(sf::RectangleShape &Snake_Head, SnakeHead_S &_Snake_Head_S) noexcept;

    inline void InitSnakeHead(sf::RectangleShape &Snake_Head, SnakeHead_S &_Snake_Head_S) noexcept;
    inline void AddSegmentToSnake(std::vector<sf::RectangleShape> &SnakeTail) const noexcept;

    inline void UpdateSnek(SnakeHead_S &_Snake_Head_S, std::vector<sf::RectangleShape> &SnakeTail) noexcept;

    inline void SetFruitInstance(Fruit* fruit) noexcept;
    inline void FruitCollision(std::vector<sf::RectangleShape> &SnakeTail, uint_least64_t &Score) noexcept;
    inline void TailCollision(sf::RectangleShape &Snake_Head, std::vector<sf::RectangleShape> &SnakeTail) noexcept;

    inline void UpdateScore() noexcept;
};

inline std::pair<uint16_t, uint16_t> GetGridPos(uint8_t x, uint8_t y) noexcept;

#pragma region Debug

GRDEBUG(
    __attribute__((always_inline))
    inline void DrawDebugGrid(sf::RenderWindow &mWindow) noexcept {
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