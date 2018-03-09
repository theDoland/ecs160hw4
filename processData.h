#ifndef PROCESSDATA_H
#define PROCESSDATA_H

#include "hashTable.h"

// gets the index of name field in the file
int getNameIndex(char* token);
// goes through the file and maps names to count
void processFile(char* line, HashTable* ht, const char* nameArr[], FILE* fstream, int nameIndex);
// gets the top ten tweeters
void getTopTweeters(const char* nameArr[], HashTable* ht, Tweeter* countArr[]);
// shifts the elements to make room to insert element
void shiftAndInsert(HashTable* ht,Tweeter* countArr[],int numTweeters,int countIndex,int hashIndex);

#endif // PROCESSDATA_H