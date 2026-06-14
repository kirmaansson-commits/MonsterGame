#include "Database.h"
#include "Character.h"
#include "Monster.h"
#include "Item.h"
#include <QtSql/QSqlError>
#include <QVariant>
#include <iostream>

Database::Database() : isOpen(false) {}

Database::~Database() {
    close();
}

bool Database::open(const std::string& path) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QString::fromStdString(path));

    if (!db.open()) {
        std::cerr << "Failed to open database: "
                  << db.lastError().text().toStdString() << std::endl;
        return false;
    }

    isOpen = true;
    createTables();
    return true;
}

void Database::close() {
    if (isOpen) {
        db.close();
        isOpen = false;
    }
}

void Database::createTables() {
    QSqlQuery q;

    q.exec("CREATE TABLE IF NOT EXISTS characters ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "name TEXT UNIQUE NOT NULL)");

    q.exec("CREATE TABLE IF NOT EXISTS monsters ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "character_id INTEGER NOT NULL, "
           "name TEXT NOT NULL, "
           "hp INTEGER NOT NULL, "
           "max_hp INTEGER NOT NULL, "
           "strength INTEGER NOT NULL, "
           "FOREIGN KEY (character_id) REFERENCES characters(id) ON DELETE CASCADE)");

    q.exec("CREATE TABLE IF NOT EXISTS monster_items ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "monster_id INTEGER NOT NULL, "
           "item_type INTEGER NOT NULL, "
           "FOREIGN KEY (monster_id) REFERENCES monsters(id) ON DELETE CASCADE)");

    q.exec("CREATE TABLE IF NOT EXISTS monster_kills ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "character_id INTEGER NOT NULL, "
           "monster_name TEXT NOT NULL, "
           "kill_count INTEGER DEFAULT 1)");

    q.exec("CREATE TABLE IF NOT EXISTS item_usage ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "character_id INTEGER NOT NULL, "
           "item_name TEXT NOT NULL, "
           "use_count INTEGER DEFAULT 1, "
           "kill_count INTEGER DEFAULT 0)");
}

int Database::saveCharacter(Character& character) {
    // Delete existing character with same name first
    deleteCharacter(character.getName());

    QSqlQuery q;
    q.prepare("INSERT INTO characters (name) VALUES (?)");
    q.addBindValue(QString::fromStdString(character.getName()));
    if (!q.exec()) {
        std::cerr << "Failed to save character: "
                  << q.lastError().text().toStdString() << std::endl;
        return -1;
    }

    int characterId = q.lastInsertId().toInt();

    // Save each monster
    for (auto& m : character.getMonsters()) {
        QSqlQuery mq;
        mq.prepare("INSERT INTO monsters (character_id, name, hp, max_hp, strength) "
                   "VALUES (?, ?, ?, ?, ?)");
        mq.addBindValue(characterId);
        mq.addBindValue(QString::fromStdString(m.getName()));
        mq.addBindValue(m.getHp());
        mq.addBindValue(m.getMaxHp());
        mq.addBindValue(m.getStrength());
        mq.exec();

        int monsterId = mq.lastInsertId().toInt();

        // Save items belonging to this monster
        for (auto* item : m.getItems()) {
            QSqlQuery iq;
            iq.prepare("INSERT INTO monster_items (monster_id, item_type) VALUES (?, ?)");
            iq.addBindValue(monsterId);
            iq.addBindValue(static_cast<int>(item->getType()));
            iq.exec();
        }
    }

    return characterId;
}

std::vector<std::string> Database::listCharacterNames() {
    std::vector<std::string> names;
    QSqlQuery q("SELECT name FROM characters ORDER BY name");
    while (q.next()) {
        names.push_back(q.value(0).toString().toStdString());
    }
    return names;
}

Character* Database::loadCharacter(const std::string& name) {
    QSqlQuery cq;
    cq.prepare("SELECT id FROM characters WHERE name = ?");
    cq.addBindValue(QString::fromStdString(name));
    if (!cq.exec() || !cq.next()) return nullptr;

    int characterId = cq.value(0).toInt();
    Character* character = new Character(name);

    // Clear default starter monsters
    character->getMonsters().clear();

    QSqlQuery mq;
    mq.prepare("SELECT id, name, hp, max_hp, strength FROM monsters WHERE character_id = ?");
    mq.addBindValue(characterId);
    mq.exec();

    while (mq.next()) {
        int monsterId = mq.value(0).toInt();
        std::string mname = mq.value(1).toString().toStdString();
        int hp = mq.value(2).toInt();
        int maxHp = mq.value(3).toInt();
        int strength = mq.value(4).toInt();

        Monster m(mname, maxHp, strength);
        // Restore current HP
        m.takeDamage(maxHp - hp);

        // Load items for this monster
        QSqlQuery iq;
        iq.prepare("SELECT item_type FROM monster_items WHERE monster_id = ?");
        iq.addBindValue(monsterId);
        iq.exec();

        while (iq.next()) {
            int itemType = iq.value(0).toInt();
            m.addItem(new Item(static_cast<ItemType>(itemType)));
        }

        character->addMonster(m);
    }

    return character;
}

