// TAD Descritor

struct TpArq
{
	char autores[150], titulo_livro[100], editora[50];
	int ano, paginas;
};

struct pAutor
{
	char sobrenome[50];
	char nome[50];
	pAutor *prox;
};

struct pListaAutor
{
	pAutor *Autor;
	pListaAutor *prox;
};

struct pLivros
{
	char titulo[100];
	int ano, paginas;
	pListaAutor *ListaAutor;
	pLivros *ant, *prox;
};

struct pEditora
{
	char editora[50];
	pLivros *pListaLivros;
	pEditora *prox;
};

struct TpDescritor
{
	int qtde;
	pEditora *inicio, *fim;
};

void InicializarDesc (TpDescritor &desc)
{
	desc.inicio = desc.fim = NULL;
	desc.qtde = 0;
};

pAutor *ncAutor (char nome[50], char sobrenome[50])
{
	pAutor *caixa = new pAutor;
	
	strcpy(caixa->sobrenome,sobrenome);
	strcpy(caixa->nome,nome);
	caixa->prox = NULL;
	
	return caixa;
}

pListaAutor *ncAutorLivro (pAutor *Autor)
{
	pListaAutor *caixa = new pListaAutor;
	
	caixa->prox = NULL;
	caixa->Autor = Autor;
	
	return caixa;
}

pListaAutor *ApontaAutor (char nome[50], char sobrenome[50], pListaAutor *ListaAutores, pAutor *Autores)
{
	pListaAutor *NC, *Atual;
	
	pAutor *Aux;	
	
	if(Autores == NULL)
		ListaAutores = NULL;
	else
	{
		Aux = Autores;
		while(Aux != NULL)
		{
			if(stricmp(nome,Aux->nome)==0 && stricmp(sobrenome,Aux->sobrenome)==0)
			{
				NC = ncAutorLivro(Aux);
				if(ListaAutores == NULL)
					ListaAutores = NC;
				else
				{
					Atual = ListaAutores;
					while(Atual->prox != NULL)
						Atual = Atual->prox;
					Atual->prox = NC;
				}
			}
			Aux = Aux->prox;
		}
	}
	
	return ListaAutores;
}

pListaAutor *BuscaAutorLivro (char TodosAutores[150], char nome[50], char sobrenome[50], pAutor *Autores)
{
	pListaAutor *ListaAutores = NULL;
	int pos=0, i=0;
	bool sobre = true;
	
	while(pos<strlen(TodosAutores))
	{
		if(sobre == true)
		{
			if(TodosAutores[pos] == ',')
			{
				sobrenome[i] = '\0';
				sobre = false;
				pos++;
				i=0;
			}
			else
				sobrenome[i++] = TodosAutores[pos];
		}
		else
		{
			if(TodosAutores[pos] == ';')
			{
				nome[i] = '\0';
				ListaAutores = ApontaAutor(nome,sobrenome,ListaAutores,Autores);
				sobre = true;
				i=0;
				pos++;					
			}
			else
				nome[i++] = TodosAutores[pos];
		}
		pos++;
	}
	nome[i] = '\0';
	ListaAutores = ApontaAutor(nome,sobrenome,ListaAutores,Autores);
	
	return ListaAutores;
}

pLivros *ncLivro (TpArq Livro, pAutor *Autores)
{
	pLivros *caixa = new pLivros;
	char nome[50], sobrenome[50];
	
	strcpy(caixa->titulo,Livro.titulo_livro);
	caixa->ListaAutor = BuscaAutorLivro(Livro.autores, nome, sobrenome, Autores);
	caixa->ano = Livro.ano;
	caixa->paginas = Livro.paginas;
	caixa->ant = NULL;
	caixa->prox = NULL;
	
	return caixa;
}

pLivros *InserirLivro (pLivros *ListaLivros, TpArq Livro, pAutor *Autores)
{
	pLivros *NC = ncLivro(Livro,Autores), *Atual;
	
	if(ListaLivros == NULL)
		ListaLivros = NC;
	else
	{
		if(stricmp(NC->titulo,ListaLivros->titulo)<0)
		{
			NC->prox = ListaLivros;
			ListaLivros->ant = NC;
			ListaLivros = NC;
		}
		else
		{
			Atual = ListaLivros;
			while(Atual->prox != NULL && stricmp(NC->titulo,Atual->prox->titulo)>0)
				Atual = Atual->prox;
			if(stricmp(NC->titulo,Atual->titulo)!=0 && stricmp(NC->titulo,Atual->prox->titulo)!=0)
				if(Atual->prox == NULL)
				{
					NC->ant = Atual;
					Atual->prox = NC;
				}
				else
				{
					NC->prox = Atual->prox;
					NC->ant = Atual;
					NC->prox->ant = NC;
					NC->ant->prox = NC;	
				}
		}
	}
	return ListaLivros;
}

