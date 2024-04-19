#pragma once

#include <SFML/Graphics.hpp>

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

inline std::pair<uint16_t, uint16_t> GetGridPos(uint16_t x, uint16_t y);

#pragma region Debug

#ifdef DEBUG_DRAW_GRID
__attribute__((always_inline))
inline void DrawDebugGrid(sf::RenderWindow mWindow) {
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