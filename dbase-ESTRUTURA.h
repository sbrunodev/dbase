// ESTRUTURA
// --------------------------------
struct numeric
{
	int valorN;
	struct numeric * prox;
	
};
typedef struct numeric numeric;
// --------------------------------

// --------------------------------
struct memochar
{
	char valorMC[50];
	struct memochar * prox;
	
};
typedef struct memochar memochar;
// --------------------------------

// --------------------------------
struct logical
{
	char valorL;
	struct logical * prox;
	
};
typedef struct logical logical;
// --------------------------------

// --------------------------------
struct date
{
	char valorD[10];
	struct date * prox;
	
};
typedef struct date date;
// --------------------------------

//---------------------------------
struct status
{
	char statu;
	struct status * prox;
	
};
typedef struct status status;
//---------------------------------

// -----CAMPO----------------------
struct campos
{
	numeric * PN;
	date * PD;
	logical * PL;
	memochar * PMC;
	
	char fieldname [30];
	char type;
	int width;
	int dec;
	
	numeric * pnumeric;
	logical * plogical;
	date * pdate;
	memochar* pmemochar;

	struct campos * Prox; 
};
typedef struct campos campos;
// --------------------------------

// -----ARQUIVO--------------------
struct arquivo
{
	char nomedbf[30];
	char data [9];
	char hora [9];
	struct arquivo *prox, *ant;
	status * pstatus;
	campos * pcampos;
};
typedef struct arquivo arquivo;
// --------------------------------

// ------UNIDADE-------------------
struct unidade 
{
	char Unidade;
	struct unidade *prox,*ant;
	arquivo * parquivo;
};
typedef struct unidade unidade;

// --------------------------------

// FIM - ESTRUTURAS 

void exibeUnidade (unidade *Uni)
{	
	if (Uni!=NULL)
	{
		printf ("%c: ",Uni->Unidade);
		exibeUnidade (Uni->prox);
	}
}



void criaUnidade (unidade**Uni) // Inicializa o programa com duas Unidades
{
	//CRIA UNIDADE C:
	unidade *UnidadeC = (unidade*) malloc (sizeof(unidade));
	UnidadeC->Unidade = 'C';
	UnidadeC->prox=NULL; 
	UnidadeC->ant= NULL; 
	UnidadeC->parquivo=NULL;
	*Uni=UnidadeC;
	
    //CRIA UNIDADE F:
	unidade *UnidadeF = (unidade*) malloc (sizeof(unidade));
	UnidadeF->Unidade = 'F';
	UnidadeF->prox=NULL; 
	UnidadeF->ant= NULL; 
	UnidadeF->parquivo=NULL;
	
	unidade *Aux;
	Aux = *Uni;
	Aux->prox=UnidadeF;
	UnidadeF->ant=Aux;
}

void BuscaArquivo (arquivo ** AuxArquivo, char * Nome)
{
	while (*AuxArquivo!=NULL && stricmp ((*AuxArquivo)->nomedbf,Nome)!=0)
				*AuxArquivo=(*AuxArquivo)->prox;	
}

void DELETE_ALL ( status *st)
{
	while (st!=NULL)
	{
		(st)->statu='0';
		st=(st)->prox;
	}
}

void RECALALL_REATIVA(status *st)
{
	while (st!=NULL)
	{
		(st)->statu='1';
		st=(st)->prox;
	}
}

