#include<stdio.h>
#include<string.h>
#include<math.h>
#include<conio.h>
#include<windows.h>

/////////////////////////////////////////////////// Structs ////////////////////////////////////////////////////////////////

struct TpLivro{
	int codeL; //chave primaria
	char nomeL[50];
	char nomeAutor[50];
	int qtdeCopias;
	float preco;
	char statusL;
};

struct TpAutor{
	char nomeA[50]; //chave primaria
	char statusA;
};

struct TpData{
	int ano;
	int mes;
	int dia;
};

struct TpCliente{
	char cpf[15]; //chave primaria
	char nomeC[50];
	int idade;
	char statusC;
};

struct TpCompra{
	int codeCompra; //chave primaria
	int codeLivro;
	int qtdeCopiasC;
	char cpfCliente[15];
	struct TpData dataAquisicao;
	char statusCompra;
};

//////////////////////////////////////////////////////// Buscas /////////////////////////////////////////////////////////////

int BuscaBinariaCodeLivro(int code, FILE *PtrArq){
	TpLivro dados;
	int i=0, m, f;
	
	fseek(PtrArq,2,0);
	f = ftell(PtrArq)/sizeof(TpLivro);
	m = f/2;
	
	fseek(PtrArq,m*sizeof(TpLivro),0);
	fread(&dados,sizeof(TpLivro),1,PtrArq);
	while(i<f && dados.codeL != code){
		
		if(code < dados.codeL){
			f = m-1;
		}
		else{
			i = m+1;
		}
		
		m=(i+f)/2;
		
		fseek(PtrArq,m*sizeof(TpLivro),0);
		fread(&dados,sizeof(TpLivro),1,PtrArq);
	}
	
	if(code == dados.codeL){
		return m;
	}
	else{
		return -1;
	}
}

int BuscaExaustivaNomeAutor(char nomeA[50], FILE *PtrAutor){
	TpAutor dados;
	rewind(PtrAutor);
	
	fread(&dados,sizeof(TpAutor),1,PtrAutor);
	while(!feof(PtrAutor) && strcmp(nomeA,dados.nomeA) != 0){
		fread(&dados,sizeof(TpAutor),1,PtrAutor);
	}
	
	if(strcmp(nomeA,dados.nomeA) == 0){
		return ftell(PtrAutor)/sizeof(TpAutor);
	}
	else{
		return -1;
	}
}

int BuscaSentinelaNomeAutor(FILE *PtrAutor,char nome[50]){
	int qtde,i=0;
	TpAutor dados;
	
	strcpy(dados.nomeA,nome);
	fwrite(&dados,sizeof(TpAutor),1,PtrAutor);
	
	fseek(PtrAutor,0,2);
	qtde = ftell(PtrAutor)/sizeof(TpAutor);
	
	rewind(PtrAutor);
	fread(&dados,sizeof(TpAutor),1,PtrAutor);
	while(strcmp(dados.nomeA,nome) != 0){
		fread(&dados,sizeof(TpAutor),1,PtrAutor);
		i++;
	}
	
	fseek(PtrAutor,(qtde-1)*sizeof(TpAutor),0);
	dados.statusA = 'I';
	fwrite(&dados,sizeof(TpAutor),1,PtrAutor);
	
	if(i != qtde-1){
		return i;
	}
	else{
		return -1;
	}
}

int BuscaSequencialCPF(char cpf[15], FILE *PtrCliente){
	TpCliente dados;
	rewind(PtrCliente);
	
	fread(&dados,sizeof(TpCliente),1,PtrCliente);
	while(!feof(PtrCliente) && strcmp(dados.cpf,cpf) > 0){
		fread(&dados,sizeof(TpCliente),1,PtrCliente);
	}
	
	if(strcmp(dados.cpf,cpf) == 0){
		return ftell(PtrCliente)/sizeof(TpCliente);
	}
	else{
		return -1;
	}
}

