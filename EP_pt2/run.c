/*
		            AUTORES

	Andre Fabiano de Carvalho - 13672425
	Nicolas Pereira Risso Vieira - 13672262
	Vitor Borges Santana - 13720129

*/
#include <string.h>
#include "implementation.c"

int main(int argc, char const *argv[])
{

	if(argc==1)
	{
		printf("Please enter one of the options below:\n");
		printf("Commands:\n");
		printf("./run [OPTIONS]\n");
		printf("-b [KEY] -->for building a tree.\n");
		printf("-s [KEY] -->for searching a [KEY].\n");
		printf("-d [KEY] -->for deleting a [KEY].\n");
		printf("Exiting now\n");
		exit(0);
	}
	
	int len,i;
    printf("How many records do you want to build from dataset?");
	scanf("%d",&len);

    bTree* tree = createTree("tree.dat",false); // cria arvore

	bTree* indice = createData("data.dat");  // cria indice 

//    recordNode *records = getData("data/dataset.csv", len);
    
//     for(i=0;i<len;i++)
//     {
// //    	insert(tree,&records[i]);
//     }

    // if(!strcmp(argv[1],"-d"))
    // {
	//     int key, i;
	// 	for(i = 2; i <= (argc - 1); i++){
	// 		sscanf(argv[i],"%d",&key);
	// 		bool res = removeFromTree(tree,key);
	// 		if(res)
	// 		{
	// 			printf("Successfull Deletion of %d key.\n", key);
	// 		}
	// 		else
	// 		{
	// 			printf("Deletion not successful. (%d key)\n", key);
	// 		}
	// 	}
	   
    // }

//        traverse(tree, tree->root);

	// if(!strcmp(argv[1],"-s"))
    // {
	//     // printf("Time Taken to build tree: %f seconds\n",timeToBuild);
	//     int key;
	//     sscanf(argv[2],"%d",&key);
	//     recordNode* res = search(tree,key);

	// 	if(res != NULL) {
	// 		printf("key\ttitulo\tgrate\tscore\trate\n");
	// 		printf("%d\t",res->codigoLivro );
	// 		printf("%s\t",res->titulo );
	// 		printf("%s\t",res->nomeCompletoPrimeiroAutor );
	// 		printf("%d\t",res->anoPublicacao );
	// 		printf("%d\n",res->rate );

	// 		free(res);
	// 	} else
	// 		printf("Record not found!");
    // }


	if(!strcmp(argv[1],"-K")) {

		char input[255];
		char *delim = " ";
		char *delim2 = ";";

		printf("Digite o comando e os valores: ");
		scanf("%s\n", input);


		char* command = strtok(input, delim);
        printf("command: %s\n", command);

		char* code = strtok(input, delim2);

		char* titulo = strtok(input, delim2);

		char* authors = strtok(input, delim2);

		char* year = strtok(input, delim2);
    

		if (strcmp(command, "I") == 0){
			recordNode* record = malloc(sizeof(recordNode));

			enterLivro(record, code, titulo, authors,year, -1); // problema com os ponteiros
		}

		if (strcmp(command, "R") == 0){
			bTreeNode* record = malloc(sizeof(bTreeNode));

			removeNode(tree, record, -1);
		}

		if (strcmp(command, "P") == 0){

			traverse(tree, tree->root);
		}


		
		if(strcmp(command, "B") == 0){ // não puxa do arquivo data.dat
			int key;
			sscanf(argv[2],"%d",&key);
			recordNode* res = search(tree,key);

			if(res != NULL) {
				printf("%d\t",res->codigoLivro );
				printf("%s\t",res->titulo );
				printf("%s\t",res->nomeCompletoPrimeiroAutor );
				printf("%d\n",res->anoPublicacao );

				free(res);
			} else
				printf("O livro nao existe na biblioteca");
    	}	

	

		printf("Comando: %d\n", command);
		printf("Código: %d\n", code);
		printf("Título: %s\n", titulo);
		printf("Autores: %s\n", authors);
		printf("Ano: %d\n", year);


		}


	

//	free(records);
	free(tree);
}	
