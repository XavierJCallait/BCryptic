#include <QDebug>

#include "database.h"

Database::Database() {
    sqlite3 *raw = nullptr;
    if (sqlite3_open(DATABASE_NAME, &raw) != SQLITE_OK) {
        qCritical() << "Error opening database: " << (raw ? sqlite3_errmsg(raw) : "Unknown error");
        if (raw) {
            sqlite3_close(raw);
        }
        throw std::runtime_error("Failed to open database");
    }
    database.reset(raw);

    Database::createTables();
}

void Database::createTables() {
    const char *create_question_answers_table = "CREATE TABLE IF NOT EXISTS " QUESTIONS_ANSWERS_TABLE " (id INTEGER PRIMARY KEY AUTOINCREMENT, question TEXT NOT NULL, answer TEXT NOT NULL);";
    if (sqlite3_exec(database.get(), create_question_answers_table, nullptr, nullptr, nullptr) != SQLITE_OK) {
        qCritical() << "Error creating question_answers table: " << sqlite3_errmsg(database.get());
        throw std::runtime_error("Failed to create questions_answers table");
    }
    const char *create_vault_table = "CREATE TABLE IF NOT EXISTS " VAULT_TABLE " (id INTEGER PRIMARY KEY AUTOINCREMENT, algorithm INTEGER NOT NULL, opslimit INTEGER NOT NULL, memlimit INTEGER NOT NULL, salt BLOB NOT NULL, nonce BLOB NOT NULL, encrypted_vk BLOB NOT NULL);";
    if (sqlite3_exec(database.get(), create_vault_table, nullptr, nullptr, nullptr) != SQLITE_OK) {
        qCritical() << "Error creating vault table: " << sqlite3_errmsg(database.get());
        throw std::runtime_error("Failed to create vault table");
    }
}

int Database::countTables() {
    const char *count_tables = "SELECT COUNT(*) FROM sqlite_master WHERE type = 'table';"; //  AND name NOT LIKE 'sqlite_%'
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(database.get(), count_tables, -1, &stmt, nullptr) != SQLITE_OK) {
        qCritical() << "Error preparing statement: " << sqlite3_errmsg(database.get());
        return -1;
    }

    int tableCount = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        tableCount = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return tableCount;
}

bool Database::storeQuestions(const std::list<std::pair<std::string, std::string>>& iter) {
    if (!database) {
        return false;
    }

    const char *insert_questions_answers = "INSERT INTO " QUESTIONS_ANSWERS_TABLE " (question, answer) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    for (const auto& pair : iter) {
        if (sqlite3_prepare_v2(database.get(), insert_questions_answers, -1, &stmt, nullptr) != SQLITE_OK) {
            qCritical() << "Error preparing statement: " << sqlite3_errmsg(database.get());
            return false;
        }
        sqlite3_bind_text(stmt, 1, pair.first.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, pair.second.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            qCritical() << "Error executing statement: " << sqlite3_errmsg(database.get());
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_finalize(stmt);
    }
    return true;
}

bool Database::fetchQuestions(const std::array<int, 3> &indices, std::array<std::string, 3> &questions) {
    if (!database) {
        return false;
    }

    const char *select_questions = "SELECT answer FROM " QUESTIONS_ANSWERS_TABLE " WHERE id IN (?, ?, ?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(database.get(), select_questions, -1, &stmt, nullptr) != SQLITE_OK) {
        qCritical() << "Error preparing statement: " << sqlite3_errmsg(database.get());
        return false;
    }

    for (int i = 0; i < 3; ++i) {
        if (sqlite3_bind_int(stmt, i + 1, indices[i]) != SQLITE_OK) {
            qWarning() << "Failed to bind parameter" << i + 1 << ":" << sqlite3_errmsg(database.get());
            sqlite3_finalize(stmt);
            return false;
        }
    }

    // Execute and fetch
    int index = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && index < 3) {
        const unsigned char *answer = sqlite3_column_text(stmt, 0);
        if (answer) {
            questions[index] = reinterpret_cast<const char *>(answer);
        }
        index++;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool Database::storeVault(ArgonParams &argonParams, VaultParams &vaultParams) {
    if (!database) {
        return false;
    }

    const char *insert_vault = "INSERT INTO " VAULT_TABLE " (algorithm, opslimit, memlimit, salt, nonce, encrypted_vk) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(database.get(), insert_vault, -1, &stmt, nullptr) != SQLITE_OK) {
        qCritical() << "Error preparing statement: " << sqlite3_errmsg(database.get());
        return false;
    }
    sqlite3_bind_int(stmt, 1, argonParams.algorithm);
    sqlite3_bind_int64(stmt, 2, argonParams.opslimit);
    sqlite3_bind_int64(stmt, 3, argonParams.memlimit);
    sqlite3_bind_blob(stmt, 4, argonParams.salt.data(), argonParams.salt.size(), SQLITE_TRANSIENT);
    sqlite3_bind_blob(stmt, 5, vaultParams.aead_nonce.data(), vaultParams.aead_nonce.size(), SQLITE_TRANSIENT);
    sqlite3_bind_blob(stmt, 6, vaultParams.encrypted_vk.data(), vaultParams.encrypted_vk.size(), SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        qCritical() << "Error executing statement: " << sqlite3_errmsg(database.get());
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);

    return true;
}

