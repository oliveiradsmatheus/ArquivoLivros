#include <stdio.h>
#include <string.h>
#include <conio2.h>
#include <ctype.h>
#include <windows.h>
#include "Headers\\TadDesc.h"
#include "Headers\\Moldura.h"
#include "Headers\\Mensagem.h"

void ImportarTxt (char ArqTxt[20])
{
	TpArq Reg;
	FILE *arqtxt = fopen (ArqTxt,"r");
	FILE *arqbin = fopen ("Livros.dat","wb");
	
	if(arqtxt == NULL)
	{
		EscrMsg((char*)"Arquivo Nao Encontrado!");
		getch();
		LimpaMsg();
	}
	else
	{
		fscanf(arqtxt,"%[^|]|%[^|]|%[^|]|%d|%d\n", &Reg.autores, &Reg.titulo_livro, &Reg.editora, &Reg.ano, &Reg.paginas);
		while(!feof(arqtxt))
		{
			fwrite(&Reg,sizeof(TpArq),1,arqbin);
			fscanf(arqtxt,"%[^|]|%[^|]|%[^|]|%d|%d\n", &Reg.autores, &Reg.titulo_livro, &Reg.editora, &Reg.ano, &Reg.paginas);
		}
		fwrite(&Reg,sizeof(TpArq),1,arqbin);
		fclose(arqtxt);
	}
	fclose(arqbin);
}

char Menu (void)
{
	textcolor(14);
	gotoxy(40,3);
	printf("TRABALHO BIMESTRAL - ESTRUTURAS DE DADOS I");
	gotoxy(12,6);
	printf(" M E N U");
	textcolor(15);
	gotoxy(4,10);
	printf("[A] - Excluir Livro");
	gotoxy(4,12);
	printf("[B] - Livros por Autor");
	gotoxy(4,14);
	printf("[C] - Livros por Editora");
	gotoxy(4,16);
	printf("[D] - Autores sem Livro");
	
	LimpaMsg();
	EscrMsg((char*)"Selecione uma Opcao no Menu ou Pressione <ESC> para Sair");
	
	return toupper(getch());
}

void ExibirAutorSemLivros (TpDescritor desc, pAutor *Autores)
{
	pAutor *aux = Autores;
	pEditora *e = desc.inicio;
	pLivros *l;
	pListaAutor *la;
	bool sem_livro = true;
	int i=9,Pag=0,j=0,qtde;
	
	qtde = ContSemLivro(desc,Autores);
	
	ExibirTexto((char*)"[D] - Autores sem Livro",4,16,12);
	LimpaTela();
	ExibirTexto((char*)" # # # AUTORES SEM LIVRO # # #",58,6,6);
	gotoxy(108,40);
	printf("Pagina: %d",Pag+1);
	ExibirTexto((char*)"Autores: ",70,7,14);
	printf("%d",qtde);
	
	if(qtde == 0)
	{
		gotoxy(33,9);
		printf("Nao Existem Autores sem Livro!");
	}
	else
	{
		while(aux != NULL)
		{
			e = desc.inicio;
			sem_livro = true;
			while(e != NULL)
			{
				l = e->pListaLivros;
				while(l != NULL)
				{
					la = l->ListaAutor;
					while(la != NULL)
					{
						if(strcmp(la->Autor->nome,aux->nome)==0 && strcmp(la->Autor->sobrenome,aux->sobrenome)==0)
						{
							sem_livro = false;
						}
						la = la->prox;
					}
					l = l->prox;
				}
				e = e->prox;
			}
			if(sem_livro == true)
			{
				if(j==30)
				{
					j=0;
					i=9;
					Pag++;
					getch();
					LimpaTela();
					ExibirTexto((char*)" # # # AUTORES SEM LIVRO # # #",58,6,6);
					gotoxy(108,40);
					printf("Pagina: %d",Pag+1);
					ExibirTexto((char*)"Autores: ",70,7,14);
					printf("%d",qtde);
				}
				gotoxy(33,i);
				textcolor(14);
				printf("[%d] - ",i-8+(Pag*30));
				textcolor(15);
				printf("%s %s",aux->nome,aux->sobrenome);
				i++;
				j++;
			}
			aux = aux->prox;
		}
	}
	getch();
}

