#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "slist.h"
#include <math.h>
#include <assert.h>

const int BASE = 29;
const int SIZE = 20;
const float RESIZE_RATIO = 0.8;
unsigned int basePowers[20];

typedef struct Dictionary Dictionary;

struct Dictionary{
    int size;
    int capacity;
    List ** cells;
};

void addWord(char * keyValue, char * value, Dictionary * dict);
void resizeDictionary(Dictionary * dict);
void checkResize(Dictionary * dict);

char toUpper(char c){
    if(c <= 'z' && c >= 'a') c += 'A' - 'a';
    return c;
}

void freeCells(List ** cells, int capacity){
    for(int i = 0; i < capacity; i++){
        if(cells[i]) freeListSL(cells[i]); 
    }
}

void freeDictionary(Dictionary * dict){
    freeCells(dict->cells, dict->capacity);
    free(dict->cells);
    free(dict);
}

unsigned int hashString(char * str){
    
    unsigned int hash = 0;
    
    for(int i = 0; i < strlen(str); i++){
        str[i] = toUpper(str[i]);
        hash += (str[i] - 'A' + 1)*basePowers[i % SIZE];
    }


    return hash;
}


void addWord(char * keyValue, char * value, Dictionary * dict){
    
    unsigned int hash = hashString(keyValue) % dict->capacity;

    if(dict->cells[hash] == NULL){
        dict->size++;
        dict->cells[hash] = newListSL();
    }

    Word * word = newWord(keyValue, value);
    Node * node = newNodeSL(word);
    
    addFirstSL(dict->cells[hash], node);
    checkResize(dict);
}

Dictionary * newDictionary (int capacity){
    assert(capacity > 0);
    Dictionary * dict = malloc(sizeof(Dictionary));
    dict->capacity = capacity;
    dict->size = 0;
    dict->cells = malloc(capacity*sizeof(List *));
    for(int i = 0; i < capacity; i++) dict->cells[i] = NULL;
    return dict;
}

void resizeDictionary(Dictionary * dict){

    Dictionary * newDict = newDictionary(2*dict->capacity);
    
    for(int i = 0; i < dict->capacity; i++){
        if(dict->cells[i]){
            Node * temp = dict->cells[i]->head;
            while(temp != NULL){
                addWord(temp->word->keyValue, temp->word->value, newDict);
                temp = temp->next;
            }
        }
    }

    freeCells(dict->cells, dict->capacity);
    free(dict->cells);
    dict->cells = newDict->cells;
    dict->capacity = newDict->capacity;
    dict->size = newDict->size;
    free(newDict);
}

void checkResize(Dictionary * dict){
    float ratio = (float) dict->size / dict->capacity;
    if(ratio > RESIZE_RATIO) resizeDictionary(dict);
}

char * getDefiniton(Dictionary * dict, char * keyValue){

    for(int i = 0; i < strlen(keyValue); i++) keyValue[i] = toUpper(keyValue[i]);
    
    unsigned int hash = hashString(keyValue) % dict->capacity;
    
    char * fail = "Invalid input or there isn't such word in dictionary";

    if(dict->cells[hash]){
        Node * node = dict->cells[hash]->head;
        while(node != NULL){
            if(!strcmp(node->word->keyValue, keyValue)){
                return node->word->value;
            }
            else{
                node = node->next;
            }
        }
        return fail;
    }
    else return fail;
}

unsigned int powInteger(unsigned int base, int exp){
    int res = 1;
    for(int i = 0; i < exp; i++) res *= base;
    return res;
}

void fillDictionary(Dictionary * dict, const char * in){
    int flag = 1;
    FILE * dictionary = fopen(in, "r");
    while(flag){
        int len = 256;
        char * buffer = malloc(len * sizeof(char));
        int i;
        for(i = 0; ; i++){
            buffer[i] = fgetc(dictionary);
            if(buffer[i] == ';') break;
        }
        
        buffer[i] = '\0';

        char * key = malloc(strlen(buffer) + 1);
        strcpy(key, buffer);
        
        for(i = 0; ; i++){
            if(i == len - 1){
                len *= 2;
                buffer = realloc(buffer, len*sizeof(char));
            }
            buffer[i] = fgetc(dictionary);
            if(buffer[i] == '\n' || buffer[i] == EOF) break;
        }
        
        if(buffer[i] == EOF) flag = 0;
        buffer[i] = '\0';

        char * value = malloc(strlen(buffer) + 1);
        strcpy(value, buffer);

        addWord(key, value, dict);
        free(key);
        free(value);
        free(buffer);
    }
    fclose(dictionary);
}


void numberOfElementsInLists(Dictionary * dict){
    
    int size = 10;
    int * result = malloc(size*sizeof(int));

    for(int i = 0; i < size; i++) result[i] = 0;

    for(int i = 0; i < dict->capacity; i++){
        if(dict->cells[i]){
            int len = dict->cells[i]->length;
            if(len >= size){
                result = realloc(result, 2*size*sizeof(int));
                for(int j = size; j < 2*size; j++) result[j] = 0;
                size = size*2;
            }
            result[len]++;
        }
        else{
            result[0]++;
        }
    } 
    puts("");
    for(int i = 0; i < size; i++){
        float ratio = (float) result[i]/dict->size;
        if(result[i]) printf("Number of lists with length %i: %i, ratio to all: %f\n", i, result[i], ratio);
    }
    puts("");

    free(result);
}


void test(int capacity){
    
    for(int i = 0; i < sizeof(basePowers)/sizeof(basePowers[0]); i++){
        basePowers[i] = powInteger(BASE, i);
    }

    Dictionary * dict = newDictionary(capacity);

    fillDictionary(dict, "dictionary.txt");

    while(1){
        char buffer[20];
        printf("Type a word to get definition: (q to quit, s to see size and capacity of Dictionary) ");
        scanf("%s", buffer);
        if(!strcmp(buffer, "q")) break;
        if(!strcmp(buffer, "s")){
            printf("Size: %i\nCapacity: %i\n", dict->size, dict->capacity);
            puts("");
            continue;
        }
        printf("%s\n", getDefiniton(dict, buffer));
        puts("");
    }

    numberOfElementsInLists(dict);
    freeDictionary(dict);
}



int main(){

    int capacity = 256;
    test(capacity);

    return 0;
}