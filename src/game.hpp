#pragma once

#include "gameconfig.h"
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
    uint8_t F_PosX, F_PosY;
    static Fruit* S_PTR_FruitInstanceOrigin;
    sf::RectangleShape F_FruitRect;

    // A hack so that Fruit can only be initialised by `Fruit X = Fruit::GetFruitInstance();`
    // This way, there is NO way that there can be multiple fruits at the same time.
    Fruit();

public:
    ~Fruit();
    static Fruit* GetFruitInstance();
    inline static void DrawFruit(sf::RenderWindow &mWindow, sf::RectangleShape F_FruitRect);
    void GenerateNewFruitPosition();
    inline std::pair<uint8_t, uint8_t> GetFruitPosition() const;

    inline sf::RectangleShape* GetFruitRect();
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

    std::vector<sf::RectangleShape> SnakeTail;

public:
    Snek(int_least64_t seed);
    ~Snek();

    int mainLoop();

private:
    inline void handleEvents();
    inline void updateGame();
    inline void drawGame();

    void KeyHandler(sf::Keyboard::Key key, SnakeHead_S &_Snake_Head_S);

private:
    inline void DrawSnake();

    inline void MoveSnake(sf::RectangleShape &SnakeTail, SnakeHead_S &_Snake_Head_S);

    inline void InitSnakeHead(sf::RectangleShape &Snake_Head);
    inline void AddSegmentToSnake(std::vector<sf::RectangleShape> &SnakeTail);

    inline void UpdateSnek(SnakeHead_S &_Snake_Head_S, std::vector<sf::RectangleShape> &SnakeTail);

    inline void SetFruitInstance(Fruit* fruit);
    inline void FruitCollision(std::vector<sf::RectangleShape> &SnakeTail);
};

inline std::pair<uint16_t, uint16_t> GetGridPos(uint8_t x, uint8_t y);

#pragma region Debug

GRDEBUG(
    __attribute__((always_inline))
    inline void DrawDebugGrid(sf::RenderWindow &mWindow) {
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