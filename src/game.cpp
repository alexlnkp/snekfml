
#include "game.hpp"

struct {
    sf::Color White = {255, 255, 255};
    sf::Color Green = {0, 255, 0};
} Colors;

#define GREEN Colors.Green
#define WHITE Colors.White

Snek::Snek() {
    mWindow.create(sf::VideoMode(640, 480), "Snake");
}

Snek::~Snek() {

}

int Snek::mainLoop() {
    while (mWindow.isOpen()) {
        handleEvents();
        updateGame();
        drawGame();
    }

    return 0;
}

void Snek::handleEvents() {
    while (mWindow.pollEvent(Event)) {
        switch (Event.type) {
        case sf::Event::Closed:
            mWindow.close(); goto exit_loop;
        default:
            goto exit_loop; // could just be a break; instead, not sure
        }
    }
exit_loop:;
}

void Snek::updateGame() {

}

void Snek::DrawSnake() {

}

void Snek::drawGame() {
    
    mWindow.clear();

    // Draw all here

    sf::CircleShape shape(50);

    // set the shape color to green
    shape.setFillColor(WHITE);

    mWindow.draw(shape);
    

    mWindow.display();
}
