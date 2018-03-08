#include "hashTable.h"

/* Calculate the top 10 tweeters by volume of tweets in a given csv file 
 * Tweeters will be printed in descending order
 */

/*
 * Check if there's even a file loaded
 * 
 * 
 * 
 */

// takes one cmd line argument
void main(int argc, char *argv[]){
    // Store the tweeters/count and collect at end 
    // Probably a hash table where (key, value) = (tweeter, count)
    HashTable* ht = newHashTable();
    const char* nameArr[MAX_TWEETER_SIZE];
    int nameArrIndex = 0; 
    // no file provided
    if(argv[1] == NULL){
        printf("No file provided!\n");
        exit(0);
    }
    // Open the file for reading
    FILE* fstream = fopen(argv[1], "r");

    // unsure about this part
    char line[1024];
    // get header line to find the name
    fgets(line, 1024, fstream);
    char* token = strtok(line, ",");
    int nameIndex = -1;
    int tempIndex = 0;

    // check the file header for the "name" category
    while(token != NULL){
        printf("%s\n", token);
        if(strcmp(token, "\"name\"") == 0){
            nameIndex = tempIndex;
            break;
        }
        token = strtok(NULL, ",");
        tempIndex++;
    }

    // name does not exist in the file
    if(nameIndex == -1){
        printf("No name field in file!\n");
        exit(0);
    }
    int hashIndex;
    // now loop through entire file to find the names and increment count
    while(fgets(line, 1024, fstream)){
        char *tmp = strdup(line);

        // skip to the name field in the line
        token = strtok(tmp, ",");
        for(int i = 0; i < nameIndex; i++){
            token = strtok(NULL, ",");
        }

        hashIndex = getTweeter(ht,token);
        // if the hashtable does not have the name
        if(hashIndex == -1){
            // add it to the hashTable
            insertTweeter(ht,token);
            // add name to list of names for later consumption
            nameArr[nameArrIndex++] = token;
        }
        else{
            // increment the count
            ht->tweets[hashIndex] += 1;
        }
        if(tmp != NULL){
            free(tmp);
        }
    }

    // Close the file
    fclose(fstream);

    // For each tweeter in the string array
    for(int i = 0; i < sizeof(nameArr); i++){

    }
    // Check the hashtable for a count
    // (?) loop through count array to check where to place the tweeter
    // if tweeter's count > lowest tweeter || array length < 10
    // add to the final array

    // print out the results
    // (?) free data
}