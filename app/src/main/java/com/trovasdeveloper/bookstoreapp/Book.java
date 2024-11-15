package com.trovasdeveloper.bookstoreapp;

import java.util.List;

public class Book {
    public String title;
    public List<String> authors;
    public String description;
    public String buyLink;
    public boolean isFavorite;

    public Book(String title, List<String> authors, String description, String buyLink, boolean isFavorite) {
        this.title = title;
        this.authors = authors;
        this.description = description;
        this.buyLink = buyLink;
        this.isFavorite = isFavorite;
    }

    // Método para verificar se é favorito
    public boolean isFavorite() {
        return isFavorite;
    }

    // Método para definir como favorito
    public void setFavorite(boolean favorite) {
        this.isFavorite = favorite;
    }
}
