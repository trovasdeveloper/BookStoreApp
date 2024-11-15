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

// Getters
    const std::string& getTitle() const { return title; }
    const std::vector<std::string>& getAuthors() const { return authors; }
    const std::string& getDescription() const { return description; }
    const std::string& getBuyLink() const { return buyLink; }

private:
    std::string title;
    std::vector<std::string> authors;
    std::string description;
    std::string buyLink;
};