int BuscaSentinelaCodigo(int code,FILE *PtrCompra){
	int qtde,i=0;
	TpCompra dados;
	
	dados.codeCompra = code;
	fwrite(&dados,sizeof(TpCompra),1,PtrCompra);
	
	fseek(PtrCompra,0,2);
	qtde = ftell(PtrCompra)/sizeof(TpCompra);
	
	rewind(PtrCompra);
	fread(&dados,sizeof(TpCompra),1,PtrCompra);
	while(code != dados.codeCompra){
		fread(&dados,sizeof(TpCompra),1,PtrCompra);
		i++;
	}
	
	fseek(PtrCompra,(qtde-1)*sizeof(TpCompra),0);
	dados.statusCompra = 'I';
	fwrite(&dados,sizeof(TpCompra),1,PtrCompra);
	
	if(i != (qtde-1)){
		return i;
	}
	else{
		return -1;
	}
}

///////////////////////////////////////////////////// Ordenacos ////////////////////////////////////////////////////////////

void insercaoDireta(FILE *PtrLivro){
	int pos,qtde;
	TpLivro dados,aux;
	
	fseek(PtrLivro,0,2);
	qtde = ftell(PtrLivro)/sizeof(TpLivro);
	
	fseek(PtrLivro,(qtde-1)*sizeof(TpLivro),0);
	fread(&dados,sizeof(TpLivro),1,PtrLivro);
	
	fseek(PtrLivro, (qtde-2)*sizeof(TpLivro),0);
	fread(&aux,sizeof(TpLivro),1,PtrLivro);
	
	while(qtde>0 && dados.codeL < aux.codeL){
		
		fseek(PtrLivro,(qtde-1)*sizeof(TpLivro),0);
		fwrite(&aux,sizeof(TpLivro),1,PtrLivro);
		
		fseek(PtrLivro,(qtde-2)*sizeof(TpLivro),0);
		fwrite(&dados,sizeof(TpLivro),1,PtrLivro);
		
		qtde--;
		
		if(qtde>1){
			fseek(PtrLivro,(qtde-1)*sizeof(TpLivro),0);
			fread(&dados,sizeof(TpLivro),1,PtrLivro);
			
			fseek(PtrLivro, (qtde-2)*sizeof(TpLivro),0);
			fread(&aux,sizeof(TpLivro),1,PtrLivro);
		}
	}
}

void BubbleSortAutor(FILE *PtrAutor){
	int qtde,i;
	TpAutor dados,aux;
	
	fseek(PtrAutor,0,2);
	qtde = ftell(PtrAutor)/sizeof(TpAutor);
	
	while(qtde > 0){
		
		for(i=0;i<qtde-1;i++){
			
			fseek(PtrAutor,i*sizeof(TpAutor),0);
			fread(&dados,sizeof(TpAutor),1,PtrAutor);
			
			fseek(PtrAutor,(i+1)*sizeof(TpAutor),0);
			fread(&aux,sizeof(TpAutor),1,PtrAutor);
			
			if(strcmp(dados.nomeA,aux.nomeA) > 0){
				fseek(PtrAutor,(i+1)*sizeof(TpAutor),0);
				fwrite(&dados,sizeof(TpAutor),1,PtrAutor);
				
				fseek(PtrAutor,i*sizeof(TpAutor),0);
				fwrite(&aux,sizeof(TpAutor),1,PtrAutor);
			}
		}
		
		qtde--;
		
	}
}

