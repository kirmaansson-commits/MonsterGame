#pragma once
#include "Character.h"
#include "Battle.h"
#include "Cave.h"
#include "Item.h"
#include "Database.h"
#include <vector>
#include <string>

class Game {
private:
    Character* player;
    int currentCharacterId;
    Database database;
    std::vector<Monster> availableMonsters;
    std::vector<Cave> caves;

    void initMonsters();
    void mainMenu();
    void newCharacter();
    void loadCharacterMenu();
    void startAdventure();
    void selectAndBattle();
    void enterCave();
    void handleDefeatedEnemy(Monster& enemy);
    void displayAvailableMonsters() const;
    void saveProgress();

    int calculatePlayerPower() const;
    void generateCaves();
    Monster makeMonsterByPower(int power) const;
    Item* randomReward() const;
    void giveItemToMonster(Item* item);

public:
    Game();
    ~Game();
    void run();
};
