#include "hashTable.h"
#include "processData.h"

/* Calculate the top 10 tweeters by volume of tweets in a given csv file 
 * Tweeters will be printed in descending order
 * Store the tweeters/count and collect at end 
 */

void main(int argc, char *argv[]){

    HashTable* ht = newHashTable();             // set up the hashTable
    const char* nameArr[MAX_TWEETER_SIZE];      // nameArray for later consumption
    Tweeter* countArr[TOP_TWEET_SIZE];          // final count array for later consumption

    // no file provided
    if(argv[1] == NULL){
        printf("No file provided!\n");
        exit(0);
    }

    // Open the file for reading
    FILE* fstream = fopen(argv[1], "r");

    char line[1024];
    fgets(line, 1024, fstream);
    char* token = strtok(line, ",");

    // get the index for where the "name" field is in file
    int nameIndex = getNameIndex(token);

    // name does not exist in the file
    if(nameIndex == -1){
        printf("No name field in file!\n");
        exit(0);
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