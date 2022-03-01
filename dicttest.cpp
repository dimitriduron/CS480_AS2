#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include "dicttree.h"
using namespace std;

//functions used for the threads created in main
extern void *startCountWords(void *);
extern void *startPopulateTree(void *);

/*
Takes care of optional arguments
Checks for valid mandatory arguments
Creates new object on heap for cross thread communication
Creates threads that will run concurrently
*/
int main(int argc, char **argv){

    /*
    This code block scans the command line arguments for optional arguments and also valid mandatory arguments
    This is done by scanning a certain number of arguments in the command line then checking a specific range
    for certain strings in *argv, this allows us to identify whether we are using default values or specific
    values, which are specified in the command line. 
    */
    string optionalArg;
    string prevArg = "x";
    int p = -1;
    int h = -1;
    int n = -1;
    int temp;
    
    if(argc > 3 && argc < 10){
        for(int i = 3; i < argc; i++){
            optionalArg = argv[i];
            if(optionalArg.compare("-p") == 0)          prevArg = "-p";
            else if(optionalArg.compare("-h") == 0)     prevArg = "-h";
            else if(optionalArg.compare("-n") == 0)     prevArg = "-n";
            else if(isdigit(*argv[i])){
                temp = stoi(optionalArg);
                if(!prevArg.compare("-p") && temp >= 10){
                    p = temp;
                }
                else if(!prevArg.compare("-p")){
                    cout << "Number of progress marks must be a number and at least 10" << endl;
                    return 0;
                }
                else if(!prevArg.compare("-h") && (temp > 0 && temp <= 10)){
                    h = temp;
                }
                else if(!prevArg.compare("-h")){
                    cout << "Hash mark interval for progress must be a number, greater than 0, and less than or equal to 10" << endl;
                    return 0;
                }
                else if(!prevArg.compare("-n") && temp){
                    n = temp;
                }
            }
            else{
                cout << "invalid argument: " << *argv[i] << endl;
                return 0;
            }
        }
    }

    prevArg = argv[1];
    if(access(prevArg.c_str(), F_OK) != 0){
        cout << "Unable to open <<" << prevArg << ">>" << endl;
        return 0;
    }
    prevArg = argv[2];
    if(access(prevArg.c_str(), F_OK) != 0){
        cout << "Unable to open <<" << prevArg << ">>" << endl;
        return 0;
    }
    
    struct threadParam* tp = new threadParam();
    struct dictNode* root = new dictNode();
    tp->root = root;
    tp->fileName[0] = argv[1];
    tp->fileName[1] = argv[2];
    if(p != -1) tp->p = p;
    if(h != -1) tp->h = h;
    if(n != -1) tp->n = n;

    //count the size of the input files to put the size into the bigger struct
    struct stat st;
    if(stat(argv[1], &st) == 0) tp->totalNumOfCharsInFile[0] = st.st_size;
    if(stat(argv[2], &st) == 0) tp->totalNumOfCharsInFile[1] = st.st_size;

    //initilize and start the threads
    pthread_t threads[2];
    int populatetree = pthread_create(&threads[0], NULL, startPopulateTree, tp);
    int countwords = pthread_create(&threads[1], NULL, startCountWords, tp);
    
    // progress bar section
    // each file will continuously be checked for how far along the characters have been processsed
    // each - or # signifies (100/p)% where p is the optional flag
    // n is being reused to signify how many times - or # has been placed, used as a math marker in a way
    int complete[2] = {0, 0};
    int hCount[2] = {0, 0};
    int count[2] = {1, 1};
    int percent = 100/tp->p;
    
    while(!(tp->finished[0] && tp->finished[1])){
        complete[0] = (tp->numOfCharsProcessedFromFile[0]*100)/tp->totalNumOfCharsInFile[0];
        complete[1] = (tp->numOfCharsProcessedFromFile[1]*100)/tp->totalNumOfCharsInFile[1];
        // given a thread might progress really fast, i made the sections repeatly check until it doesnt work
        while(complete[0] >= percent*count[0]){
            if(count[0]%tp->h == 0){
                cout << "#";
            }
            else{
                cout << "-";
            }
            count[0]++;
            cout.flush();
            if(complete[0] >= 99){
                cout << endl;
                cout << "There are " << tp->wordCountInFile[0] << " words in " << tp->fileName[0] << "." << endl;
            }
        }
        while(complete[1] >= percent*count[1]){
            if(count[1]%tp->h == 0){
                cout << "#";
            }
            else{
                cout << "-";
            }
            count[1]++;
            cout.flush();
            if(complete[1] >= 99){
                cout << endl;
                cout << "There are " << tp->wordCountInFile[1] << " words in " << tp->fileName[1] << "." << endl;
            }
        }
    }

    //cout << "There are " << tp->wordCountInFile[0] << " words in " << tp->fileName[0] << "." << endl;
    //cout << "There are " << tp->wordCountInFile[1] << " words in " << tp->fileName[1] << "." << endl;

    return 0;
}