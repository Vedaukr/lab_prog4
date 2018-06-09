#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

Word * newWord(char * keyValue, char * value);

void addFirstSL(List* list, Node* node);
void addEndSL(List* list, Node* node);

Node* newNodeSL(Word * word);
List* newListSL();

Node* deleteNodeSL(List* list, int index);
void insertNodeSL(List* list, Node* node, int index);

void freeListSL(List* list);
void freeNodeSL(Node* node);

void printListSL(List* list);