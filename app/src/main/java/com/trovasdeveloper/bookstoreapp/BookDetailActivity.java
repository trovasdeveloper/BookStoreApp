package com.trovasdeveloper.bookstoreapp;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

public class BookDetailActivity extends AppCompatActivity {

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_book_detail);

        TextView titleTextView = findViewById(R.id.book_detail_title);
        TextView authorsTextView = findViewById(R.id.book_detail_authors);
        TextView descriptionTextView = findViewById(R.id.book_detail_description);
        Button buyButton = findViewById(R.id.buy_button_detail);

        // Recebe os dados do Intent
        Intent intent = getIntent();
        String title = intent.getStringExtra("title");
        String authors = intent.getStringExtra("authors");
        String description = intent.getStringExtra("description");
        String buyLink = intent.getStringExtra("buyLink");

        titleTextView.setText(title);
        authorsTextView.setText(authors);
        descriptionTextView.setText(description);

        // Configura o botão de compra
        buyButton.setOnClickListener(v -> {
            Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(buyLink));
            startActivity(browserIntent);
        });
    }
}
