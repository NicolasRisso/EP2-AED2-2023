#include "headers.h"

#define LIMITE 1000

// Cria uma arvore B. A função possui dois modos de uso.
// quando mode = 0, uma arvore vazia é criada. Já quando mode = 1,
// a arvore é criada com os dados de um arquivo .dat já existente
bTree* createTree(char* fileName,bool mode)
{
	bTree* tree = malloc(sizeof(bTree));
    
	if(!mode) //new file
    {
        
        strcpy(tree->fileName,fileName);
        tree->fp = fopen(fileName,"w");
        fclose(tree->fp);

        tree->root = 0;
        tree->nextPos = 0;
    } else {
        FILE *oldFile = fopen("tree.dat", "r"); // arquivo para ser lido
        fread(tree, sizeof(bTree), 1, oldFile);
        fclose(oldFile);
    }

    tree->fp = fopen(fileName, "r+");
    return tree;
}

recordNode* createData() {  // cria um arquivo com os dados

    recordNode* dadosLivro = malloc(sizeof(recordNode)*LIMITE);

    FILE* arquivo = fopen("data.dat", "wb");

    fwrite(&dadosLivro, sizeof(recordNode), 1, arquivo);

    fclose(arquivo);

    return dadosLivro; 
}

// Recebe um nó já alocado em memória, o monta com os valores passados
// como parâmetros na função, insere na arvore e coloca "-1" em todos os espacos
// do vertor "children" para dizer que o nó ainda não tem filhos.
bTreeNode* nodeInit(bTreeNode* node,bool isLeaf,bTree* tree)
{
	node->isLeaf = isLeaf;
	node->noOfRecs=0;
	node->pos = tree->nextPos;
	(tree->nextPos)++;
	int i;
	for (int i = 0; i < 2*t; ++i)
	{
		node->children[i] = -1;
	}
	return node;
}

// Escreve um nó em memória
void writeFile(bTree* ptr_tree, bTreeNode* p, int pos) {
    
    if(pos == -1) {   // pos == -1 é posição não especificada. Será inserido na ultima posição
        pos = ptr_tree->nextPos++;
    }

    fseek(ptr_tree->fp, pos * sizeof(bTreeNode), 0);  // função que realiza um seek
    fwrite(p, sizeof(bTreeNode), 1, ptr_tree->fp);   // função que escreve na memoria secundária
    
}

// Lê um nó da memória
void readFile(bTree* ptr_tree, bTreeNode* p, int pos) {    
    fseek(ptr_tree->fp, pos * sizeof(bTreeNode), SEEK_SET);
    fread(p, sizeof(bTreeNode), 1, ptr_tree->fp);     // função que le a memória secudária e armazena na memoria RAM
}

// insere no nó os valores passados pela função
void enterData(recordNode* record, int id_num, char titulo[], char nomeCompletoPrimeiroAutor[], int anoPublicacao, int rate) {
    
    record->codigoLivro = id_num;
    strcpy(record->titulo, titulo);
    strcpy(record->nomeCompletoPrimeiroAutor, nomeCompletoPrimeiroAutor);
    record->anoPublicacao = anoPublicacao;
//    record->rate = rate;
    
    return;
}

// Cria uma estrutura em memória principal a partir dos dados de uma observação no arquivo csv
recordNode* getData(char *filepath, int len) {
    
    recordNode *keyRecArr = malloc(sizeof(recordNode)*len);
    char delim = ','; char line[256];
    int file_no = 0;
    int i,codigoLivro,anoPublicacao,rate;
    char titulo[255];
    char nomeCompletoPrimeiroAutor[255];
    
    FILE *inpFile = fopen(filepath, "r");
    
    while(file_no < len && fscanf(inpFile, "%d,%[^,],%[^,],%d,%d", &codigoLivro, titulo, nomeCompletoPrimeiroAutor, &anoPublicacao, &rate)) {
        enterData(&keyRecArr[file_no], codigoLivro, titulo, nomeCompletoPrimeiroAutor, anoPublicacao, rate);
        file_no++;
    }

    return keyRecArr;
}

