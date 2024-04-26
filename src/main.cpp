#include <SFML/Graphics.hpp>
#include "game.hpp"

int main(int argc, char **argv) {
    // A lifehack so the output is a bit faster for graphical debug.
    // Adds 64 bytes to the size (only for the Release config for some reason)
    GRDEBUG(std::ios_base::sync_with_stdio(false))

    Snek game(argv[1] ? static_cast<uint_fast32_t>(std::stoul(argv[1])) : time(__null));
    game.mainLoop();

    return 0;
}
