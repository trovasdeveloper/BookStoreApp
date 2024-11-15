#include <jni.h>
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <vector>
#include <iostream>
#include "include/Book.h"
#include "json.hpp"

using json = nlohmann::json;
AAssetManager* assetManager = nullptr;

extern "C" {

// Asset Manager
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

    // Open JSON in assets
    AAsset* asset = AAssetManager_open(assetManager, "books.json", AASSET_MODE_BUFFER);
    if (!asset) {
        std::cerr << "Não foi possível abrir o arquivo JSON!" << std::endl;
        return nullptr;
    }

    //JSON
    size_t fileSize = AAsset_getLength(asset);
    std::string fileContent(fileSize, '\0');
    AAsset_read(asset, &fileContent[0], fileSize);
    AAsset_close(asset);

    // Parse JSON
    json jsonData = json::parse(fileContent);
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

    std::vector<jobject> bookObjects;
    for (const auto& item : jsonData) {
        std::string title = item.value("title", "");
        std::string description = item.value("description", "");
        std::string buyLink = item.value("buyLink", "");

        // ArrayList Authors
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

        // Book Object
        jobject bookObject = env->NewObject(
                bookClass,
                bookConstructor,
                env->NewStringUTF(title.c_str()),
                authorsList,
                env->NewStringUTF(description.c_str()),
                env->NewStringUTF(buyLink.c_str()),
                jIsFavorite);

        bookObjects.push_back(bookObject);
        env->DeleteLocalRef(authorsList);
    }

    jobjectArray bookArray = env->NewObjectArray(bookObjects.size(), bookClass, nullptr);
    for (size_t i = 0; i < bookObjects.size(); ++i) {
        env->SetObjectArrayElement(bookArray, i, bookObjects[i]);
        env->DeleteLocalRef(bookObjects[i]); // Libera a referência do livro após inserir no array
    }
    return bookArray;
}

// JNI to return Favorite List
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

// TODO: SetFavoriteBook JNI

}