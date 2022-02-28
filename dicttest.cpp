#include <pthread.h>
#include <unistd.h>
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

    pthread_t threads[2];
    
    int populatetree = pthread_create(&threads[0], NULL, startPopulateTree, tp);
    int countwords = pthread_create(&threads[1], NULL, startCountWords, tp);
    
    while(!(tp->finished[0] && tp->finished[1]));

    return 0;
}