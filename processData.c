#include "processData.h"

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
    while(fgets(line, 1024, fstream)){
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