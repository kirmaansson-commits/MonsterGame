#pragma once
#include <string>
#include <vector>
#include "Monster.h"

class Character {
private:
    std::string name;
    std::vector<Monster> monsters;
    static const int MAX_MONSTERS = 4;

public:
    Character(std::string name, bool withStarters = true);

    std::string getName() const;
    std::vector<Monster>& getMonsters();
    int getMonsterCount() const;
    bool hasMonsters() const;

    bool addMonster(const Monster& monster);
    bool replaceMonster(int index, const Monster& monster);
    void displayMonsters() const;
};