//Divide um node criança de um node pai da Arvore B
void splitChild(bTree* tree, bTreeNode* x, int i, bTreeNode* y)
{
    // Cria um novo nó z e o inicializa
	bTreeNode* z = malloc(sizeof(bTreeNode));
	nodeInit(z,y->isLeaf,tree);
	z->noOfRecs = t-1;

	int j;
    // Copia metade dos registros do nó filho y para o novo nó z
	for(j=0;j<t-1;j++)
	{
		z->keyRecArr[j] = y->keyRecArr[j+t];
	}

	if(!y->isLeaf)
	{
        // Se o nó y não for uma folha, copia metade dos filhos também
		for(j=0;j<t;j++)
		{
			z->children[j] = y->children[j+t];
            y->children[j+t] = -1; // Define os filhos copiados como inválidos
		}
	}
	y->noOfRecs = t-1;

    // Desloca os filhos do nó pai x para abrir espaço para o novo nó z
	for(j=(x->noOfRecs); j >= i+1;j--)
	{
		x->children[j+1] = x->children[j];
	}
	
	x->children[i+1] = z->pos;

    // Desloca os registros do nó pai x para abrir espaço para o registro do nó y
	for(j=(x->noOfRecs) - 1; j >= i;j--)
	{
		x->keyRecArr[j+1] = x->keyRecArr[j];
	}
    // Insere o registro mediano do nó y no nó pai x
	x->keyRecArr[i] = y->keyRecArr[t-1];
	x->noOfRecs++;

    writeFile(tree, x, x->pos);
    writeFile(tree, y, y->pos);
    writeFile(tree, z, z->pos);
	free(z);
}

// Insere um valor no Node caso não esteja cheio
void insertNonFull(bTree* tree,bTreeNode* x,recordNode* record, char* filepath)
{	
	int i = (x->noOfRecs)-1;    // Índice do último registro no nó atual
	if(x->isLeaf == true)
	{
		while((i>=0) && (record->codigoLivro < x->keyRecArr[i]))
		{
			x->keyRecArr[i+1] = x->keyRecArr[i];    // Move registros maiores para a direita
			i--;
		}
		x->keyRecArr[i+1] = record->codigoLivro;     // Insere a chave do registro na arvore
		(x->noOfRecs)++;

        tree->n_elements++; // posição a ser colocada no arquivo indice


//===============================inserção no indice=====================
    recordNode *buffer = malloc(sizeof(recordNode)*LIMITE);

    FILE *indice = fopen(filepath, "w");

    fread(buffer, sizeof(recordNode), LIMITE, indice);

    buffer[tree->n_elements] = *record;

    fwrite(buffer, sizeof(recordNode), LIMITE, indice);

//    fclose(filepath);
    free(buffer);

//======================================================================
        writeFile(tree, x, x->pos);
	}
	else
	{
		while((i>=0) && (record->codigoLivro < x->keyRecArr[i]))
		{
			i=i-1;      // Encontra a posição para inserir o novo registro
		}
		bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, x->children[i+1]);
        
		if(childAtPosi->noOfRecs == (2*t-1))    // Se o nó filho estiver cheio
		{
			splitChild(tree,x,i+1,childAtPosi);     // Divide o nó filho 
			if( x->keyRecArr[i+1] < record->codigoLivro){
				i++;
			}
		}

        readFile(tree, childAtPosi, x->children[i+1]);
		insertNonFull(tree,childAtPosi,record, filepath); // Insere recursivamente o novo registro no nó filho apropriado

		free(childAtPosi);
	}
}

//Insere
void insert(bTree* tree,recordNode* record, char* filepath)
{
	if(tree->nextPos == 0) // condição de arvore vazia
	{
		tree->root = tree->nextPos;

		bTreeNode* firstNode = malloc(sizeof(bTreeNode)); // aloca espaco para o nó
		nodeInit(firstNode,true,tree); // inicia um nó
		firstNode->keyRecArr[0] = record->codigoLivro;
		(firstNode->noOfRecs)++;

        writeFile(tree, firstNode, firstNode->pos); // insere nó na arvore

		free(firstNode);
		return;
	}
	else
	{
		bTreeNode* rootCopy = malloc(sizeof(bTreeNode));
        readFile(tree, rootCopy, tree->root);

		if(rootCopy->noOfRecs == (2*t-1)) // condição de raiz cheia
		{
			bTreeNode* newRoot = malloc(sizeof(bTreeNode)); // cria um novo nó para a nova
			nodeInit(newRoot,false,tree);
			newRoot->children[0] = tree->root;

			splitChild(tree,newRoot,0,rootCopy); // divide o nó raiz e envia a mediana para o nó novo

			int i=0;
			if(newRoot->keyRecArr[0] < record->codigoLivro){
				i++;
			}
			
			bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
            readFile(tree, childAtPosi, newRoot->children[i]);
			insertNonFull(tree,childAtPosi,record, filepath); // insere o registro no nó filho não cheio

			tree->root = newRoot->pos; // arualiza a raiz da arvore
            
            

			free(newRoot);
            free(childAtPosi);
		}
		else
		{
			insertNonFull(tree,rootCopy,record, filepath); // insere o registro na arvore
		}
		free(rootCopy);
	}
}

