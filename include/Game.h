#pragma once
#include "Character.h"
#include "Battle.h"
#include "Cave.h"
#include "Item.h"
#include <vector>
#include <string>

class Game {
private:
    Character* player;
    std::vector<Monster> availableMonsters;
    std::vector<Cave> caves;

    void initMonsters();
    void mainMenu();
    void startAdventure();
    void selectAndBattle();
    void enterCave();
    void handleDefeatedEnemy(Monster& enemy);
    void displayAvailableMonsters() const;

    // Cave generation
    int calculatePlayerPower() const;
    void generateCaves();
    Monster makeMonsterByPower(int power) const;
    Item* randomReward() const;

    // Helper: give a reward item to a chosen monster
    void giveItemToMonster(Item* item);

public:
    Game();
    ~Game();
    void run();
};
