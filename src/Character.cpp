#include "Character.h"
#include <iostream>

Character::Character(std::string name, bool withStarters) : name(name) {
    if (withStarters) {
        monsters.push_back(Monster("Hobbit", 4, 1));
        monsters.push_back(Monster("Hobbit", 4, 1));
    }
}

std::string Character::getName() const { return name; }
std::vector<Monster>& Character::getMonsters() { return monsters; }
int Character::getMonsterCount() const { return monsters.size(); }
bool Character::hasMonsters() const { return !monsters.empty(); }

bool Character::addMonster(const Monster& monster) {
    if (monsters.size() < MAX_MONSTERS) {
        monsters.push_back(monster);
        return true;
    }
    return false;
}

bool Character::replaceMonster(int index, const Monster& monster) {
    if (index >= 0 && index < (int)monsters.size()) {
        monsters[index] = monster;
        return true;
    }
    return false;
}

void Character::displayMonsters() const {
    std::cout << "\n" << name << "'s monsters:" << std::endl;
    for (int i = 0; i < (int)monsters.size(); i++) {
        std::cout << "  [" << i+1 << "] ";
        monsters[i].displayStats();
    }
}
