#include "game.hpp"

extern "C" {
    #include <unistd.h>
}

#define PRINT(ARG) write(STDOUT_FILENO, ARG, sizeof(ARG))

int main() {
    Snek game;

    game.mainLoop();

    return 0;
}
