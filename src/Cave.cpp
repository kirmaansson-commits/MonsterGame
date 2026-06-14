#include "Cave.h"
#include <iostream>

Cave::Cave(std::string name, std::vector<Monster> enemies, Item* reward)
    : name(name), enemies(std::move(enemies)), reward(reward) {}

Cave::~Cave() {
    delete reward;
}

Cave::Cave(Cave&& other) noexcept
    : name(std::move(other.name)),
      enemies(std::move(other.enemies)),
      reward(other.reward) {
    other.reward = nullptr;
}

Cave& Cave::operator=(Cave&& other) noexcept {
    if (this != &other) {
        delete reward;
        name = std::move(other.name);
        enemies = std::move(other.enemies);
        reward = other.reward;
        other.reward = nullptr;
    }
    return *this;
}

std::string Cave::getName() const { return name; }
std::vector<Monster>& Cave::getEnemies() { return enemies; }
int Cave::getEnemyCount() const { return enemies.size(); }

bool Cave::isCleared() const {
    for (const auto& m : enemies) {
        if (m.isAlive()) return false;
    }
    return true;
}

Item* Cave::claimReward() {
    Item* r = reward;
    reward = nullptr;
    return r;
}

void Cave::displayInfo() const {
    std::cout << "\n=== " << name << " ===" << std::endl;
    std::cout << "Enemies inside:" << std::endl;
    for (const auto& m : enemies) {
        std::cout << "  - ";
        m.displayStats();
    }
    if (reward) {
        std::cout << "Reward: " << reward->getName() << std::endl;
    }
}
