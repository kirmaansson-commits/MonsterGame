#pragma once
#include <string>

class Monster {
private:
    std::string name;
    int hp;
    int maxHp;
    int strength;

public:
    Monster(std::string name, int hp, int strength);

    // Getters
    std::string getName() const;
    int getHp() const;
    int getMaxHp() const;
    int getStrength() const;

    // Combat
    void takeDamage(int damage);
    bool isAlive() const;

    void displayStats() const;
};
