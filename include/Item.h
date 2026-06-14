#pragma once
#include <string>
#include "Status.h"

enum class ItemType {
    Bomb,
    FireBomb,
    ThunderBomb,
    Club,
    Blizzard,
    Curse,
    PoisonVial
};

// Forward declaration
class Monster;

class Item {
private:
    ItemType type;
    std::string name;
    int damage;

public:
    Item(ItemType type);

    ItemType getType() const;
    std::string getName() const;
    int getDamage() const;

    // Use the item on a target monster
    // Returns true if the item dealt damage
    void use(Monster& target);
};