void CREATE_CriaCampo (arquivo ** arq)
{
	campos * novocampo;
	
	char FieldName[30];
	char type;
	int width;
	int dec;
	char Op='S';
	
	do
	{	
			do
			{
				 printf ("\n   Field Name: ");
				 fflush(stdin);
				 gets(FieldName);
				 
			}while (strcmp(FieldName,"\0")==0);
			
			do
			{
			    printf ("   Types: N - numeric, C - Character, L - Logical, D - Date");
				printf ("\n   Type: ");
				
				fflush(stdin);
				scanf("%c",&type);
				type=toupper(type);
				// EXIBE OS TIPO DE DADOS DISPONIVEIS
			}while (type!='N' && type!='D' && type!='L' && type!='C');
					
			printf ("   Width: ");
			scanf ("%d",&width);
			
			printf ("   Dec: ");
			scanf ("%d",&dec);
				
			novocampo = (campos*) malloc (sizeof(campos));
			strcpy (novocampo->fieldname,FieldName);
			
			novocampo->width=width;
			novocampo->dec=dec;
			novocampo->type=type;
			novocampo->Prox=NULL;
			novocampo->PN= NULL;
			novocampo->PD= NULL;
			novocampo->PMC=NULL;
			novocampo->PL= NULL;
			novocampo->pdate=NULL;
			novocampo->plogical=NULL;
			novocampo->pmemochar=NULL;
			novocampo->pnumeric=NULL;
			
			if ((*arq)->pcampos==NULL)
				(*arq)->pcampos = novocampo;
			else
			{
				
				campos *Aux;
				Aux = (*arq)->pcampos;
				while (Aux->Prox!=NULL)
					Aux=Aux->Prox;
					
				Aux->Prox=novocampo;
					
			}
			
			printf ("          Incluir novamente ? (S|N)");
			fflush(stdin);
			scanf("%c",&Op); Op=toupper(Op);
					
		}while(Op=='S');
				  
}

void Create (unidade**Uni, char *Parte)
{
	 char Data[9],Hora[9],Nome[30],Op;	
	 _strdate( Data ); _strtime( Hora );
	 
	arquivo * NovoArq = (arquivo*) malloc (sizeof(arquivo));
	
	strcpy(NovoArq->nomedbf,Parte);
	strcpy(NovoArq->data,Data);
	strcpy(NovoArq->hora,Hora);

	NovoArq->prox=NULL;
	NovoArq->ant=NULL;
	NovoArq->pcampos=NULL;
	NovoArq->pstatus=NULL;

	if ((*Uni)->parquivo==NULL)
		(*Uni)->parquivo = NovoArq;
	else
	{
		arquivo *Aux;
		Aux = (*Uni)->parquivo;
		while (Aux->prox!=NULL)
			Aux=Aux->prox;
			
		Aux->prox=NovoArq;
		NovoArq->ant=Aux;
	}	
	
	printf ("          Incluir campos ? (S|N)");
	fflush(stdin);
	scanf("%c",&Op); Op=toupper(Op);
	if (Op=='S')
	{	arquivo * AuxArquivo = (*Uni)->parquivo;
		BuscaArquivo(&AuxArquivo,Parte);
		CREATE_CriaCampo(&AuxArquivo);
	}	
}

void DIR_MostraDBF (arquivo * Arq)
{
	if (Arq==NULL)
		printf ("\n Arquivo Vazio \n ");
	else
	{
		printf ("\n");
		printf (" NOME   DATA   HORA");
		while (Arq!=NULL)
		{
			printf ("\n");
			printf ("%s ",Arq->nomedbf);
			printf ("%s ",Arq->data);
			printf ("%s ",Arq->hora);
			
			Arq=Arq->prox;
		}
		printf ("\n");
		printf ("\n");		
	}
}

void listStruct (campos * Campos)
{
	if (Campos==NULL)
		printf ("\nSem Campos\n");
	else
	{
		int i = 1;
		char Type[10];
		printf ("\n Field     FieldName       Type        Width     Dec \n");
		while (Campos!=NULL)
		{
			if (Campos->type == 'N')
				strcpy (Type,"Numeric");
			if (Campos->type == 'D')
				strcpy (Type,"Date");
			if (Campos->type == 'C')
				strcpy (Type,"Character");
			if (Campos->type == 'L')
				strcpy (Type,"Logical");
			
			printf ("   %d\t     %s\t    %s     %d \t  %d \n",i,Campos->fieldname,Type,Campos->width,Campos->dec);
			Campos = Campos->Prox;i++;
		}	
		printf ("\n\n");
	}	
}

