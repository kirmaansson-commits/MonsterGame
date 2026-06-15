#include "Item.h"
#include "Monster.h"
#include <iostream>
#include <cstdlib>

Item::Item(ItemType type) : type(type), damage(0) {
    switch (type) {
        case ItemType::Bomb:        name = "Bomb";         damage = 10; break;
        case ItemType::FireBomb:    name = "Fire Bomb";    damage = 5;  break;
        case ItemType::ThunderBomb: name = "Thunder Bomb"; damage = 10; break;
        case ItemType::Club:        name = "Club";         damage = 20; break;
        case ItemType::Blizzard:    name = "Blizzard";     damage = 0;  break;
        case ItemType::Curse:       name = "Curse";        damage = 0;  break;
        case ItemType::PoisonVial:  name = "Poison Vial";  damage = 0;  break;
    }
}

ItemType Item::getType() const { return type; }
std::string Item::getName() const { return name; }
int Item::getDamage() const { return damage; }

bool Item::use(Monster& target) {
    std::cout << "Used " << name << " on " << target.getName() << "!" << std::endl;

    bool dealtDamage = false;

    // Apply damage
    if (damage > 0) {
        target.takeDamage(damage);
        std::cout << "  Dealt " << damage << " damage! ["
                  << target.getName() << " HP: " << target.getHp() << "]" << std::endl;
        dealtDamage = true;
    }

    int roll = rand() % 100;

    // Apply status based on item type
    switch (type) {
        case ItemType::FireBomb:
            if (roll < 35) {
                target.addStatus(Status(StatusType::Stunned));
                std::cout << "  " << target.getName() << " is Stunned!" << std::endl;
            }
            break;
        case ItemType::ThunderBomb:
            if (roll < 50) {
                target.addStatus(Status(StatusType::Paralyzed));
                std::cout << "  " << target.getName() << " is Paralyzed!" << std::endl;
            }
            break;
        case ItemType::Blizzard:
            if (roll < 80) {
                target.addStatus(Status(StatusType::Frozen));
                std::cout << "  " << target.getName() << " is Frozen!" << std::endl;
            }
            break;
        case ItemType::Curse: {
            int chance = target.hasAnyStatus() ? 100 : 20;
            if (roll < chance) {
                target.addStatus(Status(StatusType::Cursed));
                std::cout << "  " << target.getName() << " is Cursed!" << std::endl;
            }
            break;
        }
        case ItemType::PoisonVial: {
            int chance = target.hasTakenDamage() ? 100 : 20;
            if (roll < chance) {
                target.addStatus(Status(StatusType::Poisoned));
                std::cout << "  " << target.getName() << " is Poisoned!" << std::endl;
            }
            break;
        }
        default: break;
    }
    return !target.isAlive();
}