pLivros *BuscaLivros (FILE *arq, char editora[50], pAutor *Autores)
{
	pLivros *pListaLivros = NULL;
	TpArq Reg;
	
	rewind(arq);	
	
	fread(&Reg,sizeof(TpArq),1,arq);
	while(!feof(arq))
	{
		if(stricmp(Reg.editora,editora)==0)
			pListaLivros = InserirLivro(pListaLivros,Reg,Autores);
		fread(&Reg,sizeof(TpArq),1,arq);
	}
	
	return pListaLivros;
}

pEditora *ncEditora (FILE *arq, char editora[50], pAutor *Autores)
{
	pEditora *caixa = new pEditora;
	
	strcpy(caixa->editora,editora);
	caixa->prox = NULL;
	caixa->pListaLivros = BuscaLivros(arq,editora,Autores);
	
	return caixa;
}

void InserirEditora (FILE *arq, char editora[50], TpDescritor &desc, pAutor *Autores)
{
	pEditora *NC = ncEditora(arq,editora,Autores), *aux;
	
	if(desc.qtde == 0)
	{
		desc.inicio = desc.fim = NC;
		desc.qtde++;
	}
	else
	{
		if(stricmp(NC->editora,desc.inicio->editora)<0)
		{
			NC->prox = desc.inicio;
			desc.inicio = NC;
			desc.qtde++;
		}
		else
		{
			if(stricmp(NC->editora,desc.fim->editora)>0)
			{
				desc.fim->prox = NC;
				desc.fim = NC;
				desc.qtde++;
			}
			else
			{
				aux = desc.inicio;
				while(stricmp(NC->editora,aux->prox->editora)>0)
					aux = aux->prox;
				if(stricmp(NC->editora,aux->editora)!=0 && stricmp(NC->editora,aux->prox->editora)!=0)
				{						
					NC->prox = aux->prox;
					aux->prox = NC;
					desc.qtde++;
				}
			}
		}
	}
}

void BuscaEditoras (char Arquivo[20], TpDescritor &desc, pAutor *Autores)
{
	TpArq Reg;
	FILE *arq = fopen(Arquivo,"rb");
	int qtde, i;
	
	if(arq == NULL)
	{
		gotoxy(3,43);
		printf("Arquivo Nao Encontrado!");
		getch();
		gotoxy(3,43);
		printf("                                                                                                                    ");
	}
	else
	{
		fseek(arq,0,2);
		qtde = ftell(arq)/sizeof(TpArq);
				
		for(i=0;i<qtde;i++)
		{
			fseek(arq,i*sizeof(TpArq),0);
			fread(&Reg,sizeof(TpArq),1,arq);
			
			InserirEditora(arq,Reg.editora,desc,Autores);
		}
		fclose(arq);
	}
}

pAutor *InserirAutor (char nome[50], char sobrenome[50], pAutor *Lista)
{
	pAutor *Aux, *NC = ncAutor(nome,sobrenome);
	
	if(Lista == NULL)
		Lista = NC;
	else
	{
		if(stricmp(NC->nome,Lista->nome)<=0 || (stricmp(NC->nome,Lista->nome)==0 && stricmp(NC->sobrenome,Lista->sobrenome)<=0))
		{
			if(stricmp(NC->nome,Lista->nome)!=0 && stricmp(NC->sobrenome,Lista->sobrenome)!=0)
			{
				NC->prox = Lista;
				Lista = NC;
			}
				
		}
		else
		{
			Aux = Lista;
			
			while(Aux->prox != NULL && stricmp(NC->nome,Aux->prox->nome)>0)
				Aux = Aux->prox;
			if(Aux->prox == NULL)
			{
				NC->prox = Aux->prox;
				Aux->prox = NC;
			}
			else
			{
				if(stricmp(NC->nome,Aux->prox->nome)==0)
					while(Aux->prox != NULL && stricmp(NC->sobrenome,Aux->prox->sobrenome)>0)
						Aux = Aux->prox;
				if(stricmp(Aux->prox->nome,NC->nome) || stricmp(Aux->prox->sobrenome,NC->sobrenome)!=0)
				{
					NC->prox = Aux->prox;
					Aux->prox = NC;
				}
			}			
		}		
	}	
	return Lista;
}

