#include <jni.h>
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <vector>
#include <iostream>
#include "include/Book.h"
#include "json.hpp" // Supondo que você esteja usando nlohmann/json

using json = nlohmann::json;
AAssetManager* assetManager = nullptr;

extern "C" {

// Inicializa o AssetManager a partir do Java para ser usado no C++
JNIEXPORT void JNICALL
Java_com_trovasdeveloper_bookstoreapp_MainActivity_initializeAssetManager(JNIEnv* env, jobject /* this */, jobject javaAssetManager) {
    assetManager = AAssetManager_fromJava(env, javaAssetManager);
}

JNIEXPORT jobjectArray JNICALL
Java_com_trovasdeveloper_bookstoreapp_MainActivity_loadBooksFromAsset(JNIEnv *env, jobject /* this */) {
    if (!assetManager) {
        std::cerr << "AssetManager não inicializado!" << std::endl;
        return nullptr;
    }

    // Abrindo o arquivo JSON em assets
    AAsset* asset = AAssetManager_open(assetManager, "books.json", AASSET_MODE_BUFFER);
    if (!asset) {
        std::cerr << "Não foi possível abrir o arquivo JSON!" << std::endl;
        return nullptr;
    }

    // Lendo o conteúdo do arquivo JSON
    size_t fileSize = AAsset_getLength(asset);
    std::string fileContent(fileSize, '\0');
    AAsset_read(asset, &fileContent[0], fileSize);
    AAsset_close(asset);

    // Parse JSON para vetor de livros
    json jsonData = json::parse(fileContent);

    // Obter a classe `Book` e seu construtor no lado Java
    jclass bookClass = env->FindClass("com/trovasdeveloper/bookstoreapp/Book");
    if (!bookClass) {
        std::cerr << "Classe Book não encontrada no Java" << std::endl;
        return nullptr;
    }

    jmethodID bookConstructor = env->GetMethodID(bookClass, "<init>", "(Ljava/lang/String;Ljava/util/List;Ljava/lang/String;Ljava/lang/String;Z)V");
    if (!bookConstructor) {
        std::cerr << "Construtor Book não encontrado" << std::endl;
        return nullptr;
    }

    // Criar lista de objetos Book
    std::vector<jobject> bookObjects;
    for (const auto& item : jsonData) {
        std::string title = item.value("title", "");
        std::string description = item.value("description", "");
        std::string buyLink = item.value("buyLink", "");

        // Criando o ArrayList de autores em Java
        jclass arrayListClass = env->FindClass("java/util/ArrayList");
        jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
        jobject authorsList = env->NewObject(arrayListClass, arrayListConstructor);

        jmethodID arrayListAddMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
        for (const auto& author : item["authors"]) {
            jstring authorStr = env->NewStringUTF(author.get<std::string>().c_str());
            env->CallBooleanMethod(authorsList, arrayListAddMethod, authorStr);
            env->DeleteLocalRef(authorStr); // Libera a referência local do autor
        }

        jboolean jIsFavorite = item.value("isFavorite", false);

        // Criando o objeto Book
        jobject bookObject = env->NewObject(
                bookClass,
                bookConstructor,
                env->NewStringUTF(title.c_str()),
                authorsList,
                env->NewStringUTF(description.c_str()),
                env->NewStringUTF(buyLink.c_str()),
                jIsFavorite);

        // Adicionando o objeto Book ao array de objetos
        bookObjects.push_back(bookObject);

        // Libera a referência local do authorsList
        env->DeleteLocalRef(authorsList);
    }

    // Criação do array de livros para retornar
    jobjectArray bookArray = env->NewObjectArray(bookObjects.size(), bookClass, nullptr);
    for (size_t i = 0; i < bookObjects.size(); ++i) {
        env->SetObjectArrayElement(bookArray, i, bookObjects[i]);
        env->DeleteLocalRef(bookObjects[i]); // Libera a referência do livro após inserir no array
    }

    return bookArray;
}



// Função JNI para retornar os livros favoritos
JNIEXPORT jobjectArray JNICALL
Java_com_trovasdeveloper_bookstoreapp_MainActivity_getFavoriteBooks(JNIEnv *env, jobject /* this */, jobjectArray allBooks) {
    jclass bookClass = env->FindClass("com/trovasdeveloper/bookstoreapp/Book");
    jfieldID isFavoriteField = env->GetFieldID(bookClass, "isFavorite", "Z");

    std::vector<jobject> favoriteBooks;

    int bookCount = env->GetArrayLength(allBooks);
    for (int i = 0; i < bookCount; i++) {
        jobject bookObject = env->GetObjectArrayElement(allBooks, i);
        jboolean isFavorite = env->GetBooleanField(bookObject, isFavoriteField);
        if (isFavorite) {
            favoriteBooks.push_back(bookObject);
        }
    }

    jobjectArray favoriteBooksArray = env->NewObjectArray(favoriteBooks.size(), bookClass, nullptr);
    for (size_t i = 0; i < favoriteBooks.size(); ++i) {
        env->SetObjectArrayElement(favoriteBooksArray, i, favoriteBooks[i]);
    }

    return favoriteBooksArray;
}

}