#pragma once

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 640

#define GAME_RESOLUTION 20 // 20 pixels per segment. all of the elements should be drawn on the grid of 20x20 pixels

constexpr uint8_t GRID_X_RESOLUTION = (WINDOW_WIDTH / GAME_RESOLUTION) - 1;
constexpr uint8_t GRID_Y_RESOLUTION = (WINDOW_HEIGHT / GAME_RESOLUTION) - 1;

struct {
    sf::Color White = {255, 255, 255};
    sf::Color Green = {0, 255, 0};
} Colors;

#define GREEN Colors.Green
#define WHITE Colors.White

// Here's how the grid should look like at the current params (each [ ] is 20x20 px in size) 1:8 scale
// NOTE: THERE ARE NO EMPTY SPACES BETWEEN THE CELLS OF THE GRID, THOSE ARE FOR EASIER UNDERSTANDING.
// [ ] [ ] [ ] [ ]
// [ ] [ ] [ ] [ ]
// [ ] [ ] [ ] [ ]
// [ ] [ ] [ ] [ ]

#define SNAKE_HEAD_WIDTH  GRID_X_RESOLUTION
#define SNAKE_HEAD_HEIGHT GRID_Y_RESOLUTION

constexpr uint8_t SNAKE_SEGMENT_WIDTH  = (SNAKE_HEAD_WIDTH - 4);
constexpr uint8_t SNAKE_SEGMENT_HEIGHT = (SNAKE_HEAD_WIDTH - 4);

// Note: framerate needed with refreshrate of 125ms is around 8FPS, but SFML behaves strangely if that's the case.
// The lower the framerate - the longer SFML initializes after creating a window.
// Also, sf::Event becomes rather laggy and unresponsive with lower framerate. I assume I have to manually lower the framerate instead.
constexpr uint8_t FRAMERATE = 60;

#pragma region Helper Functions and Macros

// A hack so the compiler trusts me for once
#define INLINE __attribute__((always_inline)) inline

// A hack so that i don't have to copy & paste `#ifdef GRAPHIC_DEBUG` . . . `#endif` for every graphic debug command
#ifdef GRAPHIC_DEBUG
#   define GRDEBUG(DEB_COM) DEB_COM;
#else
#   define GRDEBUG(DEB_COM) ;
#endif


#pragma endregion