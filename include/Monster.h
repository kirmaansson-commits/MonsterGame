#pragma once
#include <string>
#include <vector>
#include "Status.h"

class Item; // forward declaration

class Monster {
private:
    std::string name;
    int hp;
    int maxHp;
    int strength;
    bool damageTaken;
    std::vector<Status> statuses;
    std::vector<Item*> items;
    static const int MAX_ITEMS = 3;

public:
    Monster(std::string name, int hp, int strength);
    ~Monster();

    // Copy constructor and assignment (deep copy items)
    Monster(const Monster& other);
    Monster& operator=(const Monster& other);

    // Getters
    std::string getName() const;
    int getHp() const;
    int getMaxHp() const;
    int getStrength() const;

    // Combat
    void takeDamage(int damage);
    bool isAlive() const;

    // Status management
    void addStatus(const Status& status);
    bool hasAnyStatus() const;
    bool hasTakenDamage() const;
    // Process statuses at start of turn. Returns true if the monster can act.
    bool processStatuses();

    // Item management
    bool addItem(Item* item);
    int getItemCount() const;
    std::vector<Item*>& getItems();
    Item* takeItem(int index); // removes and returns item (used in battle)

    void displayStats() const;
};