void ExcluirLivro (TpDescritor &desc)
{
	pLivros *l;
	pEditora *Editora;
	int i, j, Linha = 0, Pos, qtdeLivros, Pag=0;
	char op;
	
	ExibirTexto((char*)"[A] - Excluir Livro",4,10,12);
	LimpaTela();
	
	Editora = desc.inicio; // Econtra o primeiro livro válido
	l = Editora->pListaLivros;
	if(!l)
	{
		while(Editora && !Editora->pListaLivros)
			Editora = Editora->prox;
		if(Editora)
			l = Editora->pListaLivros;
	}
	
	do
	{
		qtdeLivros = contaLivros(desc);
		LimpaMsg();
		LimpaTela();
		ExibirTexto((char*)"# # # EXCLUIR LIVRO # # #",63,6,6);
		
		if(qtdeLivros==0)
		{
			gotoxy(65,7);
			textcolor(14);
			printf("Livros Cadastrados: %d",contaLivros(desc));
			textcolor(15);
			gotoxy(108,40);
			printf("Pagina: %d",Pag+1);
			gotoxy(33,9);
			printf("Nao Existem Livros para Excluir!");
			getch();
			op=27;
		}
		else
		{
			ExibirLivros(desc,Linha,Pag,qtdeLivros);
			LimpaMsg();
			EscrMsg((char*)"Pressione <DEL> Para Excluir ");
			textcolor(14);
			printf("%s",l->titulo);
			textcolor(15);
			
			op = getch();
			switch(op)
			{
				 case -32:
					op = getch();
					switch(op)
					{
						case 80: // Baixo
							Linha++;
							l=l->prox;
							
							if(!l) // Encontra o próximo livro válido
							{
								Editora = Editora->prox;
								while(Editora && !Editora->pListaLivros)
									Editora = Editora->prox;
								if(Editora)
									l = Editora->pListaLivros;
							}
							
							if(Linha == 30 || Linha+Pag*30 == qtdeLivros)
							{
								Linha = 0;
								
								Editora = desc.inicio; // Econtra o primeiro livro válido
								l = Editora->pListaLivros;
								if(!l)
								{
									while(Editora && !Editora->pListaLivros)
										Editora = Editora->prox;
									if(Editora)
										l = Editora->pListaLivros;
								}
								
								i=0;
								while(i<Linha+Pag*30)
								{
									if(l->prox)
										l = l->prox;
									else
									{
										Editora = Editora->prox;
										while(!Editora->pListaLivros)
											Editora = Editora->prox;
										l = Editora->pListaLivros;
									}
									i++;
								}							
							}
							break;
						case 72: // Cima
							Linha--;
							
							if(Linha<0) // Vai para a última linha disponível
							{
								Linha = 29;
								if(Linha+Pag*30>qtdeLivros)
									Linha = qtdeLivros - Pag*30 - 1;
							}
							
							Editora = desc.inicio; // Econtra o primeiro livro válido
							l = Editora->pListaLivros;
							if(!l)
							{
								while(Editora && !Editora->pListaLivros)
									Editora = Editora->prox;
								if(Editora)
									l = Editora->pListaLivros;
							}
	
							i=0;
							while(i<Linha+Pag*30)
							{
								if(l->prox)
									l = l->prox;
								else
								{
									Editora = Editora->prox;
									while(!Editora->pListaLivros)
										Editora = Editora->prox;
									l = Editora->pListaLivros;
								}
								i++;
							}
							break;
						case 77: // Direita
							Pag++;
							Linha = 0;
							Pos = 30*Pag;
							i=0;
							
							Editora = desc.inicio;
							while(!Editora->pListaLivros)
								Editora = Editora->prox;
							l = Editora->pListaLivros;
							
							if(Pos<qtdeLivros)
							{
								while(i<Pos)
								{
									if(l->prox)
										l = l->prox;
									else
									{
										Editora = Editora->prox;
										while(!Editora->pListaLivros)
											Editora = Editora->prox;
										l = Editora->pListaLivros;
									}
									i++;
								}
							}
							else
								Pag = 0;
							break;
						case 75: // Esquerda
							Pag--;
							if(Pag<0)
								Pag = qtdeLivros/30;
							Linha = 0;
							Pos = 30*Pag;
							i=0;
							
							Editora = desc.inicio;
							while(!Editora->pListaLivros)
								Editora = Editora->prox;
							l = Editora->pListaLivros;
							
							if(Pos<qtdeLivros)
							{
								while(i<Pos)
								{
									if(l->prox)
										l = l->prox;
									else{
										Editora = Editora->prox;
										while(!Editora->pListaLivros)
											Editora = Editora->prox;
										l = Editora->pListaLivros;
									}
									i++;
								}
							}
							else
								Pag = 0;
							break;
						case 83:
							ExcluiEstrutura(desc,l->titulo);
							
							Linha = 0;
							Pag = 0;
							
							if(qtdeLivros > 1)
							{
								Editora = desc.inicio;
								while(!Editora->pListaLivros)
									Editora = Editora->prox;
								l = Editora->pListaLivros;
							}
							else
								op = 27;
							
							LimpaTela();
							LimpaMsg();
					}
			}
		}
		
		
	}while(op!=27);
	
	LimpaMsg();                  
}

