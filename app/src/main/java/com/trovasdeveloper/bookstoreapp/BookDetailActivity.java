package com.trovasdeveloper.bookstoreapp;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

public class BookDetailActivity extends AppCompatActivity {

    private native void addBookToFavoritesNative(String title,
                                                 String description,
                                                 String buyLink,
                                                 String[] authors);
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_book_detail);

        //Action Bar
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setTitle("Book Details");
        }

        TextView titleTextView = findViewById(R.id.book_detail_title);
        TextView authorsTextView = findViewById(R.id.book_detail_authors);
        TextView descriptionTextView = findViewById(R.id.book_detail_description);
        Button buyButton = findViewById(R.id.buy_button_detail);

        Intent intent = getIntent();
        String title = intent.getStringExtra("title");
        String authors = intent.getStringExtra("authors");
        String description = intent.getStringExtra("description");
        String buyLink = intent.getStringExtra("buyLink");

        titleTextView.setText(title);
        authorsTextView.setText(authors);
        descriptionTextView.setText(description);

        buyButton.setOnClickListener(v -> {
            Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(buyLink));
            startActivity(browserIntent);
        });

        Button addToFavoritesButton = findViewById(R.id.button_add_to_favorites);
        addToFavoritesButton.setOnClickListener(v -> {
            String[] authorsArray = authors.split(",");
            addBookToFavoritesNative(title, description, buyLink, authorsArray);
        });

    }
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            finish();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}