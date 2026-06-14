#pragma once
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <string>
#include <vector>

class Character;
class Monster;

class Database {
private:
    QSqlDatabase db;
    bool isOpen;

    void createTables();

public:
    Database();
    ~Database();

    bool open(const std::string& path = "monstergame.db");
    void close();

    // Character save/load
    int saveCharacter(Character& character);  // returns character id
    std::vector<std::string> listCharacterNames();
    Character* loadCharacter(const std::string& name);
    void deleteCharacter(const std::string& name);

    // Statistics
    void recordMonsterDefeated(int characterId, const std::string& monsterName);
    void recordItemUsed(int characterId, const std::string& itemName, bool causedKill);

    // Stats queries
    int getTotalMonstersDefeated();
    std::string getMostUsedItem(int characterId);
    std::string getMostUsedMonster(int characterId);
    void displayStatistics(int characterId);
};