void APPEND_AdicionaReg (arquivo ** arq)
{
	
	campos * cam;
	cam = (*arq)->pcampos;
	char Op;
	
		while (cam!=NULL)
		{
			printf ("      %s:",cam->fieldname);
			
			switch (cam->type)
			{
				case 'N': // Adiciona aos campos numericos, como por exemplo Código, Numero .... 
						{
							int numero=0;
							scanf ("%d",&numero);
							
							numeric * novoNumeric = (numeric*) malloc (sizeof(numeric));
							novoNumeric->valorN=numero;
							novoNumeric->prox=NULL;
							
							if (cam->pnumeric==NULL)
							{
								cam->pnumeric=novoNumeric;
								cam->PN=cam->pnumeric;
							}	
							else
							{  
								numeric * auxcamN; // Auxiliar campos de numericos
								auxcamN=cam->pnumeric;
								while (auxcamN->prox!=NULL)
									auxcamN=auxcamN->prox;
									
								auxcamN->prox=novoNumeric;
									
							}
								
						}break;
						
				 case 'C': // Adiciona aos campos de character
						{
							char palavra[cam->width];
							
							
							do
							{
								fflush(stdin);
								scanf ("%s",&palavra);
								
							}while(strcmp(palavra,"\0")==0);
							
							memochar * novoMemochar = (memochar*) malloc (sizeof(memochar)); // Faz uma nova caixinha memochar;
							
							strcpy(novoMemochar->valorMC,palavra);
							novoMemochar->prox=NULL;
							
							if (cam->pmemochar==NULL)
							{
								cam->pmemochar=novoMemochar;
								cam->PMC=cam->pmemochar;
							}
								
							else
							{  
								memochar * auxcamMC; // Auxiliar campos de numericos
								auxcamMC=cam->pmemochar;
								while (auxcamMC->prox!=NULL)
									auxcamMC=auxcamMC->prox;
									
								auxcamMC->prox=novoMemochar;
										
							}
								
						}break;
					case 'D': // Adiciona aos campos de Date
						{
							char data[10];
								
							do
							{
								fflush(stdin);
								scanf ("%s",&data);
								
							}while(strcmp(data,"\0")==0);
							
							date * novoDate = (date*) malloc (sizeof(date)); // Faz uma nova caixinha Date;
							
							strcpy(novoDate->valorD,data);
							novoDate->prox=NULL;
							
							if (cam->pdate==NULL)
							{
								cam->pdate=novoDate;
								cam->PD=cam->pdate;
							}
							else
							{  
								date * auxcamD; // Auxiliar campos de numericos
								auxcamD=cam->pdate;
								while (auxcamD->prox!=NULL)
									auxcamD=auxcamD->prox;
									
								auxcamD->prox=novoDate;			
							}
								
						}break;
						
						 case 'L': // Adiciona aos campos de Logico
						{
							char log ;
							
							
							do
							{
								printf (" 1-true | 0-false ");
								fflush(stdin);
								scanf ("%c",&log);
								
							}while(log!='1' && log!='0');
							
							logical * novoLogical = (logical*) malloc (sizeof(logical)); // Faz uma nova caixinha memochar;
							
							novoLogical->valorL=log;
							novoLogical->prox=NULL;
							
							if (cam->plogical==NULL)
							{
								cam->plogical=novoLogical;
								cam->PL=cam->plogical;
							}
								
							else
							{  
								logical * auxcamL; // Auxiliar campos de numericos
								auxcamL=cam->plogical;
								while (auxcamL->prox!=NULL)
									auxcamL=auxcamL->prox;
									
								auxcamL->prox=novoLogical;
										
							}
								
						}break;
			}
			
			cam=cam->Prox; // Passa para o proximo campo ! 
		}
		// SEMPRE QUANDO EU ADICIONAR DADOS A ESTRUTURA EU TENHO QUE DEIXAR ELE TRUE
		status * novoStatus = (status*) malloc (sizeof(status));
		novoStatus->prox=NULL;
		novoStatus->statu='1';
		
		if( (*arq)->pstatus == NULL )
			(*arq)->pstatus = novoStatus;
		else
		{
			status * auxStatus;
			auxStatus = (*arq)->pstatus;
			while (auxStatus->prox != NULL)
				auxStatus=auxStatus->prox;
				
			auxStatus->prox=novoStatus;
		}
		
}

