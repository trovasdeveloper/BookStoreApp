#pragma once

#include "Book.h"
#include "curl/android-21-x86_64/include/curl/curl.h"

#include <string>
#include <memory>
#include <vector>


class HttpClient
{
public:
    HttpClient();
    ~HttpClient();

    std::string get(const std::string& url);
    std::vector<Book> fetchBooks(const std::string& query);
private:
    void initCurl();
    void cleanupCurl();

    CURL* curl;
};

