#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TWEETER_SIZE 20000
#define TOP_TWEET_SIZE 10
#define MAX_LINE_SIZE 512

typedef struct {
    char *tweetname;
    int count;
} Tweeter;

typedef struct {
    int size;
    // int count;
    Tweeter** tweets;
} HashTable;

HashTable* newHashTable();
static Tweeter* newTweeter(const char *tweetName);
static void deleteItem(Tweeter* item);
void deleteHashTable(HashTable* ht);
static int hash(const char *tweetName, int collisions);
void insertTweeter(HashTable* ht, const char *tweetName);
int getTweeter(HashTable* ht, const char *tweetName);
// gets the index of name field in the file
int getNameIndex(char* token);
// goes through the file and maps names to count
void processFile(char* line, HashTable* ht, const char* nameArr[], FILE* fstream, int nameIndex);
// gets the top ten tweeters
void getTopTweeters(const char* nameArr[], HashTable* ht, Tweeter* countArr[]);
// shifts the elements to make room to insert element
void shiftAndInsert(HashTable* ht,Tweeter* countArr[],int numTweeters,int countIndex,int hashIndex);
void exception(char* message);


int main(int argc, char *argv[]){

    // no file provided
    if(argv[1] == NULL){
        exception("No file provided!");
    }

    HashTable* ht = newHashTable();             // set up the hashTable
    const char* nameArr[MAX_TWEETER_SIZE];      // nameArray for later consumption
    Tweeter* countArr[TOP_TWEET_SIZE];          // final count array for later consumption

    // Open the file for reading
    FILE* fstream = fopen(argv[1], "r");

    if(fstream == NULL) {
        exception("Invalid file!");
    }

    char line[MAX_LINE_SIZE];
    fgets(line, MAX_LINE_SIZE, fstream);
    char* token = strtok(line, ",");

    // get the index for where the "name" field is in file
    int nameIndex = getNameIndex(token);

    // name does not exist in the file
    if(nameIndex == -1){
        exception("No name field in file!");
    }

    // now loop through entire file to fill hash table with names and count
    processFile(line, ht, nameArr, fstream, nameIndex);

    // Close the file
    fclose(fstream);

    // Find the top ten tweeters in the hash table
    getTopTweeters(nameArr, ht, countArr);

    // print out the results
    for(int i = 0; i < TOP_TWEET_SIZE; i++){
        printf("%s: %i\n", countArr[i]->tweetname, countArr[i]->count);
    }

    // free data
    deleteHashTable(ht);
    //free(nameArr);
    //free(countArr);
}

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
    // ht->count = 0;
    // initialize entire hashtable to zeroes
    ht->tweets = calloc((size_t)ht->size, sizeof(Tweeter*));
    return ht;
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
    // ht->count++;

}

// Return the index of the hashTable so we can increment the count
int getTweeter(HashTable* ht, const char *tweetName) {
    int collisions = 0;
    int index = hash(tweetName, collisions);
    Tweeter *temp = ht->tweets[index];
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

int getNameIndex(char* token){

    int nameIndex = -1; 
    int tempIndex = 0;

    // check the file header for the "name" category
    while(token != NULL){

        if(strcmp(token, "\"name\"") == 0){
            nameIndex = tempIndex;
            break;
        }

        token = strtok(NULL, ",");
        tempIndex++;
    }
    return nameIndex;
}

void processFile(char* line, HashTable* ht, const char* nameArr[], FILE* fstream, int nameIndex){

    int hashIndex, nameArrIndex = 0;
    char* token;

    // loop through each entry and insert names
    while(fgets(line, MAX_LINE_SIZE, fstream)){
        char *tmp = strdup(line);

        // skip to the name field in the line
        token = strtok(tmp, ",");

        // move token to the name entry
        for(int i = 0; i < nameIndex; i++){
            token = strtok(NULL, ",");
        }

        // get index for the tweeter
        hashIndex = getTweeter(ht,token);

        // if the name is not in the hash table
        if(hashIndex == -1){
            // add it to the hashTable
            insertTweeter(ht,token);
            // add name to list of names for later consumption
            nameArr[nameArrIndex++] = strdup(token);
        }
        else{
            // increment the count
            ht->tweets[hashIndex]->count += 1;
        }
        if(tmp != NULL){
            free(tmp);
        }
    }
}

void getTopTweeters(const char* nameArr[], HashTable* ht, Tweeter* countArr[]){
    int hashIndex;
    int countIndex;
    int numTweeters = 0;
    
    for(int i = 0; nameArr[i] != NULL; i++){
        // Check if it has a place in the topTweeters and sort
        hashIndex = getTweeter(ht, nameArr[i]);
        for(countIndex = 0; countIndex < numTweeters; countIndex++){
            if(ht->tweets[hashIndex]->count > countArr[countIndex]->count){                
                // array cannot fit anymore data
                if(numTweeters == 10){
                    // move elements over and insert the new element
                    shiftAndInsert(ht,countArr,numTweeters,countIndex,hashIndex);
                    break;
                }
                // array fits data, move the elements in array around
                else{
                    // move elements over and insert the new element
                    shiftAndInsert(ht,countArr,numTweeters,countIndex,hashIndex);
                    numTweeters++;
                    break;
                }

            }
        }
        // append to the arr if lowest value and empty spaces in array
        if(countArr[countIndex] == NULL && numTweeters < 10){
            countArr[countIndex] = ht->tweets[hashIndex];
            numTweeters++;
        }
    }
}

void shiftAndInsert(HashTable* ht, Tweeter* countArr[], int numTweeters, int countIndex, int hashIndex){
    // shift elements over and insert
    for(int j = numTweeters; j != countIndex; j--){
        countArr[j] = countArr[j-1];
    }
    // place the data in the now available index
    countArr[countIndex] = ht->tweets[hashIndex];
}

void exception(char* message) {
    printf("%s\n", message);
    exit(1);
}