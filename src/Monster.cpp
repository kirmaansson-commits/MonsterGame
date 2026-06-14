#include "Monster.h"
#include <iostream>

Monster::Monster(std::string name, int hp, int strength)
    : name(name), hp(hp), maxHp(hp), strength(strength) {}

std::string Monster::getName() const { return name; }
int Monster::getHp() const { return hp; }
int Monster::getMaxHp() const { return maxHp; }
int Monster::getStrength() const { return strength; }

void Monster::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

bool Monster::isAlive() const {
    return hp > 0;
}

void Monster::displayStats() const {
    std::cout << name << " [HP: " << hp << "/" << maxHp
              << " | Styrke: " << strength << "]" << std::endl;
}
