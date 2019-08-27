#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio2.h>
#include <stdlib.h>
#include "dbase-ESTRUTURA.h"

void USE_Arquivo (arquivo ** arq, char Unidade) // Ve se Existe ! 
{
	clrscr();
	printf (" Unidade %c\\%s.dbf \n",Unidade,(*arq)->nomedbf);
	
	char Comando[30],Op;
	char * parte;
	int ComandoValido, AuxGoto=0,PosGoto=0;
	int SetDeleted=1;
	
	do
	{
		
		ComandoValido=0;
		printf ("\n %c\\%s: ",Unidade,(*arq)->nomedbf);
		fflush(stdin);
		gets(Comando);	
		
	
		parte = (char*)strtok(Comando, " ");
	
	    //----------------------------------------------
		if (stricmp(parte,"APPEND")==0)
		{
		
			ComandoValido=1;
			printf ("\n   APPEND \n");
			do
			{
				APPEND_AdicionaReg (&(*arq));		
				printf ("          Incluir novamente ? (S|N)");
				fflush(stdin);
				scanf("%c",&Op); Op=toupper(Op);
					
		    }while(Op=='S');		
				
		}
		//----------------------------------------------
		if (stricmp(parte,"LIST")==0)
		{
			parte = (char*)strtok(NULL, " ");
			
			if (parte==NULL)
			{
				clrscr();
				printf (" Unidade %c\\%s.dbf \n",Unidade,(*arq)->nomedbf);
				ComandoValido=1;		
				list_registros (&(*arq));
			}	
			else
		 	{
				if (stricmp(parte,"STRUCTURE")==0)
				{
				  ComandoValido=1;
					listStruct((*arq)->pcampos);
					printf ("\n\n\n");
					
				}
			}		
		}
		//----------------------------------------------
		if (stricmp(parte,"GOTO")==0)
		{
			ComandoValido=1;
			parte = (char*)strtok(NULL, " ");
			if (parte==NULL)
				printf ("   Informe um valor valido - Goto 2 \n");
			else
			{
				int TotalLinhas=ContadorRecords((*arq)->pstatus);
				PosGoto = atoi(parte);
				if (TotalLinhas>=PosGoto && PosGoto>0)
				{
					if ((*arq)->pcampos==NULL)
						printf ("\n O Arquivo nao contem Campos ! ");
					else
					{
						AuxGoto = GOTO (&(*arq),PosGoto);
						printf (" Ok !");
					}
					
				}
				else
					printf ("   Informe uma posicao Valida\n");	
			}
		}
		//----------------------------------------------
		if (stricmp(parte,"EDIT")==0)
		{
			ComandoValido=1;
			if (AuxGoto==0)
				printf ("   Escolha um Campo usando GOTO ");
			else
			{
			    printf ("   EDIT \n");
				EDIT (&(*arq));
			}			
			
		}		
		//----------------------------------------------
		if (stricmp(parte,"DISPLAY")==0)
		{
			ComandoValido=1;
			if (AuxGoto==0)
				printf ("   Escolha um Campo usando GOTO ");
			else
			{
			    printf ("   DISPLAY \n");
				DISPLAY (&(*arq));
			}			
			
		}
		//----------------------------------------------
		if (stricmp(parte,"DELETE")==0)
		{
			ComandoValido=1;
			parte = (char*)strtok(NULL, " ");
			if (parte==NULL)
			{
				if (AuxGoto==0)
					printf ("   Escolha um Campo usando GOTO ");
				else
				{
					parte = (char*)strtok(NULL, " ");	
					if (parte==NULL)	
						DELETE ((*arq)->pstatus,PosGoto);
					else
						printf ("Comando Invalido - DELETE ");
					AuxGoto=0;
				}	
				
			}
			else
				if(stricmp (parte,"ALL")==0)
				{
					DELETE_ALL ( (*arq)->pstatus );	AuxGoto=0;
				}
				else
					printf ("Comando Invalido - DELETE ALL");
		}
		//----------------------------------------------
		if (stricmp(parte,"RECALL")==0)
		{
			ComandoValido=1;
			parte = (char*)strtok(NULL, " ");
			if (parte==NULL)
			{
				// EXECUTA SÓ O RECALL
				
			}
			else
				if(stricmp (parte,"ALL")==0)
				{
					if (SetDeleted==1)
					{
						RECALALL_REATIVA((*arq)->pstatus);
					}
					else
						printf ("SET DELETED OFF - Deve ser executado previamente");

				}
				else
					printf ("Comando Invalido - DELETE ALL");
		}
		//----------------------------------------------
		if (stricmp(parte,"SET")==0)
		{
			parte = (char*)strtok(NULL, " ");
			if (stricmp(parte,"DELETED")==0)
			{
				parte = (char*)strtok(NULL, " ");
				if (stricmp(parte,"ON")==0)
				{
					SetDeleted=0; ComandoValido=1;
				}	
				if (stricmp(parte,"OFF")==0)
				{
						SetDeleted=1; ComandoValido=1;
				}
					
			}
			else
				printf ("Comando Invalido ! - SET DELETED OFF/ON");
			
		}
		
		//----------------------------------------------
		//----------------------------------------------
		if (stricmp(parte,"PACK")==0)
	  	{
	  		ComandoValido=1;
			if ((*arq)->pcampos==NULL)
			 	printf ("Nao ha campos ! ");
			else
				PACK_DELETE(&(*arq));
			  	
	  	}
		//----------------------------------------------
		if (stricmp(parte,"LOCATE")==0)
		{	
			parte = (char*)strtok(NULL, " "); //FOR
					
			if (stricmp(parte,"FOR")==0)
			{	ComandoValido=1;
						
				parte = (char*)strtok(NULL, " ");//NOME
				
				char nomeCampo[30];
				strcpy(nomeCampo,parte);		
				
				parte = (char*)strtok(NULL, " ");
				
				if (stricmp(parte,"=")==0)
				{
					parte = (char*)strtok(NULL, "=");
					if (parte!=NULL)
					{
						char Procura[30];
						strcpy(Procura,parte);
						
						if ((*arq)->pcampos==NULL)
					        printf ("O arquivo nao contem Campos ! ");
					     else
					    {
					    	campos * cam=(*arq)->pcampos;
					    	ProcuraRegistro(&cam,nomeCampo);
					    	if (cam==NULL)
					    		printf ("Campos nao encontrado ! ");
					    	else
					    	{
					    		int Record=0;
					    		Record = LOCATE_forNome(cam,Procura);
					        	
					       		if (Record==0)
					       			printf (" Nao Encontrado ! ");
					       		else
					       			printf ("Record: #%d",Record);
					    		
					    	}
					    	
						}
					}
					else
						printf ("Comando Invalido ! - Locate for CAMPOS = NOME");
					
				}
				else
					printf ("Comando Invalido ! - Locate for CAMPOS = NOME");
			}
			else
				printf ("Comando Invalido ! - Locate for CAMPOS = NOME");
		}
		//----------------------------------------------
		if (stricmp(parte,"ZAP")==0)
		{
			ComandoValido=1;
				ZAP_ExcluiReg (&(*arq));
				// EXCLUI TODOS OS STATUS DOS REGISTROS
				ExcluiStatu(&(*arq)->pstatus);
				(*arq)->pstatus=NULL;
				
		}
		//----------------------------------------------
		if (stricmp(parte,"CLEAR")==0) 
	  	{
	  		clrscr();
	  		ComandoValido=1;
	  			printf (" Unidade %c\\%s.dbf \n",Unidade,(*arq)->nomedbf);
		
	  	}
	  	//---------------------------------------------------------------------------------
	  	if (stricmp(parte,"HELP")==0)
	 	{
		 	ComandoValido=1;
	 			HelpArquivo();
	 		
	 	}
		//--------------------------------------------------------------------------------
		if(ComandoValido==0)
			printf ("Comando Invalido ! ");
	
	}while (strcmp(Comando,"\0")!=0 && strcmp(Comando,"..")!=0);
	_sleep(500);
		
}


