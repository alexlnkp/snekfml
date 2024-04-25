#include <SFML/Graphics.hpp>
#include "game.hpp"

int main(int argc, char **argv) {
    // A lifehack so the output is a bit faster for graphical debug.
    // Adds 64 bytes to the size (only for the Release config for some reason)
    GRDEBUG(std::ios_base::sync_with_stdio(false))

    // If no arguments provided - use pseudo-random seed
    // Else - use user-provided seed
    Snek game(argc > 1 ? std::stoi(argv[1]) : time(__null));
    game.mainLoop();

    return 0;
}