void exibeCampos (campos * cam)
{
	while (cam!=NULL)
	{
		printf (" |%s ",cam->fieldname);
		cam=cam->Prox;
	}
}

void exibeNumeric (numeric * num, int x, int y, status * st)
{
	y++;
	if (num==NULL)
	{
		gotoxy(x,y);
		printf ("Nao");
		gotoxy(x,y+1);
		printf ("ha");
		gotoxy(x,y+2);
		printf ("dados");
	}
		
	else
	{
		while (num!=NULL)
		{
			if (st->statu=='1')
			{
			   gotoxy (x,y);
			   printf ("%d\n",num->valorN);
			   y++;
			}
			st=st->prox;
			num=num->prox;
	
		}
	}
}

void exibeMemoChar (memochar * mc, int x, int y, status * st)
{
	y++;
	if (mc==NULL)
	{
		gotoxy(x,y);
		printf ("Nao");
		gotoxy(x,y+1);
		printf ("ha");
		gotoxy(x,y+2);
		printf ("dados");
	}
	else
	{
		while (mc!=NULL)
		{
			if(st->statu=='1')
			{
				gotoxy(x,y);
				printf ("%s\n",mc->valorMC);
				y++;
			}
			st=st->prox;
			mc=mc->prox;
			
		}
	}
}

void exibeDate (date * dat, int x, int y, status * st)
{
	y++;
	if (dat==NULL)
	{
		gotoxy(x,y);
		printf ("Nao");
		gotoxy(x,y+1);
		printf ("ha");
		gotoxy(x,y+2);
		printf ("dados");
	}
	
	else
	{
		while (dat!=NULL)
		{
			if(st->statu=='1')
			{
				gotoxy(x,y);
				printf ("%s\n",dat->valorD);
				y++;
			}
			dat=dat->prox;
			st=st->prox;
		}
	}
}

void exibeLogical (logical * l, int x, int y, status * st)
{
	y++;
	if (l==NULL)
	{
		gotoxy(x,y);
		printf ("Nao");
		gotoxy(x,y+1);
		printf ("ha");
		gotoxy(x,y+2);
		printf ("dados");
	}
	
	else
	{
		while (l!=NULL)
		{
			if (st->statu=='1')
			{
				gotoxy(x,y);
				printf ("%c\n",l->valorL);	
				y++;	
			}
			l=l->prox;
			st=st->prox;
		}
	}
}

void ProcuraRegistro (campos**cam, char nome[30])
{
	while ((*cam)!=NULL && strcmp ((*cam)->fieldname,nome)!=0)
		*cam=(*cam)->Prox;	
}

void ExcluiNumericos (numeric**num)
{	if (*num!=NULL)
	{
		ExcluiNumericos(&(*num)->prox);
		free(*num);
	}
}

void ExcluiMemochar (memochar**mchar)
{	if (*mchar!=NULL)
	{
		ExcluiMemochar(&(*mchar)->prox);
		free(*mchar);
	}
}

void ExcluiData (date**dt)
{   if (*dt!=NULL)
	{
		ExcluiData(&(*dt)->prox);
		free(*dt);
	}
}

void ExcluiLogic (logical**Log)
{	if (*Log!=NULL)
	{
		ExcluiLogic(&(*Log)->prox);
		free(*Log);
	}
}

void ExcluiStatu (status**st)
{	if (*st!=NULL)
	{
		ExcluiStatu(&(*st)->prox);
		free(*st);
	}
}

void ZAP_ExcluiReg (arquivo **arq)
{
	if ((*arq)->pcampos==NULL)
		printf ("Nao contem Registros\n");
	else
	{
		//------------------------------------------
		campos * cam;
		cam = (*arq)->pcampos;
		
		while (cam!=NULL)
		{
			// TEM QUE FAZER UM SWITCH PRA SABER QUAL É O TIPO DO CAMPO E EXCLUIR SEUS COMPONENTES ! 
			switch (cam->type)
			{
				case 'N':
				{
					ExcluiNumericos (&(cam)->pnumeric);
					cam->PN=NULL;
					cam->pnumeric=NULL;
					
				}break;
				case 'C':
				{
					ExcluiMemochar (&(cam)->pmemochar);
					cam->PMC=NULL;	
					cam->pmemochar=NULL;
				}break;
				case 'D':
				{
					ExcluiData (&(cam)->pdate);
					cam->PD=NULL;
					cam->pdate=NULL;
				}break;
				case 'L':
				{
					ExcluiLogic (&(cam)->plogical);
					cam->PL=NULL;
					cam->plogical=NULL;	
				}break;	
			}
			cam=cam->Prox;
		}
		
	}	
}

