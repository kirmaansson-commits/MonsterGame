#pragma once
#include "Monster.h"
#include "Item.h"
#include <vector>
#include <string>

class Cave {
private:
    std::string name;
    std::vector<Monster> enemies;
    Item* reward;

public:
    Cave(std::string name, std::vector<Monster> enemies, Item* reward);
    ~Cave();

    // Disable copy to avoid double-delete of reward
    Cave(const Cave&) = delete;
    Cave& operator=(const Cave&) = delete;

    // Allow move
    Cave(Cave&& other) noexcept;
    Cave& operator=(Cave&& other) noexcept;

    std::string getName() const;
    std::vector<Monster>& getEnemies();
    int getEnemyCount() const;
    bool isCleared() const;

    // Caller takes ownership of the reward item
    Item* claimReward();

    void displayInfo() const;
};
