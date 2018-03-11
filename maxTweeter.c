#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASHTABLE_SIZE 25000
#define MAX_TWEETS 20000
#define MAX_NUM_TOP_TWEETS 10
#define MAX_LINE_SIZE 512

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
// gets the index of name field in the file
int getNameIndex(char* token);
// goes through the file and maps names to count
void processFile(HashTable* ht, const char* nameArr[], FILE* fstream, int nameIndex);
// gets the top ten tweeters
void getTopTweeters(const char* nameArr[], HashTable* ht, Tweeter* countArr[]);
// shifts the elements to make room to insert element
void shiftAndInsert(HashTable* ht,Tweeter* countArr[],int numTweeters,int countIndex,int hashIndex);
void exception(char* message);
char *trimWhitespace(char *str);

// global filesize to check maximum lines
int fileSize = 0;

int main(int argc, char *argv[]){

    // no file provided
    if(argv[1] == NULL){
        exception("No file provided!");
    }

    HashTable* ht = newHashTable();             // set up the hashTable
    const char* nameArr[MAX_TWEETS];            // nameArray for later consumption
    Tweeter* countArr[MAX_NUM_TOP_TWEETS];      // final count array for later consumption

    // Open the file for reading
    FILE* fstream = fopen(argv[1], "r");

    if(fstream == NULL) {
        exception("Invalid file!");
    }

    //char line[MAX_LINE_SIZE];
    // let getline allocate lineptr and size
    char* line;
    size_t num_bytes = 0;
    if(getline(&line, &num_bytes, fstream) != -1){
        fileSize += 1;
    }

    if(strlen(line) > MAX_LINE_SIZE){
        exception("Line length exceeds limit!");
    }
    char* token = strtok(line, ",\n");
    // get the index for where the "name" field is in file
    int nameIndex = getNameIndex(token);

    // name does not exist in the file
    if(nameIndex == -1){
        exception("No name field in file!");
    }

    // now loop through entire file to fill hash table with names and count
    processFile(ht, nameArr, fstream, nameIndex);

    // Close the file
    if(fclose(fstream) != 0){
        exception("Error in closing file!");
    }

    // Find the top ten tweeters in the hash table
    getTopTweeters(nameArr, ht, countArr);

    // print out the results
    for(int i = 0; i < MAX_NUM_TOP_TWEETS; i++){
        if(countArr[i] != NULL)
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

    ht->size = HASHTABLE_SIZE;
    ht->count = 0;
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
    // if invalid characters were given
    if(hash < 0){
        // set to zero and essentially increment until you find an open space
        hash = 0; 
    }
    // placeholder hash function
    return (hash + collisions) % HASHTABLE_SIZE;
}

// Insert the object at hash location + # of collisions
void insertTweeter(HashTable* ht, const char *tweetName) {
    if(ht->count >= MAX_TWEETS) {
        exception("CSV length surpassed max size!");
    }
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

        token = strtok(NULL, ",\n");
        tempIndex++;
    }
    return nameIndex;
}

void processFile(HashTable* ht, const char* nameArr[], FILE* fstream, int nameIndex){
    char *line;
    int hashIndex, nameArrIndex = 0;
    char* token;
    size_t num_bytes = 0;

    // loop through each entry and insert names
    while(getline(&line, &num_bytes, fstream) != -1){
        fileSize += 1;
        if(fileSize >= MAX_TWEETS){
            exception("CSV length surpassed max size!");
        }

        if(strlen(line) > MAX_LINE_SIZE){
            exception("Line length exceeds limit!");
        }

        char *tmp = strdup(line);
        tmp = trimWhitespace(tmp);

        // Skip blank lines
        if (tmp[0] == '\0') {
            continue;
        }
        // skip to the name field in the line
        token = strtok(tmp, ",\n");

        // move token to the name entry
        for(int i = 0; i < nameIndex; i++){
            token = strtok(NULL, ",\n");
        }

        // If the column doesn't exist throw error
        if(token == NULL) {
            exception("Invalid line!");
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
        //if(tmp != NULL){
        //    free(tmp);
        //}
    }
}

void getTopTweeters(const char* nameArr[], HashTable* ht, Tweeter* countArr[]){
    int hashIndex;
    int countIndex;
    int numTopTweeters = 0;
    
    for(int i = 0; nameArr[i] != NULL; i++){
        // Check if it has a place in the topTweeters and sort
        hashIndex = getTweeter(ht, nameArr[i]);
        for(countIndex = 0; countIndex < numTopTweeters; countIndex++){
            if(ht->tweets[hashIndex]->count > countArr[countIndex]->count){  
                // move elements over and insert the new element
                shiftAndInsert(ht,countArr,numTopTweeters,countIndex,hashIndex);              
                // array cannot fit anymore data
                if(numTopTweeters < MAX_NUM_TOP_TWEETS){
                    numTopTweeters++;
                }
                break;
            }
        }
        // append to the arr if lowest value and empty spaces in array
        if(countArr[countIndex] == NULL && numTopTweeters < 10){
            countArr[countIndex] = ht->tweets[hashIndex];
            numTopTweeters++;
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

// Taken from
// https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
char *trimWhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}