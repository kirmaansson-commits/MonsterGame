#include "Battle.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Battle::Battle(Monster& playerMonster, Monster& enemyMonster)
    : playerMonster(playerMonster), enemyMonster(enemyMonster) {}

bool Battle::playerGoesFirst() {
    return (rand() % 2) == 0;
}

void Battle::start() {
    std::cout << "\n=== BATTLE BEGINS ===" << std::endl;
    std::cout << "Your " << playerMonster.getName()
              << " vs " << enemyMonster.getName() << std::endl;

    bool playerTurn = playerGoesFirst();

    if (playerTurn)
        std::cout << playerMonster.getName() << " goes first!" << std::endl;
    else
        std::cout << enemyMonster.getName() << " goes first!" << std::endl;

    while (playerMonster.isAlive() && enemyMonster.isAlive()) {
        if (playerTurn) {
            enemyMonster.takeDamage(playerMonster.getStrength());
            std::cout << playerMonster.getName() << " attacks "
                      << enemyMonster.getName() << " for "
                      << playerMonster.getStrength() << " damage! ";
            std::cout << "[" << enemyMonster.getName()
                      << " HP: " << enemyMonster.getHp() << "]" << std::endl;
        } else {
            playerMonster.takeDamage(enemyMonster.getStrength());
            std::cout << enemyMonster.getName() << " attacks "
                      << playerMonster.getName() << " for "
                      << enemyMonster.getStrength() << " damage! ";
            std::cout << "[" << playerMonster.getName()
                      << " HP: " << playerMonster.getHp() << "]" << std::endl;
        }
        playerTurn = !playerTurn;
    }

    std::cout << "\n=== BATTLE OVER ===" << std::endl;
    if (playerMonster.isAlive()) {
        std::cout << playerMonster.getName() << " won!" << std::endl;
    } else {
        std::cout << enemyMonster.getName() << " won!" << std::endl;
    }
}