//Percorre a árvore B e exibe os nodes
void traverse(bTree* tree, int root) {
    
    if(-1 == root) {    
        return;
    }

    bTreeNode *toPrint = malloc(sizeof(bTreeNode));
    readFile(tree, toPrint, root); // Lê o nó correspondente ao índice fornecido
    dispNode(toPrint); // Imprime o conteúdo do nó

    for(int i = 0; i < 2*t; i++) {
        traverse(tree, toPrint->children[i]); // Chama recursivamente a função 'traverse' para cada filho do nó
    }

    free(toPrint);
}

// Imprime as informações de um nó da árvore 
void dispNode(bTreeNode* node)
{
	printf("Position in node:%d\n",node->pos );
    printf("Number of Records:%d\n",node->noOfRecs );
	printf("Is leaf?:%d\n",node->isLeaf );
	printf("codigoLivros:\n");
	for(int i = 0; i < node->noOfRecs; i++)
	{
		printf("%d ", node->keyRecArr[i]);     // Imprime as chaves no nó
	}
	printf("\n");
	printf("Links:\n");
	for (int i = 0; i < 2*t; ++i)
	{
		printf("%d ",node->children[i] );       // Imprime índices dos filhos do nó
	}
	printf("\n");
    printf("\n");
}

int searchRecursive(bTree* tree, int codigoLivro, bTreeNode* root) {
    int i = 0;
    
    while(i < root->noOfRecs && codigoLivro > root->keyRecArr[i]) // descobre a posição do nó específico
        i++;
    
    
    if(i < root->noOfRecs && codigoLivro == root->keyRecArr[i])    // caso de encontro da codigoLivro
        return root->keyRecArr[i];
    
    
    else if(root->isLeaf) {
        return -1;
    }
    else {
        bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, root->children[i]);

        int found = searchRecursive(tree, codigoLivro, childAtPosi);   // realiza a busca recursivamenta
        free(childAtPosi);
        return found;
    }
}

// Realiza uma busca na árvore B para encontrar um registro com a chave especificada
int search(bTree* tree, int codigoLivro) {
    
    bTreeNode* root = malloc(sizeof(bTreeNode));
    readFile(tree, root, tree->root);

    int result = searchRecursive(tree, codigoLivro, root); // Chama a função de busca recursiva para encontrar o registro
    free(root);
    return result;
     
}

// Encontra o índice da chave dentro do nó
int findcodigoLivro(bTreeNode* node, int k) {
    int idx=0;

    while (idx < node->noOfRecs && node->keyRecArr[idx] < k)
        ++idx;
    return idx;
}

// remove o valor de uma folha
void removeFromLeaf (bTree* tree, bTreeNode *node, int idx) {
    for (int i=idx+1; i<node->noOfRecs; ++i){ // rearranja os registros
	    node->keyRecArr[i-1] = node->keyRecArr[i];
    }
    node->noOfRecs--; // registra a remoção
}

// Remove um node não folha da árvore B 
void removeFromNonLeaf(bTree* tree, bTreeNode *node, int idx) {
 
    int k = node->keyRecArr[idx];
    
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx+1]); 
 
    
    // Se o filho do nó atual tem pelo menos t chaves, encontra o predecessor, substitui a chave a ser removida e remove recursivamente o predecessor
    if (child->noOfRecs >= t) {
        int pred = getPred(tree, node, idx);
        node->keyRecArr[idx] = pred;
        removeNode(tree, child, pred); 
    }
 
    // Se o filho do nó atual não tem pelo menos t chaves, mas o irmão tem, encontra o sucessor, substitui a chave a ser removida e remove recursivamente o sucessor
    else if  (sibling->noOfRecs >= t)
    {
        int succ = getSucc(tree, node, idx);
        node->keyRecArr[idx] = succ;
        removeNode(tree, sibling, succ); 
    }
 
        else { // Se ambos o filho e o irmão do nó atual têm apenas t-1 chaves, mescla o filho e o irmão e remove recursivamente a chave
        child = merge(tree, node, idx); 
        removeNode(tree, child, k);
	    return; 
    }
    
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(child);
    free(sibling);
}

