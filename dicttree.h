
#ifndef DICTTREE_H_
#define DICTTREE_H_

#include <iostream>
#include <string.h>
#include <pthread.h>
#include <fstream>
using namespace std;

//default the data to NULL/false so it doesnt need to be specified later
//the array of dictNodes is to represent each of the viable characters to continue the dictTree
struct dictNode{
    bool isWord = false;
    dictNode* next[27] = {NULL};
};

//the main two methods being used within main in the thread countwords.cpp to construct the tree and count
void addWord(dictNode*, string, int);
int countWords(dictNode*, string, int);

//Small struct used to hold useful data for usage in the functions executing per threads
//All of the arrays reference the dict for index 0 and test file for index 1
struct threadParam{
    struct dictNode *root;
    long totalNumOfCharsInFile[2] = {0, 0};
    long *numOfCharsProcessedFromFile[2] = {0, 0};
    long wordCountInFile[2] = {0, 0};
    string fileName[2];
    bool finished[2] = {false, false};
    int p = 50;
    int h = 10;
    int n = 1;
};

#endif