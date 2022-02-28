#include "dicttree.h"

//This thread will populate the root node with words from the dictionary file from the input
void *startPopulateTree(void *tpAddr){
    struct threadParam *tp;
    tp = (struct threadParam *)tpAddr;

    //read the words from the dictionary doc to setup the dict tree
    ifstream dictFile(tp->fileName[0]);
    string line;

    //dump the words from the inputted dictionary into the
    while (getline(dictFile, line)){
        addWord(tp->root, line, 0);
        tp->wordCountInFile[0]++;
    }
    dictFile.close();
    cout << "There are " << tp->wordCountInFile[0] << " words in " << tp->fileName[0] << "." << endl;
    tp->finished[0] = true;
    return NULL;
}
