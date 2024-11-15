//
// Created by andre on 11/15/2024.
//
#include <jni.h>
#include "include/BookDatabase.h"
#include "include/Book.h"
#include <sqlite3.h>
#include <sstream>

BookDatabase::BookDatabase(const std::string& dbPath)
{
    sqlite3_open(dbPath.c_str(), &db);
}

BookDatabase::~BookDatabase()
{
    if (db){
        sqlite3_close(db);
    }
}

extern "C"
{
JNIEXPORT void JNICALL
Java_com_trovasdeveloper_bookstoreapp_BookDetailActivity_addBookToFavoritesNative(JNIEnv *env,
                                                                                  jobject /* this */,
                                                                                  jstring title,
                                                                                  jstring description,
                                                                                  jstring buyLink,
                                                                                  jobjectArray authors)
{
    const char *nativeTitle = env->GetStringUTFChars(title, 0);
    const char *nativeDescription = env->GetStringUTFChars(description, 0);
    const char *nativeBuyLink = env->GetStringUTFChars(buyLink, 0);

    std::vector<std::string> nativeAuthors;
    jsize authorsLength = env->GetArrayLength(authors);
    for (jsize i = 0; i < authorsLength; ++i) {
        jstring author = (jstring) env->GetObjectArrayElement(authors, i);
        const char *nativeAuthor = env->GetStringUTFChars(author, 0);
        nativeAuthors.push_back(nativeAuthor);
        env->ReleaseStringUTFChars(author, nativeAuthor);
    }

    Book book(nativeTitle, nativeAuthors, nativeDescription, nativeBuyLink);
    BookDatabase bookDatabase(
            "/data/data/com.trovasdeveloper.bookstoreapp/databases/bookstore.db");
    bookDatabase.addBookToFavorites(book);

    env->ReleaseStringUTFChars(title, nativeTitle);
    env->ReleaseStringUTFChars(description, nativeDescription);
    env->ReleaseStringUTFChars(buyLink, nativeBuyLink);
}

// Not used
void BookDatabase::setFavorite(int bookId, bool isFavorite) {
    const char *sql = "UPDATE books SET is_favorite = ? WHERE id = ?";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, isFavorite ? 1 : 0);
    sqlite3_bind_int(stmt, 2, bookId);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

std::string joinAuthors(const std::vector<std::string> &authors) {
    std::ostringstream oss;
    for (size_t i = 0; i < authors.size(); ++i) {
        oss << authors[i];
        if (i < authors.size() - 1) {
            oss << ", ";
        }
    }
    return oss.str();
}

void BookDatabase::addBookToFavorites(const Book &book)
{
    const char *sql = "INSERT INTO favorites (title, authors, description, buy_link) VALUES (?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, book.getTitle().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, joinAuthors(book.getAuthors()).c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, book.getDescription().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, book.getBuyLink().c_str(), -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

}