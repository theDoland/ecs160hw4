#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TWEETER_SIZE 20000
#define TOP_TWEET_SIZE 10

typedef struct {
    char *tweetname;
    int count;
} Tweeter;

typedef struct {
    int size;
    int count;
    Tweeter** tweets;
} HashTable;

HashTable* newHashTable();
static Tweeter* newTweeter(const char *tweetName);
static void deleteItem(Tweeter* item);
void deleteHashTable(HashTable* ht);
static int hash(const char *tweetName, int collisions);
void insertTweeter(HashTable* ht, const char *tweetName);
int getTweeter(HashTable* ht, const char *tweetName);

#endif // HASHTABLE_H