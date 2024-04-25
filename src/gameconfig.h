#pragma once

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 640

#define GAME_RESOLUTION 20 // 20 pixels per segment. All of the elements should be drawn on the grid of 20x20 pixels

// Note: framerate needed with refreshrate of 125ms is around 8FPS, but SFML behaves strangely if that's the case.
// The lower the framerate - the longer SFML initializes after creating a window.
// Also, sf::Event becomes rather laggy and unresponsive with lower framerate. I assume I have to manually lower the framerate instead.
constexpr uint8_t FRAMERATE = 60;

constexpr uint8_t GRID_X_RESOLUTION = (WINDOW_WIDTH / GAME_RESOLUTION);
constexpr uint8_t GRID_Y_RESOLUTION = (WINDOW_HEIGHT / GAME_RESOLUTION);

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

#define FRUIT_POINTS_WORTH 10

#define FONT_FILE_PATH "assets/NUMBERS_ONLY_prstrtk.ttf"
constexpr uint8_t FONT_SIZE = (GAME_RESOLUTION + 12);

constexpr uint8_t SNAKE_SEGMENT_WIDTH  = (SNAKE_HEAD_WIDTH - 4);
constexpr uint8_t SNAKE_SEGMENT_HEIGHT = (SNAKE_HEAD_HEIGHT - 4);
constexpr std::pair<int8_t, int8_t> SNAKE_SEGMENT_ANCHOR = std::make_pair(-1, -1);

constexpr uint8_t GRID_MID_POS_X = ((GRID_X_RESOLUTION >> 1) - 1);
constexpr uint8_t GRID_MID_POS_Y = ((GRID_Y_RESOLUTION >> 1) - 1);

constexpr uint8_t FRUIT_WIDTH  = (SNAKE_HEAD_WIDTH - 12);
constexpr uint8_t FRUIT_HEIGHT = (SNAKE_HEAD_HEIGHT - 12);
constexpr std::pair<int8_t, int8_t> FRUIT_ANCHOR = std::make_pair(15, 15);

#pragma region Helper Functions and Macros

// A hack so the compiler trusts me for once
// Spoiler: compiler got fussy
// Spoiler X2: apparently if you force compiler to NOT inline a function - binary gets even bigger for some reason
#define FASTINL __attribute__((always_inline))

#define SIGN(T) (0 < T) - (T < 0)

// A hack so that i don't have to copy & paste `#ifdef GRAPHIC_DEBUG` . . . `#endif` for every graphic debug command
#ifdef GRAPHIC_DEBUG
#   define GRDEBUG(DEB_COM) DEB_COM;
#else
#   define GRDEBUG(DEB_COM) ;
#endif


#pragma endregion