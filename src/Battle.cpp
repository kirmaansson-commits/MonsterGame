#include "Battle.h"
#include <iostream>
#include <cstdlib>

Battle::Battle(std::vector<Monster>& playerTeam, Monster& enemyMonster)
    : playerTeam(playerTeam), enemyMonster(enemyMonster) {}

bool Battle::playerGoesFirst() {
    return (rand() % 2) == 0;
}

bool Battle::start() {
    std::cout << "\n=== BATTLE BEGINS ===" << std::endl;
    std::cout << "Enemy: ";
    enemyMonster.displayStats();

    // Loop through all player monsters
    for (size_t i = 0; i < playerTeam.size(); ) {
        Monster& current = playerTeam[i];

        std::cout << "\n--- " << current.getName() << " enters the battle! ---" << std::endl;

        bool playerTurn = playerGoesFirst();
        if (playerTurn)
            std::cout << current.getName() << " goes first!" << std::endl;
        else
            std::cout << enemyMonster.getName() << " goes first!" << std::endl;

        while (current.isAlive() && enemyMonster.isAlive()) {
            if (playerTurn) {
                enemyMonster.takeDamage(current.getStrength());
                std::cout << current.getName() << " attacks "
                          << enemyMonster.getName() << " for "
                          << current.getStrength() << " damage! ";
                std::cout << "[" << enemyMonster.getName()
                          << " HP: " << enemyMonster.getHp() << "]" << std::endl;
            } else {
                current.takeDamage(enemyMonster.getStrength());
                std::cout << enemyMonster.getName() << " attacks "
                          << current.getName() << " for "
                          << enemyMonster.getStrength() << " damage! ";
                std::cout << "[" << current.getName()
                          << " HP: " << current.getHp() << "]" << std::endl;
            }
            playerTurn = !playerTurn;
        }

        if (!enemyMonster.isAlive()) {
            std::cout << "\n=== BATTLE OVER ===" << std::endl;
            std::cout << "You defeated " << enemyMonster.getName() << "!" << std::endl;
            return true;
        }

        // Current monster died – remove it and send in the next one
        std::cout << current.getName() << " was defeated!" << std::endl;
        playerTeam.erase(playerTeam.begin() + i);
    }

    std::cout << "\n=== BATTLE OVER ===" << std::endl;
    std::cout << "All your monsters were defeated by " << enemyMonster.getName() << "!" << std::endl;
    return false;
}
