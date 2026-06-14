#include "Monster.h"
#include "Item.h"
#include <iostream>
#include <cstdlib>

Monster::Monster(std::string name, int hp, int strength)
    : name(name), hp(hp), maxHp(hp), strength(strength), damageTaken(false) {}

Monster::~Monster() {
    for (Item* item : items) delete item;
}

Monster::Monster(const Monster& other)
    : name(other.name), hp(other.hp), maxHp(other.maxHp),
      strength(other.strength), damageTaken(other.damageTaken),
      statuses(other.statuses) {
    for (Item* item : other.items) {
        items.push_back(new Item(*item));
    }
}

Monster& Monster::operator=(const Monster& other) {
    if (this != &other) {
        for (Item* item : items) delete item;
        items.clear();

        name = other.name;
        hp = other.hp;
        maxHp = other.maxHp;
        strength = other.strength;
        damageTaken = other.damageTaken;
        statuses = other.statuses;
        for (Item* item : other.items) {
            items.push_back(new Item(*item));
        }
    }
    return *this;
}

std::string Monster::getName() const { return name; }
int Monster::getHp() const { return hp; }
int Monster::getMaxHp() const { return maxHp; }
int Monster::getStrength() const { return strength; }

void Monster::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
    damageTaken = true;
}

bool Monster::isAlive() const { return hp > 0; }

void Monster::addStatus(const Status& status) {
    statuses.push_back(status);
}

bool Monster::hasAnyStatus() const { return !statuses.empty(); }
bool Monster::hasTakenDamage() const { return damageTaken; }

bool Monster::processStatuses() {
    bool canAct = true;

    for (auto it = statuses.begin(); it != statuses.end(); ) {
        switch (it->getType()) {
            case StatusType::Poisoned:
                takeDamage(2);
                std::cout << "  " << name << " takes 2 poison damage! [HP: "
                          << hp << "]" << std::endl;
                break;
            case StatusType::Frozen:
                std::cout << "  " << name << " is Frozen and cannot act!" << std::endl;
                canAct = false;
                break;
            case StatusType::Paralyzed:
                if ((rand() % 100) < 30) {
                    std::cout << "  " << name << " is Paralyzed and loses the turn!" << std::endl;
                    canAct = false;
                }
                break;
            case StatusType::Stunned:
                std::cout << "  " << name << " is Stunned and misses the turn!" << std::endl;
                canAct = false;
                break;
            case StatusType::Cursed:
                // Curse damage is applied when dealing damage – handled in Battle
                std::cout << "  " << name << " is Cursed..." << std::endl;
                break;
        }

        it->decrementDuration();
        if (it->isExpired()) {
            it = statuses.erase(it);
        } else {
            ++it;
        }
    }

    return canAct;
}

bool Monster::addItem(Item* item) {
    if ((int)items.size() < MAX_ITEMS) {
        items.push_back(item);
        return true;
    }
    return false;
}

int Monster::getItemCount() const { return items.size(); }
std::vector<Item*>& Monster::getItems() { return items; }

Item* Monster::takeItem(int index) {
    if (index < 0 || index >= (int)items.size()) return nullptr;
    Item* item = items[index];
    items.erase(items.begin() + index);
    return item;
}

void Monster::displayStats() const {
    std::cout << name << " [HP: " << hp << "/" << maxHp
              << " | Strength: " << strength;
    if (!statuses.empty()) {
        std::cout << " | Status: ";
        for (size_t i = 0; i < statuses.size(); i++) {
            std::cout << statuses[i].getName();
            if (i < statuses.size() - 1) std::cout << ", ";
        }
    }
    if (!items.empty()) {
        std::cout << " | Items: " << items.size() << "/3";
    }
    std::cout << "]" << std::endl;
}
