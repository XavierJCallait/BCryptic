#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <list>
#include <sqlite3.h>

#include "vault.h"

#define QUESTIONS_ANSWERS_TABLE "questions_answers"
#define SALTS_TABLE "salts"
#define VAULT_TABLE "vault"
#define DATABASE_NAME "mydb.sqlite"

class Database {

public:
    static Database& getInstance() {
        static Database instance;
        return instance;
    }

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    Database(Database&&) = delete;
    Database& operator=(Database&&) = delete;

    void createTables();
    bool storeQuestions(const std::list<std::pair<std::string, std::string>>& iter);
    bool storeVault(ArgonParams &argonParams, VaultParams &vaultParams);
    bool fetchVault(ArgonParams &argonParams, VaultParams &vaultParams);

private:
    Database();
    ~Database() = default;

	  struct SQLiteCloser {
        void operator()(sqlite3* db) const {
            if (db) sqlite3_close(db);
        }
    };
    std::unique_ptr<sqlite3, SQLiteCloser> database;
};

#endif