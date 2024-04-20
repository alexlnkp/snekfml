#pragma once

#include "gameconfig.h"
struct Velocity {
    int8_t X;
    int8_t Y;
};

struct SnakeSegment {
    std::pair<uint8_t, uint8_t> position; // X,Y grid position [0;32]
};

struct SnakeHead {
    std::pair<uint8_t, uint8_t> position; // X,Y grid position [0;32]
    Velocity velocity;
};

class Snek {
private:
    sf::Time currentTime;
    sf::Time prevDeltaTime;
    sf::Time deltaTime;

    sf::Clock deltaClock;

    sf::Time tenTime;
    sf::Clock tenTimer;

    sf::RenderWindow mWindow;
    sf::Event Event;

    SnakeHead _Snake_Head = {};

    std::vector<sf::RectangleShape> snake;

public:
    Snek(int_least64_t seed);
    ~Snek();

    int mainLoop();

private:
    inline void handleEvents();
    inline void updateGame();
    inline void drawGame();

    void KeyHandler(sf::Keyboard::Key key, SnakeHead &_Snake_Head);

private:
    inline void DrawSnake();

    inline void MoveSnake(sf::RectangleShape &snake, SnakeHead &_Snake_Head);

    inline void InitSnakeHead(sf::RectangleShape &Snake_Head);
    inline void AddSegmentToSnake(std::vector<sf::RectangleShape> &snake);

    inline void UpdateSnek(SnakeHead &_Snake_Head, std::vector<sf::RectangleShape> &snake);
};

class Fruit {
private:
    uint8_t posX, posY;
    static Fruit* instance;

public:
    Fruit();
    ~Fruit();
    static Fruit* GetFruitInstance();
    inline std::pair<uint8_t, uint8_t> GetFruitPosition() const;

};


inline std::pair<uint16_t, uint16_t> GetGridPos(uint8_t x, uint8_t y);

#pragma region Debug

#ifdef GRAPHIC_DEBUG
__attribute__((always_inline))
inline void DrawDebugGrid(sf::RenderWindow &mWindow) {
    for (int i = 0; i < GRID_X_RESOLUTION + 1; i++) {
        for (int j = 0; j < GRID_Y_RESOLUTION + 1; j++) {
            sf::RectangleShape grSeg({10, 10});
            grSeg.setFillColor(sf::Color(100, 100, 100));
            auto GridPos = GetGridPos(i, j);
            grSeg.setPosition({GridPos.first, GridPos.second});
            mWindow.draw(grSeg);
        }
    }
}
#endif

#pragma endregion