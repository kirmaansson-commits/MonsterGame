#pragma once
#include "Monster.h"
#include <vector>

class Battle {
private:
    Monster& playerMonster;
    Monster& enemyMonster;

    bool playerGoesFirst();

public:
    Battle(Monster& playerMonster, Monster& enemyMonster);
    void start();
};