void Database::deleteCharacter(const std::string& name) {
    QSqlQuery q;
    q.prepare("DELETE FROM characters WHERE name = ?");
    q.addBindValue(QString::fromStdString(name));
    q.exec();
    // Cascade delete handles monsters and items
}

void Database::recordMonsterDefeated(int characterId, const std::string& monsterName) {
    QSqlQuery q;
    q.prepare("SELECT id, kill_count FROM monster_kills "
              "WHERE character_id = ? AND monster_name = ?");
    q.addBindValue(characterId);
    q.addBindValue(QString::fromStdString(monsterName));
    q.exec();

    if (q.next()) {
        int id = q.value(0).toInt();
        int count = q.value(1).toInt();
        QSqlQuery uq;
        uq.prepare("UPDATE monster_kills SET kill_count = ? WHERE id = ?");
        uq.addBindValue(count + 1);
        uq.addBindValue(id);
        uq.exec();
    } else {
        QSqlQuery iq;
        iq.prepare("INSERT INTO monster_kills (character_id, monster_name, kill_count) "
                   "VALUES (?, ?, 1)");
        iq.addBindValue(characterId);
        iq.addBindValue(QString::fromStdString(monsterName));
        iq.exec();
    }
}

void Database::recordItemUsed(int characterId, const std::string& itemName, bool causedKill) {
    QSqlQuery q;
    q.prepare("SELECT id, use_count, kill_count FROM item_usage "
              "WHERE character_id = ? AND item_name = ?");
    q.addBindValue(characterId);
    q.addBindValue(QString::fromStdString(itemName));
    q.exec();

    if (q.next()) {
        int id = q.value(0).toInt();
        int useCount = q.value(1).toInt();
        int killCount = q.value(2).toInt();
        QSqlQuery uq;
        uq.prepare("UPDATE item_usage SET use_count = ?, kill_count = ? WHERE id = ?");
        uq.addBindValue(useCount + 1);
        uq.addBindValue(killCount + (causedKill ? 1 : 0));
        uq.addBindValue(id);
        uq.exec();
    } else {
        QSqlQuery iq;
        iq.prepare("INSERT INTO item_usage (character_id, item_name, use_count, kill_count) "
                   "VALUES (?, ?, 1, ?)");
        iq.addBindValue(characterId);
        iq.addBindValue(QString::fromStdString(itemName));
        iq.addBindValue(causedKill ? 1 : 0);
        iq.exec();
    }
}

int Database::getTotalMonstersDefeated() {
    QSqlQuery q("SELECT SUM(kill_count) FROM monster_kills");
    if (q.next()) return q.value(0).toInt();
    return 0;
}

std::string Database::getMostUsedItem(int characterId) {
    QSqlQuery q;
    q.prepare("SELECT item_name FROM item_usage WHERE character_id = ? "
              "ORDER BY use_count DESC LIMIT 1");
    q.addBindValue(characterId);
    q.exec();
    if (q.next()) return q.value(0).toString().toStdString();
    return "None";
}

std::string Database::getMostUsedMonster(int characterId) {
    QSqlQuery q;
    q.prepare("SELECT monster_name FROM monster_kills WHERE character_id = ? "
              "ORDER BY kill_count DESC LIMIT 1");
    q.addBindValue(characterId);
    q.exec();
    if (q.next()) return q.value(0).toString().toStdString();
    return "None";
}

void Database::displayStatistics(int characterId) {
    std::cout << "\n=== STATISTICS ===" << std::endl;
    std::cout << "Total monsters defeated (all time): "
              << getTotalMonstersDefeated() << std::endl;
    std::cout << "Most used item: " << getMostUsedItem(characterId) << std::endl;
    std::cout << "Most defeated monster: " << getMostUsedMonster(characterId) << std::endl;
}