void ARQUIVO (unidade**Uni)
{
	char Comando[30],*parte,Op;
	int ComandoValido;
		
	clrscr();
	printf (" Unidade %C\n",(*Uni)->Unidade);
	do
	{
		printf ("\n root\\%c: ",(*Uni)->Unidade);
		fflush(stdin);
		gets(Comando);
		ComandoValido=0;
		parte = (char*)strtok(Comando," "); // LE A PRIMEIRA PARTE DO NOME ATÉ ACHAR A VIRGULA
		
		arquivo * AuxArquivo;		
		// ---------------------- CRIA ARQUIVO !! -------------------------------------------
	 	if (stricmp (parte,"CREATE")==0)
		{
		  ComandoValido=1;
		  AuxArquivo = (*Uni)->parquivo; // APONTA PARA OS ARQUIVO DA UNIDADE
		  parte = (char*)strtok(NULL, " ");// LE A SEGUNDA PARTE DO NOME
		   if (parte==NULL) // SE A SEGUNDA PARTE FOR NULO NÃO CRIA !! 
		   	 printf ("Informe um nome valido, Ex: Create Nome \n");
		   else
		   {
			  	// VE SE O ARQUIVO EXISTE ! 
				BuscaArquivo(&AuxArquivo,parte);
					
				if (AuxArquivo==NULL) // CASO NÃO EXISTA, CRIA O ARQUIVO ! 
				{
					Create (&(*Uni), parte); 
				}
				else//CASO O ARQUIVO JÁ EXISTA APENAS ACRESENTA MAIS CAMPOS NA ESTRUTURA
				{	
				    printf ("          Incluir novos campos ? (S|N)");
					fflush(stdin);
					scanf("%c",&Op); Op=toupper(Op);
					if (Op=='S')
					 	CREATE_CriaCampo(&AuxArquivo);
				}
		  }  
		    
		}
		//------------------------------------------------------------------------------------

		// ---------------------- USE !! -----------------------------------------------------	
		if (stricmp (parte,"USE")==0)
		{
			AuxArquivo = (*Uni)->parquivo; // APONTA PARA OS ARQUIVO DA UNIDADE
			ComandoValido=1;
			parte = (char*)strtok(NULL, " "); // LE A SEGUNDA PARTE DO NOME
			
		    if (parte==NULL) // SE A SEGUNDA PARTE FOR NULO NÃO CRIA !! 
		  		printf ("Informe um nome valido, Ex: Use Nome \n");
		  	else
		  	{
		  		// VE SE O ARQUIVO EXISTE ! 
				BuscaArquivo(&AuxArquivo,parte);
				
				if (AuxArquivo==NULL)
					printf ("\n Arquivo nao Existente! \n");
				else//ACHOU
				{	
				   USE_Arquivo(&AuxArquivo,(*Uni)->Unidade);
				}
		  	}
		}
		//------------------------------------------------------------------------------
		
		//------------------------------------------------------------------------------
		if (stricmp(parte,"DIR")==0)
		{
			ComandoValido=1;
			DIR_MostraDBF((*Uni)->parquivo);	
		}
		//-------------------------------------------------------------------------------
	
		//-------------------------------------------------------------------------------
		if (stricmp(Comando,"CLEAR")==0) 
	  	{
	  		clrscr();
	  		ComandoValido=1;
	  		printf (" Unidade %C",(*Uni)->Unidade);
		
	  	}
	  	//---------------------------------------------------------------------------------
	  
	  	//---------------------------------------------------------------------------------
	  	if (stricmp(Comando,"HELP")==0)
	 	{
	 		Help();
	 		ComandoValido=1;
	 	}
		//--------------------------------------------------------------------------------
		if (stricmp(Comando,"..")==0)
	 		ComandoValido=1;
		
		if(ComandoValido==0)
			printf ("Comando Invalido ! ");
		
	}while (strcmp(Comando,"\0")!=0 && strcmp(Comando,"..")!=0);
	_sleep(500);
}


