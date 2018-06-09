#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

static char * strDup (const char *s) {
    char *d = malloc (strlen (s) + 1);   // Space for length plus nul
    if (d == NULL) return NULL;          // No memory
    strcpy (d,s);                        // Copy the characters
    return d;                            // Return the new string
}


typedef struct Word Word;

struct Word{
    char * keyValue;
    char * value;
};

typedef struct Node {
    Word * word;
    struct Node *next;
} Node;

typedef struct List {
    Node * head;
    size_t length;
} List;


List* newListSL(){
    List * list = (List *)malloc(sizeof(List));
    list->head = NULL;
    list->length = 0;
    return list;
}

Word * newWord(char * keyValue, char * value){
    Word * word = malloc(sizeof(Word));
    word->keyValue = strDup(keyValue);
    word->value = strDup(value);
    return word;
}

Node * newNodeSL(Word * word){
    Node* node = malloc(sizeof(Node));
    node->next = NULL;
    node->word = word;
    return node;
}

void addFirstSL(List* list, Node* node){
    node->next = list->head;
    list->head = node;
    list->length++;
}

void addEndSL(List* list, Node* node){
    if(list->head == NULL){
        list->head = node;
        list->length++;
        return;
    }
    Node* cur = list->head;
    while(cur->next != NULL) cur = cur->next;
    cur->next = node;
    list->length++;
}

Node* deleteNodeSL(List* list, int index){
    
    assert(index >= 0);
    
    Node* cur = list->head;
    if(cur == NULL) return NULL;

    if(index == 0){
        list->head = list->head->next;
        list->length--;
        return cur;
    }

    int i = 0;
    while(i < index - 1){
        i++;
        if(cur->next != NULL) cur = cur->next;
        else return NULL;
    }
    
    if(cur->next == NULL) return NULL;

    Node* deleted = cur->next;
    cur->next = deleted->next;
    list->length--; 
    return deleted;

}

void insertNodeSL(List* list, Node* node, int index){
    
    assert(index >= 0);
    if(index > list->length) return;

    int i = 0;

    if(index == 0){
        addFirstSL(list, node);
        return;
    }
    if(index == list->length){
        addEndSL(list, node);
        return;
    }


    Node* cur = list->head;
    while(i < index - 1){
        i++;
        if(cur->next != NULL) cur = cur->next;
        else return;
    }

    node->next = cur->next;
    cur->next = node;
    list->length++;

}

/* void printListSL(List* list){
    Node* cur = list->head;
    while(cur != NULL){
        printf("%s --> ", cur->value);
        cur = cur->next;
    }
    printf("NULL\n");
} */

static void freeWord(Word * word){
    free(word->value);
    free(word->keyValue);
    free(word);
}

void freeNodeSL(Node* node){
    freeWord(node->word);
    free(node);
}

void freeListSL(List* list){
    if(list == NULL) return;
    
    Node* cur = list->head;
    if(cur == NULL){
        free(list);
        return;
    }
    
    Node* next = cur->next;
    
    while(next != NULL){
        freeNodeSL(cur);
        cur = next;
        next = next->next;
    }
    
    freeNodeSL(cur);
    free(list);
}