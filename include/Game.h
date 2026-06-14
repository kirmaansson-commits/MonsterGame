#pragma once
#include "Character.h"
#include "Battle.h"
#include <vector>
#include <string>

class Game {
private:
    Character* player;

    // Available monsters in the world
    std::vector<Monster> availableMonsters;

    void initMonsters();
    void mainMenu();
    void startAdventure();
    void battleMenu();
    void selectAndBattle();
    void handleDefeatedEnemy(Monster& enemy);
    void displayAvailableMonsters() const;

public:
    Game();
    ~Game();
    void run();
};
