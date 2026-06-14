#pragma once
#include "Monster.h"
#include <vector>

class Battle {
private:
    std::vector<Monster>& playerTeam;
    Monster& enemyMonster;

    bool playerGoesFirst();
    // Returns true if turn played out, false if monster died from status
    void playerTurn(Monster& current);
    void enemyTurn(Monster& current);

public:
    Battle(std::vector<Monster>& playerTeam, Monster& enemyMonster);
    bool start(); // returns true if player won
};
