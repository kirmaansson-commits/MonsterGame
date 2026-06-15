#include "Battle.h"
#include "Item.h"
#include "Database.h"
#include <iostream>
#include <cstdlib>
#include <limits>

Battle::Battle(std::vector<Monster>& playerTeam, Monster& enemyMonster,
               Database* db, int characterId)
    : playerTeam(playerTeam), enemyMonster(enemyMonster),
      db(db), characterId(characterId) {}

bool Battle::playerGoesFirst() { return (rand() % 2) == 0; }

void Battle::playerTurn(Monster& current) {
    std::cout << "\n--- " << current.getName() << "'s turn ---" << std::endl;

    if (!current.processStatuses()) return;
    if (!current.isAlive()) return;

    std::cout << "[1] Attack" << std::endl;
    if (current.getItemCount() > 0) std::cout << "[2] Use item" << std::endl;
    std::cout << "Choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 2 && current.getItemCount() > 0) {
        std::cout << "Select item:" << std::endl;
        for (int i = 0; i < current.getItemCount(); i++) {
            std::cout << "  [" << i+1 << "] " << current.getItems()[i]->getName() << std::endl;
        }
        std::cout << "Choice: ";
        int itemChoice;
        std::cin >> itemChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        itemChoice--;

        Item* item = current.takeItem(itemChoice);
        if (item) {
            std::string itemName = item->getName();
            bool killed = item->use(enemyMonster);
            if (db && characterId >= 0) {
                db->recordItemUsed(characterId, itemName, killed);
            }
            delete item;
        } else {
            std::cout << "Invalid item." << std::endl;
        }
    } else {
        enemyMonster.takeDamage(current.getStrength());
        std::cout << current.getName() << " attacks " << enemyMonster.getName()
                  << " for " << current.getStrength() << " damage! ["
                  << enemyMonster.getName() << " HP: " << enemyMonster.getHp()
                  << "]" << std::endl;
    }
}

void Battle::enemyTurn(Monster& current) {
    std::cout << "\n--- " << enemyMonster.getName() << "'s turn ---" << std::endl;
    if (!enemyMonster.processStatuses()) return;
    if (!enemyMonster.isAlive()) return;

    current.takeDamage(enemyMonster.getStrength());
    std::cout << enemyMonster.getName() << " attacks " << current.getName()
              << " for " << enemyMonster.getStrength() << " damage! ["
              << current.getName() << " HP: " << current.getHp() << "]" << std::endl;
}

bool Battle::start() {
    std::cout << "\n=== BATTLE BEGINS ===" << std::endl;
    std::cout << "Enemy: ";
    enemyMonster.displayStats();

    for (size_t i = 0; i < playerTeam.size(); ) {
        Monster& current = playerTeam[i];
        std::cout << "\n*** " << current.getName() << " enters the battle! ***" << std::endl;
        current.displayStats();

        bool playerFirst = playerGoesFirst();
        std::cout << (playerFirst ? current.getName() : enemyMonster.getName())
                  << " goes first!" << std::endl;

        while (current.isAlive() && enemyMonster.isAlive()) {
            if (playerFirst) {
                playerTurn(current);
                if (enemyMonster.isAlive()) enemyTurn(current);
            } else {
                enemyTurn(current);
                if (current.isAlive()) playerTurn(current);
            }
        }

        if (!enemyMonster.isAlive()) {
            std::cout << "\n=== BATTLE OVER ===" << std::endl;
            std::cout << "You defeated " << enemyMonster.getName() << "!" << std::endl;
            if (db && characterId >= 0) {
                db->recordMonsterDefeated(characterId, enemyMonster.getName());
            }
            return true;
        }

        std::cout << current.getName() << " was defeated!" << std::endl;
        playerTeam.erase(playerTeam.begin() + i);
    }

    std::cout << "\n=== BATTLE OVER ===" << std::endl;
    std::cout << "All your monsters were defeated by " << enemyMonster.getName() << "!" << std::endl;
    return false;
}
