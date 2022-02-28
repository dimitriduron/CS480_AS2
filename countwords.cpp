#include "dicttree.h"

void *startCountWords(void *tpAddr){
    struct threadParam *tp;
    tp = (struct threadParam *)tpAddr;
    string line;

    //characters we want to seperate 
    const char* delimiters = "\n\r !\"#$%&()*+,-./0123456789:;<=>?@[\\]^_`{|}~";

    ifstream testFile(tp->fileName[1]);
    char delimWord[100];
    char* word;
    bool wordSet;
    int num;
    
    /*
    this section will read in the lines of the test file
    each line will then enter the loop that will read each word within the line given the deliminiters
    given that we want to only use lowercase letters, I converted to lowercase but also stored the original
        word into another variable given the output format.
    at the end of the line, word will be NULL, so we check for the NULL after word is found. The while condition
        doesn't check soon enough so I put a quick if statement after word is set within the loop
    */
    while(getline(testFile, line)){
        tp->numOfCharsProcessedFromFile[1] += line.length();
        wordSet = false;
        do{
            strcpy(delimWord, line.c_str());
            if(wordSet){
                word = strtok(NULL, delimiters);
            }
            else{
                wordSet = true;
                word = strtok(delimWord, delimiters);
            }
            //word doesnt reach the do while condition so this if prevents any seg faults
            if(word == NULL) break;
            
            //we know we have a word here so now we can iterate words in file
            tp->wordCountInFile[1]++;

            //change to lower for correct input
            //cout << word << " ";
            for(int i = 0; word[i] != '\0'; i++){
                word[i] = tolower(word[i]);
            }
            
            num = countWords(tp->root, word, 0);
            
            //cout << countWords(tp->root, word, 0) << endl;
        }while(word != NULL);
    }
    testFile.close();
    
    tp->finished[1] = true;
    return NULL;
}