void SelecaoDiretaCPF(FILE *PtrCliente){
	TpCliente dados,aux;
	int qtde,i=0,posMaior;
	char maior[15];
	
	fseek(PtrCliente,0,2);
	qtde = ftell(PtrCliente)/sizeof(TpCliente);
	
	rewind(PtrCliente);
	fread(&dados,sizeof(TpCliente),1,PtrCliente);
		
	while(qtde > 0){
		
		strcpy(maior,dados.cpf);
		posMaior = 0;
		
		for(i=1;i<qtde;i++){
			
			fseek(PtrCliente,i*sizeof(TpCliente),0);
			fread(&dados,sizeof(TpCliente),1,PtrCliente);
			
			if(strcmp(maior,dados.cpf) < 0){
				strcpy(maior,dados.cpf);
				posMaior = i;
			}
			
		}
		
		if(posMaior < qtde-1){
			
			fseek(PtrCliente,posMaior*sizeof(TpCliente),0);
			fread(&aux,sizeof(TpCliente),1,PtrCliente);
			
			fseek(PtrCliente,(qtde-1)*sizeof(TpCliente),0);
			fread(&dados,sizeof(TpCliente),1,PtrCliente);
			
			fseek(PtrCliente,posMaior*sizeof(TpCliente),0);
			fwrite(&dados,sizeof(TpCliente),1,PtrCliente);
			
			fseek(PtrCliente,(qtde-1)*sizeof(TpCliente),0);
			fwrite(&aux,sizeof(TpCliente),1,PtrCliente);
		}
		
		qtde--;
		
		rewind(PtrCliente);
		fread(&dados,sizeof(TpCliente),1,PtrCliente);
	}
}


/////////////////////////////////////////////////////// Funcoes /////////////////////////////////////////////////////////////

char MenuC(void){
	char opc;
	system("cls");
	printf("***************** CADASTRAR ********************\n\n");
	printf("[A] Cadastrar livro\n");
	printf("[B] Cadastrar autor do livro\n");
	printf("[C] Cadastrar cliente\n");
	printf("[D] Cadastrar compra\n");
	printf("[ESC] Voltar ao menu\n");
	
	return opc = getch();
}

void CadLivro(void){
	int code, valid, valid1;
	char nomeA[50];
	TpLivro livro;
	
	FILE *PtrLivro = fopen("Livros.dat","ab+");
	FILE *PtrAutor = fopen("Autores.dat","rb+");
	
	system("cls");
	
	printf("*********************** CADASTRO DE LIVROS ***************************\n\n");
	printf("Digite o codigo do Livro:\n");
	scanf("%d",&code);
	
	valid = BuscaBinariaCodeLivro(code,PtrLivro);
	
	if(valid != -1){
		system("cls");
		printf("Livro cadastrado!\n");
		Sleep(2000);
	}
	else{
		printf("Digite o nome do autor:\n");
		fflush(stdin);
		gets(nomeA);
		
		valid1 = BuscaExaustivaNomeAutor(nomeA,PtrAutor);
		
		if(valid1 == -1){
			system("cls");
			printf("Autor nao cadastrado!\n");
			Sleep(2000);
		}
		else{
			
			livro.codeL = code;
			strcpy(livro.nomeAutor,nomeA);
			
			printf("Digite o nome do livro:\n");
			fflush(stdin);
			gets(livro.nomeL);
			
			printf("Digite o preco do livro (R$):\n");
			scanf("%f",&livro.preco);
			printf("Digite a quantidade de copias:\n");
			scanf("%d",&livro.qtdeCopias);
			
			livro.statusL = 'A';
			
			fseek(PtrLivro,2,0);
			fwrite(&livro,sizeof(TpLivro),1,PtrLivro);
			
			system("cls");
			printf("Livro cadastrado com sucesso!\n");
			Sleep(2000);
			
			insercaoDireta(PtrLivro);
		}
	}
	
	fclose(PtrLivro);
	fclose(PtrAutor);
}

void CadAutor(void){
	int pos, valid;
	char nome[50];
	TpAutor dados;
	
	FILE *PtrAutor = fopen("Autores.dat","ab+");

	system("cls");
	printf("*********************** CADASTRO DE AUTORES ***************************\n\n");
	
	printf("Digite o nome do autor:\n");
	fflush(stdin);
	gets(nome);
	
	valid = BuscaSentinelaNomeAutor(PtrAutor,nome);
	
	if(valid == -1){
		strcpy(dados.nomeA,nome);
		dados.statusA = 'A';
		
		fseek(PtrAutor,2,0);
		fwrite(&dados,sizeof(TpAutor),1,PtrAutor);
		
		BubbleSortAutor(PtrAutor);
		
		system("cls");
		printf("Autor cadastrado com sucesso!\n");
		Sleep(2000);
	}
	else{
		system("cls");
		printf("Autor já cadastrado!\n");
		Sleep(2000);
	}
	
	fclose(PtrAutor);
}

