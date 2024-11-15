package com.trovasdeveloper.bookstoreapp;

import androidx.room.Entity;
import androidx.room.PrimaryKey;
import androidx.room.ColumnInfo;

@Entity(tableName = "books")
public class BookEntity {
    @PrimaryKey(autoGenerate = true)
    public int id;

    public String title;

    @ColumnInfo(name = "authors")
    public String authors;

    public String description;
    public String buyLink;

    @ColumnInfo(name = "is_favorite")
    public boolean isFavorite;

        public BookEntity(String title, String authors, String description, String buyLink, boolean isFavorite) {
        this.title = title;
        this.authors = authors;
        this.description = description;
        this.buyLink = buyLink;
        this.isFavorite = isFavorite;
    }
}
