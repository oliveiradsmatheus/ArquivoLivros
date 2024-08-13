void EscrMsg (char mensagem[])
{
	gotoxy(3,43);
	textcolor(12);
	printf("Mensagem: ");
	textcolor(15);
	printf("%s",mensagem);
}

void ExibirTexto(char frase[100],int Coluna, int Linha, int Cor)
{
	gotoxy(Coluna,Linha);
	textcolor(Cor);
	printf("%s",frase);
	textcolor(15);
}

void LimpaMsg (void)
{
	gotoxy(3,43);
	printf("                                                                                                                    ");
}

void MsgFim (void)
{
	textcolor(14);
	gotoxy(40,3);
	printf("TRABALHO BIMESTRAL - ESTRUTURAS DE DADOS I");
	gotoxy(45,10);
	printf("LIVRARIA - DESCRITOR E PONTEIROS");
	gotoxy(42,13);
	printf("262319152 - FELIPE OLIVEIRA DE CARVALHO");
	gotoxy(43,14);
	printf("262319446 - MATHEUS OLIVEIRA DA SILVA");
	gotoxy(44,15);
	printf("262318806 - VICTOR TERRENGUI BRANDI");
	gotoxy(45,43);
	printf("PROF. DR. LEANDRO LUIZ DE ALMEIDA");
}
