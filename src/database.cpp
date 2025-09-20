#include <QDebug>

#include "database.h"

Database::Database() {
    sqlite3* raw = nullptr;
    if (sqlite3_open(DATABASE_NAME, &raw) != SQLITE_OK) {
        qCritical() << "Error opening database: " << (raw ? sqlite3_errmsg(raw) : "Unknown error");
        if (raw) {
            sqlite3_close(raw);
        }
        throw std::runtime_error("Failed to open database");
    }
    database.reset(raw);

    std::string create_question_answers_table = "CREATE TABLE IF NOT EXISTS questions_answers (id INTEGER PRIMARY KEY AUTOINCREMENT, question TEXT NOT NULL, answer TEXT NOT NULL);";
    if (sqlite3_exec(database.get(), create_question_answers_table.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        qCritical() << "Error creating question_answers table: " << sqlite3_errmsg(database.get());
        throw std::runtime_error("Failed to create questions_answers table");
    }
    std::string create_salts_table = "CREATE TABLE IF NOT EXISTS salts (id INTEGER PRIMARY KEY AUTOINCREMENT, usage TEXT NOT NULL, salt TEXT NOT NULL, esalt1 TEXT NOT NULL, esalt2 TEXT NOT NULL, esalt3 TEXT NOT NULL);";
    if (sqlite3_exec(database.get(), create_salts_table.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        qCritical() << "Error creating salts table: " << sqlite3_errmsg(database.get());
        throw std::runtime_error("Failed to create salts table");
    }
}

bool Database::storeQuestions(const std::list<std::pair<std::string, std::string>>& iter) {
    if (!database) {
        return false;
    }

    std::string insert_questions_answers = "INSERT INTO " QUESTIONS_ANSWERS_TABLE " (question, answer) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    for (const auto& pair : iter) {
        if (sqlite3_prepare_v2(database.get(), insert_questions_answers.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
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