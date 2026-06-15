#include <QCoreApplication>
#include "Game.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    Game game;
    game.run();
    return 0;
}
