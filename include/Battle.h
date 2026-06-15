#pragma once
#include "Monster.h"
#include <vector>

class Database;

class Battle {
private:
    std::vector<Monster>& playerTeam;
    Monster& enemyMonster;
    Database* db;
    int characterId;

    bool playerGoesFirst();
    void playerTurn(Monster& current);
    void enemyTurn(Monster& current);

public:
    Battle(std::vector<Monster>& playerTeam, Monster& enemyMonster,
           Database* db = nullptr, int characterId = -1);
    bool start();
};