int ContAutor (pAutor *Autores)
{
	int Qtde=0;
	
	while(Autores != NULL)
	{
		Qtde++;
		Autores = Autores->prox;
	}
	
	return Qtde;
}

int ContLivro (pLivros *Livros)
{
	int Qtde = 0;
	
	while(Livros != NULL)
	{
		Qtde++;
		Livros = Livros->prox;
	}
	
	return Qtde;
}

void RelatorioLivroEditora (pEditora *Editora)
{
	pLivros *Livro = Editora->pListaLivros;
	pListaAutor *Autor;
	int qtde = ContLivro(Livro), pg=1, i=9, j=0;
	
	LimpaMsg();
	EscrMsg((char*)"Pressione Qualquer Tecla para Voltar");
	LimpaTela();
	ExibirTexto((char*)"EDITORA: ",33,6,6);
	printf("%s",Editora->editora);
	ExibirTexto((char*)"Livros: ",33,7,14);
	printf("[%d]",qtde);
	
	while(Livro!=NULL)
	{
		gotoxy(108,40);
		printf("Pagina: %d",pg);
		if(j<4)
		{
			ExibirTexto((char*)"Titulo: ",33,i,12);
			printf("%s",Livro->titulo);
			i++;
			ExibirTexto((char*)"Ano: ",33,i,14);
			printf("%d, ",Livro->ano);
			ExibirTexto((char*)"Paginas: ",44,i,14);
			printf("%d",Livro->paginas);
			i++;
			ExibirTexto((char*)"Autores:",33,i,14);
			i++;
			
			Autor = Livro->ListaAutor;
			while(Autor != NULL)
			{
				gotoxy(33,i);
				printf("%s, %s",Autor->Autor->sobrenome,Autor->Autor->nome);
				i++;
				
				Autor = Autor->prox;
			}
			
			gotoxy(33,i);
			i++;
			Livro = Livro->prox;
		}
		else
		{
			i=9;
			j=-1;
			pg++;
			getch();
			LimpaTela();
			ExibirTexto((char*)"EDITORA: ",33,6,6);
			printf("%s",Editora->editora);
			ExibirTexto((char*)"Livros: ",33,7,14);
			printf("[%d]",qtde);
		}
		j++;
	}
}