void CadCliente(void){
	int pos;
	char cpf[15];
	TpCliente cliente;
	
	FILE *PtrCliente = fopen("Clientes.dat","ab+");
	
	system("cls");
	printf("*********************** CADASTRO DE CLIENTES ***************************\n\n");
	
	SelecaoDiretaCPF(PtrCliente);
	
	printf("Digite o cpf do cliente [xxx.xxx.xxx-xx]:\n");
	fflush(stdin);
	gets(cpf);
	
	pos = BuscaSequencialCPF(cpf,PtrCliente);
	
	if(pos == -1){
		strcpy(cliente.cpf,cpf);
		
		printf("Digite o nome do cliente:\n");
		fflush(stdin);
		gets(cliente.nomeC);
		
		printf("Digite a idade do cliente:\n");
		scanf("%d", &cliente.idade);
		
		cliente.statusC = 'A';
		
		fseek(PtrCliente,2,0);
		fwrite(&cliente,sizeof(TpCliente),1,PtrCliente);
		
		system("cls");
		printf("Cliente cadastrado com sucesso!\n");
		Sleep(2000);
	}
	else{
		system("cls");
		printf("Cliente ja cadastrado!\n");
		Sleep(2000);
	}
	
	fclose(PtrCliente);
}

void CadCompra(void){
	int code,codeL,valid,valid1,valid2;
	char cpf[15];
	TpCompra compra;
	
	FILE *PtrCompra = fopen("Compras.dat","ab+");
	FILE *PtrLivro = fopen("Livros.dat", "rb+");
	FILE *PtrCliente = fopen("Clientes.dat", "rb+");
	
	system("cls");
	printf("*********************** CADASTRO DE COMPRAS ***************************\n\n");
	
	printf("Digite o codigo da compra:\n");
	scanf("%d", &code);
	
	valid = BuscaSentinelaCodigo(code,PtrCompra);
	
	if(valid == -1){
		
		printf("Digite o codigo do livro:\n");
		scanf("%d", &codeL);
		
		valid1 = BuscaBinariaCodeLivro(codeL,PtrLivro);
		
		if(valid1 == -1){
			system("cls");
			printf("Livro nao cadastrado!\n");
			Sleep(2000);
		}
		else{
			printf("Digite o cpf do cliente [xxx.xxx.xxx-xx]:\n");
			fflush(stdin);
			gets(cpf);
			
			valid2 = BuscaSequencialCPF(cpf,PtrCliente);
			
			if(valid2 == -1){
				system("cls");
				printf("Cliente nao cadastrado!\n");
				Sleep(2000);
			}
			else{
				compra.codeCompra = code;
				compra.codeLivro = codeL;
				strcpy(compra.cpfCliente,cpf);
				compra.statusCompra = 'A';
				
				printf("Digite o dia de aquisicao [DD]:\n");
				scanf("%d",&compra.dataAquisicao.dia);
				printf("Digite o mes de aquisicao [MM]:\n");
				scanf("%d",&compra.dataAquisicao.mes);
				printf("Digite o ano de aquisicao [AAAA]:\n");
				scanf("%d",&compra.dataAquisicao.ano);
				
				printf("Digite a quantidade de copias compradas:\n");
				scanf("%d",&compra.qtdeCopiasC);
				
				fseek(PtrCompra,2,0);
				fwrite(&compra,sizeof(TpCompra),1,PtrCompra);
				
				system("cls");
				printf("Compra cadastrada com sucesso!\n");
				Sleep(2000);
			}
		}
		
	}
	else{
		system("cls");
		printf("Compra ja cadastrada!\n");
		Sleep(2000);
	}
	
	fclose(PtrCompra);
	fclose(PtrLivro);
	fclose(PtrCliente);
}

