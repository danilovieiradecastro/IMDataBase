// IMDB.cpp : Defines the entry point for the console application.
//

//#define COUNT_COLISAO   //define para obter o contador de coliões de cada registro hash descomentar no arquivo stdafx.h
//#define MEDIR_TEMPO //define para obter as medições de tempo descomentar no arquivo stdafx.h
#include <iostream>
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* strtol */
#include <iostream>
#include "stdafx.h"
#include <tchar.h>
int _tmain(int argc, _TCHAR* argv[])
{
	using namespace std;
	DataBase * db = new DataBase();
	
	bool opcaoCarregar = true;
	string str;
	bool execute = true;

	Tabela* tabl;
	int i;
	string* colunas;
	string nomeColuna = "";
	string nomeTabela;
	string resultado;
	while (execute)
	{

		cout << "menu de opcoes:"<<endl;
		if (opcaoCarregar)
		{
			cout << "1 - carregar o arquivo usda.sql (tem que estar na mesma pasta do executavel)" << endl;
			cout << "2 - carregar de um arquivo especifico." << endl;
		}
		cout << "3 - criar uma tabela" << endl;
		cout << "4 - consultar dados de uma tabela pela chave primaria" << endl;
		cout << "5 - inserir em uma tabela" << endl;
		cout << "6 - remover linha de uma tabela" << endl;
		cout << "7 - contar registros da tabela (SELECT COUNT)" << endl;
		cout << "8 - Realizar Inner Join" << endl;

		cout << "9 - Realizar Left Outer Join" << endl;
		cout << "A - Realizar Rigt Outer Join" << endl;
		cout << "X - sair" << endl;
		
		
		char option = getchar();
		getline(cin, str);
		switch (option)
		{
		case ('1'):
			if (!opcaoCarregar)
			{
				system("CLS"); 
				cout << "opção invalida." << endl;
			}
			else
			{
				system("CLS");
				cout << "carregando..." << endl;
				db->criarDBArquivo("");
			}
			break;
		case '2':
			if (!opcaoCarregar)
			{
				system("CLS");
				cout << "opcao invalida." << endl;
			}
			else
			{
				cout << "digite o caminho do arquivo" << endl;
				getline(cin, str);
				cout << "carregando..." << endl;
				db->criarDBArquivo(str);
			}
			break;
		case '3':
			nomeColuna = "";
			nomeTabela = "";
			system("CLS");
			cout << "digite o nome da tabela a ser criada: " << endl;
			getline(cin, nomeTabela);
			
			tabl = db->criarTabela(nomeTabela);
			while (nomeColuna != "X")
			{
				cout << "digite o nome da coluna (digite X para sair): " << endl;
				getline(cin, nomeColuna);
				if (nomeColuna == "")
				{
					cout << "nome da coluna invalido " << endl;
				}
				else if (nomeColuna != "X")
				{
					tabl->addColumn(nomeColuna);
				}

			}
			colunas = new string[tabl->getCoulumCount()];
			 i = 0;
			 nomeColuna = "";
			while (nomeColuna != "X" && i < tabl->getCoulumCount())
			{
				cout << "digite o nome da coluna para fazer parte da chave primaria (digite X para sair): " << endl;
				getline(cin, nomeColuna);
				if (nomeColuna == "")
				{
					cout << "nome da coluna invalido " << endl;
				}
				else if (nomeColuna != "X")
				{
					if (tabl->getColumnIndex(nomeColuna) <0)
					{
						cout << "nome da coluna invalido " << endl;
					}
					else
					{
						colunas[i] =nomeColuna;
						i++;
					}
				}

			}
			if (i > 0)
			{
				tabl->createIndex(colunas, i);
			}
			
			break;
		case '4' :
			system("CLS");
			tabl = NULL;
			while (tabl == NULL)
			{

				
				cout << "digite o nome da tabela (X para sair):" << endl;
				getline(cin, nomeTabela);
				
				tabl = db->findTable(nomeTabela);
				if (nomeTabela == "X")
				{
					break;
				}
				else if(tabl == NULL)
				{
					cout << "nome invalido" << endl;
				}
			}
			if (tabl)
			{
				cout << "digite a chave de busca (digite o valor de todos os campos juntos sem espaço):" << endl;
				getline(cin, str);
				resultado = tabl->findRow(str);
				cout << resultado << endl;
				system("pause");
			}

			system("CLS");
			break;
		case '5' :
			system("CLS");
			tabl = NULL;
			while (tabl == NULL)
			{


				cout << "digite o nome da tabela (digite X para sair):" << endl;
				getline(cin, nomeTabela);

				tabl = db->findTable(nomeTabela);
				
				 if (nomeTabela == "X")
				{
					break;
				}
				else if (tabl == NULL)
				{
					cout << "nome invalido" << endl;
				}
			}
			resultado = "";
			nomeColuna = "";
			if (tabl)
			{
				i = 0;
				string* buff = new string[tabl->getCoulumCount()];
				while (nomeColuna != "X" && i < tabl->getCoulumCount())
				{
					cout << "digite o valor da coluna numero: " << i + 1 << " (digite X para sair): " << endl;
					getline(cin, nomeColuna);
					if (nomeColuna != "X")
					{
						buff[i] = nomeColuna;
						i++;
					}

				}
				if (i >= tabl->getCoulumCount() && nomeColuna != "X")
				tabl->addRow(buff,i);
			}
			break;

		case '6':
			system("CLS");
			tabl = NULL;
			while (tabl == NULL)
			{


				cout << "digite o nome da tabela (digite X para sair):" << endl;
				getline(cin, nomeTabela);

				tabl = db->findTable(nomeTabela);

				if (nomeTabela == "X")
				{
					break;
				}
				else if (tabl == NULL)
				{
					cout << "nome invalido" << endl;
				}
			}
			if (tabl)
			{
				cout << "digite a chave de busca (digite o valor de todos os campos juntos sem espaço) para remover o registro:" << endl;
				getline(cin, str);
			    if (!tabl->deleteRow(str))
				{
					cout << "não foi possivel encontrar a linha com a chave: " << str << endl;
				}
				else
				{
					cout << "linha removida com sucesso:"<< endl;
				}
				
				system("pause");
			}
			break;
		case '7':
			system("CLS");
			tabl = NULL;
			while (tabl == NULL)
			{


				cout << "digite o nome da tabela (digite X para sair):" << endl;
				getline(cin, nomeTabela);

				tabl = db->findTable(nomeTabela);

				if (nomeTabela == "X")
				{
					break;
				}
				else if (tabl == NULL)
				{
					cout << "nome invalido" << endl;
				}
			}
			if (tabl)
			{
				nomeColuna = "";
				while (nomeColuna != "X")
				{
					cout << "digite o nome da coluna de busca (digite X para contar todos os registros): " << endl;
					getline(cin, nomeColuna);
					if (nomeColuna == "")
					{
						cout << "nome da coluna invalido " << endl;
					}
					else if (nomeColuna != "X")
					{
						if (tabl->getColumnIndex(nomeColuna) < 0)
						{
							cout << "nome da coluna invalido " << endl;
						}
						else
						{
							break;
						}
					}

				}
				
				if (nomeColuna == "X")
				{
					cout << nomeTabela << " " << tabl->getRowCount() << " Registros" << endl;
				}
				else
				{
					cout << "digite a chave de busca para contar os registros:" << endl;
					getline(cin, str);
					cout << nomeTabela << " " << tabl->countByColumn(nomeColuna, str) << " Registros encontrados" << endl;
					
				}	
				system("pause");
			}
			
			break;
		case '8':
		case '9':
		case 'a':
		case 'A':
			system("CLS");
			tabl = NULL;
			while (tabl == NULL)
			{


				cout << "digite o nome da tabela (digite X para sair):" << endl;
				getline(cin, nomeTabela);

				tabl = db->findTable(nomeTabela);

				if (nomeTabela == "X")
				{
					break;
				}
				else if (tabl == NULL)
				{
					cout << "nome invalido" << endl;
				}
			}
			if (tabl)
			{
				Tabela* tablDest = NULL;
				while (tablDest == NULL)
				{


					cout << "digite o nome da tabela Destino (digite X para sair):" << endl;
					getline(cin, nomeTabela);

					tablDest = db->findTable(nomeTabela);

					if (nomeTabela == "X")
					{
						break;
					}
					else if (tabl == NULL)
					{
						cout << "nome invalido" << endl;
					}
				}
				if (tablDest)
				{
					cout << "digite T para exibir apena o contador de registros" << endl;
		
					getline(cin, str);
					bool val = (str == "T") || str == "t";
					if (option == '8')
					  cout << tabl->innerJoin(tablDest, val);
					else if (option == '9')
					  cout << tabl->leftJoin(tablDest, val);
					else if (option == 'a'|| option == 'A')
						cout << tabl->rightJoin(tablDest, val);


				}
			}
			break;
		
		
		case 'X':
			execute = false;
			break;
		default:
			system("CLS");
			cout << "opcao invalida." << endl;
			break;
		} 
	}
		return 0;
}