int ContadorRecords (status * st)
{
	if(st!=NULL)
	{
		int i=0;
		while(st!=NULL)
		{
			
			st=st->prox;
			i++;
			
		}
		return i;
	}
	else
		return 0;
}

int ContadorRecordsON (status * st)
{
	if(st!=NULL)
	{
		int i=0;
		while(st!=NULL)
		{	
		
			if (st->statu=='1')
				i++;
				
			st=st->prox;
		
		}
		return i;
	}
	else
		return 0;
}

void PACK_DELETE (arquivo ** arq)
{
	status * st; campos * cam;
	
	cam=(*arq)->pcampos;
		st = (*arq)->status;
	while (cam!=NULL)
	{
		st = (*arq)->status;
		if (st->statu=='1')
			printf ("ADDADS");
		switch (cam->type)
		{
			case 'N':
			{
				numeric * num;
				num = cam->pnumeric;	st = (*arq)->status;
				while (num!=NULL)
				{
					if (st->statu=='0')
					{
						printf ("Exclui:%d",num->valorN);
					}
					else
					{
						
					}
					num=num->prox;
					st=st->prox;
					
				}
				
			}
			
		}
		
		cam=cam->prox;
	}
}


void list_registros (arquivo ** arq)
{
	if ((*arq)->pcampos==NULL)
		printf (" Nao ha campos\n");
	else
	{
		campos * cam;
		cam = (*arq)->pcampos; 
		int x=2,y=4,Record=1,a=y,ContRecord=0;
		
		gotoxy(x,y);
		printf ("Record");
		
		while (cam!=NULL)
		{
			x=x+12;
			gotoxy(x,y);
			printf ("%s",cam->fieldname);
			
			switch (cam->type)
			{
				case 'N':
						{
							exibeNumeric(cam->pnumeric,x,y+1,(*arq)->pstatus); // CONTRECORD GAMBIARRA
						
						}break;
						
				case 'C':
						{
							exibeMemoChar(cam->pmemochar,x,y+1,(*arq)->pstatus);	
						}break;
						
				case 'D':
						{
							exibeDate(cam->pdate,x,y+1,(*arq)->pstatus);	
						}break;
						
				case 'L':
						{
							exibeLogical(cam->plogical,x,y+1,(*arq)->pstatus);	
						}break;
			}
			cam=cam->Prox;
		}
		
		ContRecord = ContadorRecordsON((*arq)->pstatus);
		while (ContRecord>0)
		{
			gotoxy(2,a+2);
			printf ("#%d",Record);
			a++;
			Record++;
			ContRecord--;
		}
	}	
}

void HelpArquivo ()
{
	printf (" HELP \n");

	printf ("     |Append - Insere dados aos campos  \n");
	printf ("     |List Structure - Exibe a estrutura\n");
	printf ("     |List   - Exibe os dados da Estrutura  \n");
	printf ("     |Clear  - Limpa a Tela  \n");
	printf ("     |..     - Volta para unidade  \n");
	printf ("     |Pack   - Exclui campos de um determinado arquivo  \n");
	printf ("  \n");
	
}

int LOCATE_forNome (campos * cam, char Nome[30])
{
	int Pos=1;
		if (cam->type=='C')
		{			
			memochar * auxChar;
			auxChar = cam->pmemochar;
			
			while (auxChar!=NULL && strcmp(auxChar->valorMC,Nome)!=0)
			{
				auxChar=auxChar->prox;
				Pos++;
			}	
			
			if (auxChar==NULL)
				return 0;
			else
				return Pos;
		}	
	
	/*	if (cam->type=='N')
		{			
			numeric * auxNum;
			auxNum = cam->pnumeric;
			int Num = atoi(Nome);
			while (auxNum!=NULL && strcmp(auxNum->valorN,Num)!=0)
			{
				auxNum=auxNum->prox;
				Pos++;
			}	
			
			if (auxNum==NULL)
				return 0;
			else
				return Pos;
		}*/		
}

