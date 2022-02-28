#include "dicttree.h"

extern dictNode* newNode();

/*
No explicit return type, but the new word is added onto the tree

Recursively go through the nodes where each node represents an individual character
If the node doesn't exist yet, create a new dictNode to represent the new character at the nextIndex location
Set the node to true if the end of the word is the current node

nextIndex is meant to represent the index within *next[] that we will be accessing, versus index being the
index of the char within word
*/
void addWord(dictNode* head, string word, int index){
    int nextIndex;
    
    // check for the index being within correct bounds of word
    if(index < word.length()){
        //set the char value for being places within the tree
        if(word[index] == '\'') nextIndex = 26;
        else                    nextIndex = word[index] - 'a';
        
        //only set the new node if it hasn't been filled yet, otherwise recursively iterate into it
        if(head->next[nextIndex] == NULL){
            head->next[nextIndex] = newNode();
        }
        addWord(head->next[nextIndex], word, index+1);
    }
    //label the node as the end of the word
    else{
        head->isWord = true;
    }
}
/*
Returns the amount(int) of words that word are a part of

Recursively search the tree for word, will return 0 if word doesn't exist. 
After the word is found, recursively search all children next nodes for more words.
Each word found increments count and then count is returned at the end
*/
int countWords(dictNode* head, string word, int index){
    int nextIndex;
    int count = 0;

    // goes into this section when we are looking for the actual word, will return 0 if word doesn't exist in dict
    if(index < word.length()){
        if(word[index] == '\'') nextIndex = 26;
        else                    nextIndex = word[index] - 'a';
        // if the next char doesnt exist, return 0 since the word isnt in the dictionary tree
        if(head->next[nextIndex] == NULL)   return count;
        
        count = countWords(head->next[nextIndex], word, index+1);
    }
    // this section is for after we find the word, count is incremented when a word is found and count is returned after
    else{
        if(head->isWord) count++;
        for(nextIndex = 0; nextIndex < 27; nextIndex++){
            if(head->next[nextIndex] != NULL){
                count += countWords(head->next[nextIndex], word, index);
            }
        }
    }
    return count;
}

/*creates a simple new dictNode
Is set as a pointer so it can easily be referred to within other methods
*/
dictNode* newNode(){
    dictNode* n = new dictNode();
    return n;
}
