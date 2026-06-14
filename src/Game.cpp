#include "Game.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>

Game::Game() : player(nullptr) {
    srand(time(0));
    initMonsters();
}

Game::~Game() {
    delete player;
}

void Game::initMonsters() {
    availableMonsters = {
        Monster("Hobbit",           4,   1),
        Monster("Orc",              6,   2),
        Monster("Warg",             8,   3),
        Monster("Uruk-hai",         12,  4),
        Monster("Troll",            20,  5),
        Monster("Ringwraith",       35,  7),
        Monster("Smaug the Dragon", 100, 10)
    };
}

void Game::run() {
    std::cout << "================================" << std::endl;
    std::cout << "   Welcome to MiddelEarth!   " << std::endl;
    std::cout << "================================" << std::endl;
    mainMenu();
}

void Game::mainMenu() {
    while (true) {
        std::cout << "\n=== MAIN MENU ===" << std::endl;
        std::cout << "[1] New Character" << std::endl;
        std::cout << "[2] Quit" << std::endl;
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            delete player;
            player = nullptr;

            std::string name;
            std::cout << "Enter character name: ";
            std::getline(std::cin, name);

            player = new Character(name);
            std::cout << "\nWelcome, " << name << "! You start with 2 Hobbits." << std::endl;
            startAdventure();

        } else if (choice == 2) {
            std::cout << "Goodbye!" << std::endl;
            break;
        } else {
            std::cout << "Invalid choice." << std::endl;
        }
    }
}

void Game::startAdventure() {
    while (true) {
        std::cout << "\n=== ADVENTURE ===" << std::endl;
        player->displayMonsters();

        std::cout << "\n[1] Battle an enemy" << std::endl;
        std::cout << "[2] Return to main menu" << std::endl;
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            if (!player->hasMonsters()) {
                std::cout << "All your monsters have been defeated! Returning to main menu." << std::endl;
                break;
            }
            selectAndBattle();
        } else if (choice == 2) {
            break;
        } else {
            std::cout << "Invalid choice." << std::endl;
        }

        // Check if player has any monsters left
        if (!player->hasMonsters()) {
            std::cout << "\nAll your monsters have been defeated! Returning to main menu." << std::endl;
            break;
        }
    }
}

void Game::selectAndBattle() {
    // Select enemy monster
    std::cout << "\nSelect an enemy to fight:" << std::endl;
    displayAvailableMonsters();
    std::cout << "Choice: ";

    int enemyChoice;
    std::cin >> enemyChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    enemyChoice--;

    if (enemyChoice < 0 || enemyChoice >= (int)availableMonsters.size()) {
        std::cout << "Invalid choice." << std::endl;
        return;
    }

    // Create the enemy for this battle
    Monster enemy = availableMonsters[enemyChoice];

    Battle battle(player->getMonsters(), enemy);
    bool won = battle.start();

    if (won) {
        handleDefeatedEnemy(enemy);
    }
}

void Game::handleDefeatedEnemy(Monster& enemy) {
    std::cout << "\nYou defeated " << enemy.getName() << "!" << std::endl;

    // Find the full HP version from available monsters
    Monster fresh = enemy;
    for (const auto& m : availableMonsters) {
        if (m.getName() == enemy.getName()) {
            fresh = m; // fresh copy with full HP
            break;
        }
    }

    if (player->getMonsterCount() < 4) {
        std::cout << "Add " << fresh.getName() << " to your team? [1] Yes  [2] No: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            player->addMonster(fresh);
            std::cout << fresh.getName() << " joined your team!" << std::endl;
        }
    } else {
        std::cout << "Your team is full (4/4)! Replace a monster?" << std::endl;
        std::cout << "[1] Yes  [2] No: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            player->displayMonsters();
            std::cout << "Replace which monster? ";
            int idx;
            std::cin >> idx;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            idx--;
            player->replaceMonster(idx, fresh);
            std::cout << "Monster replaced!" << std::endl;
        }
    }
}


void Game::displayAvailableMonsters() const {
    for (int i = 0; i < (int)availableMonsters.size(); i++) {
        std::cout << "  [" << i+1 << "] ";
        availableMonsters[i].displayStats();
    }
}