int GOTO (arquivo ** arq, int Record)
{
		campos * cam;
		
		numeric * num; memochar * mchar; date * d; logical * l;
		cam = (*arq)->pcampos;
		int Pos;
		
		while (cam!=NULL)
		{
			Pos=Record;
			switch (cam->type)
			{
				case 'N':
				{
					num = cam->pnumeric;
					while(num!=NULL && Pos>1)
					{
						num = num->prox;
						Pos--;
					}
					if (num!=NULL)
						cam->PN=num;				
				}break;
				
				case 'C':
				{	
					mchar = cam->pmemochar;
					while(mchar!=NULL && Pos>1)
					{
						mchar = mchar->prox;
						Pos--;
					}
					if (mchar!=NULL)
						cam->PMC=mchar;		
				}break;
				
				case 'D':
				{	
					d = cam->pdate;
					while(d!=NULL && Pos>1)
					{
						d = d->prox;
						Pos--;
					}	
					if (d!=NULL)
						cam->PD=d;			
				}break;
				
				case 'L':
				{
					l = cam->plogical;
					while(l!=NULL && Pos>1)
					{
						l = l->prox;
						Pos--;
					}	
					if (l!=NULL)
						cam->PL=l;			
				}break;
					
			}
				
			cam=cam->Prox;
		}
		  return 1;
}

void DISPLAY (arquivo ** arq)
{
	campos * cam;
	cam = (*arq)->pcampos;
	
	while (cam!=NULL)
	{
		printf ("   %s: ",cam->fieldname);
		switch (cam->type)
		{
			case 'N':
			{
				printf ("   %d \n",cam->PN->valorN);
				
			}break;
			
			case 'C':
			{
				printf ("   %s \n",cam->PMC->valorMC);
				
			}break;
			
			case 'D':
			{
				printf ("   %s \n",cam->PD->valorD);
				
			}break;
			
			case 'L':
			{
				printf ("   %c \n",cam->PL->valorL);
				
			}break;	
		}
		
		cam=cam->Prox;
	}	
}

void EDIT (arquivo ** arq)
{
	campos * cam;
	cam = (*arq)->pcampos;
	
	while (cam!=NULL)
	{
		printf (" %s: ",cam->fieldname);
		switch (cam->type)
		{
			case 'N':
			{
				int ValorN;
				scanf ("%d",&ValorN);
				cam->PN->valorN=ValorN;
				
			}break;
			
			case 'C':
			{
				char ValorMC[50];
				fflush(stdin);
				gets(ValorMC);
				strcpy(cam->PMC->valorMC,ValorMC);
				
			}break;
			
			case 'D':
			{
				char ValorData[50];
				fflush(stdin);
				gets(ValorData);
				strcpy(cam->PD->valorD,ValorData);
				
			}break;
			
			case 'L':
			{
				char Logical;
				fflush(stdin);
				scanf("%c",&Logical);
				cam->PL->valorL=Logical;
				
			}break;	
		}
		
		cam=cam->Prox;
	}	
}

void DELETE (status * st, int Pos)
{

	int i=1;
	while (i<Pos)
	{
		st=st->prox;
		i++;
	}
	st->statu='0';

}


void Help()
{
	printf (" HELP \n");	
	printf ("     |Create - Cria Estrutura.DBF ou Adiciona campos caso ja exista\n");
	printf ("     |Use    - Usa a Estrutura.DBF  \n");
	printf ("     |Clear  - Limpa Tela \n");
	printf ("     |..     - Volta p/ Raiz \n");
}


void HelpUnidade()
{
	printf (" HELP \n");
	
	printf ("     |Set default to Unidade: \n");
	printf ("     |Clear \n");
	printf ("     |Quit \n");
}



