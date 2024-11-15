//
// Created by andre on 11/15/2024.
//

#ifndef BOOKDATABASE_H
#define BOOKDATABASE_H

#include "Book.h"

#include <string>
#include <sqlite3.h>

class BookDatabase
{
public:
    BookDatabase(const std::string& dbPath);
    ~BookDatabase();

    void setFavorite(int bookId, bool isFavorite);
    void addBookToFavorites(const Book& book);

    struct sqlite3* db;
};

#endif // BOOKDATABASE_H