void Cadastrar(){
	char opc;
	do{
		system("cls");
		opc = MenuC();
		
		switch(toupper(opc)){
			case 'A': CadLivro();
			break;
			case 'B': CadAutor();
			break;
			case 'C': CadCliente();
			break;
			case 'D': CadCompra();
			break;
			default: system("cls");
			break;
		}
	}while(opc != 27);
}

void EdLivro(void){
	TpLivro livro;
	int code,valid,valid2;
	char nomeA[50];
	
	FILE *PtrLivro = fopen("Livros.dat","rb+");
	FILE *PtrAutor = fopen("Autores.dat","rb+");
	
	system("cls");
	printf("*********************** EDITAR LIVROS ***************************\n\n");
	
	printf("Digite o codigo do livro:\n");
	scanf("%d", &code);
	
	valid = BuscaBinariaCodeLivro(code,PtrLivro);
	
	if(valid == -1){
		system("cls");
		printf("Livro nao encontrado!\n");
		Sleep(2000);
	}
	else{
		
		printf("Digite o nome do autor:\n");
		fflush(stdin);
		gets(nomeA);
		
		valid2 = BuscaSentinelaNomeAutor(PtrAutor,nomeA);
		
		if(valid2 == -1){
			system("cls");
			printf("Autor nao encontrado!\n");
			Sleep(2000);
		}
		else{

			fseek(PtrLivro,valid*sizeof(TpLivro),0);
			fread(&livro,sizeof(TpLivro),1,PtrLivro);

			strcpy(livro.nomeAutor,nomeA);
			
			printf("Digite o nome do livro:\n");
			fflush(stdin);
			gets(livro.nomeL);
			
			printf("Digite o preco:\n");
			scanf("%f",&livro.preco);
			printf("Digite a quantidade de copias:\n");
			scanf("%d",&livro.qtdeCopias);
			
			fseek(PtrLivro,valid*sizeof(TpLivro),0);
			fwrite(&livro,sizeof(TpLivro),1,PtrLivro);
			
			system("cls");
			printf("Editado com sucesso!\n");
			Sleep(2000);
		}
	}

	fclose(PtrLivro);
	fclose(PtrAutor);
}

void EdAutor(void){

	system("cls");
	printf("*********************** EDITAR AUTORES ***************************\n\n");	
}

void EdCliente(void){
	
	system("cls");
	printf("*********************** EDITAR CLIENTES ***************************\n\n");
	
}

void EdCompra(void){
	
	system("cls");
	printf("*********************** EDITAR COMPRAS ***************************\n\n");
}

char MenuEd(void){
	char opc;
	
	printf("***************** EDITAR ********************\n\n");
	printf("[A] Editar livro\n");
	printf("[B] Editar autor do livro\n");
	printf("[C] Editar cliente\n");
	printf("[D] Editar compra\n");
	printf("[ESC] Voltar ao menu\n");
	
	return opc = getch();
}

void Editar(){
	char opc;
	do{ 
		system("cls");
		opc = MenuEd();
		switch(toupper(opc)){
			case 'A': EdLivro();
			break;
			case 'B': EdAutor();
			break;
			case 'C': EdCliente();
			break;
			case 'D': EdCompra();
			break;
			default: system("cls");
			break;
		}
	}while(opc != 27);
}

void ExcLivro(void){
	
}

void ExcAutor(void){
	
}

void ExcCliente(void){
	
}

void ExcCompra(void){
	
}

char MenuExc(void){
	char opc;
	
	printf("***************** EXCLUIR ********************\n\n");
	printf("[A] Excluir livro\n");
	printf("[B] Excluir autor do livro\n");
	printf("[C] Excluir cliente\n");
	printf("[D] Excluir compra\n");
	printf("[ESC] Voltar ao menu\n");
	return opc = getch();
}

