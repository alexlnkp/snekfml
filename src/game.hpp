#pragma once

#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 640

#define GAME_RESOLUTION 20 // 20 pixels per segment. all of the elements should be drawn on the grid of 20x20 pixels

constexpr uint8_t GRID_X_RESOLUTION = (WINDOW_WIDTH / GAME_RESOLUTION);
constexpr uint8_t GRID_Y_RESOLUTION = (WINDOW_HEIGHT / GAME_RESOLUTION);

class Snek {
private:
    sf::RenderWindow mWindow;
    sf::Event Event;

    std::vector<sf::RectangleShape> snake;

public:
    Snek();
    ~Snek();

    int mainLoop();

private:
    inline void handleEvents();
    inline void updateGame();
    inline void drawGame();

private:
    inline void DrawSnake();

    inline void InitSnakeHead();
    inline void AddSegmentToSnake();
};

class Fruit {
private:



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