// remove um nó da arvore
void removeNode(bTree* tree, bTreeNode* node, int k) {

    int idx = findcodigoLivro(node, k);
    if (idx < node->noOfRecs && node->keyRecArr[idx] == k) { // se a chave está no nó
        if (node->isLeaf){
	        removeFromLeaf(tree, node, idx); // Remove a chave 'k' do nó folha
        } else {
            removeFromNonLeaf(tree, node, idx); // Remove a chave 'k' do nó não folha
        }
        
	    writeFile(tree, node, node->pos); // escreve alteração no arquivo
    }
    else {
       
        if (node->isLeaf) {
//		    return false;
       	}
 
        bool flag = idx==node->noOfRecs;
 

        bTreeNode *childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, node->children[idx]);

        if (childAtPosi->noOfRecs < t) {
            // Se o nó filho estiver com menos de 't' registros,
            // preenche o nó filho com registros dos nós irmãos
            fill(tree, node, idx);
            readFile(tree, childAtPosi, node->children[idx]);
        }

        if (flag && idx > node->noOfRecs) {
            // Caso a flag seja verdadeira e o índice seja maior que o número de registros,
            // lê o nó irmão à esquerda do nó filho e remove a chave 'k' dele
            bTreeNode *sibling = malloc(sizeof(bTreeNode));
            readFile(tree, sibling, node->children[idx-1]);
            removeNode(tree, sibling, k); // chama recursivamente a remoção

            writeFile(tree, sibling, sibling->pos);
            free(sibling);
        }
        else {
            removeNode(tree, childAtPosi, k);
        }
        
        writeFile(tree, childAtPosi, childAtPosi->pos);
        free(childAtPosi);
    }
}

// Obtém o predecessor de uma chave em um nó da árvore B
int getPred(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *curr = malloc(sizeof(bTreeNode));
    readFile(tree, curr, node->children[idx]);

    while (!curr->isLeaf){ // Percorre a arvore até encontrar folha
        readFile(tree, curr, curr->children[curr->noOfRecs]);
    }
        
    int result = curr->keyRecArr[curr->noOfRecs-1]; // Retorna o último registro no nó folha
    free(curr);
    return result;
}

// Obtém o sucessor de uma chave em um nó da árvore B 
int getSucc(bTree* tree, bTreeNode *node, int idx) {
 
    bTreeNode *curr = malloc(sizeof(bTreeNode));
    readFile(tree, curr, node->children[idx+1]); 
    while (!curr->isLeaf){ // Percorre os nós filhos até encontrar uma folha (nó sem filhos)
        readFile(tree, curr, curr->children[0]);
    }
 
    
    int result = curr->keyRecArr[0]; // Retorna o primeiro registro (sucessor) no nó folha
    free(curr);
    return result;
}
 
//Preenche um node da árvore B após remoção de uma chave
void fill(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *cPrev = malloc(sizeof(bTreeNode));
    bTreeNode *cSucc = malloc(sizeof(bTreeNode));

    readFile(tree, cPrev, node->children[idx-1]);
    readFile(tree, cSucc, node->children[idx+1]);
    
    if (idx!=0 && cPrev->noOfRecs>=t) { // Se o índice não for zero e o filho anterior tiver pelo menos t chaves, realiza uma operação de empréstimo do filho anterior
        borrowFromPrev(tree, node, idx);
    }
 

    else if (idx!=node->noOfRecs && cSucc->noOfRecs>=t) { // Se o índice não for o último e o filho sucessor tiver pelo menos t chaves, realiza uma operação de empréstimo do filho sucessor
        borrowFromNext(tree, node, idx);
    }
 
    else { // Caso contrário, mescla o nó com seu irmão correspondente
        if (idx != node->noOfRecs)
            merge(tree, node, idx);
        else
            merge(tree, node, idx-1);
    }

    free(cPrev);
    free(cSucc);

    return;
}

//Realiza a operação de empréstimo de chaves de um irmão anterior para um nó filho em uma árvore B  
void borrowFromPrev(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx-1]);
    
    // Desloca os registros no nó filho para acomodar o espaço para a nova chave
    for (int i=child->noOfRecs-1; i>=0; --i)
        child->keyRecArr[i+1] = child->keyRecArr[i];
    
    // Desloca os ponteiros para os filhos no nó filho, se existirem
    if (!child->isLeaf) {
        for(int i=child->noOfRecs; i>=0; --i)
            child->children[i+1] = child->children[i];
    }
 
    // Copia a última chave do nó irmão para o nó filho
    child->keyRecArr[0] = node->keyRecArr[idx-1];
 
    if (!node->isLeaf) {// Copia o último ponteiro para o filho do nó irmão para o nó filho, se existirem
        child->children[0] = sibling->children[sibling->noOfRecs];
        sibling->children[sibling->noOfRecs] = -1; 
    }
 
    node->keyRecArr[idx-1] = sibling->keyRecArr[sibling->noOfRecs-1]; // Atualiza a chave no nó atual com a última chave do nó irmão
 
    child->noOfRecs += 1;
    sibling->noOfRecs -= 1;
    
    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);
    
    free(child);
    free(sibling);

    return;
}

 //Realiza um empréstimo de chaves do irmão posterior para um nó filho