bool Database::fetchVault(ArgonParams &argonParams, VaultParams &vaultParams) {
    if (!database) {
        return false;
    }

    const char *select_vault = "SELECT * FROM " VAULT_TABLE " ORDER BY id ASC LIMIT 1;";
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(database.get(), select_vault, -1, &stmt, nullptr) != SQLITE_OK) {
        qCritical() << "Error preparing statment: " << sqlite3_errmsg(database.get());
        return false;
    }

    bool success = false;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        argonParams.algorithm = sqlite3_column_int(stmt, 1);
        argonParams.opslimit  = static_cast<unsigned long long>(sqlite3_column_int64(stmt, 2));
        argonParams.memlimit  = static_cast<size_t>(sqlite3_column_int64(stmt, 3));

        const void *salt_ptr = sqlite3_column_blob(stmt, 4);
        int salt_size        = sqlite3_column_bytes(stmt, 4);
        argonParams.salt.assign(
            static_cast<const unsigned char*>(salt_ptr),
            static_cast<const unsigned char*>(salt_ptr) + salt_size
        );

        const void *nonce_ptr = sqlite3_column_blob(stmt, 5);
        int nonce_size        = sqlite3_column_bytes(stmt, 5);
        vaultParams.aead_nonce.assign(
            static_cast<const unsigned char*>(nonce_ptr),
            static_cast<const unsigned char*>(nonce_ptr) + nonce_size
        );

        const void *vk_ptr = sqlite3_column_blob(stmt, 6);
        int vk_size        = sqlite3_column_bytes(stmt, 6);
        vaultParams.encrypted_vk.assign(
            static_cast<const unsigned char*>(vk_ptr),
            static_cast<const unsigned char*>(vk_ptr) + vk_size
        );
        success = true;
    } else {
        qCritical() << "No vault rows found";
    }

    sqlite3_finalize(stmt);

    return success;
}

bool Database::doesTableExists(const std::string &tableName) {
    const char *check_table = "SELECT name FROM sqlite_master WHERE type = 'table' AND name = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(database.get(), check_table, -1, &stmt, nullptr) != SQLITE_OK) {
        qCritical() << "Error preparing statement:" << sqlite3_errmsg(database.get());
        return false;
    }

    sqlite3_bind_text(stmt, 1, tableName.c_str(), -1, SQLITE_TRANSIENT);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = true;
    }

    sqlite3_finalize(stmt);
    return exists;
}

// sanitize input
bool Database::createDatabaseTable(const std::string &tableName) {
    std::string new_database_table = "CREATE TABLE IF NOT EXISTS " + tableName + " (id INTEGER PRIMARY KEY AUTOINCREMENT, title VARCHAR(128) NOT NULL, url VARCHAR(128) NOT NULL, notes TEXT);";
    if (sqlite3_exec(database.get(), new_database_table.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        qCritical() << "Error creating database table: " << sqlite3_errmsg(database.get());
        return false;
    }
    return true;
}

bool Database::getTableNames(std::vector<std::string> &tableNames) {
    const char *get_table_names = "SELECT name FROM sqlite_master WHERE type = 'table' AND name NOT IN ('vault', 'questions_answers', 'sqlite_sequence');";
    
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(database.get(), get_table_names, -1, &stmt, nullptr) != SQLITE_OK) {
        qCritical() << "Error preparing statement:" << sqlite3_errmsg(database.get());
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *name = sqlite3_column_text(stmt, 0);
        if (name) {
            std::string tableName(reinterpret_cast<const char *>(name));
            qDebug() << tableName;
            tableNames.emplace_back(tableName);
        }
    }

    sqlite3_finalize(stmt);
    return true;
}
