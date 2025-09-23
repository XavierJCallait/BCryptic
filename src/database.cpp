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
    const char *create_salts_table = "CREATE TABLE IF NOT EXISTS " SALTS_TABLE " (id INTEGER PRIMARY KEY AUTOINCREMENT, usage TEXT NOT NULL, salt TEXT NOT NULL, esalt1 TEXT NOT NULL, esalt2 TEXT NOT NULL, esalt3 TEXT NOT NULL);";
    if (sqlite3_exec(database.get(), create_salts_table, nullptr, nullptr, nullptr) != SQLITE_OK) {
        qCritical() << "Error creating salts table: " << sqlite3_errmsg(database.get());
        throw std::runtime_error("Failed to create salts table");
    }
    const char *create_vault_table = "CREATE TABLE IF NOT EXISTS " VAULT_TABLE " (id INTEGER PRIMARY KEY AUTOINCREMENT, algorithm INTEGER NOT NULL, opslimit INTEGER NOT NULL, memlimit INTEGER NOT NULL, salt BLOB NOT NULL, nonce BLOB NOT NULL, encrypted_vk BLOB NOT NULL);";
    if (sqlite3_exec(database.get(), create_vault_table, nullptr, nullptr, nullptr) != SQLITE_OK) {
        qCritical() << "Error creating vault table: " << sqlite3_errmsg(database.get());
        throw std::runtime_error("Failed to create vault table");
    }
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
    sqlite3_bind_blob(stmt, 4, argonParams.salt.data(), argonParams.salt.size(), SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 5, vaultParams.aead_nonce.data(), vaultParams.aead_nonce.size(), SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 6, vaultParams.encrypted_vk.data(), vaultParams.encrypted_vk.size(), SQLITE_STATIC);
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