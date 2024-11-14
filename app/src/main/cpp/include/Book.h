#pragma once


#include <string>
#include <vector>

class Book
{
public: 
    Book(
        const std::string& title,
        const std::vector<std::string>& authors,
        const std::string& description,
        const std::string& buyLink) : title(title), authors(authors), description(description), buyLink(buyLink) {}

    std::string title;
    std::vector<std::string> authors;
    std::string description;
    std::string buyLink;
};