pAutor *BuscaAutores (char Arquivo[20])
{
	FILE *arq = fopen(Arquivo,"rb");
	pAutor *ListaAutores = NULL;
	TpArq Reg;
	char sobrenome[50],nome[50];
	int pos,i;
	bool sobre;
	
	if(arq == NULL)
	{
		gotoxy(3,43);
		printf("Arquivo Nao Encontrado!");
		getch();
		gotoxy(3,43);
		printf("                                                                                                                    ");
	}
	else
	{
		fread(&Reg,sizeof(TpArq),1,arq);
		while(!feof(arq))
		{
			pos=0;
			i=0;
			sobre = true;
			while(pos<strlen(Reg.autores))
			{
				if(sobre == true)
				{
					if(Reg.autores[pos] == ',')
					{
						sobrenome[i] = '\0';
						sobre = false;
						pos++;
						i=0;
					}
					else
						sobrenome[i++] = Reg.autores[pos];
				}
				else
				{
					if(Reg.autores[pos] == ';')
					{
						nome[i] = '\0';
						ListaAutores = InserirAutor(nome,sobrenome,ListaAutores);
						sobre = true;
						i=0;
						pos++;
					}
					else
						nome[i++] = Reg.autores[pos];
				}
				pos++;
			}
			nome[i] = '\0';	
			ListaAutores = InserirAutor(nome,sobrenome,ListaAutores);
			
			fread(&Reg,sizeof(TpArq),1,arq);
		}
		fclose(arq);
	}
	
	return ListaAutores;
}

void ExibirAutores (pAutor *Autores, int Linha, int Qtde, int Pag)
{
	int i=9, j=0;
	
	gotoxy(70,7);
	textcolor(14);
	printf("Autores: %d",Qtde);
	textcolor(15);
	gotoxy(108,40);
	printf("Pagina: %d",Pag+1);
	
	while(j<(30*Pag))
	{
		Autores = Autores->prox;
		j++;
	}
	
	j=0;
	
	while(Autores != NULL && j<30)
	{
		gotoxy(33,i);
		if(i==Linha+9)
		{
			textbackground(7);
			textcolor(0);
			printf("[%d] - %s %s",i-8+(30*Pag), Autores->nome, Autores->sobrenome);
			gotoxy(33,i);
			textbackground(0);
			textcolor(15);
			
			Autores = Autores->prox;
		}
		else
		{
			textcolor(14);
			printf("[%d] - ",i-8+(30*Pag));
			textcolor(15);
			printf("%s %s",Autores->nome, Autores->sobrenome);
			Autores = Autores->prox;
		}
		i++;
		j++;
	}
}

void ExibirEditoras (TpDescritor desc, int Linha, int Pag)
{
	pEditora *editora = desc.inicio;
	int i=9, j=0;
	
	gotoxy(70,7);
	textcolor(14);
	printf("Editoras: %d",desc.qtde);
	textcolor(15);
	gotoxy(108,40);
	printf("Pagina: %d",Pag+1);
	
	while(j<(30*Pag))
	{
		editora = editora->prox;
		j++;
	}
	
	j=0;
	
	while(editora != NULL && j<30)
	{
		gotoxy(33,i);
		if(i==Linha+9)
		{
			textbackground(7);
			textcolor(0);
			printf("[%d] - %s",i-8+(Pag*30), editora->editora);
			gotoxy(33,i);
			textbackground(0);
			textcolor(15);
		}
		else
		{
			textcolor(14);
			printf("[%d] - ",i-8+(Pag*30));
			textcolor(15);
			printf("%s",editora->editora);
		}
		i++;
		j++;
			
		editora = editora->prox;
	}
}

void ExcluirArq(char titulo[100])
{
	FILE *PtrA = fopen("Livros.dat","rb");
	FILE *PtrN = fopen("temp.dat","wb");
	TpArq Reg;
	
	if(PtrA == NULL)
	{
		fclose(PtrN);
		gotoxy(3,43);
		printf("Arquivo Nao Encontrado!");
		getch();
		gotoxy(3,43);
		printf("                                                                                                                    ");
	}
	else
	{
		fread(&Reg,sizeof(TpArq),1,PtrA);
		while(!feof(PtrA))
		{
			if(stricmp(Reg.titulo_livro,titulo)!=0)
				fwrite(&Reg,sizeof(TpArq),1,PtrN);
			fread(&Reg,sizeof(TpArq),1,PtrA);
		}
		
		fclose(PtrA);
		fclose(PtrN);
		
		remove("Livros.dat");
		rename("temp.dat","Livros.dat");
	}
}

