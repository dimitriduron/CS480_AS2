#include "dicttree.h"

void *startCountWords(void *tpAddr){
    //turn the void object into type threadParam
    struct threadParam *tp;
    tp = (struct threadParam *)tpAddr;

    //characters we want to seperate 
    const char* delimiters = "\n\r !\"#$%&()*+,-./0123456789:;<=>?@[\\]^_`{|}~";

    //needed variables within the code block
    ifstream testFile(tp->fileName[1]);
    ofstream newFile("count_words.txt");
    string line;
    string wordStr;
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
        tp->numOfCharsProcessedFromFile[1] += line.length() + 1;
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
            tp->wordCountInFile[1]++;

            wordStr = word;
            //change to lower for correct input
            for(int i = 0; word[i] != '\0'; i++){
                word[i] = tolower(word[i]);
            }
            
            num = countWords(tp->root, word, 0);

            if(num >= tp->n){
                newFile << wordStr << " " << num << endl;
            }
            
        }while(word != NULL);
    }
    newFile.close();
    testFile.close();
    
    tp->finished[1] = true;
    return NULL;
}