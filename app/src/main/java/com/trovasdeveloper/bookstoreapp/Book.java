package com.trovasdeveloper.bookstoreapp;

import java.util.List;


public class Book {
    private int id;
    public String title;
    public List<String> authors;
    public String description;
    public String buyLink;
    public boolean isFavorite;
    public Book(String title,
                List<String> authors,
                String description,
                String buyLink,
                boolean isFavorite)
    {
        this.title = title;
        this.authors = authors;
        this.description = description;
        this.buyLink = buyLink;
        this.isFavorite = isFavorite;
    }
    public String getTitle() {
        return title;
    }

    public List<String> getAuthors() {
        return authors;
    }

    public String getDescription() {
        return description;
    }

    public String getBuyLink() {
        return buyLink;
    }

    public boolean isFavorite() {
        return isFavorite;
    }

    public void setFavorite(boolean favorite) {
        this.isFavorite = favorite;
    }
}