void RelatorioLivroAutor (TpDescritor desc, char nome[50], char sobrenome[50])
{
	pEditora *Editora = desc.inicio;
	pLivros *Livro;
	pListaAutor *Autores, *AuxLista;
	int pg=1, qtLivro = ContLivroAutor(desc,nome,sobrenome), i=9, j=0;
	
	LimpaMsg();
	EscrMsg((char*)"Pressione Qualquer Tecla para Voltar");
	LimpaTela();
	ExibirTexto((char*)"AUTOR: ",33,6,6);
	printf("%s, %s",sobrenome, nome);
	ExibirTexto((char*)"Livros: ",33,7,14);
	printf("[%d]",qtLivro);
	
	while(Editora)
	{
		Livro = Editora->pListaLivros;
		while(Livro)
		{
			Autores = Livro->ListaAutor;
			while(Autores)
			{
				gotoxy(108,40);
				printf("Pagina: %d",pg);
				if(stricmp(nome,Autores->Autor->nome)==0 && stricmp(sobrenome,Autores->Autor->sobrenome)==0)
				{
					if(j==4)
					{
						i=9;
						pg++;
						getch();
						LimpaTela();
						ExibirTexto((char*)"AUTOR: ",33,6,6);
						printf("%s, %s",sobrenome, nome);
						ExibirTexto((char*)"Livros: ",33,7,14);
						printf("[%d]",qtLivro);
						j=0;
					}
					ExibirTexto((char*)"Titulo: ",33,i,12);
					printf("%s",Livro->titulo);
					i++;
					ExibirTexto((char*)"Ano: ",33,i,14);
					printf("%d, ",Livro->ano);
					ExibirTexto((char*)"Paginas: ",44,i,14);
					printf("%d",Livro->paginas);
					i++;
					ExibirTexto((char*)"Autores:",33,i,14);
					i++;
					
					AuxLista = Livro->ListaAutor;								
					while(AuxLista != NULL)
					{
						gotoxy(33,i);
						printf("%s, %s",AuxLista->Autor->sobrenome,AuxLista->Autor->nome);
						i++;
						AuxLista = AuxLista->prox;
					}
					gotoxy(33,i);
					i++;
					j++;
				}
				Autores = Autores->prox;
			}
			Livro = Livro->prox;
		}
		Editora = Editora->prox;
	}
}

void LivrosAutor (TpDescritor &desc, pAutor *Autores)
{
	pAutor *Autor = Autores;
	int i, Linha = 0, Pag=0, Pos, qtAutor;
	char op;
	
	qtAutor = ContAutor(Autores);
	
	ExibirTexto((char*)"[B] - Livros por Autor",4,12,12);
	LimpaTela();
	
	do
	{
		LimpaMsg();
		LimpaTela();
		EscrMsg((char*)"Selecione uma Autor ou Pressione <ESC> para Voltar");
		ExibirTexto((char*)" # # # LIVROS POR AUTOR # # #",60,6,6);
		ExibirAutores(Autores,Linha,qtAutor,Pag);
		
		op = getch();
		switch (op)
		{
			case -32:
				op = getch();
				switch(op)
				{
					case 80: // Baixo
						Linha++;
						Autor = Autor->prox;
						if(Linha == 30 || Linha+Pag*30==qtAutor)
						{
							Linha = 0;
							Autor = Autores;
							i=0;
							while(i<Linha+Pag*30)
							{
								Autor = Autor->prox;
								i++;
							}
						}
						break;
					case 72: // Cima
						Linha--;
						if(Linha<0)
						{
							Linha = 29;
							if(Linha+Pag*30>qtAutor)
								Linha = qtAutor - Pag*30 - 1;
						}
						Autor = Autores;
						i=0;
						while(i<Linha+Pag*30)
						{
							Autor = Autor->prox;
							i++;
						}
						break;
					case 77: // Direita
						Pag++;
						Linha = 0;
						Pos = 30*Pag;
						i=0;
						Autor = Autores;
						if(Pos<qtAutor)
						{
							while(i<Pos)
							{
								Autor = Autor->prox;
								i++;
							}
						}
						else
							Pag = 0;
						break;
					case 75: // Esquerda
						Pag--;
						if(Pag<0)
							Pag = qtAutor/30;
						Linha = 0;
						Pos = 30*Pag;
						i=0;
						Autor = Autores;
						if(Pos<qtAutor)
						{
							while(i<Pos)
							{
								Autor = Autor->prox;
								i++;
							}
						}
						else
							Pag = 0;
				}
				break;
			case 13:
				RelatorioLivroAutor(desc,Autor->nome,Autor->sobrenome);
				getch();
				LimpaTela();
				LimpaMsg();
		}
	}while(op!=27);
	LimpaMsg();
}

