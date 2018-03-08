#include "hashTable.h"
// Inspiration from: https://github.com/jamesroutley/write-a-hash-table/tree/master/02-hash-table

// initialize and return a new hash table object
static Tweeter* newTweeter(const char *tweetName){
    Tweeter* item = malloc(sizeof(Tweeter));

    // store the name and start the count
    item->tweetname = strdup(tweetName);
    item->count = 1;
    return item;
}

// Initializes the hashtable 
HashTable* newHashTable() {
    HashTable* ht = malloc(sizeof(HashTable));

    // Maximum hashtable size is 20000
    ht->size = MAX_TWEETER_SIZE;
    ht->count = 0;
    // initialize entire hashtable to zeroes
    ht->tweets = calloc((size_t)ht->size, sizeof(Tweeter*));
}

// Deletes each item
static void deleteItem(Tweeter* item){
    free(item->tweetname);
    free(item);
}

// Delete hash table and all of its elements
void deleteHashTable(HashTable* ht){
    // loop through each element in the hash
    for(int i = 0; i < ht->size; i++){
        Tweeter* tweet = ht->tweets[i];
        if(tweet != NULL){
            deleteItem(tweet);
        }
    }
    free(ht->tweets);
    free(ht);
}
// Hash function, collisions start at 0 and increment until success
static int hash(const char *tweetName, int collisions){
    int hash = 0;

    // convert the string to an ascii value for the hash
    for(int i = 0; i < strlen(tweetName); i++){
        hash += (int)tweetName[i];
    }
    // placeholder hash function
    return (hash + collisions) % 20000;
}

// Insert the object at hash location + # of collisions
void insertTweeter(HashTable* ht, const char *tweetName) {
    Tweeter* item = newTweeter(tweetName);
    int collisions = 0;
    int index = hash(item->tweetname, collisions);
    Tweeter* tempItem = ht->tweets[index];

    // loop until you find a free index
    while(tempItem != NULL){
        index = hash(item->tweetname, ++collisions);
        tempItem = ht->tweets[index];
    }

    // place the item in the hash and increment hashtable count
    ht->tweets[index] = item;
    ht->count++;

}

// Return the index of the hashTable so we can increment the count
int getTweeter(HashTable* ht, const char *tweetName) {
    int collisions = 0;
    int index = hash(tweetName, collisions);
    Tweeter* temp = ht->tweets[index];
    while(temp != NULL){
        // if you find the same tweeter name
        if(strcmp(temp->tweetname,tweetName) == 0){
            return index;
        }
        else{
            index = hash(tweetName, ++collisions);
            temp = ht->tweets[index];
        }
    }
    // Could not find the tweeter in the hashtable
    return -1;
}

