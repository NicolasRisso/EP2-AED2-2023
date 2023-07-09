/*
		            AUTORES

	Andre Fabiano de Carvalho - 13672425
	Nicolas Pereira Risso Vieira - 13672262
	Vitor Borges Santana - 13720129

*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define t 5

// 1 , AFG , DIST , 93 , 18

char *strsep(char **stringp, const char *delim) { 
    char *rv = *stringp; 
    if (rv) { 
        *stringp += strcspn(*stringp, delim); 
        if (**stringp) 
            *(*stringp)++ = '\0'; 
        else 
            *stringp = 0; } 
    return rv; 
}

struct rec
{
	bool isValid;
	int codigoLivro;
	// rest
	char titulo[30];
	char nomeCompletoPrimeiroAutor[30];
	int anoPublicacao;
	int rate;
};
typedef struct rec recordNode;

struct bTreeNode
{
	bool isLeaf; 
	int pos; 
	int noOfRecs;
	bool isValid;

	recordNode* keyRecArr[2 * t - 1];
	recordNode* posRecArr[2 * t - 1];
	int children[2 * t]; 
};
typedef struct bTreeNode bTreeNode;

struct tree
{
	char fileName[20];
	FILE* fp;
	int root;
	int nextPos;
};
typedef struct tree bTree;

bTree* createTree(char* fileName,bool mode);
bTreeNode* nodeInit(bTreeNode* node,bool isLeaf,bTree* tree);
void insert(bTree* tree,recordNode* record);
void delete(bTree* tree,int key);
void traverse(bTree* tree, int root);
void dispNode(bTreeNode* node);
void writeFile(bTree* ptr_tree, bTreeNode* p, int pos);
void readFile(bTree* ptr_tree, bTreeNode* p, int pos);


void enterData(recordNode* record, int id_num, char titulo[], char nomeCompletoPrimeiroAutor[], int anoPublicacao, int Rate);
recordNode* getData(char *filepath, int len);
recordNode* search(bTree* tree, int key);
recordNode* searchRecursive(bTree* tree, int key, bTreeNode* root);
bool removeFromTree(bTree* tree, int key);
bTreeNode* merge(bTree* tree, bTreeNode *node, int idx);
void borrowFromNext(bTree* tree, bTreeNode *node, int idx);
void borrowFromPrev(bTree* tree, bTreeNode *node, int idx);
void fill(bTree* tree, bTreeNode *node, int idx);
recordNode *getSucc(bTree* tree, bTreeNode *node, int idx);
recordNode *getPred(bTree* tree, bTreeNode *node, int idx);
void removeFromNonLeaf(bTree* tree, bTreeNode *node, int idx);
void removeFromLeaf (bTree* tree, bTreeNode *node, int idx);
void removeNode(bTree* tree, bTreeNode* node, int k);
int findKey(bTreeNode* node, int k);
