#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define t 5

// 1 , AFG , DIST , 93 , 18

// Definição da estrutura de um registro
struct rec
{
	int key;                // Chave do registro
	// rest
	char country[5];
	char grate[10];
	int score;
	int rate;
};
typedef struct rec recordNode;

// Definição da estrutura de um nó da árvore B
struct bTreeNode
{
	bool isLeaf;            // Indica se o nó é uma folha
	int pos;                // Posição do nó no arquivo
	int noOfRecs;           // Número de registros no nó
	recordNode* recordArr[2 * t - 1];   // Array de registros
	int children[2 * t];    // Array de índices dos filhos do nó
};
typedef struct bTreeNode bTreeNode;

// Definição da estrutura da árvore B
struct tree
{
	char fileName[20];      // Nome do arquivo onde a árvore é armazenada
	FILE* fp;               // Ponteiro para o arquivo
	int root;               // Índice da raiz da árvore
	int nextPos;            // Próxima posição disponível no arquivo
};
typedef struct tree bTree;

// Função para criar a árvore B
bTree* createTree(char* fileName, bool mode);

// Função para inicializar um nó da árvore B
bTreeNode* nodeInit(bTreeNode* node, bool isLeaf, bTree* tree);

// Função para inserir um registro na árvore B
void insert(bTree* tree, recordNode* record);

// Função para excluir um registro da árvore B
void delete(bTree* tree, int key);

// Função para percorrer a árvore B e exibir os registros
void traverse(bTree* tree, int root);

// Função para exibir um nó da árvore B
void dispNode(bTreeNode* node);

// Função para escrever a árvore B em um arquivo
void writeFile(bTree* ptr_tree, bTreeNode* p, int pos);

// Função para ler a árvore B de um arquivo
void readFile(bTree* ptr_tree, bTreeNode* p, int pos);

// Função para preencher os dados de um registro
void enterData(recordNode* record, int id_num, char country[], char Grate[], int Score, int Rate);

// Função para obter os dados de um arquivo
recordNode* getData(char* filepath, int len);

// Função para pesquisar um registro na árvore B
recordNode* search(bTree* tree, int key);

// Função auxiliar para pesquisar um registro de forma recursiva
recordNode* searchRecursive(bTree* tree, int key, bTreeNode* root);

// Função para remover um registro da árvore B
bool removeFromTree(bTree* tree, int key);

// Função para mesclar dois nós da árvore B
bTreeNode* merge(bTree* tree, bTreeNode* node, int idx);

// Função para obter o sucessor de um registro na árvore B
recordNode* getSucc(bTree* tree, bTreeNode* node, int idx);

// Função para obter o predecessor de um registro na árvore B
recordNode* getPred(bTree* tree, bTreeNode* node, int idx);

// Função auxiliar para remover um registro de um nó não folha
void removeFromNonLeaf(bTree* tree, bTreeNode* node, int idx);

// Função auxiliar para remover um registro de um nó folha
void removeFromLeaf(bTree* tree, bTreeNode* node, int idx);

// Função para remover um nó da árvore B
void removeNode(bTree* tree, bTreeNode* node, int k);

// Função para encontrar a posição de uma chave em um nó
int findKey(bTreeNode* node, int k);
