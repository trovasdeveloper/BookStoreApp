package com.trovasdeveloper.bookstoreapp;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import android.util.Log;
import android.content.Intent;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.widget.Button;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("bookstoreapp");
    }
    private static final String TAG = "MainActivity";
    public native Book[] loadBooksFromAsset();
    public native Book[] getFavoriteBooks(Book[] allBooks);
    public native void initializeAssetManager(AssetManager assetManager);
    private Button filterButton;
    private boolean showingFavorites = false;
    private List<Book> bookList;
    private BookAdapter bookAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initializeAssetManager(getAssets());

        // load book list
        bookList = Arrays.asList(loadBooksFromAsset());

        //RecyclerView
        RecyclerView bookListRecyclerView = findViewById(R.id.book_list);
        bookListRecyclerView.setLayoutManager(new LinearLayoutManager(this));

        // BookAdapter Init
        bookAdapter = new BookAdapter(bookList, this::showBookDetails);
        bookListRecyclerView.setAdapter(bookAdapter);

        // Filter Button
        filterButton = findViewById(R.id.filter_favorites);
        filterButton.setOnClickListener(view -> {
            filterButton.setText(showingFavorites ? "Show Favorites" : "Show All");
            toggleFavorites();
        });
    }

    private void toggleFavorites() {
        showingFavorites = !showingFavorites;
        Log.d(TAG, "toggleFavorites() - " + showingFavorites);
        updateBookList();
    }

    private void updateBookList() {
        List<Book> displayList;
      if(showingFavorites) {
          displayList = new ArrayList<>();
          for(Book book : bookList) {
              if(book.isFavorite()) {
                  displayList.add(book);
              }
          }
      }
      else {
          displayList = new ArrayList<>(bookList); // Cria uma cópia mutável da lista original
      }
        bookAdapter.updateBooks(displayList); // Atualiza o adapter com a lista apropriada
    }

    private void showBookDetails(Book book) {
        Intent intent = new Intent(this, BookDetailActivity.class);
        intent.putExtra("title", book.title);
        intent.putExtra("authors", String.join(", ", book.authors));
        intent.putExtra("description", book.description);
        intent.putExtra("buyLink", book.buyLink);
        startActivity(intent);
    }
}