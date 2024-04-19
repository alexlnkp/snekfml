#pragma once

#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 640

#define GAME_RESOLUTION 20 // 20 pixels per segment. all of the elements should be drawn on the grid of 20x20 pixels

constexpr uint8_t GRID_X_RESOLUTION = (WINDOW_WIDTH / GAME_RESOLUTION);
constexpr uint8_t GRID_Y_RESOLUTION = (WINDOW_HEIGHT / GAME_RESOLUTION);

struct Velocity {
    int8_t X;
    int8_t Y;
};

struct SnakeSegment {
    std::pair<uint8_t, uint8_t> position;
};

struct SnakeHead {
    std::pair<uint8_t, uint8_t> position;
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
    sf::RectangleShape Snake_Head;

    std::vector<sf::RectangleShape> snake;

public:
    Snek();
    ~Snek();

    int mainLoop();

private:
    inline void handleEvents();
    inline void updateGame(SnakeHead &_Snake_Head, std::vector<sf::RectangleShape> &snake);
    inline void drawGame();

    void KeyHandler(sf::Keyboard::Key key, SnakeHead &_Snake_Head);

private:
    inline void DrawSnake();
    inline void MoveSnake(std::vector<sf::RectangleShape> &snake, uint16_t X, uint16_t Y);

    inline void InitSnakeHead(sf::RectangleShape &Snake_Head);
    inline void AddSegmentToSnake();
};

class Fruit {
private:
    uint8_t posX, posY;

public:
    Fruit();
    ~Fruit();
    inline uint8_t GetFruitPosition();

private:
    inline void InitFruit();
    inline uint8_t CreateRandomPosition();

};

inline std::pair<uint16_t, uint16_t> GetGridPos(uint8_t x, uint8_t y);

#pragma region Debug

#ifdef DEBUG
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