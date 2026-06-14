#include "Game.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>

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
    std::cout << "   Welcome to Monster Quest!   " << std::endl;
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

        std::cout << "\n[1] Battle a single enemy" << std::endl;
        std::cout << "[2] Explore a cave" << std::endl;
        std::cout << "[3] Return to main menu" << std::endl;
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            if (!player->hasMonsters()) {
                std::cout << "All your monsters have been defeated!" << std::endl;
                break;
            }
            selectAndBattle();
        } else if (choice == 2) {
            if (!player->hasMonsters()) {
                std::cout << "All your monsters have been defeated!" << std::endl;
                break;
            }
            enterCave();
        } else if (choice == 3) {
            break;
        } else {
            std::cout << "Invalid choice." << std::endl;
        }

        if (!player->hasMonsters()) {
            std::cout << "\nAll your monsters have been defeated! Returning to main menu." << std::endl;
            break;
        }
    }
}

void Game::selectAndBattle() {
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

    Monster enemy = availableMonsters[enemyChoice];
    Battle battle(player->getMonsters(), enemy);
    bool won = battle.start();

    if (won) {
        handleDefeatedEnemy(enemy);
    }
}

void Game::enterCave() {
    generateCaves();

    std::cout << "\n=== CAVES ===" << std::endl;
    for (size_t i = 0; i < caves.size(); i++) {
        std::cout << "[" << i+1 << "] " << caves[i].getName()
                  << " (" << caves[i].getEnemyCount() << " enemies)" << std::endl;
    }
    std::cout << "[0] Back" << std::endl;
    std::cout << "Choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) return;
    choice--;

    if (choice < 0 || choice >= (int)caves.size()) {
        std::cout << "Invalid choice." << std::endl;
        return;
    }

    Cave& cave = caves[choice];
    cave.displayInfo();

    // Fight each enemy in the cave one by one
    for (auto& enemy : cave.getEnemies()) {
        if (!player->hasMonsters()) break;

        std::cout << "\n>>> A " << enemy.getName() << " appears! <<<" << std::endl;
        Battle battle(player->getMonsters(), enemy);
        bool won = battle.start();

        if (!won) {
            std::cout << "You failed to clear the cave!" << std::endl;
            return;
        }
    }

    if (cave.isCleared()) {
        std::cout << "\n*** You cleared " << cave.getName() << "! ***" << std::endl;
        Item* reward = cave.claimReward();
        if (reward) {
            std::cout << "You found: " << reward->getName() << "!" << std::endl;
            giveItemToMonster(reward);
        }
    }
}

void Game::giveItemToMonster(Item* item) {
    if (!player->hasMonsters()) {
        delete item;
        return;
    }

    std::cout << "\nGive " << item->getName() << " to which monster?" << std::endl;
    player->displayMonsters();
    std::cout << "[0] Discard item" << std::endl;
    std::cout << "Choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        delete item;
        return;
    }
    choice--;

    if (choice < 0 || choice >= player->getMonsterCount()) {
        std::cout << "Invalid choice. Item discarded." << std::endl;
        delete item;
        return;
    }

    Monster& target = player->getMonsters()[choice];
    if (!target.addItem(item)) {
        std::cout << target.getName() << "'s inventory is full (3/3). Item discarded." << std::endl;
        delete item;
    } else {
        std::cout << item->getName() << " given to " << target.getName() << "!" << std::endl;
    }
}

void Game::handleDefeatedEnemy(Monster& enemy) {
    std::cout << "\nYou defeated " << enemy.getName() << "!" << std::endl;

    Monster fresh = enemy;
    for (const auto& m : availableMonsters) {
        if (m.getName() == enemy.getName()) {
            fresh = m;
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
        std::cout << "Your team is full (4/4)! Replace a monster? [1] Yes  [2] No: ";
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

int Game::calculatePlayerPower() const {
    if (!player->hasMonsters()) return 1;
    int total = 0;
    for (const auto& m : player->getMonsters()) {
        total += m.getMaxHp() + m.getStrength() * 2;
    }
    return total / player->getMonsterCount();
}

Monster Game::makeMonsterByPower(int power) const {
    // Pick the monster from availableMonsters closest to the target power
    Monster best = availableMonsters[0];
    int bestDiff = std::abs((best.getMaxHp() + best.getStrength() * 2) - power);

    for (const auto& m : availableMonsters) {
        int diff = std::abs((m.getMaxHp() + m.getStrength() * 2) - power);
        if (diff < bestDiff) {
            bestDiff = diff;
            best = m;
        }
    }
    return best;
}

Item* Game::randomReward() const {
    int roll = rand() % 7;
    return new Item(static_cast<ItemType>(roll));
}

void Game::generateCaves() {
    caves.clear();
    int basePower = calculatePlayerPower();

    std::vector<std::string> caveNames = {
        "Goblin Hollow", "Misty Cavern", "Dragon's Lair"
    };

    std::vector<int> multipliers = { 100, 130, 170 }; // % of base power

    for (size_t i = 0; i < caveNames.size(); i++) {
        int targetPower = (basePower * multipliers[i]) / 100;
        std::vector<Monster> enemies;
        int enemyCount = 2 + (i); // 2, 3, 4 enemies per cave

        for (int j = 0; j < enemyCount; j++) {
            enemies.push_back(makeMonsterByPower(targetPower));
        }

        caves.emplace_back(caveNames[i], std::move(enemies), randomReward());
    }
}
