#include "HttpClient.h"
#include "Book.h"

#include "nlohmann/json.hpp"
#include "curl/android-21-x86_64/include/curl/curl.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <vector>

using json = nlohmann::json;


static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s)
{
    size_t newLength = size * nmemb;
    s->append((char*)contents, newLength);
    return newLength;
}

HttpClient::HttpClient()
{
    initCurl();
}

HttpClient::~HttpClient() {
    cleanupCurl();
}


void HttpClient::initCurl()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();
    if (!curl)
    {
        throw std::runtime_error("Failed to initialize cURL");
    }
}

void HttpClient::cleanupCurl()
{
    curl_global_cleanup();
}

//HTTP request using libcurl
std::string HttpClient::get(const std::string& url)
{
    curl= curl_easy_init();
    std::string readBuffer;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::string error_msg = "curl_easy_perform() failed: " + std::string(curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            throw std::runtime_error(error_msg);
        }
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

//Construct API GoogleBooks to request JSON response
std::vector<Book> HttpClient::fetchBooks(const std::string& query)
{
    std::ostringstream oss ;
    oss << "https://www.googleapis.com/books/v1/volumes?q=" << query << "&maxResults=20&startIndex=0";
    std::string url = oss.str();
    std::string response = get(url);

    std::vector<Book> books;

    try {
        json jsonResponse = json::parse(response);
        if (jsonResponse.contains("items")) {
            for (const auto& item : jsonResponse["items"]) {
                std::string title = item["volumeInfo"]["title"];
                std::vector<std::string> authors = item["volumeInfo"]["authors"].get<std::vector<std::string>>();
                std::string description = item["volumeInfo"].value("description", "");
                std::string buyLink = item["volumeInfo"]["saleInfo"].value("buyLink", "");

                books.emplace_back(title, authors, description, buyLink);
            }
        }
    }
    catch (json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        // TODO
    }

    return books;

}