void Executa ()
{
	char Comando [30];
	unidade *Uni; // Unidade Principal
	criaUnidade (&Uni);
	
	printf (" Unidades - ");
	exibeUnidade(Uni);
	printf ("\n");
	int Ok=0;
	do
	{
	  printf ("\n root: ");
	  fflush(stdin);
	  gets(Comando);	
	  	//Comando = strupr(Comando);
	  Ok=0; // VALIDA O COMANDO
	  //switch ?
	  
	  	if (stricmp(Comando,"CLEAR")==0) 
	  	{
	  		Ok=1;
			clrscr();
	  		printf (" Unidades - ");
			exibeUnidade(Uni);
			printf ("\n");
	  	}
	  		
	  	unidade *DentroUnidade=Uni;
		if (stricmp(Comando,"SET DEFAULT TO C:")==0) 
		{
			Ok=1;
			//DentroUnidade=Uni; // Aponta para as Unidades
			// printf ("Entrou %c",DentroUnidade->Unidade);
			_sleep(500);
			ARQUIVO (&DentroUnidade);
			// Limpa tudo que estavam fazendo e volta para a raiz
			clrscr();
	  		printf (" Unidades - ");
			exibeUnidade(Uni);
			printf ("\n");
		}
	
	  	if (stricmp(Comando,"SET DEFAULT TO F:")==0) 
	 	{
	 		Ok=1;
			DentroUnidade = DentroUnidade->prox; // Recebe o Prox para chegar na unidade F 
	 		// Modulo Arq : Manda apenas a unidade que vai trabalhar !
	 		_sleep(500);
	 		ARQUIVO (&DentroUnidade);
	 		clrscr();
	  		printf (" Unidades - ");
			exibeUnidade(Uni);
			printf ("\n");
	 	
	 	}
	 	
	 	if (stricmp(Comando,"HELP")==0)
	 	{
	 		Ok=1;
			HelpUnidade();
	 	}
	 	
		if (Ok==0 && stricmp(Comando,"QUIT")!=0)
			printf ("      Informe um comando valido\n");
		
	
	}while(stricmp(Comando,"QUIT")!=0 );		
}


int main ()
{	
	printf ("\t\t dbase, em caso de duvidas HELP\n");
	Executa ();	
}
