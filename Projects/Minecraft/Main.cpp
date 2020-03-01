#include "Game.hpp"

int main() {
    Minecraft::Game game;
    while (game.shouldRun()) {
        game.update();
    }
}
