package com.trovasdeveloper.bookstoreapp;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.content.Intent;
import android.content.res.AssetManager;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.widget.Button;

import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("bookstoreapp");
    }

    public native Book[] loadBooksFromAsset();
    public native Book[] getFavoriteBooks(Book[] allBooks);
    public native void initializeAssetManager(AssetManager assetManager);


    private Button filterButton;
    private boolean showingFavorites = false;
    private List<Book> bookList;
    private List<Book> favoriteBooks;
    private BookAdapter bookAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initializeAssetManager(getAssets());

        // Carregar a lista de livros e a lista de favoritos
        bookList = Arrays.asList(loadBooksFromAsset());
        favoriteBooks = Arrays.asList(getFavoriteBooks(bookList.toArray(new Book[0])));

        // Configurar o RecyclerView e o BookAdapter
        RecyclerView bookListRecyclerView = findViewById(R.id.book_list);
        bookListRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        bookAdapter = new BookAdapter(bookList, this::showBookDetails); // Inicializar o BookAdapter com a lista de livros
        bookListRecyclerView.setAdapter(bookAdapter); // Associar o adapter ao RecyclerView

        // Configurar o botão de filtro de favoritos
        filterButton = findViewById(R.id.filter_favorites);
        filterButton.setOnClickListener(view -> {
            showingFavorites = !showingFavorites;
            filterButton.setText(showingFavorites ? "Show All" : "Show Favorites");
            toggleFavorites();
        });
    }

    private void toggleFavorites() {
        List<Book> booksToShow = showingFavorites ? favoriteBooks : bookList;
        bookAdapter.updateBooks(booksToShow); // Atualiza a lista no Adapter
    }

    private void showBookDetails(Book book) {
        // Lógica para mostrar detalhes do livro
        Intent intent = new Intent(this, BookDetailActivity.class);
        intent.putExtra("title", book.title);
        intent.putExtra("authors", String.join(", ", book.authors));
        intent.putExtra("description", book.description);
        intent.putExtra("buyLink", book.buyLink);
        startActivity(intent);
    }
}