void borrowFromNext(bTree* tree, bTreeNode *node, int idx) {
 
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx+1]);
    
    
    child->keyRecArr[(child->noOfRecs)] = node->keyRecArr[idx]; // Move a chave do irmão para o nó filho
 
    if (!(child->isLeaf))
        child->children[(child->noOfRecs)+1] = sibling->children[0];
 
    
    node->keyRecArr[idx] = sibling->keyRecArr[0];   // Atualiza a chave no nó pai com a primeira chave do irmão posterior
 
    for (int i=1; i<sibling->noOfRecs; ++i) // Desloca as chaves restantes no irmão posterior para preencher o espaço vazio
        sibling->keyRecArr[i-1] = sibling->keyRecArr[i];
 
    if (!sibling->isLeaf) {
        for(int i=1; i<=sibling->noOfRecs; ++i)
            sibling->children[i-1] = sibling->children[i];

        sibling->children[sibling->noOfRecs] = -1; //RESPOSTA:
    }
 
    // Incrementa o número de registros no nó filho e decrementa no irmão posterior
    child->noOfRecs += 1;
    sibling->noOfRecs -= 1;
    
    writeFile(tree, node, node->pos); // reescreve os nós atualizados
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(child);
    free(sibling);

    return;
}

//Mescla um nó filho com seu irmão posterior, movendo as chaves e filhos relevantes, resultando em um único nó.
bTreeNode* merge(bTree* tree, bTreeNode *node, int idx) {
    // Aloca memória para o nó filho e o irmão posterior
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx+1]);
    
    child->keyRecArr[t-1] = node->keyRecArr[idx]; // Move a chave do nó pai para o nó filho
 
    for (int i=0; i<sibling->noOfRecs; ++i) // Move as chaves do irmão posterior para o nó filho
        child->keyRecArr[i+t] = sibling->keyRecArr[i];
 
    if (!child->isLeaf) { // Move os filhos do irmão posterior para o nó filho, se existirem
        for(int i=0; i<=sibling->noOfRecs; ++i)
            child->children[i+t] = sibling->children[i];
    }

    for (int i=idx+1; i<node->noOfRecs; ++i) // Reposiciona as chaves do nó pai e os filhos após a fusão
        node->keyRecArr[i-1] = node->keyRecArr[i];
 
    for (int i=idx+2; i<=node->noOfRecs; ++i) 
        node->children[i-1] = node->children[i];
    node->children[node->noOfRecs] = -1; // Define o último filho do nó pai como -1
    child->noOfRecs += sibling->noOfRecs+1;
    node->noOfRecs--;
 
    if(node->noOfRecs == 0) {
        tree->root = node->children[0];
    }

    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(sibling);

    return child;
}

// Remover uma observação a partir da chave
bool removeFromTree(bTree* tree, int codigoLivro) {
    bTreeNode *root = malloc(sizeof(bTreeNode));
    readFile(tree, root, tree->root);

    bool found = search(tree, codigoLivro); // Verifica se a chave 'codigoLivro' está presente na árvore
    if(found);
        removeNode(tree, root, codigoLivro); // Se a chave foi encontrada, remove-a da árvore

    free(root);
    return found;
}

//Percorre todos os registros de um arquivo da árvore B e exibe as informações de cada nó
void hardPrint(bTree* tree) {
    bTreeNode* lido = (bTreeNode*) malloc(sizeof(bTreeNode));
    for(int i = 0; i < tree->nextPos; i++) { // Percorre os registros do arquivo
        fseek(tree->fp, i * sizeof(bTreeNode), SEEK_SET); // Posiciona o ponteiro de leitura no registro atual
        fread(lido, sizeof(bTreeNode), 1, tree->fp);

        if(lido->isLeaf <= 1) // Verifica se é folha e exibe as informações
            dispNode(lido);
        else
            printf("ERRO: isLeaf = %i\n\n", lido->isLeaf);
    }

    free(lido);
}

//Realiza as duas operações de impressão na árvore B
void doublePrint(bTree* tree) {
    printf("=================");
    printf("\nTraverse\n");
    traverse(tree, tree->root);

    printf("=================");
    printf("\nHard print\n");
    hardPrint(tree);
}