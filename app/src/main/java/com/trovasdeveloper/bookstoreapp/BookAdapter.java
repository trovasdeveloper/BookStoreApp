package com.trovasdeveloper.bookstoreapp;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;
import java.util.List;

public class BookAdapter extends RecyclerView.Adapter<BookAdapter.BookViewHolder>
{

    private List<Book> books; // Removido o 'final' para permitir atualizações
    private final OnBookClickListener listener;

    public BookAdapter(List<Book> books, OnBookClickListener listener) {
        this.books = books;
        this.listener = listener;
    }

    @NonNull
    @Override
    public BookViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.book_item, parent, false);
        return new BookViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull BookViewHolder holder, int position) {
        holder.bind(books.get(position), listener);
    }

    @Override
    public int getItemCount() {
        return books.size();
    }

    public static class BookViewHolder extends RecyclerView.ViewHolder {
        private final TextView titleTextView;
        private final TextView authorsTextView;
        private final TextView descriptionTextView;
        private final Button buyButton;

        public BookViewHolder(@NonNull View itemView) {
            super(itemView);
            titleTextView = itemView.findViewById(R.id.book_title);
            authorsTextView = itemView.findViewById(R.id.book_authors);
            descriptionTextView = itemView.findViewById(R.id.book_description);
            buyButton = itemView.findViewById(R.id.buy_button);
        }

        public void bind(final Book book, final OnBookClickListener listener) {
            titleTextView.setText(book.title);
            authorsTextView.setText(String.join(", ", book.authors));
            descriptionTextView.setText(book.description);

            // Abre o link de compra no navegador quando o botão é clicado
            buyButton.setOnClickListener(v -> {
                Context context = v.getContext();
                Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(book.buyLink));
                context.startActivity(browserIntent);
            });

            // Configura o clique no item para acionar o listener
            itemView.setOnClickListener(v -> {
                if (listener != null) {
                    listener.onBookClick(book);
                }
            });

            itemView.setOnClickListener(v -> {
                Intent intent = new Intent(v.getContext(), BookDetailActivity.class);
                intent.putExtra("title", book.title);
                intent.putExtra("authors", String.join(", ", book.authors));
                intent.putExtra("description", book.description);
                intent.putExtra("buyLink", book.buyLink);
                v.getContext().startActivity(intent);
            });
        }
    }

    public interface OnBookClickListener {
        void onBookClick(Book book);
    }

    // Método para atualizar a lista de livros
    public void updateBooks(List<Book> newBooks) {
        books = new ArrayList<>(newBooks);
        notifyDataSetChanged();
    }
/*
    // Atualiza o status de favorito de um livro específico
    public void updateBookFavoriteStatus(Book updatedBook) {
        int index = -1;
        for (int i = 0; i < books.size(); i++) {
            if (books.get(i).getId() == updatedBook.getId()) {
                index = i;
                break;
            }
        }
        if (index != -1) {
            books.set(index, updatedBook);
            notifyItemChanged(index);
        }
    }*/

}
