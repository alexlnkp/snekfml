#include "game.hpp"

int main(int argc, char **argv) {
    
    // If no arguments provided - use pseudo-random seed
    // Else - use user-provided seed
    Snek game(argc > 1 ? std::stoi(argv[1]) : time(__null));
    game.mainLoop();

    return 0;
}