void Excluir(){
	char opc;
	do{
		system("cls");
		opc = MenuExc();
		switch(toupper(opc)){
			case 'A': ExcLivro();
			break;
			case 'B': ExcAutor();
			break;
			case 'C': ExcCliente();
			break;
			case 'D': ExcCompra();
			break;
			default: system("cls");
			break;
		}
	}while(opc != 27);
}

void RelLivro(void){
	TpLivro dados;
	FILE *PtrLivro = fopen("Livros.dat","rb+");
	
	system("cls");
	
	rewind(PtrLivro);
	fread(&dados,sizeof(TpLivro),1,PtrLivro);
	
	while(!feof(PtrLivro)){
		
		printf("******* LIVRO %d  ********\n\n",dados.codeL);
		printf("Nome do livro: %s\n",dados.nomeL);
		printf("Nome do autor: %s\n",dados.nomeAutor);
		printf("Preco: R$ %.2f\n",dados.preco);
		printf("Quantidade de copias: %d\n",dados.qtdeCopias);
		
		fread(&dados,sizeof(TpLivro),1,PtrLivro);
	}
	
	getch();
	fclose(PtrLivro);
}

void RelAutor(void){
	TpAutor dados;
	FILE *PtrAutor = fopen("Autores.dat","rb+");
	
	system("cls");
	printf("***************** AUTORES ********************\n\n");
	
	rewind(PtrAutor);
	fread(&dados,sizeof(TpAutor),1,PtrAutor);
	
	while(!feof(PtrAutor)){
		
		printf("Nome do autor: %s\n",dados.nomeA);
		printf("***************************\n\n");
		
		fread(&dados,sizeof(TpAutor),1,PtrAutor);
	}
	
	getch();
	fclose(PtrAutor);
}

void RelCliente(void){
	
}

void RelCompra(void){
	
}

char MenuR(void){
	char opc;
	
	printf("***************** RELATORIO ********************\n\n");
	printf("[A] Relatorio de livros cadastrados\n"); //ordenados por codigo;
	printf("[B] Relatorio de autores\n"); //ordenados por nome;
	printf("[C] Relatorio de clientes\n"); //ordenados por nome;
	printf("[D] Relatorio de compras\n"); //ordenada por codigo;
	printf("[E] Relatorio de livros por autor\n"); //ordenada por codigo do livro;
	printf("[ESC] Voltar ao menu\n");
	return opc = getch();
}

void Relatorio(){
	char opc;
	do{
		system("cls");
		opc = MenuR();
		switch(toupper(opc)){
			case 'A': RelLivro();
			break;
			case 'B': RelAutor();
			break;
			case 'C': RelCliente();
			break;
			case 'D': RelCompra();
			break;
			default: system("cls");
			break;
		}
	}while(opc != 27);
}

char Menu(void){
	char opc;
	
	printf("***************** MENU INICIAL ********************\n\n");
	printf("[A] Cadastrar\n");
	printf("[B] Editar\n");
	printf("[C] Excluir\n");
	printf("[D] Relatorios\n");
	printf("[ESC] Finalizar\n");
	return opc = getch();
}

void Abertura(void){
	char opc;
	do{
		system("cls");
		opc = Menu();
		switch(toupper(opc)){
			case 'A': Cadastrar();
			break;
			case 'B': Editar();
			break;
			case 'C': Excluir();
			break;
			case 'D': Relatorio();
			break;
			default: system("cls");
					 printf("Programa finalizado!\n");
			break;
		}
	}while(opc != 27);

}

/////////////////////////////////////////////////////// MAIN ///////////////////////////////////////////////////////////////

int main(void){
	
	FILE *PtrLivro = fopen("Livros.dat", "ab+");
	FILE *PtrCliente = fopen("Clientes.dat", "ab+");
	FILE *PtrAutor = fopen("Autores.dat", "ab+");
	FILE *PtrCompra = fopen("Compras.dat","ab+");
	
	fclose(PtrLivro);
	fclose(PtrCliente);
	fclose(PtrAutor);
	fclose(PtrCompra);

	Abertura();
	
	return 0;
}