void LivrosEditora (TpDescritor &desc)
{
	pEditora *Editora = desc.inicio;
	int i, Linha = 0, Pag=0, Pos;
	char op;
		
	ExibirTexto((char*)"[C] - Livros por Editora",4,14,12);
	LimpaTela();
	
	do
	{
		LimpaMsg();
		LimpaTela();
		EscrMsg((char*)"Selecione uma Editora ou Pressione <ESC> para Voltar");
		ExibirTexto((char*)"# # # LIVROS POR EDITORA # # #",60,6,6);
		ExibirEditoras(desc,Linha,Pag);
		
		op = getch();
		switch (op)
		{
			case -32:
				op = getch();
				switch(op)
				{
					case 80: // Baixo
						Linha++;
						Editora = Editora->prox;
						if(Linha == 30 || Linha+Pag*30==desc.qtde)
						{
							Linha = 0;
							Editora = desc.inicio;
							i=0;
							while(i<Linha+Pag*30)
							{
								Editora = Editora->prox;
								i++;
							}
						}
						break;
					case 72: // Cima
						Linha--;
						if(Linha<0)
						{
							Linha = 29;
							if(Linha+Pag*30>desc.qtde)
								Linha = desc.qtde - Pag*30 - 1;
						}
						Editora = desc.inicio;
						i=0;
						while(i<Linha+Pag*30)
						{
							Editora = Editora->prox;
							i++;
						}
						break;
					case 77: // Direita
						Pag++;
						Linha = 0;
						Pos = 30*Pag;
						i=0;
						Editora = desc.inicio;
						if(Pos<desc.qtde)
						{
							while(i<Pos)
							{
								Editora = Editora->prox;
								i++;
							}
						}
						else
							Pag = 0;
						break;
					case 75: // Esquerda
						Pag--;
						if(Pag<0)
							Pag = desc.qtde/30;
						Linha = 0;
						Pos = 30*Pag;
						i=0;
						Editora = desc.inicio;
						if(Pos<desc.qtde)
						{
							while(i<Pos)
							{
								Editora = Editora->prox;
								i++;
							}
						}
						else
							Pag = 0;
				}
				break;
			case 13:
				RelatorioLivroEditora(Editora);
				getch();
				LimpaTela();
				LimpaMsg();
		}
	}while(op!=27);
	LimpaMsg();
}

void TesteExcBin (void)
{
	TpArq Reg;
	FILE *Ptr = fopen("Livros.dat","rb");
	int i=6, j=0;
	
	if(Ptr != NULL)
	{
		fread(&Reg,sizeof(TpArq),1,Ptr);
		while(!feof(Ptr) && j<30)
		{
			if(j==30)
			{
				j=0;
				getch();
				LimpaTela();
			}
			gotoxy(33,i);
			printf("%s",Reg.titulo_livro);
			fread(&Reg,sizeof(TpArq),1,Ptr);
			i++;
			j++;
		}
		fclose(Ptr);
	}
	getch();
}

int main (void)
{
	TpDescritor desc;
	pAutor *Autores;
	char op;
	
	RetiraCursor();
	Dimensao();
	SetConsoleTitle("DOUTOR LEANDRAO ME DE UM 10");
	
	MolduraCompleta();
	
	ImportarTxt((char*)"Livros.txt");
	
	Autores = BuscaAutores((char*)"Livros.dat");
	
	InicializarDesc(desc);
	
	BuscaEditoras((char*)"Livros.dat",desc,Autores);	
	
	do
	{
		op = Menu();
		switch(op)
		{
			case 'A':	ExcluirLivro(desc);
						break;
			case 'B':	LivrosAutor(desc,Autores);
						break;
			case 'C':	LivrosEditora(desc);
						break;
			case 'D':	ExibirAutorSemLivros(desc,Autores);
						break;
			case 'X': 	TesteExcBin(); // Apenas para verificação caso o Professor queira ver que realmente está excluindo do arquivo binário
		}
		LimpaTela();
	}while(op!=27);
	
	MoldFim();
	MsgFim();
	getch();
	
	return 0;
}