int ContSemLivro (TpDescritor desc, pAutor *Autores)
{
	pAutor *aux = Autores;
	pEditora *e = desc.inicio;
	pLivros *l;
	pListaAutor *la;
	bool sem_livro;
	int Qtde=0;
	
	
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
			Qtde++;
		aux = aux->prox;
	}
	
	return Qtde;
}

int ContLivroAutor (TpDescritor desc, char nome[50], char sobrenome[50])
{
	pEditora *Editora = desc.inicio;
	pLivros *Livros;
	pListaAutor *Autor;
	int Qtde=0;
	
	while(Editora != NULL)
	{
		Livros = Editora->pListaLivros;
		while(Livros != NULL)
		{
			Autor = Livros->ListaAutor;
			while(Autor != NULL)
			{
				if(stricmp(nome,Autor->Autor->nome)==0 && stricmp(sobrenome,Autor->Autor->sobrenome)==0)
					Qtde++;
				Autor = Autor->prox;
			}
			Livros = Livros->prox;
		}
		Editora = Editora->prox;
	}
	
	return Qtde;
}

int contaLivros(TpDescritor desc)
{
	pEditora *Editora = desc.inicio;
	pLivros *Livro;
	int Qtde=0;
	
	while(Editora != NULL)
	{
		Livro = Editora->pListaLivros;
		while(Livro != NULL)
		{
			Qtde++;
			Livro = Livro->prox;
		}
		Editora = Editora->prox;
	}
	
	return Qtde;
}

void ExibirLivros (TpDescritor desc, int Linha, int Pag, int Qtde)
{
	pEditora *editora;
	pLivros *l;
	int i=9, j=0;
	
	gotoxy(65,7);
	textcolor(14);
	printf("Livros Cadastrados: %d",contaLivros(desc));
	textcolor(15);
	gotoxy(108,40);
	printf("Pagina: %d",Pag+1);
	
	editora = desc.inicio;
	while(!editora->pListaLivros)
		editora = editora->prox;
	l = editora->pListaLivros;
	
	while(j<(30*Pag))
	{
		if(l->prox)
			l = l->prox;
		else
		{
			editora = editora->prox;
			while(!editora->pListaLivros)
				editora = editora->prox;
			l = editora->pListaLivros;
		}
		j++;
	}
	
	j=0;
	
	while(editora)
	{
		if(j>0)
			l = editora->pListaLivros;
		while(l && j<30)
		{
			gotoxy(33,i);
			if(Linha+9 == i)
			{
				textbackground(7);
				textcolor(0);
				printf("[%d] - %s",i-8+(30*Pag), l->titulo);
				textbackground(0);
				textcolor(15);
			}
			else
			{
				textcolor(14);
				printf("[%d] - ",i-8+(30*Pag));
				textcolor(15);
				printf("%s",l->titulo);
			}
			l = l->prox;
			i++;
			j++;
		}
		editora = editora->prox;
	}
}

void ExcluiEstrutura(TpDescritor &d, char titulo[100])
{
	pEditora *e = d.inicio;
	pLivros *aux;
	bool exc = false;
	char TituloLivro[100];
	
	strcpy(TituloLivro,titulo);
	
	while(e != NULL && exc == false)
	{
		aux = e->pListaLivros;
		while(aux != NULL && exc == false)
		{
			if(strcmp(aux->titulo,titulo) == 0)
			{
				if(!aux->prox && !aux->ant)
				{
					delete aux;
					e->pListaLivros = NULL;
				}
				else
					if(!aux->prox)
					{
						aux->ant->prox=NULL;
						delete aux;
					}
					else
						if(!aux->ant)
						{
							e->pListaLivros=aux->prox;
							e->pListaLivros->ant=NULL;
							delete aux;
						}
						else
						{
							aux->ant->prox=aux->prox;
							aux->prox->ant=aux->ant;
							delete aux;
						}
				ExcluirArq(TituloLivro);
				exc = true;
			}
			if(!exc)
				aux=aux->prox;
		}
		if(!exc)
			e=e->prox;
	}
}
