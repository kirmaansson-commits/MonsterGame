#pragma once
#include "Monster.h"
#include <vector>

class Battle {
private:
    std::vector<Monster>& playerTeam;
    Monster& enemyMonster;

    bool playerGoesFirst();

public:
    Battle(std::vector<Monster>& playerTeam, Monster& enemyMonster);
    bool start(); // returns true if player won
};
